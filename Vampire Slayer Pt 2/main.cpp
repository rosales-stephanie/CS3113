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
#define OBJECT_COUNT 544
#define ENEMY_COUNT 1

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
    
    GLuint floorTextureID = Util::LoadTexture("100_1180_seamless.JPG");
    Mesh *floorMesh = new Mesh();
    floorMesh->LoadOBJ("cube.obj");
    
    GLuint crateTextureID = Util::LoadTexture("crate1_diffuse.png");
    Mesh *crateMesh = new Mesh();
    crateMesh->LoadOBJ("cube.obj");
    
    state.objects[0].position = glm::vec3(0,0,0);
    state.objects[0].scale = glm::vec3(20,1,20);
    state.objects[0].rotation = glm::vec3(0,0,0);
    state.objects[0].acceleration = glm::vec3(0,0,0);
    state.objects[0].velocity = glm::vec3(0,0,10); //constant speed towards you
    state.objects[0].textureID = floorTextureID;
    state.objects[0].mesh = floorMesh;
    //wall 1
    int count = 1;
    for (int i = 8; i > 4; i--) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(i,j,-3);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
    }
    //wall 2
    int depth = 1;
    for (int i = 0; i < 5; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(8,j,depth);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
        depth -= 1;
    }
    //wall 4
    for (int i = 8; i < 11; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(i,j,2);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
    }
    // wall 3
    depth = -1;
    for (int i = 0; i < 10; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(10.5,j,depth);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
        depth -= 1;
    }
    //wall 5
    for (int i = 10; i > 6; i--) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(i,j,-6);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
    }
    //wall 6
    for (int i = 11; i > -11; i--) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(i,j,-10.5);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
    }
    //wall 7
    depth = 0;
    for (int i = 0; i < 11; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(-10.5,j,depth);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
        depth -= 1;
    }
    //wall 8
    depth = 11;
    for (int i = 0; i < 7; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(-10.5,j,depth);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
        depth -= 1;
    }
    //wall 9
    depth = 2;
    for (int i = 0; i < 9; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(10,j,depth);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
        depth += 1;
    }
    //wall 10
    for (int i = -11; i < 11; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(i,j,10.5);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
    }
    //wall 11
    for (int i = 9; i > 6; i--) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(i,j,7);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
    }
    //wall 12
    for (int i = 4; i > 0; i--) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(i,j,5);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
    }
    //wall 13
    depth = 10.5;
    for (int i = 0; i < 3; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(4,j,depth);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
        depth -= 1;
    }
    //wall 14
    depth = 5;
    for (int i = 0; i < 3; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(3,j,depth);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
        depth -= 1;
    }
    //wall 15
    depth = -3;
    for (int i = 0; i < 3; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(4,j,depth);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
        depth -= 1;
    }
    //wall 16
    for (int i = 4; i > 0; i--) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(i,j,0);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
    }
    //wall 17
    depth = -1;
    for (int i = 0; i < 10; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(1,j,depth);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
        depth -= 1;
    }
    //wall 18
    depth = 10.5;
    for (int i = 0; i < 6; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(1,j,depth);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
        depth -= 1;
    }
    //wall 19
    for (int i = -10; i < -3; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(i,j,4);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
    }
    //wall 20
    depth = 10.5;
    for (int i = 0; i < 4; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(-7,j,depth);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
        depth -= 1;
    }
    //wall 21
    depth = -4;
    for (int i = 0; i < 7; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(-6,j,depth);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
        depth -= 1;
    }
    //wall 22
    depth = -4;
    for (int i = 0; i < 4; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(-3,j,depth);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
        depth -= 1;
    }
    //wall 23
    for (int i = -2; i < 1; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(i,j,-4);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
    }
    //wall 24
    for (int i = -8; i < -3; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(i,j,-1);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
    }
    //wall 25
    depth = 6;
    for (int i = 0; i < 7; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(-4,j,depth);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
        depth -= 1;
    }
    //wall 26
    for (int i = -4; i < -1; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(i,j,6);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
    }
    //wall 27
    depth = 6;
    for (int i = 0; i < 5; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(-2,j,depth);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
        depth -= 1;
    }
    //wall 28
    depth = -8;
    for (int i = 0; i < 3; i++) {
        for (int j = 1; j < 4; j++){
            state.objects[count].position = glm::vec3(4,j,depth);
            state.objects[count].textureID = crateTextureID;
            state.objects[count].mesh = crateMesh;
            state.objects[count].entityType = BOX;
            count += 1;
        }
        depth -= 1;
    }

    GLuint enemyTextureID = Util::LoadTexture("me.png");
    for (int i = 0; i < ENEMY_COUNT; i++) {
         state.enemies[i].billboard = true;
         state.enemies[i].textureID = enemyTextureID;
         state.enemies[i].position = glm::vec3(9, 1, -4);
         state.enemies[i].rotation = glm::vec3(0, 0, 0);
         state.enemies[i].acceleration = glm::vec3(0, 0, 0);
    }
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
    if (keys[SDL_SCANCODE_LEFT]){
        state.player.rotation.y += 1.0f;
    }
    else if(keys[SDL_SCANCODE_RIGHT]){
        state.player.rotation.y -= 1.0f;
    }
    state.player.velocity.x = 0;
    state.player.velocity.z = 0;
    
    if(keys[SDL_SCANCODE_UP]){
        state.player.velocity.z = cos(glm::radians(state.player.rotation.y)) * -2.0f;
        state.player.velocity.x = sin(glm::radians(state.player.rotation.y)) * -2.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]){
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
        state.player.Update(FIXED_TIMESTEP, &state.player, state.objects, OBJECT_COUNT);
         for (int i = 0; i < ENEMY_COUNT; i ++){
            state.enemies[i].Update(FIXED_TIMESTEP, &state.player, state.objects, OBJECT_COUNT);
        }
        
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0);
    viewMatrix = glm::rotate(viewMatrix, glm::radians(state.player.rotation.y), glm::vec3(0,-1.0f,0));
    viewMatrix = glm::translate(viewMatrix, -state.player.position);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);
    for (int i = 0; i < OBJECT_COUNT; i++) {
        state.objects[i].Render(&program);
    }
   for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }
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
