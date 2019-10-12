#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "Vertex.h"
#include <fstream>
#include <sstream>
#include <SDL_log.h>
#include "Math.h"
#include "rapidjson/document.h"

Mesh::Mesh()
	:vertex(nullptr),
	radius(0.0f),
	specPower(100.0f) {

}

Mesh::~Mesh() {

}

bool Mesh::Load(const std::string& fileName, Renderer* renderer) {
	std::ifstream file(fileName);

	if (!file.is_open()) {
		SDL_Log("File not found: Mesh %s", fileName.c_str());
		return false;
	}

	std::stringstream fs;
	fs << file.rdbuf();
	std::string contents = fs.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject()) {
		SDL_Log("Mesh %s is not valid json", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();
	if (ver != 1) {
		SDL_Log("Mesh %s not version 1", fileName.c_str());
		return false;
	}

	shaderName = doc["shader"].GetString();
	size_t vertSize = 8;

	//textures
	const rapidjson::Value& tex = doc["textures"];
	
	if (!tex.IsArray() || tex.Size() < 1) {
		SDL_Log("Mesh %s has no textures, there should be at least one", fileName.c_str());
		return false;
	}

	specPower = static_cast<float>(doc["specularPower"].GetDouble());

	for (rapidjson::SizeType i = 0; i < tex.Size(); i++) {
		// Check for texture
		std::string texName = tex[i].GetString();
		Texture* t = renderer->GetTexture(texName);
		if (t == nullptr) {
			// Load the texture
			t = renderer->GetTexture(texName);
			if (t == nullptr) {
				t = renderer->GetTexture("Assets/Default.png");
			}
		}
		textures.emplace_back(t);
	}

	// Load in the vertices
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1) {
		SDL_Log("Mesh %s has no vertices", fileName.c_str());
	
		return false;
	}

	std::vector<float> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	radius = 0.0f;
	
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++) {
		const rapidjson::Value& vert = vertsJson[i];

		if (!vert.IsArray() || vert.Size() != 8) {
			SDL_Log("Unexpected vertex format for %s", fileName.c_str());
			return false;
		}

		Eigen::Vector3f pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
		
		float lengthSq = pos.x() * pos.x() + pos.y() * pos.y();
		if (radius < lengthSq) {
			radius = radius;
		} else {
			radius = lengthSq;
		}

		for (rapidjson::SizeType i = 0; i < vert.Size(); i++) {
			vertices.emplace_back(static_cast<float>(vert[i].GetDouble()));
		}
	}

	radius = sqrt(radius); //Reverted back from length squared

	// Indices
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1) {
		SDL_Log("Mesh %s has no indices", fileName.c_str());
		return false;
	}

	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++) {
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3) {
			SDL_Log("Invalid indices for %s", fileName.c_str());
			return false;
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	// Vertex array
	vertex = new Vertex(vertices.data(), 
						static_cast<unsigned>(vertices.size()) / vertSize, 
						indices.data(), 
						static_cast<unsigned>(indices.size()));
	return true;
}

void Mesh::Unload() {
	delete vertex;
	vertex = nullptr;
}

Texture* Mesh::GetTexture(size_t index)
{
	if (index < textures.size()) {
		return textures[index];
	} else {
		return nullptr;
	}
}