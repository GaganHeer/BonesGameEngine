#include "Shader.h"
#include <SDL.h>
#include <fstream>
#include <sstream>

Shader::Shader() :
	shaderProgram(0),
	vertexShader(0),
	fragShader(0) {

}

Shader::~Shader() {

}

bool Shader::Load(const std::string& vertName, const std::string& fragName) {
	if (!CompileShader(vertName,
		GL_VERTEX_SHADER,
		vertexShader) ||
		!CompileShader(fragName,
			GL_FRAGMENT_SHADER,
			fragShader)) {
		return false;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	if (!IsValidProgram()) {
		return false;
	}

	return true;
}

void Shader::Unload() {
	glDeleteProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
}

void Shader::SetActive() {
	glUseProgram(shaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Eigen::Matrix4f& matrix) {
	GLuint loc = glGetUniformLocation(shaderProgram, name);
	glUniformMatrix4fv(loc, 1, GL_TRUE, reinterpret_cast<const float*>(&matrix));
}

bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader) {
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open()) {
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		outShader = glCreateShader(shaderType);

		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);

		if (!IsCompiled(outShader)) {
			SDL_Log("Failed to compile shader %s", fileName.c_str());
			return false;
		}
	} else {
		SDL_Log("Shader file not found: %s", fileName.c_str());
		return false;
	}

	return true;
}

bool Shader::IsCompiled(GLuint shader) {
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL Compile Failed:\n%s", buffer);
		return false;
	}

	return true;
}

bool Shader::IsValidProgram() {
	GLint status;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}

	return true;
}