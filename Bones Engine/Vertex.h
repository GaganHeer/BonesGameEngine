#pragma once
class Vertex
{
public:
	Vertex(const float* verts, unsigned int numVerts, const unsigned int* indices, unsigned int numIndices);
	~Vertex();

	void SetActive();

	unsigned int GetNumIndices() const { return numIndices; }
	unsigned int GetNumVerts() const { return numVerts; }
private:
	unsigned numVerts;
	unsigned int numIndices;
	unsigned int vertexBuffer;
	unsigned int indexBuffer;
	unsigned int vertexArray;
};

