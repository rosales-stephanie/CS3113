//
//  Level2.cpp
//  SDLProject
//
//  Created by Mayra Piasta on 10/30/19.
//  Copyright © 2019 ctg. All rights reserved.
//

#include "Level2.h"

#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8
unsigned int level2_data[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};


void Level2::Initialize() {
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);
    state.player.entityType = PLAYER;
    state.player.isStatic = false;
    state.player.width = 1.0f;
    state.player.position = glm::vec3(5, 0, 0);
    state.player.acceleration = glm::vec3(0, -9.81f, 0);
    state.player.textureID = Util::LoadTexture("me.png");

    state.enemies.entityType = ENEMY;
    state.enemies.isStatic = false;
    state.enemies.width = 1.0f;
    state.enemies.position = glm::vec3(2, 0, 0);
    state.enemies.acceleration = glm::vec3(0, -9.81f, 0);
    state.enemies.textureID = Util::LoadTexture("evil.png");
    
    state.nextLevel = -1;
}
void Level2::Update(float deltaTime) {
    state.player.Update(deltaTime, NULL, 0, state.map);

    state.enemies.Update(deltaTime, NULL, 0, state.map);
    if (state.enemies.entityType == ENEMY){
        state.enemies.AI(state.player);
    }

	//die if you go off the screen
	if (state.player.position.y < -10) {
		state.nextLevel = 5;
	}

    if (state.player.position.x >= 14) {
        state.nextLevel = 3;
    }
}
void Level2::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player.Render(program);
    state.enemies.Render(program);

}
