#pragma once
#include <GL/glew.h>
#include <string>
#include <Eigen/dense>

class Shader
{
public:
	Shader();
	~Shader();

	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	void SetActive();
	void SetMatrixUniform(const char* name, const Eigen::Matrix4f& matrix);
	void SetVectorUniform(const char* name, const Eigen::Vector3f& vector);
	void SetFloatUniform(const char* name, float value);
private:
	bool CompileShader(const std::string& fileName,
		GLenum shaderType,
		GLuint& outShader);
	bool IsCompiled(GLuint shader);
	bool IsValidProgram();
private:
	GLuint vertexShader;
	GLuint fragShader;
	GLuint shaderProgram;
};

