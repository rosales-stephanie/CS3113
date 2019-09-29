#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix, projectionMatrix2;

float lastTicks = 0;

Entity player(glm::vec3(-4.5, 0, 0));
Entity player2(glm::vec3(4.5, 0, 0));
glm::vec3 ball = glm::vec3(0, 0, 0);

glm::vec3 ball_movement = glm::vec3(1,0,0);

GLuint ballTextureId;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    return textureID;
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    player.textureID = LoadTexture("paddle-board.png");
    player2.textureID = LoadTexture("paddle-board.png");
    ballTextureId = LoadTexture("square-pixilart.png");
    
    player.speed = 2;
    player2.speed = 2;
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void collisionDetection(const Entity& paddle, glm::vec3 ball)  {
    float x2 = paddle.getX();
    float y2 = paddle.getY();
    float x1 = ball.x;
    float y1 = ball.y;
    float xdist = fabs(x2 - x1);
    float ydist = fabs(y2 - y1);
    if (xdist < 0 && ydist < 0){
        if (paddle.getY() >= 2.5){
            ball.y = -1;
            if (paddle.getX() <= -4.5){
                ball.x = 1; }
            else {
                ball.x = -1; }
        }
        else {
            ball.y = 1;
            if (paddle.getX() <= -4.5){
                ball.x = 1;
            }
            else {
                ball.x = -1; }
        }
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
        }
    }
    // Check for pressed/held keys below
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_W])
    {
        player.changeMovementY(1);
    }
    else if  (keys[SDL_SCANCODE_S])
    {
        player.changeMovementY(-1);
    }
    else if (keys[SDL_SCANCODE_UP])
    {
        player2.changeMovementY(1);
    }
    else if (keys[SDL_SCANCODE_DOWN])
    {
        player2.changeMovementY(-1);
    }
}

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    player.Update(deltaTime);
    player2.Update(deltaTime);
    
    float ball_speed = 2;
    ball += ball_movement * ball_speed * deltaTime;
    

    if (ball.y >= 3.75){
        ball_movement.y = -1;
        ball_movement.x = -1;
    }
    if (ball.y <= -3.75){
        ball_movement.y = 1;
        ball_movement.x = 1;
    }
    if (ball.x >= 5.0){
        ball_movement.y = 1;
        ball_movement.x = -1;
    }
    if (ball.x <= -5.0){
        ball_movement.y = -1;
        ball_movement.x = 1;
    }

}

void DrawTexturedPlayerX (){
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, ball);
    program.SetModelMatrix(modelMatrix);
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, ballTextureId);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    player.Render(&program);
    player2.Render(&program);
    
    DrawTexturedPlayerX();
    
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
