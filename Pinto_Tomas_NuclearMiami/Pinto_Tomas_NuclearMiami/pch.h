#ifndef PCH_H
#define PCH_H

// SDL libs
#pragma comment(lib, "sdl2.lib")
#pragma comment(lib, "SDL2main.lib")

// OpenGL libs
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"Glu32.lib")

// SDL extension libs 
#pragma comment(lib, "SDL2_image.lib")  
#pragma comment(lib, "SDL2_ttf.lib") 
#pragma comment(lib, "SDL2_mixer.lib")  

// SDL and OpenGL Includes
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <SDL_image.h>
#include <SDL_ttf.h> 
#include <SDL_mixer.h> 
#include <gl/GLU.h>

#include <iostream>
#include <algorithm>
#include <assert.h>

#include "structs.h"
#include "utils.h"

#include "Game.h"
#include "Core.h"

#include "SoundEffect.h"
#include "SoundStream.h"
#include "Texture.h"
#include "Sprite.h"
#include "TextRenderer.h"

#include "ResourceManager.h"
#include "DebugLogger.h"

#include "Scene.h"
#include "GameObject.h"

#if 0
#define GAME_RELEASE
#endif

#if 0
#define DEBUG_DRAW
#endif

#endif //PCH_H
