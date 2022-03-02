#include "shaderreader.h"

#include "context.h"
#include "modules/message.h"
#include "utils.h"

ShaderReader::ShaderReader(void* context)
		: context(context) {}

ShaderReader::ShaderReader(void* context,
		const std::string& vertexShaderPath,
		const std::string& fragmentShaderPath,
		bool isDynamic)
		: context(context)
		, dynamicShaders(isDynamic) {
	this->vertexShaderPath = vertexShaderPath;
	this->fragmentShaderPath = fragmentShaderPath;

	this->Load();
}

ShaderReader::~ShaderReader() {
	this->Clean();
}

bool ShaderReader::Load() {
	// Check if filepaths were given
	if (this->vertexShaderPath.empty()
			|| this->fragmentShaderPath.empty())
		return false;

	// Load files’ content
	std::string vertexShaderContent =
			this->GetFileContent(this->vertexShaderPath);
	std::string fragmentShaderContent =
			this->GetFileContent(this->fragmentShaderPath);

	// Check if both files have content
	if (vertexShaderContent.empty()) {
		// If vertex shader was not read, send alert to user
		if (this->context != nullptr) {
			((Context*) this->context)->AddModule(
					new AlertMessageModule(this->context,
							"Failed to load file '"
									+ this->vertexShaderPath + "'."));
		}
		return false;
	}
	if (fragmentShaderContent.empty()) {
		// If fragment shader was not read, send alert to user
		if (this->context != nullptr) {
			((Context*) this->context)->AddModule(
					new AlertMessageModule(this->context,
							"Failed to load file '"
									+ this->fragmentShaderPath + "'."));
		}
		return false;
	}

	// Create program
	GLuint tmpProgramID = glCreateProgram();
	GLuint tmpVertexShaderID, tmpFragmentShaderID;

	// Load vertex shader
	{
		// Create shader
		tmpVertexShaderID = glCreateShader(GL_VERTEX_SHADER);

		// Load shader source code
		const GLchar* content = vertexShaderContent.c_str();
		glShaderSource(tmpVertexShaderID, 1, (const GLchar**) &content, 0);

		// Compile
		glCompileShader(tmpVertexShaderID);

		// Check compilation status
		int compiled;
		glGetShaderiv(tmpVertexShaderID, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			// If errors, delete new shader and program
			glDeleteShader(tmpVertexShaderID);
			glDeleteProgram(tmpProgramID);

			// Send alert to user
			if (this->context != nullptr) {
				((Context*) this->context)->AddModule(new AlertMessageModule(
						this->context, "Failed to compile file '"
								+ this->vertexShaderPath + "'."));
			}

			return false;
		}

		// Attach the shader to the program
		glAttachShader(tmpProgramID, tmpVertexShaderID);
	}

	// Load fragment shader
	{
		// Create shader
		tmpFragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Load shader source code
		const GLchar* content = fragmentShaderContent.c_str();
		glShaderSource(tmpFragmentShaderID, 1, (const GLchar**) &content, 0);

		// Compile
		glCompileShader(tmpFragmentShaderID);

		// Check compilation status
		int compiled;
		glGetShaderiv(tmpFragmentShaderID, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			// If errors, delete new shaders and program
			glDeleteShader(tmpVertexShaderID);
			glDeleteShader(tmpFragmentShaderID);
			glDeleteProgram(tmpProgramID);

			// Send alert to user
			((Context*) this->context)->AddModule(new AlertMessageModule(
					this->context, "Failed to compile file '"
							+ this->fragmentShaderPath + "'."));

			return false;
		}

		// Attach the shader to the program
		glAttachShader(tmpProgramID, tmpFragmentShaderID);
	}

	// Link the program
	glLinkProgram(tmpProgramID);

	// Check linkage status
	int linked;
	glGetProgramiv(tmpProgramID, GL_LINK_STATUS, &linked);
	if (linked != ((int) GL_TRUE)) {
		// If errors, delete new shaders and program
		glDeleteShader(tmpVertexShaderID);
		glDeleteShader(tmpFragmentShaderID);
		glDeleteProgram(tmpProgramID);

		((Context*) this->context)->AddModule(new AlertMessageModule(
					this->context, "Failed to link shaders."));

		return false;
	}

	// Replace current program and shaders by new ones
	this->Clean();
	this->programID = tmpProgramID;
	this->vertexShaderID = tmpVertexShaderID;
	this->fragmentShaderID = tmpFragmentShaderID;
	this->isLoaded = true;

	return true;
}

bool ShaderReader::LoadFiles(const std::string& vertexShaderPath,
		const std::string& fragmentShaderPath,
		bool isDynamic) {
	this->vertexShaderPath = vertexShaderPath;
	this->fragmentShaderPath = fragmentShaderPath;
	this->dynamicShaders = isDynamic;

	return this->Load();
}

void ShaderReader::Activate() const {
	assert(this->isLoaded);
	glUseProgram(this->programID);
}

void ShaderReader::Deactivate() const {
	glUseProgram(0);
}

int ShaderReader::GetUniformLocation(const std::string& name) const {
	assert(this->isLoaded);
	return glGetUniformLocation(this->programID, name.c_str());
}

int ShaderReader::GetAttribLocation(const std::string& name) const {
	assert(this->isLoaded);
	return glGetAttribLocation(this->programID, name.c_str());
}

bool ShaderReader::ExportShaders(const std::string& vertexShaderPath,
		const std::string& fragmentShaderPath) {
	return (SaveTextFile(vertexShaderPath,
					GetFileContent(this->vertexShaderPath))
			&& SaveTextFile(fragmentShaderPath,
					GetFileContent(this->fragmentShaderPath)));
}

bool ShaderReader::IsLoaded() {
	return this->isLoaded;
}

std::string ShaderReader::GetVertexShaderPath() {
	return this->vertexShaderPath;
}

std::string ShaderReader::GetFragmentShaderPath() {
	return this->fragmentShaderPath;
}

void ShaderReader::Clean() {
	if (!this->isLoaded)
		return;
	glDeleteShader(this->vertexShaderID);
	glDeleteShader(this->fragmentShaderID);
	glDeleteProgram(this->programID);
	this->isLoaded = false;
}

std::string ShaderReader::GetFileContent(const std::string& path) {
	std::string content = LoadTextFile(path);
	return content;
}
