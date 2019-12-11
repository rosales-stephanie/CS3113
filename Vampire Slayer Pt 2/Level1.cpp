
#include <vector>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Level1.h"

//#define LEVEL1_WIDTH 14
//#define LEVEL1_HEIGHT 8 

//#define ENEMY_COUNT 2
//int ENEMY_COUNT = 2;

#define OBJECT_COUNT 544
#define ENEMY_COUNT 1

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


void Level1::Initialize() {
	state.startTime = SDL_GetTicks()/1000.0f;
	state.currLevel = 1;
	GLuint floorTextureID = Util::LoadTexture("dirt.jfif");
	Mesh* floorMesh = new Mesh();
	floorMesh->LoadOBJ("cube.obj");

	GLuint crateTextureID = Util::LoadTexture("Metal_Plate.jpg");
	Mesh* crateMesh = new Mesh();
	crateMesh->LoadOBJ("cube.obj");

	state.player.position = glm::vec3(9, 1, 0);
	state.player.acceleration = glm::vec3(0, 0, 0);

	state.objects[0].position = glm::vec3(0, 0, 0);
	state.objects[0].scale = glm::vec3(20, 1, 20);
	state.objects[0].rotation = glm::vec3(0, 0, 0);
	state.objects[0].acceleration = glm::vec3(0, 0, 0);
	state.objects[0].velocity = glm::vec3(0, 0, 10); //constant speed towards you
	state.objects[0].textureID = floorTextureID;
	state.objects[0].mesh = floorMesh;
	//wall 1
	int count = 1;
	for (int i = 8; i > 4; i--) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(i, j, -3);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
	}
	//wall 2
	int depth = 1;
	for (int i = 0; i < 5; i++) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(8, j, depth);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
		depth -= 1;
	}
	//wall 4
	for (int i = 8; i < 11; i++) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(i, j, 2);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
	}
	// wall 3
	depth = -1;
	for (int i = 0; i < 10; i++) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(10.5, j, depth);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
		depth -= 1;
	}
	//wall 5
	for (int i = 10; i > 6; i--) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(i, j, -6);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
	}
	//wall 6
	for (int i = 11; i > -11; i--) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(i, j, -10.5);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
	}
	//wall 7
	depth = 0;
	for (int i = 0; i < 11; i++) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(-10.5, j, depth);
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
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(-10.5, j, depth);
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
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(10, j, depth);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
		depth += 1;
	}
	//wall 10
	for (int i = -11; i < 11; i++) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(i, j, 10.5);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
	}
	//wall 11
	for (int i = 9; i > 6; i--) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(i, j, 7);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
	}
	//wall 12
	for (int i = 4; i > 0; i--) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(i, j, 5);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
	}
	//wall 13
	depth = 10.5;
	for (int i = 0; i < 3; i++) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(4, j, depth);
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
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(3, j, depth);
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
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(4, j, depth);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
		depth -= 1;
	}
	//wall 16
	for (int i = 4; i > 0; i--) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(i, j, 0);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
	}
	//wall 17
	depth = -1;
	for (int i = 0; i < 10; i++) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(1, j, depth);
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
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(1, j, depth);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
		depth -= 1;
	}
	//wall 19
	for (int i = -10; i < -3; i++) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(i, j, 4);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
	}
	//wall 20
	depth = 10.5;
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(-7, j, depth);
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
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(-6, j, depth);
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
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(-3, j, depth);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
		depth -= 1;
	}
	//wall 23
	for (int i = -2; i < 1; i++) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(i, j, -4);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
	}
	//wall 24
	for (int i = -8; i < -3; i++) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(i, j, -1);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
	}
	//wall 25
	depth = 6;
	for (int i = 0; i < 7; i++) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(-4, j, depth);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
		depth -= 1;
	}
	//wall 26
	for (int i = -4; i < -1; i++) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(i, j, 6);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
	}
	//wall 27
	depth = 6;
	for (int i = 0; i < 5; i++) {
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(-2, j, depth);
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
		for (int j = 1; j < 4; j++) {
			state.objects[count].position = glm::vec3(4, j, depth);
			state.objects[count].textureID = crateTextureID;
			state.objects[count].mesh = crateMesh;
			state.objects[count].entityType = BOX;
			count += 1;
		}
		depth -= 1;
	}

	GLuint enemyTextureID = Util::LoadTexture("vampire.png");
	for (int i = 0; i < ENEMY_COUNT; i++) {
		state.enemies[i].billboard = true;
		state.enemies[i].textureID = enemyTextureID;
		state.enemies[i].position = glm::vec3(-3, 1, 6);
		state.enemies[i].rotation = glm::vec3(0, 0, 0);
		state.enemies[i].acceleration = glm::vec3(0, 0, 0);
	}

	//Mix_PlayChannel(-1, state.win_sound, 1);
	state.nextLevel = -1;
}
void Level1::Update(float deltaTime) {
	//state.player.Update(deltaTime, NULL, 0, 0);

	
	state.player.Update(deltaTime, &state.player, state.objects, OBJECT_COUNT);
	for (int i = 0; i < ENEMY_COUNT; i++) {
		state.enemies[i].Update(deltaTime, &state.player, state.objects, OBJECT_COUNT);
		if (state.player.CheckCollision(&state.enemies[i])) {
			//Mix_PlayChannel(-1, state.lose_sound, 1);
		}
	}

	//if you reach the end of the maze, play a win sound
	if (state.player.position.x <= -9 && -1 <= state.player.position.z && state.player.position.z <= 4) {
		Mix_PlayChannel(-1, state.win_sound, 1);
		state.nextLevel = 1;
		state.player.velocity = 0;
		state.player.acceleration = 0;
	}

	//you have two minutes, otherwise you lose
	if ((state.time - state.startTime) >= 120) {
		Mix_PlayChannel(-1, state.lose_sound, 1);
		state.player.velocity = 0;
		state.player.acceleration = 0;
	}

	
}
void Level1::Render(ShaderProgram* program) {
	//state.map->Render(program);
	//state.player.Render(program);


	for (int i = 0; i < OBJECT_COUNT; i++) {
		state.objects[i].Render(program);
	}

	//if you dont push spacebar, and didnt see an enemy, thennnn you didnt kill them, so render em

	for (int i = 0; i < ENEMY_COUNT; i++) {		
		state.enemies[i].Render(program);
	}

}
