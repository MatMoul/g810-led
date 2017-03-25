#include "CustomEffects.h"

#include "Keyboard.h"

#include <atomic>
#include <chrono>
#include <thread>

// Global variables to know when a user activate input
std::atomic< bool > KeyPressed;

void Test1Effects( LedKeyboard& kbd )
{
	LedKeyboard::Key key = LedKeyboard::Key::r;
	LedKeyboard::Color color;
	color.red = 0;
	color.green = 0;
	color.blue = 0;
	
	while ( !KeyPressed )
	{
		++color.red;
		LedKeyboard::KeyValue keyValue = { key, color };
		if (! kbd.open()) return;
		if (! kbd.setKey(keyValue)) return;
		if(! kbd.commit()) return;
		
		//std::cout << "TEST" << std::endl;
		std::this_thread::sleep_for ( std::chrono::milliseconds( 10 ) );
	}
}

int StartEffectsAndWaitForUser( LedKeyboard& kbd )
{
	// Start the effects on a thread
	std::thread lThread( Test1Effects, std::ref( kbd ) );
	
	// Wait for user input
	std::cout << "Press enter to quit";
	getchar();
	
	// Stop the thread and wait for it to finish
	KeyPressed = true;
	lThread.join();
	
	return 0;
}


int StartCustomEffects( LedKeyboard& kbd, int argc, char** argv )
{
	if ( argc < 1 )
	{
		// Not enough parameters
		return 1;
	}
	
	KeyPressed = false;
	std::string Type = argv[0];
	if ( Type == "test1" )
	{
		return StartEffectsAndWaitForUser( kbd );
	}
	else
	{
		// No custom effects of this name
		std::cout << "No effects of name: " << Type << std::endl;
	}
	
	// First param is the custom effects to create. Convert it to an enum
	
	/*for ( int i = 0; i < argc; ++i )
	{
		std::cout << argv[i] << std::endl;
	}*/
	return 0;
}
