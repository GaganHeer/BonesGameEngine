#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL.h>
#include <Eigen/Dense>
#include "Math.h"

struct DirectionalLight {
	//Light direction
	Eigen::Vector3f direction;
	//Diffuse colour
	Eigen::Vector3f diffuseColour;
	//Spcular colour
	Eigen::Vector3f specColour;
};

class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();

	bool Initialize(float screenWidth, float screenHeight);
	void Shutdown();
	void UnloadData();

	void Draw();

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	void AddMeshComp(class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);

	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);

	void SetViewMatrix(const Eigen::Matrix4f& view) {
		this->view = view;
	}

	void SetAmbientLight(const Eigen::Vector3f& ambient) {
		ambientLight = ambient;
	}

	DirectionalLight& GetDirectionalLight() {
		return dirLight;
	}

	float GetScreenWidth() const {
		return screenWidth;
	}

	float GetScreenHeight() const {
		return screenHeight;
	}
private:
	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(class Shader* shader);

	std::unordered_map<std::string, class Texture*> textures;

	std::unordered_map<std::string, class Mesh*> meshes;

	std::vector<class SpriteComponent*> sprites;

	std::vector<class MeshComponent*> meshComponents;

	class Game* game;

	class Shader* spriteShader;

	class Vertex* spriteVerts;

	class Shader* meshShader;

	Eigen::Matrix4f view;
	Eigen::Matrix4f projection;

	float screenWidth;
	float screenHeight;

	Eigen::Vector3f ambientLight;
	DirectionalLight dirLight;

	SDL_Window* window;
	SDL_GLContext context;
};

