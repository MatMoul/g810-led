#include "CustomEffects.h"

#include "Keyboard.h"

#include <atomic>
#include <chrono>
#include <functional>
#include <thread>
#include <vector>

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

void Test2Effects( LedKeyboard& kbd )
{
	LedKeyboard::Key key = LedKeyboard::Key::t;
	LedKeyboard::Color color;
	color.red = 0;
	color.green = 0;
	color.blue = 0;
	
	while ( !KeyPressed )
	{
		++color.blue;
		LedKeyboard::KeyValue keyValue = { key, color };
		if (! kbd.open()) return;
		if (! kbd.setKey(keyValue)) return;
		if(! kbd.commit()) return;
		
		//std::cout << "TEST" << std::endl;
		std::this_thread::sleep_for ( std::chrono::milliseconds( 10 ) );
	}
}

///////////////////////////////////////////////////////////////////////////////////

int StartEffectsAndWaitForUser( LedKeyboard& kbd, std::vector< std::function< void( LedKeyboard& ) > >& EffectFunctions )
{
	if ( EffectFunctions.size() == 0 )
	{
		std::cout << "No effects to apply." << std::endl;
		return 1;
	}
	
	// Start all the effects on a thread
	std::vector< std::thread > ThreadList;
	for( unsigned int i = 0; i < EffectFunctions.size(); ++i )
	{
		ThreadList.emplace_back( EffectFunctions[i], std::ref( kbd ) );
	}
	
	// Wait for user input
	std::cout << "Press enter to quit";
	getchar();
	
	// Stop the threads and wait for them to finish
	KeyPressed = true;
	for( unsigned int i = 0; i < ThreadList.size(); ++i )
	{
		ThreadList[i].join();
	}
	
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
	
	// Each arguments is an effects. Check all the arguments to get the list of all wanted effects
	std::vector< std::function< void( LedKeyboard& ) > > EffectFunctions;
	for ( int i = 0; i < argc; ++i )
	{
		std::string Type = argv[i];
		if ( Type == "test1" ) EffectFunctions.emplace_back( Test1Effects );
		else if ( Type == "test2" ) EffectFunctions.emplace_back( Test2Effects );
		else
		{
			// No custom effects of this name
			std::cout << "No effects of name: " << Type << std::endl;
		}
	}
	
	return StartEffectsAndWaitForUser( kbd, EffectFunctions );
}
