#include "Scene.h"
class GameOver : public Scene {
public:
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program) override;
};

