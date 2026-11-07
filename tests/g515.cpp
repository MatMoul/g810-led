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

#include <chrono>
#include <iostream>
#include <vector>
#include <utility>
#include <stdint.h>
#include <thread>
#include <cassert>
#include <sys/socket.h>
#include <unistd.h>
#define private public
#include "../src/classes/Keyboard.h"
#undef private

int main() {
    int sockets[2];
    assert(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, sockets) == 0);
    LedKeyboard keyboard;
    keyboard.g515Fd = sockets[0];
    keyboard.g515RGB = 6; keyboard.g515Keys = 7;
    keyboard.currentDevice.model = LedKeyboard::KeyboardModel::g515;
    keyboard.m_isOpen = true;
    using Bytes = std::vector<unsigned char>;
    const std::vector<Bytes> expected = {
        {6, 5, 1, 3, 4},
        {7, 5, 1, 254, 255, 0, 0},
        {7, 7, 0},
        {7, 1, 23, 0, 255, 0, 104, 0, 0, 255, 255},
        {7, 7, 0},
        {6, 5, 1, 0, 0},
        {7, 7, 0}
    };
    std::thread device([&] {
        for (size_t i = 0; i < expected.size(); ++i) {
            unsigned char data[64];
            assert(read(sockets[1], data, sizeof(data)) == 20);
            assert(data[0] == 0x11 && data[1] == 0xff);
            assert(data[2] == expected[i][0]);
            assert((data[3] >> 4) == expected[i][1]);
            for (size_t j = 2; j < expected[i].size(); ++j)
                assert(data[j+2] == expected[i][j]);
            // An unrelated event must not be accepted as a command reply.
            unsigned char unrelated[20] = {0x11, 0xff, 0x01, 0x00};
            assert(write(sockets[1], unrelated, 20) == 20);
            if (i == expected.size() - 1) {
                unsigned char error[20] = {0x11, 0xff, 0xff, data[2], data[3], 2};
                assert(write(sockets[1], error, 20) == 20);
            } else assert(write(sockets[1], data, 20) == 20);
        }
    });
    assert(keyboard.setAllKeys({255, 0, 0}));
    assert(keyboard.commit());
    assert(keyboard.setKeys({{LedKeyboard::Key::w, {0,255,0}},
                             {LedKeyboard::Key::ctrl_left, {0,0,255}}}));
    assert(keyboard.commit());
    assert(keyboard.setOnBoardMode(LedKeyboard::OnBoardMode::board));
    assert(!keyboard.g515Claimed);
    assert(!keyboard.g515Request(7, 7, {0}));
    device.join();
    assert(keyboard.close());
    unsigned char eof;
    assert(read(sockets[1], &eof, 1) == 0);
    close(sockets[1]);
    std::cout << "PASS: colour, key mapping, commit, release, reply matching, error propagation, close\n";
}
