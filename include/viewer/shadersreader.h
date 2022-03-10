#ifndef SHADERSLOADER_H
#define SHADERSLOADER_H

#include "opengl.h"

#include <unordered_map>

// Shaders pre-processor macros’ names
#define SPPM_NB_DIR_LIGHTS	"NB_DIR_LIGHTS"
#define SPPM_NB_PT_LIGHTS	"NB_PT_LIGHTS"

std::string GetShaderLog(GLuint shader);

class ShadersReader
{
public:
	ShadersReader(void* context);
	ShadersReader(void* context,
			const std::string& vertexShaderPath,
			const std::string& fragmentShaderPath,
			bool isDynamic = false);
	~ShadersReader();

	bool Load();
	bool LoadFiles(const std::string& vertexShaderPath,
			const std::string& fragmentShaderPath,
			bool isDynamic = false);

	void Activate() const;
	void Deactivate() const;

	void SetPreProcessorMacro(const std::string& name,
			const std::string& value);

	int GetUniformLocation(const std::string& name) const;
	int GetAttribLocation(const std::string& name) const;

	bool ExportShaders(const std::string& vertexShaderPath,
			const std::string& fragmentShaderPath);

	bool AreDynamic();
	bool AreLoaded();

	const std::string& GetVertexShaderPath();
	const std::string& GetFragmentShaderPath();
	const std::string& GetVertexShaderSource();
	const std::string& GetFragmentShaderSource();

private:
	void Clean();
	std::string GetFileContent(const std::string& path);

	void* context = nullptr;

	std::string vertexShaderPath;
	std::string fragmentShaderPath;
	bool dynamicShaders = false;

	std::string vertexShaderSource;
	std::string fragmentShaderSource;

	std::unordered_map<std::string, std::string> preprocessorMacros;

	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	bool areLoaded = false;
};

#endif // SHADERSLOADER_H
