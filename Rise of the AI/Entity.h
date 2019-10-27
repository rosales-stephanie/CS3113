#pragma once
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

enum  EntityType { PLAYER, PLATFORM, COIN, ENEMY };

enum AIState { IDLE, WALKING };

class Entity {
public:
    
    EntityType entityType;
    enum AIState aiState;
    enum AIType {WALKER};
    bool lost;
    bool isStatic;
    bool isActive;
    AIType aiType;
    
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    void AIWalker(Entity player);
    float width;
    float height;
    
    float speed;
    
    GLuint textureID;
    
    Entity();
    
    bool CheckCollision(Entity other);
    
    void CheckCollisionsX(Entity *objects, int objectCount);
    void CheckCollisionsY(Entity *objects, int objectCount);
    void AI(Entity player);
    void Update(float deltaTime, Entity player, Entity *objects, int objectCount, Entity *enemies, int enemyCount);
    void Render(ShaderProgram *program);
    
    void Jump();
    
    bool collidedTop;
    bool collidedBottom;
    bool collidedLeft;
    bool collidedRight;
    
};



