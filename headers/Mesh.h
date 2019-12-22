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

	class Texture* GetTexture(size_t index);

	class VertexArray* GetVertexArray() { 
		return vertexArray; 
	}

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
	float specPower;
	std::vector<class Texture*> _textures;
	VertexArray* vertexArray;
	std::string shaderName;
	float radius;
};