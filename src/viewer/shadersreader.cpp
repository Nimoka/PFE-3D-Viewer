#include "shadersreader.h"

#include <string>

#include "context.h"
#include "modules/message.h"
#include "utils.h"

std::string GetShaderLog(GLuint shader) {
	GLint size = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
	gl::GLchar* data = (GLchar*) malloc(sizeof(gl::GLchar) * (size + 1));
	glGetShaderInfoLog(shader, size, NULL, data);

	std::string text = std::string((char*) data);
	delete data;

	return text;
}

ShadersReader::ShadersReader(void* context)
		: context(context) {}

ShadersReader::ShadersReader(void* context,
		const std::string& vertexShaderPath,
		const std::string& fragmentShaderPath,
		bool areDynamic)
		: context(context)
		, dynamicShaders(areDynamic) {
	this->vertexShaderPath = vertexShaderPath;
	this->fragmentShaderPath = fragmentShaderPath;

	this->preprocessorMacros[SPPM_NB_DIR_LIGHTS] = "0";
	this->preprocessorMacros[SPPM_NB_PT_LIGHTS] = "0";
	this->Load();
}

ShadersReader::~ShadersReader() {
	this->Clean();
}

bool ShadersReader::Load() {
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
			// If errors, send alert to user
			if (this->context != nullptr) {
				((Context*) this->context)->AddModule(new AlertMessageModule(
						this->context, "Failed to compile file '"
								+ this->vertexShaderPath + "':\n"
								+ GetShaderLog(tmpVertexShaderID)));
			}

			// Delete new shader and program
			glDeleteShader(tmpVertexShaderID);
			glDeleteProgram(tmpProgramID);
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
			// If errors, send alert to user
			if (this->context != nullptr) {
				((Context*) this->context)->AddModule(new AlertMessageModule(
						this->context, "Failed to compile file '"
								+ this->fragmentShaderPath + "':\n"
								+ GetShaderLog(tmpFragmentShaderID)));
			}

			// Delete new shaders and program
			glDeleteShader(tmpVertexShaderID);
			glDeleteShader(tmpFragmentShaderID);
			glDeleteProgram(tmpProgramID);
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

		if (this->context != nullptr) {
			((Context*) this->context)->AddModule(new AlertMessageModule(
						this->context, "Failed to link shaders."));
		}

		return false;
	}

	// Replace current program and shaders by new ones
	this->Clean();
	this->programID = tmpProgramID;
	this->vertexShaderID = tmpVertexShaderID;
	this->fragmentShaderID = tmpFragmentShaderID;
	this->vertexShaderSource = vertexShaderContent;
	this->fragmentShaderSource = fragmentShaderContent;
	this->areLoaded = true;

	return true;
}

bool ShadersReader::LoadFiles(const std::string& vertexShaderPath,
		const std::string& fragmentShaderPath,
		bool areDynamic) {
	this->vertexShaderPath = vertexShaderPath;
	this->fragmentShaderPath = fragmentShaderPath;
	this->dynamicShaders = areDynamic;

	return this->Load();
}

void ShadersReader::Activate() const {
	// Kill the program if it went here without shaders compiled
	// to avoid possible problems with OpenGL (renderering on wrong shaders).
	// The ‘aren’t loaded’ scenario must be handled before.
	assert(this->areLoaded);
	glUseProgram(this->programID);
}

void ShadersReader::Deactivate() const {
	glUseProgram(0);
}

void ShadersReader::SetPreProcessorMacro(const std::string& name,
			const std::string& value) {
	this->preprocessorMacros[name] = value;
}

int ShadersReader::GetUniformLocation(const std::string& name) const {
	// Kill the program if it went here without shaders compiled
	// to avoid possible problems with OpenGL (renderering on wrong shaders).
	// The ‘aren’t loaded’ scenario must be handled before.
	assert(this->areLoaded);
	return glGetUniformLocation(this->programID, name.c_str());
}

int ShadersReader::GetAttribLocation(const std::string& name) const {
	// Kill the program if it went here without shaders compiled
	// to avoid possible problems with OpenGL (renderering on wrong shaders).
	// The ‘aren’t loaded’ scenario must be handled before.
	assert(this->areLoaded);
	return glGetAttribLocation(this->programID, name.c_str());
}

bool ShadersReader::ExportShaders(const std::string& vertexShaderPath,
		const std::string& fragmentShaderPath) {
	return (SaveTextFile(vertexShaderPath, this->vertexShaderSource)
			&& SaveTextFile(fragmentShaderPath, this->fragmentShaderSource));
}

bool ShadersReader::AreDynamic() {
	return this->dynamicShaders;
}

bool ShadersReader::AreLoaded() {
	return this->areLoaded;
}

const std::string& ShadersReader::GetVertexShaderPath() {
	return this->vertexShaderPath;
}

const std::string& ShadersReader::GetFragmentShaderPath() {
	return this->fragmentShaderPath;
}

const std::string& ShadersReader::GetVertexShaderSource() {
	return this->vertexShaderSource;
}

const std::string& ShadersReader::GetFragmentShaderSource() {
	return this->fragmentShaderSource;
}

void ShadersReader::Clean() {
	if (!this->areLoaded)
		return;
	glDeleteShader(this->vertexShaderID);
	glDeleteShader(this->fragmentShaderID);
	glDeleteProgram(this->programID);
	this->areLoaded = false;
}

std::string ShadersReader::GetFileContent(const std::string& path) {
	std::string content = LoadTextFile(path);

	// If nothing to add, don’t change anything
	if (this->preprocessorMacros.size() == 0)
		return content;

	// Search for first line after #version
	std::size_t insertPoint = content.find("#version");
	insertPoint = content.find('\n', insertPoint);
	if (insertPoint != std::string::npos)
		insertPoint++;
	else
		content += '\n';

	// Add preprocessor macros (#define)
	for (const std::pair<const std::string, std::string>& item:
			this->preprocessorMacros) {
		content.insert(insertPoint,
				"#define " + item.first + ' ' + item.second + '\n');
	}

	return content;
}
