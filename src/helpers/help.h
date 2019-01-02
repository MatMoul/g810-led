#ifndef HELP_HELPER
#define HELP_HELPER

#include <iostream>

namespace help {
	
	enum class KeyboardFeatures : uint16_t {
		none = 0,
		rgb = 1,
		intensity = 2,
		commit = 4,
		logo1 = 8,
		logo2 = 16,
		numpad = 32,
		multimedia = 64,
		gkeys = 128,
		setall = 256,
		setgroup = 512,
		setkey = 1024,
		setregion = 2048,
		setindicators = 4096,
		poweronfx = 8192,
		// "user-stored lighting" can be recalled with backlight+7 on the G Pro
		userstoredlighting = 16384,
		// fx features
		
		all = rgb | intensity | commit | logo1 | logo2 | numpad | multimedia | gkeys |
		      setall | setgroup | setkey | setregion | setindicators | poweronfx | userstoredlighting,
		
		g213 = rgb | logo1 | numpad | multimedia | setall | setregion | setindicators | poweronfx | userstoredlighting,
		g410 = rgb | commit | setall | setgroup | setkey | poweronfx | userstoredlighting,
		g413 = intensity | setall | userstoredlighting,
		g513 = rgb | commit | numpad | setall | setgroup | setkey | setindicators | poweronfx | userstoredlighting,
		g610 = intensity | commit | logo1 | numpad | multimedia | setall | setgroup | setkey | setindicators | poweronfx | userstoredlighting,
		g810 = rgb | commit | logo1 | numpad | multimedia | setall | setgroup | setkey | setindicators | poweronfx,
		g910 = rgb | commit | logo1 | logo2 | numpad | multimedia | gkeys | setall | setgroup | setkey | setindicators | poweronfx | userstoredlighting,
		gpro = rgb | commit | logo1 | setall | setgroup | setkey | setindicators | poweronfx | userstoredlighting
	};
	inline KeyboardFeatures operator|(KeyboardFeatures a, KeyboardFeatures b);
	
	KeyboardFeatures getKeyboardFeatures(std::string cmdName);
	
	void usage(char *arg0);
	void keys(char *arg0);
	void effects(char *arg0);
	void samples(char *arg0);
	
}

#ifndef VERSION
#define VERSION "unspecified"
#endif

#endif
