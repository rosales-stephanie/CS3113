#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"

#include "Scene.h"
#include "MainMenu.h"
#include "Level1.h"
#include "Win.h"
#include "GameOver.h"
#include "SDL_mixer.h"



SDL_Window* displayWindow;
bool gameIsRunning = true;

//program is 3d, program2 is 2d
ShaderProgram program, program2;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

glm::mat4 uiViewMatrix, uiProjectionMatrix;;

GLuint fontTextureID;

Scene* currentScene;
Scene* sceneList[4];

//int curr_level = 0;
Mix_Music* music;


void SwitchToScene(Scene* scene) {
	currentScene = scene;
	currentScene->Initialize();
}

//GameState state;


void Initialize() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	displayWindow = SDL_CreateWindow("3D!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif
	//audio stuff
	int Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	music = Mix_LoadMUS("organ_music.mp3");
	//currentScene->state.win_sound = Mix_LoadWAV("win.wav");
	//currentScene->state.lose_sound = Mix_LoadWAV("lose.wav");

	Mix_PlayMusic(music, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 4);


	glViewport(0, 0, 1280, 720);

	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    program2.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

	fontTextureID = Util::LoadTexture("font1.png");

	sceneList[0] = new MainMenu();
	sceneList[1] = new Level1();
	sceneList[2] = new Win();
	sceneList[3] = new GameOver();
        
	glUseProgram(program.programID);
    glUseProgram(program2.programID);

	uiViewMatrix = glm::mat4(1.0f);
	viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);


	projectionMatrix = glm::perspective(45.0f, 1.777f, 0.1f, 100.0f);
	uiProjectionMatrix = glm::ortho(-6.4f, 6.4f, -3.6f, 3.6f, -1.0f, 1.0f);
	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	program2.SetProjectionMatrix(uiProjectionMatrix);
	program2.SetViewMatrix(viewMatrix);
	program2.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
  

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	
	
	SwitchToScene(sceneList[0]);
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        currentScene->state.player.killEnemy = true;
                        break;
                }
                break;
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_LEFT]){
        currentScene->state.player.rotation.y += 1.0f;
    }
    else if(keys[SDL_SCANCODE_RIGHT]){
		currentScene->state.player.rotation.y -= 1.0f;
    }
	currentScene->state.player.velocity.x = 0;
	currentScene->state.player.velocity.z = 0;
    
    if(keys[SDL_SCANCODE_UP]){
		currentScene->state.player.velocity.z = cos(glm::radians(currentScene->state.player.rotation.y)) * -2.0f;
		currentScene->state.player.velocity.x = sin(glm::radians(currentScene->state.player.rotation.y)) * -2.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]){
		currentScene->state.player.velocity.z = cos(glm::radians(currentScene->state.player.rotation.y)) * 2.0f;
		currentScene->state.player.velocity.x = sin(glm::radians(currentScene->state.player.rotation.y)) * 2.0f;
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;

	currentScene->state.time = ticks;

    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP) {
		currentScene->Update(FIXED_TIMESTEP);
          
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0);
    viewMatrix = glm::rotate(viewMatrix, glm::radians(currentScene->state.player.rotation.y), glm::vec3(0,-1.0f,0));
    viewMatrix = glm::translate(viewMatrix, -(currentScene->state.player.position));
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);
    program2.SetViewMatrix(viewMatrix);

   /*
	for (int i = 0; i < OBJECT_COUNT; i++) {
        state.objects[i].Render(&program);
    }
   for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }
	*/

	//supposed to be in 3d
	if (sceneList[1]->state.currLevel == Level1().state.currLevel) {
	    currentScene->Render(&program2);
    }
    else {
	    currentScene->Render(&program);
    }
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
	Mix_FreeMusic(music);
	Mix_FreeChunk(currentScene->state.lose_sound);
	Mix_FreeChunk(currentScene->state.win_sound);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
		if (currentScene->state.nextLevel >= 0) {
			SwitchToScene(sceneList[currentScene->state.nextLevel]);
		}
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
