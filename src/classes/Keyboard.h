#ifndef DEF_KEYBOARD
#define DEF_KEYBOARD

#include <libusb.h>

#include <cstdint>
#include <iostream>
#include <map>
#include <vector>
#include <functional>



class Keyboard
{
public:
    // clang-format off
    enum class KeyboardProtocol { generic, g910 };
    enum class PowerOnEffect : uint8_t { rainbow = 0x01, color };

    enum class FxType { BreathingKeys, BreathingLogo, ColorKeys, ColorLogo, ColorCycleKeys, ColorCycleLogo, CWaveKeys, HWaveKeys, VWaveKeys };

    enum class KeyAddressGroup : uint8_t
    {
        logo        = 0x00,
        indicators, multimedia, gkeys, keys
    };

    enum class Key : uint16_t
    {
        // logo
        logo        = static_cast<uint8_t>(KeyAddressGroup::logo) << 8 | 0x01,
        logo2,

        // indicators
        backlight   = static_cast<uint8_t>(KeyAddressGroup::indicators) << 8| 0x01,
        game, caps, scroll, num,

        // multimedia
        next        = static_cast<uint8_t>(KeyAddressGroup::multimedia) << 8 | 0xb5,
        prev, stop,
        play        = static_cast<uint8_t>(KeyAddressGroup::multimedia) << 8 | 0xcd,
        mute        = static_cast<uint8_t>(KeyAddressGroup::multimedia) << 8 | 0xe2,

        // gkeys
        g1          = static_cast<uint8_t>(KeyAddressGroup::gkeys) << 8 | 0x01,
        g2, g3, g4, g5, g6, g7, g8, g9,

        // keys
        a           = static_cast<uint8_t>(KeyAddressGroup::keys) << 8 | 0x04,
        b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
        n1, n2, n3, n4, n5, n6, n7, n8, n9, n0,
        enter, esc, backspace, tab, space, minus, equal, open_bracket, close_bracket,
        backslash, dollar, semicolon, quote, tilde, comma, period, slash, caps_lock,
        f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
        print_screen, scroll_lock, pause_break, insert, home, page_up, del, end, page_down,
        arrow_right, arrow_left, arrow_bottom, arrow_top, num_lock, num_slash, num_asterisk,
        num_minus, num_plus, num_enter,
        num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9, num_0,
        num_dot, intl_backslash, menu,

        ctrl_left   = static_cast<uint8_t>(KeyAddressGroup::keys) << 8 | 0xe0,
        shift_left, alt_left, win_left,
        ctrl_right, shift_right, alt_right, win_right
    };


    using KeyNameMap_t = std::map<std::string, Key>;
    const KeyNameMap_t keyNameMap = {
        { "logo", Key::logo },
        { "logo2", Key::logo2 },
        { "back_light", Key::backlight },
        { "backlight", Key::backlight },
        { "light", Key::backlight },
        { "game_mode", Key::game },
        { "gamemode", Key::game },
        { "game", Key::game },
        { "caps_indicator", Key::caps },
        { "capsindicator", Key::caps },
        { "caps", Key::caps },
        { "scroll_indicator", Key::scroll },
        { "scrollindicator", Key::scroll },
        { "scroll", Key::scroll },
        { "num_indicator", Key::num },
        { "numindicator", Key::num },
        { "num", Key::num },
        { "next", Key::next },
        { "prev", Key::prev },
        { "previous", Key::prev },
        { "stop", Key::stop },
        { "play_pause", Key::play },
        { "playpause", Key::play },
        { "play", Key::play },
        { "mute", Key::mute },
        { "a", Key::a },
        { "b", Key::b },
        { "c", Key::c },
        { "d", Key::d },
        { "e", Key::e },
        { "f", Key::f },
        { "g", Key::g },
        { "h", Key::h },
        { "i", Key::i },
        { "j", Key::j },
        { "k", Key::k },
        { "l", Key::l },
        { "m", Key::m },
        { "n", Key::n },
        { "o", Key::o },
        { "p", Key::p },
        { "q", Key::q },
        { "r", Key::r },
        { "s", Key::s },
        { "t", Key::t },
        { "u", Key::u },
        { "v", Key::v },
        { "w", Key::w },
        { "x", Key::x },
        { "z", Key::z },
        { "y", Key::y },
        { "1", Key::n1 },
        { "one", Key::n1 },
        { "2", Key::n2 },
        { "two", Key::n2 },
        { "3", Key::n3 },
        { "three", Key::n3 },
        { "4", Key::n4 },
        { "four", Key::n4 },
        { "5", Key::n5 },
        { "five", Key::n5 },
        { "6", Key::n6 },
        { "six", Key::n6 },
        { "7", Key::n7 },
        { "seven", Key::n7 },
        { "8", Key::n8 },
        { "eight", Key::n8 },
        { "9", Key::n9 },
        { "nine", Key::n9 },
        { "0", Key::n0 },
        { "zero", Key::n0 },
        { "enter", Key::enter },
        { "esc", Key::esc },
        { "escape", Key::esc },
        { "back", Key::backspace },
        { "backspace", Key::backspace },
        { "tab", Key::tab },
        { "space", Key::space },
        { "tilde", Key::tilde },
        { "~", Key::tilde },
        { "minus", Key::minus },
        { "-", Key::minus },
        { "equal", Key::equal },
        { "=", Key::equal },
        { "open_bracket", Key::open_bracket },
        { "[", Key::open_bracket },
        { "close_bracket", Key::close_bracket },
        { "]", Key::close_bracket },
        { "backslash", Key::backslash },
        { "\\", Key::backslash },
        { "semicolon", Key::semicolon },
        { ";", Key::semicolon },
        { "quote", Key::quote },
        { "\"", Key::quote },
        { "dollar", Key::dollar },
        { "$", Key::dollar },
        { "comma", Key::comma },
        { ",", Key::comma },
        { "period", Key::period },
        { ".", Key::period },
        { "slash", Key::slash },
        { "/", Key::slash },
        { "caps_lock", Key::caps_lock },
        { "capslock", Key::caps_lock },
        { "f1", Key::f1 },
        { "f2", Key::f2 },
        { "f3", Key::f3 },
        { "f4", Key::f4 },
        { "f5", Key::f5 },
        { "f6", Key::f6 },
        { "f7", Key::f7 },
        { "f8", Key::f8 },
        { "f9", Key::f9 },
        { "f10", Key::f10 },
        { "f11", Key::f11 },
        { "f12", Key::f12 },
        { "print_screen", Key::print_screen },
        { "printscreen", Key::print_screen },
        { "printscr", Key::print_screen },
        { "print", Key::print_screen },
        { "scroll_lock", Key::scroll_lock },
        { "scrolllock", Key::scroll_lock },
        { "pause_break", Key::pause_break },
        { "pausebreak", Key::pause_break },
        { "pause", Key::pause_break },
        { "break", Key::pause_break },
        { "insert", Key::insert },
        { "ins", Key::insert },
        { "home", Key::home },
        { "page_up", Key::page_up },
        { "pageup", Key::page_up },
        { "delete", Key::del },
        { "del", Key::del },
        { "end", Key::end },
        { "page_down", Key::page_down },
        { "pagedown", Key::page_down },
        { "arrow_right", Key::arrow_right },
        { "arrowright", Key::arrow_right },
        { "right", Key::arrow_right },
        { "arrow_left", Key::arrow_left },
        { "arrowleft", Key::arrow_left },
        { "left", Key::arrow_left },
        { "arrow_bottom", Key::arrow_bottom },
        { "arrowbottom", Key::arrow_bottom },
        { "bottom", Key::arrow_bottom },
        { "arrow_top", Key::arrow_top },
        { "arrowtop", Key::arrow_top },
        { "top", Key::arrow_top },
        { "num_lock", Key::num_lock },
        { "numlock", Key::num_lock },
        { "num/", Key::num_slash },
        { "num_slash", Key::num_slash },
        { "numslash", Key::num_slash },
        { "num*", Key::num_asterisk },
        { "num_asterisk", Key::num_asterisk },
        { "numasterisk", Key::num_asterisk },
        { "num-", Key::num_minus },
        { "num_minus", Key::num_minus },
        { "numminus", Key::num_minus },
        { "num+", Key::num_plus },
        { "num_plus", Key::num_plus },
        { "numplus", Key::num_plus },
        { "numenter", Key::num_enter },
        { "num1", Key::num_1 },
        { "num2", Key::num_2 },
        { "num3", Key::num_3 },
        { "num4", Key::num_4 },
        { "num5", Key::num_5 },
        { "num6", Key::num_6 },
        { "num7", Key::num_7 },
        { "num8", Key::num_8 },
        { "num9", Key::num_9 },
        { "num0", Key::num_0 },
        { "num.", Key::num_dot },
        { "num_period", Key::num_dot },
        { "numperiod", Key::num_dot },
        { "intl_backslash", Key::intl_backslash },
        { "<", Key::intl_backslash },
        { "menu", Key::menu },
        { "ctrl_left", Key::ctrl_left },
        { "ctrlleft", Key::ctrl_left },
        { "ctrll", Key::ctrl_left },
        { "shift_left", Key::shift_left },
        { "shiftleft", Key::shift_left },
        { "shiftl", Key::shift_left },
        { "alt_left", Key::alt_left },
        { "altleft", Key::alt_left },
        { "altl", Key::alt_left },
        { "win_left", Key::win_left },
        { "winleft", Key::win_left },
        { "winl", Key::win_left },
        { "meta_left", Key::win_left },
        { "metaleft", Key::win_left },
        { "metal", Key::win_left },
        { "ctrl_right", Key::ctrl_right },
        { "ctrlright", Key::ctrl_right },
        { "ctrlr", Key::ctrl_right },
        { "shift_right", Key::shift_right },
        { "shiftright", Key::shift_right },
        { "shiftr", Key::shift_right },
        { "alt_right", Key::alt_right },
        { "altright", Key::alt_right },
        { "altr", Key::alt_right },
        { "altgr", Key::alt_right },
        { "win_right", Key::win_right },
        { "winright", Key::win_right },
        { "winr", Key::win_right },
        { "meta_right", Key::win_right },
        { "metaright", Key::win_right },
        { "metar", Key::win_right },
        { "g1", Key::g1 },
        { "g2", Key::g2 },
        { "g3", Key::g3 },
        { "g4", Key::g4 },
        { "g5", Key::g5 },
        { "g6", Key::g6 },
        { "g7", Key::g7 },
        { "g8", Key::g8 },
        { "g9", Key::g9 },
    };

    enum class KeyGroup { logo, indicators, multimedia, fkeys, modifiers, arrows, numeric, functions, keys, gkeys };

    struct KeyColors { uint8_t red; uint8_t green; uint8_t blue; };
    struct KeyValue { Key key; KeyColors colors; };

    typedef std::vector<unsigned char> byte_buffer_t;
    typedef std::vector<KeyValue> KeyValueArray;
    typedef std::vector<Key> KeyArray;

    // clang-format on

    const KeyArray keyGroupLogo = { Key::logo, Key::logo2 };
    const KeyArray keyGroupIndicators = { Key::caps, Key::num, Key::scroll, Key::game, Key::backlight };
    const KeyArray keyGroupMultimedia = { Key::next, Key::prev, Key::stop, Key::play, Key::mute };
    const KeyArray keyGroupFKeys = { Key::f1, Key::f2, Key::f3, Key::f4, Key::f5, Key::f6, Key::f7, Key::f8, Key::f9, Key::f10, Key::f11, Key::f12 };
    const KeyArray keyGroupModifiers = { Key::shift_left, Key::ctrl_left, Key::win_left, Key::alt_left, Key::alt_right, Key::win_right, Key::ctrl_right, Key::shift_right, Key::menu };
    const KeyArray keyGroupArrows = { Key::arrow_top, Key::arrow_left, Key::arrow_bottom, Key::arrow_right };
    const KeyArray keyGroupNumeric = { Key::num_1, Key::num_2, Key::num_3, Key::num_4, Key::num_5, Key::num_6, Key::num_7, Key::num_8, Key::num_9, Key::num_0, Key::num_dot, Key::num_enter, Key::num_plus, Key::num_minus, Key::num_asterisk, Key::num_slash, Key::num_lock };
    const KeyArray keyGroupFunctions = { Key::esc, Key::scroll_lock,
        Key::insert, Key::del, Key::home, Key::end, Key::page_up, Key::page_down, Key::print_screen, Key::pause_break,
        Key::n1, Key::n2, Key::n3, Key::n4, Key::n5, Key::n6, Key::n7, Key::n8, Key::n9, Key::n0,
        Key::tab, Key::caps_lock, Key::space, Key::backspace, Key::enter };
    const KeyArray keyGroupKeys = { Key::a, Key::b, Key::c, Key::d, Key::e, Key::f, Key::g, Key::h, Key::i, Key::j, Key::k, Key::l, Key::m, Key::n, Key::o, Key::p, Key::q, Key::r, Key::s, Key::t, Key::u, Key::v, Key::w, Key::x, Key::y, Key::z,
        Key::tilde, Key::minus, Key::equal,
        Key::open_bracket, Key::close_bracket, Key::backslash,
        Key::semicolon, Key::quote, Key::dollar,
        Key::intl_backslash, Key::comma, Key::period, Key::slash };
    const KeyArray keyGroupGKeys = { Key::g1, Key::g2, Key::g3, Key::g4, Key::g5, Key::g6, Key::g7, Key::g8, Key::g9 };

    const std::map<KeyGroup, std::reference_wrapper<const KeyArray>> keyGroupArrayMap =
    {
        {KeyGroup::logo, std::cref(keyGroupLogo)},
        {KeyGroup::indicators, std::cref(keyGroupIndicators)},
        {KeyGroup::multimedia, std::cref(keyGroupMultimedia)},
        {KeyGroup::fkeys, std::cref(keyGroupFKeys)},
        {KeyGroup::modifiers, std::cref(keyGroupModifiers)},
        {KeyGroup::arrows, std::cref(keyGroupArrows)},
        {KeyGroup::numeric, std::cref(keyGroupNumeric)},
        {KeyGroup::functions, std::cref(keyGroupFunctions)},
        {KeyGroup::keys, std::cref(keyGroupKeys)},
        {KeyGroup::gkeys, std::cref(keyGroupGKeys)}
    };

    bool isAttached();
    bool attach();
    bool detach();
    bool commit();

    bool parsePowerOnEffect(const std::string& effect, PowerOnEffect& powerOnEffect);
    bool parseKey(const std::string& key_string, Key& key);
    bool parseKeyGroup(const std::string& key, KeyGroup& keyGroup);
    bool parseColor(const std::string& color_text, KeyColors& color);
    bool parseSpeed(const std::string& speed, uint8_t& speedValue);

    bool setPowerOnEffect(PowerOnEffect powerOnEffect);
    bool setKey(KeyValue keyValue);
    bool setKeys(const KeyValueArray& keyValues);
    bool setAllKeys(KeyColors color);
    bool setGroupKeys(KeyGroup keyGroup, KeyColors color);
    bool setFXColorKeys(KeyColors colors);
    bool setFXColorLogo(KeyColors colors);
    bool setFXBreathingKeys(KeyColors colors, uint8_t speed);
    bool setFXBreathingLogo(KeyColors colors, uint8_t speed);
    bool setFXColorCycleKeys(uint8_t speed);
    bool setFXColorCycleLogo(uint8_t speed);
    bool setFXHWaveKeys(uint8_t speed);
    bool setFXVWaveKeys(uint8_t speed);
    bool setFXCWaveKeys(uint8_t speed);

private:
    bool m_isAttached = false;
    bool m_isKernellDetached = false;
    KeyboardProtocol kbdProtocol = KeyboardProtocol::generic;
    libusb_device** devs;
    libusb_device_handle* dev_handle;
    libusb_context* ctx = nullptr;

    byte_buffer_t getKeyGroupAddress(KeyboardProtocol kbdProtocol, KeyAddressGroup addressGroup);
    byte_buffer_t getFxAddress(KeyboardProtocol protocol);

    bool setFx(FxType fxType, uint8_t speed = 0, KeyColors color = {0,0,0} );
    bool sendDataInternal(const byte_buffer_t& data);
    bool setKeysInternal(const KeyValueArray& keyValues);
};

#endif
