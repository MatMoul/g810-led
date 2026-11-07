/*
  This file is part of g810-led.

  g810-led is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, version 3 of the License.

  g810-led is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with g810-led.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Keyboard.h"
#include <glob.h>
#include <fstream>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

std::vector<std::pair<std::string, LedKeyboard::DeviceInfo>> LedKeyboard::findG515() {
    std::vector<std::pair<std::string, DeviceInfo>> found;
    glob_t paths = {};
    if (glob("/sys/class/hidraw/hidraw*/device/uevent", 0, nullptr, &paths) == 0) {
        for (size_t i = 0; i < paths.gl_pathc; ++i) {
            std::ifstream input(paths.gl_pathv[i]);
            std::string line, serial;
            bool id = false, interface = false;
            while (std::getline(input, line)) {
                if (line == "HID_ID=0003:0000046D:0000C358") id = true;
                if (line.find("HID_PHYS=") == 0 && line.size() >= 7 &&
                    line.compare(line.size()-7, 7, "/input2") == 0) interface = true;
                if (line.find("HID_UNIQ=") == 0) serial = line.substr(9);
            }
            if (!id || !interface) continue;
            std::string path(paths.gl_pathv[i]);
            const std::string prefix = "/sys/class/hidraw/";
            const size_t start = prefix.size();
            DeviceInfo info;
            info.vendorID = 0x046d; info.productID = 0xc358;
            info.manufacturer = "Logitech"; info.product = "G515 TKL";
            info.serialNumber = serial; info.model = KeyboardModel::g515;
            found.push_back({"/dev/" + path.substr(start, path.find('/', start)-start), info});
        }
    }
    globfree(&paths);
    return found;
}

bool LedKeyboard::g515Request(uint8_t feature, uint8_t function,
                             const byte_buffer_t &params, byte_buffer_t *reply) {
    if (g515Fd < 0 || params.size() > 16) return false;
    g515Sequence = (g515Sequence % 15) + 1;
    const uint8_t command = (function << 4) | g515Sequence;
    byte_buffer_t packet = {0x11, 0xff, feature, command};
    packet.insert(packet.end(), params.begin(), params.end());
    packet.resize(20, 0);
    if (::write(g515Fd, packet.data(), packet.size()) != 20) {
        std::cerr << "G515 write failed: " << std::strerror(errno) << std::endl;
        return false;
    }
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    while (std::chrono::steady_clock::now() < deadline) {
        const int ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            deadline - std::chrono::steady_clock::now()).count();
        pollfd p = {g515Fd, POLLIN, 0};
        int status = ::poll(&p, 1, ms > 0 ? ms : 1);
        if (status < 0 && errno == EINTR) continue;
        if (status <= 0 || (p.revents & (POLLERR | POLLHUP | POLLNVAL))) break;
        unsigned char data[64];
        const ssize_t size = ::read(g515Fd, data, sizeof(data));
        if (size < 0 && (errno == EAGAIN || errno == EINTR)) continue;
        if (size == 0 || (size < 0 && errno != EAGAIN && errno != EINTR)) {
            std::cerr << "G515 read failed or device disconnected" << std::endl;
            return false;
        }
        if (size < 4 || (data[0] != 0x11 && data[0] != 0x10) || data[1] != 0xff) continue;
        if (size >= 6 && data[2] == 0xff && data[3] == feature && data[4] == command) {
            std::cerr << "G515 rejected feature " << int(feature) << ", function "
                      << int(function) << ", error " << int(data[5]) << std::endl;
            return false;
        }
        if (data[2] == feature && data[3] == command) {
            if (reply) reply->assign(data + 4, data + size);
            return true;
        }
    }
    std::cerr << "G515 timed out waiting for feature " << int(feature)
              << ", function " << int(function) << std::endl;
    return false;
}

bool LedKeyboard::openG515(const std::string &path, const DeviceInfo &info) {
    g515Fd = ::open(path.c_str(), O_RDWR | O_NONBLOCK | O_CLOEXEC);
    if (g515Fd < 0) {
        std::cerr << "Cannot open " << path << ": " << std::strerror(errno) << std::endl;
        return false;
    }
    currentDevice = info;
    m_isOpen = true;
    g515Claimed = false;
    byte_buffer_t reply;
    if (!g515Request(0, 0, {0x80, 0x71}, &reply) || reply.size() < 3 || !reply[0]) {
        close(); errno = ENODEV; return false;
    }
    g515RGB = reply[0];
    if (!g515Request(0, 0, {0x80, 0x81}, &reply) || reply.size() < 3 || !reply[0]) {
        close(); errno = ENODEV; return false;
    }
    g515Keys = reply[0];
    return true;
}

bool LedKeyboard::claimG515() {
    if (g515Claimed) return true;
    if (!g515Request(g515RGB, 5, {1, 3, 4})) return false;
    g515Claimed = true;
    usleep(200000);
    return true;
}

bool LedKeyboard::setG515Keys(const KeyValueArray &values) {
    // Translate HID keyboard usage IDs to Logitech LED zone IDs.
    // Other key groups have different meanings and are deliberately rejected.
    for (const auto &value : values) {
        if ((static_cast<uint16_t>(value.key) >> 8) != 4) {
            std::cerr << "G515: only ordinary keyboard keys are supported" << std::endl;
            return false;
        }
    }
    if (!claimG515()) return false;
    for (size_t i = 0; i < values.size(); i += 4) {
        byte_buffer_t data;
        for (size_t j = i; j < values.size() && j < i + 4; ++j) {
            const uint8_t usage = static_cast<uint16_t>(values[j].key) & 0xff;
            data.push_back(usage >= 0xe0 && usage <= 0xe7 ? usage - 0x78 : usage - 3);
            data.push_back(values[j].color.red);
            data.push_back(values[j].color.green);
            data.push_back(values[j].color.blue);
        }
        if (data.size() < 16) data.push_back(0xff);
        if (!g515Request(g515Keys, 1, data)) return false;
    }
    return true;
}
