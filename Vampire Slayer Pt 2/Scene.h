#pragma once
#define GL_SILENCE_DEPRECATION
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Util.h"
#include "Entity.h"
#include "SDL_mixer.h"
//#include "Map.h"

#define OBJECT_COUNT 544
#define ENEMY_COUNT 1

struct GameState {
	float startTime = 0;
	float time = 0;
	int currLevel;
	int nextLevel;
	Entity player;
	Entity objects[OBJECT_COUNT];
	Entity enemies[ENEMY_COUNT];
	Mix_Chunk* win_sound = Mix_LoadWAV("win.wav");
	Mix_Chunk* lose_sound = Mix_LoadWAV("lose.wav");
};

class Scene {
public:
	GameState state;
	virtual void Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(ShaderProgram* program) = 0;
};