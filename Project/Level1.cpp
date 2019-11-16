//
//  Level1.cpp
//  SDLProject
//
//  Created by Mayra Piasta on 10/30/19.
//  Copyright © 2019 ctg. All rights reserved.
//

#include "Level1.h"
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8 
unsigned int level1_data[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};


void Level1::Initialize() {
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    state.player.entityType = PLAYER;
    state.player.isStatic = false;
    state.player.width = 1.0f;
    state.player.position = glm::vec3(5, 0, 0);
    state.player.acceleration = glm::vec3(0, -9.81f, 0);
    state.player.textureID = Util::LoadTexture("me.png");

    state.enemies.entityType = ENEMY;
    state.enemies.isStatic = false;
    state.enemies.width = 1.0f;
    state.enemies.position = glm::vec3(2, -2.25, 0);
    state.enemies.acceleration = glm::vec3(0, -9.81f, 0);
    state.enemies.textureID = Util::LoadTexture("evil.png");
    
    state.nextLevel = -1;
}
void Level1::Update(float deltaTime) {
    state.player.Update(deltaTime, NULL, 0, state.map);

    state.enemies.Update(deltaTime, NULL, 0, state.map);
    if (state.enemies.entityType == ENEMY){
        state.enemies.AI(state.player);
    }

	//die if you run out of lives
	if (state.player.lives == 0) {
		state.nextLevel = 5;
	}

	//lose a life  if you fall off the screen
	if (state.player.position.y < -10) {
		state.player.lives -= 1;
		state.nextLevel = 1;
	}
	//if touched by ai 
	if (state.player.CheckCollision(state.enemies)) {
        state.enemies.position.x -= 1;
		state.player.lives -= 1;
	}

    if (state.player.position.x >= 14) {
        state.nextLevel = 2;
    }

}
void Level1::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player.Render(program);
    state.enemies.Render(program);

}
