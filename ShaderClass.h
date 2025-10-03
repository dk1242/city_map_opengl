#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader {
public:
	GLuint ID;
	std::string shaderType;
	Shader(const char* vertexFile, const char* fragmentFile, const char* shaderType);
	Shader(const char* computeShaderFile, const char* shaderType);

	void Activate() const;
	void Delete() const;
private:
	void compileErrors(unsigned int shader, const char* type);
};

#endif
