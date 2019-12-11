//
//  Start.cpp
//  SDLProject
//
//  Created by Mayra Piasta on 10/30/19.
//  Copyright © 2019 ctg. All rights reserved.
//

#include "MainMenu.h"

void MainMenu::Initialize() {
	//projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
	state.currLevel = 0;
	state.nextLevel = -1;
}
void MainMenu::Update(float deltaTime) {

	// Check for pressed/held keys below
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_RETURN])
	{
		state.nextLevel = 1;
		//give the player 3 lives again
		//state.player.lives = 3;
	}
}

void MainMenu::Render(ShaderProgram* program) {
	GLuint fontTextureID;
	fontTextureID = Util::LoadTexture("font1.png");
	Util::DrawText(program, fontTextureID, "Menu", 1.0f, -0.5f, glm::vec3(-2.5, 2, 0));
	Util::DrawText(program, fontTextureID, "The Vampire Slayer", 1.0f, -0.5f, glm::vec3(-4.5, 0, 0));
	Util::DrawText(program, fontTextureID, "Returns!", 1.0f, -0.5f, glm::vec3(-2.5, -1, 0));
	Util::DrawText(program, fontTextureID, "Press Enter to start", 0.9f, -0.5f, glm::vec3(-4.0, -2, 0));

}
