
#include "Scene.h"
class MainMenu : public Scene {
public:
	//glm::mat4 projectionMatrix;
	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program) override;
};
