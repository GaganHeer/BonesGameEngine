#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL.h>
#include "Math.h"

struct DirectionalLight
{
	// Direction of light
	Vector3 direction;
	// Diffuse color
	Vector3 diffuseColor;
	// Specular color
	Vector3 specColor;
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

	void AddPointLight(class PointLightComponent* light);
	void RemovePointLight(class PointLightComponent* light);

	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);

	void SetViewMatrix(const Matrix4& newView) { 
		view = newView; 
	}

	void SetAmbientLight(const Vector3& newAmbient) { 
		ambientLight = newAmbient; 
	}

	DirectionalLight& GetDirectionalLight() { 
		return dirLight; 
	}

	Vector3 Unproject(const Vector3& screenPoint) const;

	// Gets start point and direction of screen vector
	void GetScreenDirection(Vector3& outStart, Vector3& outDir) const;

	float GetScreenWidth() const { 
		return screenWidth; 
	}

	float GetScreenHeight() const { 
		return screenHeight; 
	}

	void SetMirrorView(const Matrix4& view) { mirrorView = view; }
	void SetDepthMapView(const Matrix4& view) { depthMapView = view; }
	Texture* GetDepthMap() { return depthMapTexture; }
	class Texture* GetMirrorTexture() { return mirrorTexture; }
	class GBuffer* GetGBuffer() { return _GBuffer; }
private:
	void Draw3DScene(unsigned int framebuffer, const Matrix4& view, const Matrix4& proj,
		float viewPortScale = 1.0f, bool lit = true);
	void DrawDepthMap(unsigned int franebuffer, const Matrix4& view, const Matrix4& proj,
		float viewPortScale = 1.0f, bool lit = true);
	bool CreateMirrorTarget();
	void DrawFromGBuffer();
	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(class Shader* shader, const Matrix4& view);

	std::unordered_map<std::string, class Texture*> textures;
	std::unordered_map<std::string, class Mesh*> meshes;
	std::unordered_map<std::string, class Font*> fonts;
	std::vector<class SpriteComponent*> sprites;
	std::vector<class MeshComponent*> meshComps;
	std::vector<class SkeletalMeshComponent*> skeletalMeshes;


	class Game* game;
	class Shader* spriteShader;
	class VertexArray* spriteVerts;
	class Shader* meshShader;
	class Shader* skinnedShader;
	class Shader* depthShader;

	Matrix4 view;
	Matrix4 projection;

	float screenWidth;
	float screenHeight;

	Vector3 ambientLight;
	DirectionalLight dirLight;

	unsigned int mirrorBuffer;
	class Texture* mirrorTexture;
	Matrix4 mirrorView;

	unsigned int depthMapFBO;
	class Texture* depthMapTexture;
	Matrix4 depthMapView;

	class GBuffer* _GBuffer;
	// GBuffer shader
	class Shader* GGlobalShader;
	class Shader* GPointLightShader;
	std::vector<class PointLightComponent*> pointLights;
	class Mesh* pointLightMesh;

	SDL_Window* window;
	SDL_GLContext context;
};