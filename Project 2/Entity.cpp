#include "Entity.h"

Entity::Entity(glm::vec3 vec)
{
    player_position = vec;
    player_movement = glm::vec3(0, 0, 0);
    speed = 1;
}

void Entity::Update(float deltaTime)
{
    player_position += player_movement * speed * deltaTime;
    if (player_position.y >= 3.75){
        player_movement.y = -1;
    }
    if (player_position.y <= -3.75){
        player_movement.y = 1;
    }
}

void Entity::Render(ShaderProgram *program) {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, player_position);
    program->SetModelMatrix(modelMatrix);
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::changeMovementY(int n){
    player_movement.y = n;
    if (glm::length(player_movement) > 1.0f)
    {
        player_movement = glm::normalize(player_movement);
    }
}

void Entity::changeMovementX(int n){
    player_movement.x = n;
    if (glm::length(player_movement) > 1.0f)
    {
        player_movement = glm::normalize(player_movement);
    }
}

float Entity::getX() const {
return player_position.x;
}

float Entity::getY() const {
    return player_position.y;
}
