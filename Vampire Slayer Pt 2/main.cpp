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

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;
#define OBJECT_COUNT 43
#define ENEMY_COUNT 20

struct GameState {
    Entity player;
    Entity objects[OBJECT_COUNT];
    Entity enemies[ENEMY_COUNT];
};

GameState state;
float cubeVertices[] = {
   -0.5,  0.5, -0.5, -0.5,  0.5,  0.5,  0.5,  0.5,  0.5,
    -0.5,  0.5, -0.5,  0.5,  0.5,  0.5,  0.5,  0.5, -0.5,
     0.5, -0.5, -0.5,  0.5, -0.5,  0.5, -0.5, -0.5,  0.5,
     0.5, -0.5, -0.5, -0.5, -0.5,  0.5, -0.5, -0.5, -0.5,
    -0.5,  0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5,  0.5,
    -0.5,  0.5, -0.5, -0.5, -0.5,  0.5, -0.5,  0.5,  0.5,
     0.5,  0.5,  0.5,  0.5, -0.5,  0.5,  0.5, -0.5, -0.5,
     0.5,  0.5,  0.5,  0.5, -0.5, -0.5,  0.5,  0.5, -0.5,
    -0.5,  0.5,  0.5, -0.5, -0.5,  0.5,  0.5, -0.5,  0.5,
    -0.5,  0.5,  0.5,  0.5, -0.5,  0.5,  0.5,  0.5,  0.5,
     0.5,  0.5, -0.5,  0.5, -0.5, -0.5, -0.5, -0.5, -0.5,
     0.5,  0.5, -0.5, -0.5, -0.5, -0.5, -0.5,  0.5, -0.5
};

float cubeTexCoords[] = {
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f
};

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("3D!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 720);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
   //change projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    projectionMatrix = glm::perspective(45.0f, 1.777f, 0.1f, 100.0f);
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    state.player.position =  glm::vec3(9,1,0);
    state.player.acceleration = glm::vec3(0,0,0);
    //GLuint objectTexture = Util::LoadTexture("crate1_diffuse.png");
    GLuint objectTextureID = Util::LoadTexture("pikachu.png");
    Mesh *pikaMesh = new Mesh();
    pikaMesh->LoadOBJ("pikachu.obj");
    
    GLuint shipTextureID = Util::LoadTexture("ship.png");
    Mesh *shipMesh = new Mesh();
    pikaMesh->LoadOBJ("ship.obj");
    
    GLuint floorTextureID = Util::LoadTexture("100_1180_seamless.JPG");
    Mesh *floorMesh = new Mesh();
    floorMesh->LoadOBJ("cube.obj");
    
    GLuint crateTextureID = Util::LoadTexture("crate1_diffuse.png");
    Mesh *crateMesh = new Mesh();
    crateMesh->LoadOBJ("cube.obj");
    
//    state.objects[0].position = glm::vec3(-2,0,-100);
//    state.objects[0].acceleration = glm::vec3(0,0,1);
//    state.objects[0].vertices = cubeVertices;
//    state.objects[0].velocity = glm::vec3(0,0,10); //constant speed towards you
//    state.objects[0].texCoords = cubeTexCoords;
//    state.objects[0].numVertices = 36;
//    state.objects[0].textureID = objectTexture;
//
    
    state.objects[0].position = glm::vec3(0,0,0);
    state.objects[0].scale = glm::vec3(20,1,20);
    state.objects[0].rotation = glm::vec3(0,0,0);
    state.objects[0].acceleration = glm::vec3(0,0,0);
    //state.objects[0].vertices = cubeVertices;
    state.objects[0].velocity = glm::vec3(0,0,10); //constant speed towards you
    state.objects[0].textureID = floorTextureID;
    state.objects[0].mesh = floorMesh;
    int count = 1;
    for (int i = 0; i < 11; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(i,j,-5);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
    }
    int depth = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(7,j,depth);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
        depth -= 1;
    }
 /*   state.objects[1].position = glm::vec3(0,1,-5); // z equals far away ness
    state.objects[1].textureID = crateTextureID;
    state.objects[1].mesh = crateMesh;
    state.objects[1].entityType = BOX;
    
    state.objects[2].position = glm::vec3(1,1,-5); // z equals far away ness
    state.objects[2].textureID = crateTextureID;
    state.objects[2].mesh = crateMesh;
    state.objects[2].entityType = BOX;
    
    state.objects[3].position = glm::vec3(1,2,-5); // z equals far away ness
    state.objects[3].textureID = crateTextureID;
    state.objects[3].mesh = crateMesh;
    state.objects[3].entityType = BOX;
    */
    GLuint enemyTextureID = Util::LoadTexture("me.png");
    for (int i = 0; i < ENEMY_COUNT; i++) {
         state.enemies[i].billboard = true;
         state.enemies[i].textureID = enemyTextureID;
         state.enemies[i].position = glm::vec3(rand() % 20 - 10, 1, rand() % 20 - 10);
         state.enemies[i].rotation = glm::vec3(0, 0, 0);
         state.enemies[i].acceleration = glm::vec3(0, 0, 0);
    }
//    state.objects[1].scale = glm::vec3(1,1,1);
//    state.objects[1].rotation = glm::vec3(0,0,0);
//    state.objects[1].acceleration = glm::vec3(0,0,0);
//    //state.objects[0].vertices = cubeVertices;
//    state.objects[1].velocity = glm::vec3(0,0,0); //constant speed towards you
//    state.objects[1].textureID = crateTextureID;
//    state.objects[1].mesh = crateMesh;
    
//    state.objects[1].position = glm::vec3(2,0,-100); // z equals far away ness
//    state.objects[1].acceleration = glm::vec3(0,0,1); //z = falling towards you
//    state.objects[1].vertices = cubeVertices;
//    state.objects[1].velocity = glm::vec3(0,0,10); //constant speed towards you
//    state.objects[1].texCoords = cubeTexCoords;
//    state.objects[1].numVertices = 36;
//    state.objects[1].textureID = objectTexture;
    
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
                        // Some sort of action
                        break;
                        
                }
                break;
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_A]){
        state.player.rotation.y += 1.0f;
    }
    else if(keys[SDL_SCANCODE_D]){
        state.player.rotation.y -= 1.0f;
    }
    state.player.velocity.x = 0;
    state.player.velocity.z = 0;
    
    if(keys[SDL_SCANCODE_W]){
        state.player.velocity.z = cos(glm::radians(state.player.rotation.y)) * -2.0f;
        state.player.velocity.x = sin(glm::radians(state.player.rotation.y)) * -2.0f;
    }
    else if (keys[SDL_SCANCODE_S]){
        state.player.velocity.z = cos(glm::radians(state.player.rotation.y)) * 2.0f;
        state.player.velocity.x = sin(glm::radians(state.player.rotation.y)) * 2.0f;
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP) {
        //state.player.Update(FIXED_TIMESTEP);
        state.player.Update(FIXED_TIMESTEP, &state.player, state.objects, OBJECT_COUNT);
        for (int i = 0; i < ENEMY_COUNT; i ++){
            state.objects[i].Update(FIXED_TIMESTEP, &state.player, state.objects, OBJECT_COUNT);
        }
        
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0);
    viewMatrix = glm::translate(viewMatrix, -state.player.position);
    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::rotate(viewMatrix, glm::radians(state.player.rotation.y), glm::vec3(0,-1.0f,0));
    viewMatrix = glm::translate(viewMatrix, -state.player.position);

}


void Render() {
    //glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);
    //state.player.Render(&program);
    for (int i = 0; i < OBJECT_COUNT; i++) {
        state.objects[i].Render(&program);
    }
   /*for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }*/
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
