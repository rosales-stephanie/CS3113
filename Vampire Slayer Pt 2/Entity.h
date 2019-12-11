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
#include "Mesh.h"
enum EntityType { NONE, FLOOR, BOX, ENEMY };
class Entity {
public:
    EntityType entityType;
    bool billboard;
    float width;
    float height;
    float depth;

	bool isClose(Entity* other);
    bool CheckCollision(Entity *other);
    bool killEnemy;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 rotation;
    glm::vec3 scale;
    
    Mesh* mesh;
    
    float *vertices;
    float *texCoords;
    int numVertices;
    
    GLuint textureID;
    
    Entity();
    void Update(float deltaTime, Entity *player, Entity *objects, int objectCount);
    //void Update(float deltaTime);
    void Render(ShaderProgram *program);
    void DrawBillboard(ShaderProgram *program);
};



