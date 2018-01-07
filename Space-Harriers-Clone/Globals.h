#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "MemLeaks.h"
#include "SDL/include/SDL_rect.h"

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum class update_status
{
	UPDATE_CONTINUE,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Useful typedefs ---------
typedef unsigned int uint;

// Deletes a buffer
template<typename T>
inline void RELEASE(T*& x) {
	if (x != nullptr)
	{
		delete x;
		x = nullptr;
	}
}

// Deletes an array of buffers
template<typename T>
inline void RELEASE_ARRAY(T*& x) {
	if( x != nullptr ) {
		delete[] x;
		x = nullptr;
	}
}

// Configuration -----------
#define SCREEN_WIDTH 1152
#define SCREEN_HEIGHT 720
#define FULLSCREEN false
#define VSYNC true
#define TITLE "Space Harriers Clone"

#endif //__GLOBALS_H__