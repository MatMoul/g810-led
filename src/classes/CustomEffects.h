#ifndef __CUSTOM_EFFECTS__
#define __CUSTOM_EFFECTS__

#include <iostream>

class LedKeyboard;

// Start the custom effects and wait for user input before quitting.
// Assume the given arg are ONLY refering to the effects (so the program name and arguments are ommited).
int StartCustomEffects( LedKeyboard& kbd, int argc, char** argv );

#endif // __CUSTOM_EFFECTS__
