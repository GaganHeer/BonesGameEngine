#pragma once
class VertexArray
{
public:
	enum Layout {
		PosNormTex,
		PosNormSkinTex
	};

	VertexArray(const void* verts, unsigned int numVerts, Layout layout, const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	void SetActive();

	unsigned int GetNumIndices() const { 
		return numIndices; 
	}

	unsigned int GetNumVerts() const { 
		return numVerts; 
	}

private:
	//number vertices in the vertex buffer
	unsigned int numVerts;
	//numbdf indices in the index buffer
	unsigned int numIndices;
	//OpenGL ID of the vertex buffer
	unsigned int vertexBuffer;
	//OpenGL ID of the index buffer
	unsigned int indexBuffer;
	//OpenGL ID of the vertex array object
	unsigned int vertexArray;
};