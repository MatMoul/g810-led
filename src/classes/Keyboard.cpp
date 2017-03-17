#include "Keyboard.h"

#include <iostream>
#include <unistd.h>
#include <vector>

#if defined(hidapi)
	#include "hidapi/hidapi.h"
#elif defined(libusb)
	#include "libusb-1.0/libusb.h"
#endif


using namespace std;



LedKeyboard::~LedKeyboard() {
	close();
}


bool LedKeyboard::listKeyboards() {
	#if defined(hidapi)
		if (hid_init() < 0) return false;
		
		struct hid_device_info *devs, *dev;
		devs = hid_enumerate(0x0, 0x0);
		dev = devs;
		while (dev) {
			for (int i=0; i<(int)SuportedKeyboards.size(); i++) {
				if (dev->vendor_id == SuportedKeyboards[i][0]) {
					if (dev->product_id == SuportedKeyboards[i][1]) {
						cout<<"0x"<<std::hex<<dev->vendor_id \
							<<" 0x"<<std::hex<<dev->product_id \
							<<" "<<dev->serial_number \
							<<" "<<dev->path<<" ";
						dev = dev->next;
						cout<<dev->serial_number<<" "<<dev->path<<endl;
						i++;
						break;
					}
				}
			}
			dev = dev->next;
		}
		hid_free_enumeration(devs);
		
		hid_exit();
		return true;
		
	#elif defined(libusb)
		libusb_context *ctx = NULL;
		if(libusb_init(&m_ctx) < 0) return false;
		
		libusb_device **devs;
		ssize_t cnt = libusb_get_device_list(ctx, &devs);
		for(ssize_t i = 0; i < cnt; i++) {
			libusb_device *device = devs[i];
			libusb_device_descriptor desc;
			libusb_get_device_descriptor(device, &desc);
			for (int i=0; i<(int)SuportedKeyboards.size(); i++) {
				if (desc.idVendor == SuportedKeyboards[i][0]) {
					if (desc.idProduct == SuportedKeyboards[i][1]) {
						cout<<"0x"<<std::hex<<desc.idVendor \
							<<" 0x"<<std::hex<<desc.idProduct<<endl;
						break;
					}
				}
			}
		}
		libusb_free_device_list(devs, 1);
		
		libusb_exit(m_ctx);
		return true;
	#endif
	
	return false;
}


bool LedKeyboard::isOpen() {
	return m_isOpen;
}

bool LedKeyboard::open() {
	if (m_isOpen) return true;
	
	#if defined(hidapi)
		if (hid_init() < 0) return false;
		
		if (m_keyboardModel == KeyboardModel::unknown) {
			struct hid_device_info *devs, *dev;
			devs = hid_enumerate(0x0, 0x0);
			dev = devs;
			m_keyboardModel = KeyboardModel::unknown;
			while (dev) {
				for (int i=0; i<(int)SuportedKeyboards.size(); i++) {
					if (dev->vendor_id == SuportedKeyboards[i][0]) {
						if (dev->product_id == SuportedKeyboards[i][1]) {
							m_vendorID = dev->vendor_id;
							m_productID = dev->product_id;
							m_keyboardModel = (KeyboardModel)SuportedKeyboards[i][2];
							break;
						}
					}
				}
				if (m_keyboardModel != KeyboardModel::unknown) break;
				dev = dev->next;
			}
			hid_free_enumeration(devs);
			
			if (! dev) {
				cout<<"Keyboard not found"<<endl;
				m_keyboardModel = KeyboardModel::unknown;
				hid_exit();
				return false;
			}
		}
		
		m_hidHandle = hid_open(m_vendorID, m_productID, NULL);
		
		if(m_hidHandle == 0) {
			hid_exit();
			return false;
		}
		
		m_isOpen = true;
		return true;
		
	#elif defined(libusb)
		if(libusb_init(&m_ctx) < 0) return false;
		
		if (m_keyboardModel == KeyboardModel::unknown) {
			libusb_device **devs;
			ssize_t cnt = libusb_get_device_list(m_ctx, &devs);
			if(cnt >= 0) {
				for(ssize_t i = 0; i < cnt; i++) {
					libusb_device *device = devs[i];
					libusb_device_descriptor desc;
					libusb_get_device_descriptor(device, &desc);
					for (int i=0; i<(int)SuportedKeyboards.size(); i++) {
						if (desc.idVendor == SuportedKeyboards[i][0]) {
							if (desc.idProduct == SuportedKeyboards[i][1]) {
								m_vendorID = desc.idVendor;
								m_productID = desc.idProduct;
								m_keyboardModel = (KeyboardModel)SuportedKeyboards[i][2];
								break;
							}
						}
					}
					if (m_keyboardModel != KeyboardModel::unknown) break;
				}
				libusb_free_device_list(devs, 1);
			}
		}
		
		if (m_keyboardModel == KeyboardModel::unknown) {
			cout<<"Keyboard not found"<<endl;
			libusb_exit(m_ctx);
			m_ctx = NULL;
			return false;
		}
		
		m_hidHandle = libusb_open_device_with_vid_pid(m_ctx, m_vendorID, m_productID);
		if(m_hidHandle == 0) {
			libusb_exit(m_ctx);
			m_ctx = NULL;
			return false;
		}
		
		if(libusb_kernel_driver_active(m_hidHandle, 1) == 1) {
			if(libusb_detach_kernel_driver(m_hidHandle, 1) != 0) {
				libusb_exit(m_ctx);
				m_ctx = NULL;
				return false;
			}
			m_isKernellDetached = true;
		}
		
		if(libusb_claim_interface(m_hidHandle, 1) < 0) {
			if(m_isKernellDetached==true) {
				libusb_attach_kernel_driver(m_hidHandle, 1);
				m_isKernellDetached = false;
			}
			libusb_exit(m_ctx);
			m_ctx = NULL;
			return false;
		}
		
		m_isOpen = true;
		return true;
		
	#endif
	
	return false;
}

bool LedKeyboard::close() {
	if (! m_isOpen) return true;
	m_isOpen = false;
	
	#if defined(hidapi)
		hid_close(m_hidHandle);
		m_hidHandle = 0;
		hid_exit();
		return true;
		
	#elif defined(libusb)
		if(libusb_release_interface(m_hidHandle, 1) != 0) return false;
		if(m_isKernellDetached==true) {
			libusb_attach_kernel_driver(m_hidHandle, 1);
			m_isKernellDetached = false;
		}
		libusb_close(m_hidHandle);
		m_hidHandle = NULL;
		libusb_exit(m_ctx);
		m_ctx = NULL;
		return true;
	#endif
	
	return false;
}


LedKeyboard::KeyboardModel LedKeyboard::getKeyboardModel() {
	return m_keyboardModel;
}


bool LedKeyboard::commit() {
	byte_buffer_t data;
	switch (m_keyboardModel) {
		case KeyboardModel::g410:
		case KeyboardModel::g610:
		case KeyboardModel::g810:
			data = { 0x11, 0xff, 0x0c, 0x5a };
			break;
		case KeyboardModel::g910:
			data = { 0x11, 0xff, 0x0f, 0x5d };
			break;
		default:
			return false;
	}
	data.resize(20, 0x00);
	return sendDataInternal(data);
}

bool LedKeyboard::setKey(LedKeyboard::KeyValue keyValue) {
	return setKeys(KeyValueArray {keyValue});
}

bool LedKeyboard::setKeys(KeyValueArray keyValues) {
	if (keyValues.empty()) return false;
	
	bool retval = true;
	
	vector<vector<KeyValue>> SortedKeys = {
		{}, // Logo AddressGroup
		{}, // Indicators AddressGroup
		{}, // Multimedia AddressGroup
		{}, // GKeys AddressGroup
		{} // Keys AddressGroup
	};
	
	for (uint8_t i = 0; i < keyValues.size(); i++) {
		switch(static_cast<LedKeyboard::KeyAddressGroup>(static_cast<uint16_t>(keyValues[i].key) >> 8 )) {
			case LedKeyboard::KeyAddressGroup::logo:
				switch (m_keyboardModel) {
					case LedKeyboard::KeyboardModel::g410:
					case LedKeyboard::KeyboardModel::g610:
					case LedKeyboard::KeyboardModel::g810:
						if (SortedKeys[0].size() <= 1 && keyValues[i].key == LedKeyboard::Key::logo)
							SortedKeys[0].push_back(keyValues[i]);
						break;
					case LedKeyboard::KeyboardModel::g910:
						if (SortedKeys[0].size() <= 2) SortedKeys[0].push_back(keyValues[i]);
						break;
					default:
						break;
				}
				break;
			case LedKeyboard::KeyAddressGroup::indicators:
				if (SortedKeys[1].size() <= 5) SortedKeys[1].push_back(keyValues[i]);
				break;
			case LedKeyboard::KeyAddressGroup::multimedia:
				switch (m_keyboardModel) {
					case LedKeyboard::KeyboardModel::g610:
					case LedKeyboard::KeyboardModel::g810:
						if (SortedKeys[2].size() <= 5) SortedKeys[2].push_back(keyValues[i]);
						break;
					default:
						break;
				}
				break;
			case LedKeyboard::KeyAddressGroup::gkeys:
				switch (m_keyboardModel) {
					case LedKeyboard::KeyboardModel::g910:
						if (SortedKeys[3].size() <= 9) SortedKeys[3].push_back(keyValues[i]);
						break;
					default:
						break;
				}
				break;
			case LedKeyboard::KeyAddressGroup::keys:
				switch (m_keyboardModel) {
					case LedKeyboard::KeyboardModel::g610:
					case LedKeyboard::KeyboardModel::g810:
					case LedKeyboard::KeyboardModel::g910:
						if (SortedKeys[4].size() <= 120) SortedKeys[4].push_back(keyValues[i]);
						break;
					case LedKeyboard::KeyboardModel::g410:
						if (SortedKeys[4].size() <= 120)
							if (keyValues[i].key < LedKeyboard::Key::num_lock || keyValues[i].key > LedKeyboard::Key::num_dot)
								SortedKeys[4].push_back(keyValues[i]);
						break;
					default:
						break;
				}
				break;
		}
	}
	
	for (uint8_t kag = 0; kag < 5; kag++) {
		
		if (SortedKeys[kag].size() > 0) {
			
			uint8_t gi = 0;
			while (gi < SortedKeys[kag].size()) {
				
				size_t data_size = 0;
				byte_buffer_t data = {};
				
				switch (kag) {
					case 0:
						data_size = 20;
						data = getKeyGroupAddress(LedKeyboard::KeyAddressGroup::logo);
						break;
					case 1:
						data_size = 64;
						data = getKeyGroupAddress(LedKeyboard::KeyAddressGroup::indicators);
						break;
					case 2:
						data_size = 64;
						data = getKeyGroupAddress(LedKeyboard::KeyAddressGroup::multimedia);
						break;
					case 3:
						data_size = 64;
						data = getKeyGroupAddress(LedKeyboard::KeyAddressGroup::gkeys);
						break;
					case 4:
						data_size = 64;
						data = getKeyGroupAddress(LedKeyboard::KeyAddressGroup::keys);
						break;
				}
				
				const uint8_t maxKeyCount = (data_size - 8) / 4;
				
				if (data.size() > 0) {
					
					for (uint8_t i = 0; i < maxKeyCount; i++) {
						if (gi + i < SortedKeys[kag].size()) {
							data.push_back(static_cast<uint8_t>(
								static_cast<uint16_t>(SortedKeys[kag][gi+i].key) & 0x00ff));
							data.push_back(SortedKeys[kag][gi+i].color.red);
							data.push_back(SortedKeys[kag][gi+i].color.green);
							data.push_back(SortedKeys[kag][gi+i].color.blue);
						}
					}
					
					data.resize(data_size, 0x00);
					
					if (retval) retval = sendDataInternal(data);
					else sendDataInternal(data);
					
				}
				
				gi = gi + maxKeyCount;
			}
			
		}
	}
	return retval;
}

bool LedKeyboard::setGroupKeys(KeyGroup keyGroup, LedKeyboard::Color color) {
	KeyValueArray keyValues;
	
	KeyArray keyArray;
	
	switch (keyGroup) {
		case KeyGroup::logo:
			keyArray = keyGroupLogo;
			break;
		case KeyGroup::indicators:
			keyArray = keyGroupIndicators;
			break;
		case KeyGroup::gkeys:
			keyArray = keyGroupGKeys;
			break;
		case KeyGroup::multimedia:
			keyArray = keyGroupMultimedia;
			break;
		case KeyGroup::fkeys:
			keyArray = keyGroupFKeys;
			break;
		case KeyGroup::modifiers:
			keyArray = keyGroupModifiers;
			break;
		case KeyGroup::arrows:
			keyArray = keyGroupArrows;
			break;
		case KeyGroup::numeric:
			keyArray = keyGroupNumeric;
			break;
		case KeyGroup::functions:
			keyArray = keyGroupFunctions;
			break;
		case KeyGroup::keys:
			keyArray = keyGroupKeys;
			break;
		default:
			break;
	}
	
	for (uint8_t i = 0; i < keyArray.size(); i++) keyValues.push_back({keyArray[i], color});
	
	return setKeys(keyValues);
}

bool LedKeyboard::setAllKeys(LedKeyboard::Color color) {
	KeyValueArray keyValues;
	for (uint8_t i = 0; i < keyGroupLogo.size(); i++) keyValues.push_back({keyGroupLogo[i], color});
	for (uint8_t i = 0; i < keyGroupIndicators.size(); i++) keyValues.push_back({keyGroupIndicators[i], color});
	for (uint8_t i = 0; i < keyGroupMultimedia.size(); i++) keyValues.push_back({keyGroupMultimedia[i], color});
	for (uint8_t i = 0; i < keyGroupGKeys.size(); i++) keyValues.push_back({keyGroupGKeys[i], color});
	for (uint8_t i = 0; i < keyGroupFKeys.size(); i++) keyValues.push_back({keyGroupFKeys[i], color});
	for (uint8_t i = 0; i < keyGroupFunctions.size(); i++) keyValues.push_back({keyGroupFunctions[i], color});
	for (uint8_t i = 0; i < keyGroupArrows.size(); i++) keyValues.push_back({keyGroupArrows[i], color});
	for (uint8_t i = 0; i < keyGroupNumeric.size(); i++) keyValues.push_back({keyGroupNumeric[i], color});
	for (uint8_t i = 0; i < keyGroupModifiers.size(); i++) keyValues.push_back({keyGroupModifiers[i], color});
	for (uint8_t i = 0; i < keyGroupKeys.size(); i++) keyValues.push_back({keyGroupKeys[i], color});
	return setKeys(keyValues);
}


bool LedKeyboard::setMRKey(uint8_t value) {
	LedKeyboard::byte_buffer_t data;
	switch (m_keyboardModel) {
		case KeyboardModel::g910:
			switch (value) {
				case 0x00:
				case 0x01:
					data = { 0x11, 0xff, 0x0a, 0x0e, value };
					data.resize(20, 0x00);
					return sendDataInternal(data);
				default:
					break;
			}
			break;
		default:
			break;
	}
	return false;
}

bool LedKeyboard::setMNKey(uint8_t value) {
	LedKeyboard::byte_buffer_t data;
	switch (m_keyboardModel) {
		case KeyboardModel::g910:
			switch (value) {
				case 0x00:
				case 0x01:
				case 0x02:
				case 0x03:
				case 0x04:
				case 0x05:
				case 0x06:
				case 0x07:
					data = { 0x11, 0xff, 0x09, 0x1e, value };
					data.resize(20, 0x00);
					return sendDataInternal(data);
				default:
					break;
			}
			break;
		default:
			break;
	}
	return false;
}

bool LedKeyboard::setGKeysMode(uint8_t value) {
	LedKeyboard::byte_buffer_t data;
	switch (m_keyboardModel) {
		case KeyboardModel::g910:
			switch (value) {
				case 0x00:
				case 0x01:
					data = { 0x11, 0xff, 0x08, 0x2e, value };
					data.resize(20, 0x00);
					return sendDataInternal(data);
				default:
					break;
			}
			break;
		default:
			break;
	}
	return false;
}


bool LedKeyboard::setStartupMode(StartupMode startupMode) {
	byte_buffer_t data;
	switch (m_keyboardModel) {
		case KeyboardModel::g410:
		case KeyboardModel::g610:
		case KeyboardModel::g810:
			data = { 0x11, 0xff, 0x0d, 0x5a, 0x00, 0x01 };
			break;
		case KeyboardModel::g910:
			data = { 0x11, 0xff, 0x10, 0x5e, 0x00, 0x01 };
			break;
		default:
			return false;
	}
	data.push_back((unsigned char)startupMode);
	data.resize(20, 0x00);
	return sendDataInternal(data);
}


bool LedKeyboard::setNativeEffect(NativeEffect effect, NativeEffectPart part, uint8_t speed, Color color) {
	uint8_t protocolByte = 0;
	
	switch (m_keyboardModel) {
		case KeyboardModel::g410: // Unconfirmed
		case KeyboardModel::g610: // Unconfirmed
		case KeyboardModel::g810:
			protocolByte = 0x0d;
			break;
		case KeyboardModel::g910:
			protocolByte = 0x10;
			break;
		default:
			return false;
	}
	
	byte_buffer_t data;
	
	switch (effect) {
		
		case NativeEffect::color:
			data = { 0x11, 0xff, protocolByte, 0x3c, (uint8_t)part, 0x01, color.red, color.green, color.blue, 0x02 };
			break;
		case NativeEffect::breathing:
			data = {
				0x11, 0xff, protocolByte, 0x3c, (uint8_t)part, 0x02,
				color.red, color.green, color.blue, speed, 
				0x10, 0x00, 0x64 
			};
			break;
		case NativeEffect::cycle:
			data = {
				0x11, 0xff, protocolByte, 0x3c, (uint8_t)part, 0x03,
				0x00, 0x00, 0x00, 0x00, 0x00, speed, 0x00, 0x00, 0x64
			};
			break;
		case NativeEffect::hwave:
			switch (part) {
				case NativeEffectPart::logo:
					setNativeEffect(NativeEffect::color, part, 0, Color({0x00, 0xff, 0xff}));
					break;
				default:
					data = {
						0x11, 0xff, protocolByte, 0x3c, (uint8_t)part, 0x04,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x01, 0x64, speed
					};
					break;
			}
			break;
		case NativeEffect::vwave:
			switch (part) {
				case NativeEffectPart::logo:
					setNativeEffect(NativeEffect::color, part, 0, Color({0x00, 0xff, 0xff}));
					break;
				default:
					data = {
						0x11, 0xff, protocolByte, 0x3c, (uint8_t)part, 0x04,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x02, 0x64, speed
					};
					break;
			}
			break;
		case NativeEffect::cwave:
			switch (part) {
				case NativeEffectPart::logo:
					setNativeEffect(NativeEffect::color, part, 0, Color({0x00, 0xff, 0xff}));
					break;
				default:
					data = {
						0x11, 0xff, protocolByte, 0x3c, (uint8_t)part, 0x04,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x03, 0x64, speed
					};
					break;
			}
			break;
		
		default:
			return false;
	}
	
	data.resize(20, 0x00);
	return sendDataInternal(data);
}


bool LedKeyboard::sendDataInternal(byte_buffer_t &data) {
	if (! m_isOpen) return false;
	
	if (data.size() > 0) {
		#if defined(hidapi)
			data.insert(data.begin(), 0x00);
			if (hid_write(m_hidHandle, const_cast<unsigned char*>(data.data()), data.size()) < 0) {
				std::cout<<"Error: Can not write to hidraw, try with the libusb version"<<std::endl;
				return false;
			}
			byte_buffer_t data2;
			data2.resize(21, 0x00);
			hid_read_timeout(m_hidHandle, const_cast<unsigned char*>(data2.data()), data2.size(), 1);
			return true;
		#elif defined(libusb)
			if (data.size() > 20) {
				if(libusb_control_transfer(m_hidHandle, 0x21, 0x09, 0x0212, 1, 
						const_cast<unsigned char*>(data.data()), data.size(), 2000) < 0)
					return false;
			} else {
				if(libusb_control_transfer(m_hidHandle, 0x21, 0x09, 0x0211, 1, 
						const_cast<unsigned char*>(data.data()), data.size(), 2000) < 0)
					return false;
			}
			usleep(1000);
			unsigned char buffer[64];
			int len = 0;
			libusb_interrupt_transfer(m_hidHandle, 0x82, buffer, sizeof(buffer), &len, 1);
			return true;
		#endif
	}
	
	return false;
}

LedKeyboard::byte_buffer_t LedKeyboard::getKeyGroupAddress(LedKeyboard::KeyAddressGroup keyAddressGroup) {
	switch (m_keyboardModel) {
		case KeyboardModel::g410:
		case KeyboardModel::g610:
		case KeyboardModel::g810:
			switch (keyAddressGroup) {
				case LedKeyboard::KeyAddressGroup::logo:
					return { 0x11, 0xff, 0x0c, 0x3a, 0x00, 0x10, 0x00, 0x01 };
				case LedKeyboard::KeyAddressGroup::indicators:
					return { 0x12, 0xff, 0x0c, 0x3a, 0x00, 0x40, 0x00, 0x05 };
				case LedKeyboard::KeyAddressGroup::gkeys:
					return {};
				case LedKeyboard::KeyAddressGroup::multimedia:
					return { 0x12, 0xff, 0x0c, 0x3a, 0x00, 0x02, 0x00, 0x05 };
				case LedKeyboard::KeyAddressGroup::keys:
					return { 0x12, 0xff, 0x0c, 0x3a, 0x00, 0x01, 0x00, 0x0e };
			}
			break;
		case KeyboardModel::g910:
			switch (keyAddressGroup) {
				case LedKeyboard::KeyAddressGroup::logo:
					return { 0x11, 0xff, 0x0f, 0x3a, 0x00, 0x10, 0x00, 0x02 };
				case LedKeyboard::KeyAddressGroup::indicators:
					return { 0x12, 0xff, 0x0c, 0x3a, 0x00, 0x40, 0x00, 0x05 };
				case LedKeyboard::KeyAddressGroup::gkeys:
					return { 0x12, 0xff, 0x0f, 0x3e, 0x00, 0x04, 0x00, 0x09 };
				case LedKeyboard::KeyAddressGroup::multimedia:
					return {};
				case LedKeyboard::KeyAddressGroup::keys:
					return { 0x12, 0xff, 0x0f, 0x3d, 0x00, 0x01, 0x00, 0x0e };
			}
			break;
		default:
			break;
	}
	return {};
}
