#include "Keyboard.h"

#include "libusb.h"

#include <algorithm>
#include <cassert>
#include <map>
#include <unistd.h>
#include <vector>

namespace helper
{
    uint8_t keyToByte(Keyboard::Key key)
    {
        return static_cast<uint8_t>(static_cast<uint16_t>(key) & 0x00ff);
    }

    Keyboard::KeyAddressGroup keyToKeyAddressGroup(Keyboard::Key key)
    {
        return static_cast<Keyboard::KeyAddressGroup>(static_cast<uint16_t>(key) >> 8 );
    }

    Keyboard::byte_buffer_t keyValuesToBytes(const Keyboard::KeyValueArray& keyValues)
    {
        Keyboard::byte_buffer_t buf;
        for (const auto& keyValue : keyValues)
        {
            buf.push_back(keyToByte(keyValue.key));
            buf.push_back(keyValue.colors.red);
            buf.push_back(keyValue.colors.green);
            buf.push_back(keyValue.colors.blue);
        }
        return buf;
    }

    void append(Keyboard::byte_buffer_t& target, const Keyboard::byte_buffer_t& source)
    {
        target.insert(std::end(target), std::begin(source), std::end(source));
    }
}

bool Keyboard::isAttached()
{
    return m_isAttached;
}

//libusb_device_descriptor desc = {
//    0, // bLength
//    0, // bDescriptorType
//    0, // bcdUSB
//    0, // bDeviceClass
//    0, // bDeviceSubClass
//    0, // bDeviceProtocol
//    0, // bMaxPacketSize0
//    0, // idVendor
//    0, // idProduct
//    0, // bcdDevice
//    0, // iManufacturer
//    0, // iProduct
//    0, // iSerialNumber
//    0  // bNumConfigurations
//};

bool Keyboard::attach()
{
    static const u_int16_t LOGITECH_VID = 0x046d;

    if (m_isAttached == true)
        return false;
    int r;
    r = libusb_init(&ctx);
    if (r < 0)
        return false;

    libusb_device** devs;
    ssize_t cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0)
        return false;
    u_int16_t pid = 0;
    for (ssize_t i = 0; i < cnt; i++)
    {
        libusb_device_descriptor desc = {};
        libusb_get_device_descriptor(devs[i], &desc);

        if (desc.idVendor == LOGITECH_VID)
        {
            bool found_supported_keyboard = false;
            switch (desc.idProduct)
            {
                case 0xc331: // G810 spectrum
                case 0xc337: // G810 spectrum
                case 0xc330: // G410 spectrum
                case 0xc333: // G610 spectrum
                case 0xc338: // G610 spectrum
                case 0xc32b: // G910 spark
                case 0xc335: // G910 spectrum
                    pid = desc.idProduct;
                    found_supported_keyboard = true;
                    break;
                default:;
            }
            if (found_supported_keyboard)
            {
                kbdProtocol = (0xc32b == pid || 0xc335 == pid) ? KeyboardProtocol::g910 : KeyboardProtocol::generic;
                break;
            }
        }
    }
    libusb_free_device_list(devs, 1);
    if (pid == 0)
    {
        libusb_exit(ctx);
        ctx = nullptr;
        return false;
    }

    dev_handle = libusb_open_device_with_vid_pid(ctx, LOGITECH_VID, pid);
    if (dev_handle == nullptr)
    {
        libusb_exit(ctx);
        ctx = nullptr;
        return false;
    }
    if (libusb_kernel_driver_active(dev_handle, 1) == 1)
    {
        libusb_detach_kernel_driver(dev_handle, 1);
        m_isKernellDetached = true;
    }
    r = libusb_claim_interface(dev_handle, 1);
    if (r < 0)
        return false;
    m_isAttached = true;
    return true;
}

bool Keyboard::detach()
{
    if (m_isAttached == false)
        return false;
    int r;
    r = libusb_release_interface(dev_handle, 1);
    if (r != 0)
        return false;
    if (m_isKernellDetached == true)
    {
        libusb_attach_kernel_driver(dev_handle, 1);
        m_isKernellDetached = false;
    }
    libusb_close(dev_handle);
    dev_handle = nullptr;
    libusb_exit(ctx);
    ctx = nullptr;
    m_isAttached = false;
    return true;
}

bool Keyboard::commit()
{
    if (!m_isAttached)
    {
        return false;
    }

    static const Keyboard::byte_buffer_t CommitGeneric = { 0x11, 0xff, 0x0c, 0x5a };
    static const Keyboard::byte_buffer_t CommitG910 = { 0x11, 0xff, 0x0f, 0x5d };

    byte_buffer_t data = kbdProtocol == KeyboardProtocol::g910 ? CommitG910 : CommitGeneric;
    data.resize(20, 0x00);

    return sendDataInternal(data);
}

bool Keyboard::parsePowerOnEffect(const std::string& effect, PowerOnEffect& powerOnEffect)
{
    if (effect == "rainbow")
        powerOnEffect = PowerOnEffect::rainbow;
    else if (effect == "color")
        powerOnEffect = PowerOnEffect::color;
    else
        return false;
    return true;
}

bool Keyboard::parseKey(const std::string& key_string, Key& key)
{
    auto it = keyNameMap.find(key_string);
    if (it != std::end(keyNameMap))
    {
        key = it->second;
        return true;
    }
    return false;
}

bool Keyboard::parseKeyGroup(const std::string& key, KeyGroup& keyGroup)
{
    if (key == "logo")
        keyGroup = KeyGroup::logo;
    else if (key == "indicators")
        keyGroup = KeyGroup::indicators;
    else if (key == "multimedia")
        keyGroup = KeyGroup::multimedia;
    else if (key == "fkeys")
        keyGroup = KeyGroup::fkeys;
    else if (key == "modifiers")
        keyGroup = KeyGroup::modifiers;
    else if (key == "arrows")
        keyGroup = KeyGroup::arrows;
    else if (key == "numeric")
        keyGroup = KeyGroup::numeric;
    else if (key == "functions")
        keyGroup = KeyGroup::functions;
    else if (key == "keys")
        keyGroup = KeyGroup::keys;
    else if (key == "gkeys")
        keyGroup = KeyGroup::gkeys;
    else
        return false;
    return true;
}

bool Keyboard::parseColor(const std::string& color_text, KeyColors& color)
{
    std::string color_text_internal = color_text;
    if (color_text_internal.length() == 2)
        color_text_internal = color_text_internal + "0000"; // For G610
    if (color_text_internal.length() != 6)
        return false;
    color.red = std::stoul("0x" + color_text_internal.substr(0, 2), nullptr, 16);
    color.green = std::stoul("0x" + color_text_internal.substr(2, 2), nullptr, 16);
    color.blue = std::stoul("0x" + color_text_internal.substr(4, 2), nullptr, 16);
    return true;
}

bool Keyboard::parseSpeed(const std::string& speed, uint8_t& speedValue)
{
    std::string speed_internal = speed;
    if (speed_internal.length() == 1)
        speed_internal = speed_internal + "0";
    if (speed_internal.length() != 2)
        return false;
    speedValue = std::stoul("0x" + speed_internal, nullptr, 16);
    return true;
}

bool Keyboard::sendDataInternal(const byte_buffer_t& data)
{
    if (m_isAttached == false)
        return false;
    int r;
    if (data.size() > 20)
        r = libusb_control_transfer(dev_handle, 0x21, 0x09, 0x0212, 1, const_cast<unsigned char*>(data.data()), data.size(), 2000);
    else
        r = libusb_control_transfer(dev_handle, 0x21, 0x09, 0x0211, 1, const_cast<unsigned char*>(data.data()), data.size(), 2000);
    usleep(1000);
    if (r < 0)
        return false;
    return true;
}

Keyboard::byte_buffer_t Keyboard::getKeyGroupAddress(KeyboardProtocol kbdProtocol, KeyAddressGroup addressGroup)
{
    switch (kbdProtocol)
    {
        case KeyboardProtocol::generic:
            switch (addressGroup)
            {
                case KeyAddressGroup::logo:
                    return { 0x11, 0xff, 0x0c, 0x3a, 0x00, 0x10, 0x00, 0x01 };
                case KeyAddressGroup::indicators:
                    return { 0x12, 0xff, 0x0c, 0x3a, 0x00, 0x40, 0x00, 0x05 };
                case KeyAddressGroup::multimedia:
                    return { 0x12, 0xff, 0x0c, 0x3a, 0x00, 0x02, 0x00, 0x05 };
                case KeyAddressGroup::keys:
                    return { 0x12, 0xff, 0x0c, 0x3a, 0x00, 0x01, 0x00, 0x0e };
                case KeyAddressGroup::gkeys:
                    return { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // weird, but this is what happens with the original code
            }

        case KeyboardProtocol::g910: // gkeys and mkeys seem not changeable
            switch (addressGroup)
            {
                case KeyAddressGroup::logo:
                    return { 0x11, 0xff, 0x0f, 0x3a, 0x00, 0x10, 0x00, 0x02 };
                case KeyAddressGroup::indicators:
                    return { 0x12, 0xff, 0x0c, 0x3a, 0x00, 0x40, 0x00, 0x05 };
                case KeyAddressGroup::keys:
                    return { 0x12, 0xff, 0x0f, 0x3d, 0x00, 0x01, 0x00, 0x0e };
                case KeyAddressGroup::gkeys:
                    return { 0x12, 0xff, 0x0f, 0x3e, 0x00, 0x04, 0x00, 0x09 };
                case KeyAddressGroup::multimedia:
                    return { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // weird, but this is what happens with the original code
            }
    }

    assert(false); // can't get to here

    return { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
}

Keyboard::byte_buffer_t Keyboard::getFxAddress(KeyboardProtocol protocol)
{
    switch (protocol)
    {
        case KeyboardProtocol::generic:
            return { 0x11, 0xff, 0x0d, 0x3c };
        case KeyboardProtocol::g910:
            return { 0x11, 0xff, 0x10, 0x3c };
        default:;
    }
    return {};
}

// this method should only be called with keys of the same address group.. assert below
bool Keyboard::setKeysInternal(const KeyValueArray& keyValues)
{
    if (keyValues.empty())
    {
        return false;
    }


    for ( const auto& keyValue : keyValues)
    {
        assert( helper::keyToKeyAddressGroup(keyValue.key) == helper::keyToKeyAddressGroup(keyValues.front().key) );
    }

    auto split = [](const byte_buffer_t& buf, unsigned int batch_size) -> std::vector<byte_buffer_t> {
        std::vector<byte_buffer_t> buffers;
        auto remaining_bytes = buf.size();
        auto it = std::begin(buf);
        while (remaining_bytes > 0)
        {
            auto current_batch_size = remaining_bytes > batch_size ? batch_size : remaining_bytes;
            buffers.push_back({ it, std::next(it, current_batch_size) });
            it += current_batch_size;
            remaining_bytes -= current_batch_size;
        }
        return buffers;
    };

    KeyAddressGroup addressGroup = helper::keyToKeyAddressGroup(keyValues.front().key);
    byte_buffer_t header{ getKeyGroupAddress(kbdProtocol, addressGroup) };
    byte_buffer_t key_stream{ helper::keyValuesToBytes(keyValues) };

    const auto send_size = addressGroup == KeyAddressGroup::logo ? 20 : 64;
    const auto batch_size = send_size - header.size();
    const auto batches = split(key_stream, batch_size);
    for (auto batch : batches)
    {
        batch.insert(std::begin(batch), std::begin(header), std::end(header));
        batch.resize(send_size, 0x00); // pad to size
        sendDataInternal(batch);
    }

    return true;
}
bool Keyboard::setPowerOnEffect(PowerOnEffect powerOnEffect)
{
    auto powerOnEffectCommand = [this](uint8_t effect) -> byte_buffer_t {
        switch (kbdProtocol)
        {
            case KeyboardProtocol::generic:
                return { 0x11, 0xff, 0x0d, 0x5a, 0x00, 0x01, effect };
            case KeyboardProtocol::g910:
                return { 0x11, 0xff, 0x10, 0x5e, 0x00, 0x01, effect };
        }
        return {};
    };

    byte_buffer_t data{ powerOnEffectCommand(static_cast<uint8_t>(powerOnEffect)) };
    data.resize(20, 0x00);

    return sendDataInternal(data);
}

bool Keyboard::setKey(KeyValue keyValue)
{
    return setKeysInternal(KeyValueArray{ keyValue });
}

bool Keyboard::setKeys(const KeyValueArray& keyValues)
{
    std::map<KeyAddressGroup, KeyValueArray> valuesMap;
    for (const auto& keyValue : keyValues)
    {
        valuesMap[helper::keyToKeyAddressGroup(keyValue.key)].push_back(keyValue);
    }
    for (const auto& x : valuesMap)
    {
        setKeysInternal(x.second);
    }
    return true;
}

bool Keyboard::setAllKeys(KeyColors color) // shouldn't this function simply call fx-color instead?
{
    //   return setFx(FxType::Color, color);  // alternative to below

    // clang-format off
    return setGroupKeys(KeyGroup::arrows, color) &&
        setGroupKeys(KeyGroup::fkeys, color) &&
        setGroupKeys(KeyGroup::functions, color) &&
        setGroupKeys(KeyGroup::gkeys, color) &&
        setGroupKeys(KeyGroup::indicators, color) &&
        setGroupKeys(KeyGroup::keys, color) &&
        setGroupKeys(KeyGroup::logo, color) &&
        setGroupKeys(KeyGroup::modifiers, color) &&
        setGroupKeys(KeyGroup::multimedia, color) &&
        setGroupKeys(KeyGroup::numeric, color);
    // clang-format on
}

bool Keyboard::setGroupKeys(KeyGroup keyGroup, KeyColors color)
{
    KeyValueArray keyValues;
    const KeyArray& keyAddressArray = keyGroupArrayMap.find(keyGroup)->second;
    for (auto& key : keyAddressArray)
    {
        keyValues.push_back({ key, color });
    }
    return setKeysInternal(keyValues);
}

bool Keyboard::setFx(FxType fxType, uint8_t speed, KeyColors color)
{
    byte_buffer_t buf{ getFxAddress(kbdProtocol) };

    switch (fxType)
    {
        case FxType::BreathingKeys:
        {
            helper::append(buf, { 0x00, 0x02, color.red, color.green, color.blue, speed, 0x10, 0x00, 0x64 });
        }
        break;

        case FxType::BreathingLogo:
        {
            helper::append(buf, { 0x01, 0x02, color.red, color.green, color.blue, speed, 0x10, 0x00, 0x64 });
        }
        break;

        case FxType::ColorKeys:
        {
            helper::append(buf, { 0x00, 0x01, color.red, color.green, color.blue, 0x02 });
        }
        break;

        case FxType::ColorLogo:
        {
            helper::append(buf, { 0x01, 0x01, color.red, color.green, color.blue, 0x02 });
        }
        break;

        case FxType::ColorCycleKeys:
        {
            helper::append(buf, { 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, speed, 0x00, 0x00, 0x64 });
        }
        break;

        case FxType::ColorCycleLogo:
        {
            helper::append(buf, { 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, speed, 0x00, 0x00, 0x64 });
        }
        break;

        case FxType::CWaveKeys:
        {
            helper::append(buf, { 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x03, 0x64, speed });
        }
        break;

        case FxType::HWaveKeys:
        {
            helper::append(buf, { 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x01, 0x64, speed });
        }
        break;

        case FxType::VWaveKeys:
        {
            helper::append(buf, { 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x02, 0x64, speed });
        }
        break;
    }

    buf.resize(20, 0x00);

    return sendDataInternal(buf);
}

bool Keyboard::setFXColorKeys(KeyColors color)
{
    return setFx(FxType::ColorKeys, 0, color);
}

bool Keyboard::setFXColorLogo(KeyColors color)
{
    return setFx(FxType::ColorLogo, 0, color);
}

bool Keyboard::setFXBreathingKeys(KeyColors color, uint8_t speed)
{
    return setFx(FxType::BreathingKeys, speed, color);
}

bool Keyboard::setFXBreathingLogo(KeyColors color, uint8_t speed)
{
    return setFx(FxType::BreathingLogo, speed, color);
}

bool Keyboard::setFXColorCycleKeys(uint8_t speed)
{
    return setFx(FxType::ColorCycleKeys, speed);
}

bool Keyboard::setFXColorCycleLogo(uint8_t speed)
{
    return setFx(FxType::ColorCycleLogo, speed);
}

bool Keyboard::setFXHWaveKeys(uint8_t speed)
{
    return setFx(FxType::HWaveKeys, speed);
}

bool Keyboard::setFXVWaveKeys(uint8_t speed)
{
    return setFx(FxType::VWaveKeys, speed);
}

bool Keyboard::setFXCWaveKeys(uint8_t speed)
{
    return setFx(FxType::CWaveKeys, speed);
}
