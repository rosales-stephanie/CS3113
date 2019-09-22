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

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program, texturedProgram;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

float textured_player_x = 0.0f;
float player_x_rotate = 0.0f;

float textured_player_y = 0.0f;
float player_scale = 1.0f;

float ai_x = 0.0f;
float ai_rotate = 0.0f;

GLuint playerXTextureID;
GLuint playerYTextureID;

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
    displayWindow = SDL_CreateWindow("Triangle!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL); //x & y
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480); // Draw on whole window
    
    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
    
    texturedProgram.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    playerXTextureID = LoadTexture("bird-pixilart.png");
    playerYTextureID = LoadTexture("pawprint-pixilart.png");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 0.0f, 0.0f, 1.0f); // make triangle red
    
    texturedProgram.SetProjectionMatrix(projectionMatrix);
    texturedProgram.SetViewMatrix(viewMatrix);
    texturedProgram.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(texturedProgram.programID); // use shader
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // start w/ this background color
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

void Update() {
    ai_x -= 0.01f;
    ai_rotate += 1.0f;
    
    textured_player_x += 0.01f;
    player_x_rotate -= 1.0f;
    
    textured_player_y += 0.01f;
    player_scale -= 0.01f;

}

void DrawAI (){
    float vertices[] = { 0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5f };
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(ai_x, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(ai_rotate), glm::vec3(0.0f, 0.0f,1.0f));
    
    program.SetModelMatrix(modelMatrix);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 3); // draw triangles
    glDisableVertexAttribArray(program.positionAttribute);
}

void DrawTexturedPlayerX (){
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(textured_player_x, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(player_x_rotate), glm::vec3(0.0f, 0.0f,1.0f));
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, playerXTextureID);
    
    texturedProgram.SetModelMatrix(modelMatrix);
    
    glVertexAttribPointer(texturedProgram.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(texturedProgram.positionAttribute);
    
    glVertexAttribPointer(texturedProgram.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(texturedProgram.texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(texturedProgram.positionAttribute);
    glDisableVertexAttribArray(texturedProgram.texCoordAttribute);
}

void DrawTexturedPlayerY (){
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(textured_player_y, -1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(player_scale, 1.0f, 0.0f));

    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, playerYTextureID);
    
    texturedProgram.SetModelMatrix(modelMatrix);
    
    glVertexAttribPointer(texturedProgram.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(texturedProgram.positionAttribute);
    
    glVertexAttribPointer(texturedProgram.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(texturedProgram.texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(texturedProgram.positionAttribute);
    glDisableVertexAttribArray(texturedProgram.texCoordAttribute);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT); // clear
    
    DrawAI();
    DrawTexturedPlayerX();
    DrawTexturedPlayerY();
    
    SDL_GL_SwapWindow(displayWindow); //put on screen
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

