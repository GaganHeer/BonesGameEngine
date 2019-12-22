#include "Shader.h"
#include "Texture.h"
#include <SDL.h>
#include <fstream>
#include <sstream>

Shader::Shader()
	: shaderProgram(0)
	, vertexShader(0)
	, fragShader(0)
{
	
}

Shader::~Shader()
{

}

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	// Compile vertex and pixel shaders
	if (!CompileShader(vertName,
		GL_VERTEX_SHADER,
		vertexShader) ||
		!CompileShader(fragName,
			GL_FRAGMENT_SHADER,
			fragShader))
	{
		return false;
	}
	
	// Now create a shader program that
	// links together the vertex/frag shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	
	// Verify that the program linked successfully
	if (!IsValidProgram())
	{
		return false;
	}
	
	return true;
}

void Shader::Unload()
{
	// Delete the program/shaders
	glDeleteProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
}

void Shader::SetActive()
{
	// Set this program as the active one
	glUseProgram(shaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	// Find the uniform by this name
	GLuint loc = glGetUniformLocation(shaderProgram, name);
	// Send the matrix data to the uniform
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

void Shader::SetMatrixUniforms(const char* name, Matrix4* matrices, unsigned count)
{
	GLuint loc = glGetUniformLocation(shaderProgram, name);
	// Send the matrix data to the uniform
	glUniformMatrix4fv(loc, count, GL_TRUE, matrices->GetAsFloatPtr());
}

void Shader::SetVectorUniform(const char* name, const Vector3& vector)
{
	GLuint loc = glGetUniformLocation(shaderProgram, name);
	// Send the vector data
	glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::SetVector2Uniform(const char* name, const Vector2& vector)
{
	GLuint loc = glGetUniformLocation(shaderProgram, name);
	// Send the vector data
	glUniform2fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::SetFloatUniform(const char* name, float value)
{
	GLuint loc = glGetUniformLocation(shaderProgram, name);
	// Send the float data
	glUniform1f(loc, value);
}

void Shader::SetIntUniform(const char* name, int value)
{
	GLuint loc = glGetUniformLocation(shaderProgram, name);
	// Send the float data
	glUniform1i(loc, value);
}

bool Shader::CompileShader(const std::string& fileName,
	GLenum shaderType,
	GLuint& outShader)
{
	// Open file
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		// Read all the text into a string
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();
		
		// Create a shader of the specified type
		outShader = glCreateShader(shaderType);
		// Set the source characters and try to compile
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);
		
		if (!IsCompiled(outShader))
		{
			SDL_Log("Failed to compile shader %s", fileName.c_str());
			return false;
		}
	}
	else
	{
		SDL_Log("Shader file not found: %s", fileName.c_str());
		return false;
	}
	
	return true;
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	// Query the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL Compile Failed:\n%s", buffer);
		return false;
	}
	
	return true;
}

bool Shader::IsValidProgram()
{
	
	GLint status;
	// Query the link status
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(shaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}
	
	return true;
}
