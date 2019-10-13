#pragma once
#include <vector>
#include <string>

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();

	class Vertex* GetVertex() {
		return vertex;
	}

	class Texture* GetTexture(size_t index);

	const std::string& GetShaderName() const {
		return shaderName;
	}

	float GetRadius() const {
		return radius;
	}

	float GetSpecPower() const {
		return specPower;
	}
private:
	std::vector<class Texture*> textures;
	class Vertex* vertex;
	std::string shaderName;
	float radius;
	float specPower;
};

