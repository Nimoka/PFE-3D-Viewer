#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include "opengl.h"

std::string GetShaderLog(GLuint shader);

class ShaderReader
{
public:
	ShaderReader(void* context);
	ShaderReader(void* context,
			const std::string& vertexShaderPath,
			const std::string& fragmentShaderPath,
			bool isDynamic = false);
	~ShaderReader();

	bool Load();
	bool LoadFiles(const std::string& vertexShaderPath,
			const std::string& fragmentShaderPath,
			bool isDynamic = false);

	void Activate() const;
	void Deactivate() const;

	int GetUniformLocation(const std::string& name) const;
	int GetAttribLocation(const std::string& name) const;

	bool ExportShaders(const std::string& vertexShaderPath,
			const std::string& fragmentShaderPath);

	bool IsLoaded();

	std::string GetVertexShaderPath();
	std::string GetFragmentShaderPath();

private:
	void Clean();
	std::string GetFileContent(const std::string& path);

	void* context = nullptr;

	std::string vertexShaderPath;
	std::string fragmentShaderPath;
	bool dynamicShaders = false;

	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	bool isLoaded = false;
};

#endif // SHADERLOADER_H
