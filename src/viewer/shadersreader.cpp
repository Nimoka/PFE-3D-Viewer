#include "shadersreader.h"

#include <string>
#include <vector>

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
		: context(context) {
	this->SetDefaultMacrosValues();
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
		const std::string& fragmentShaderPath) {
	this->vertexShaderPath = vertexShaderPath;
	this->fragmentShaderPath = fragmentShaderPath;

	return this->Load();
}

bool ShadersReader::LoadFiles(const std::string& vertexShaderPath,
		const std::string& fragmentShaderPath,
		MaterialList* materialsPaths) {
	this->materialsPaths = materialsPaths;
	this->specificMaterialPath = nullptr;
	return this->LoadFiles(vertexShaderPath, fragmentShaderPath);
}

bool ShadersReader::LoadFiles(const std::string& vertexShaderPath,
		const std::string& fragmentShaderPath,
		std::string* specificMaterialPath) {
	this->specificMaterialPath = specificMaterialPath;
	this->materialsPaths = nullptr;
	return this->LoadFiles(vertexShaderPath, fragmentShaderPath);
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

void ShadersReader::SetDefaultMacrosValues() {
	this->preprocessorMacros[SPPM_NB_DIR_LIGHTS] = "0";
	this->preprocessorMacros[SPPM_NB_PT_LIGHTS] = "0";
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

	bool defineMaterialID = true;

	// Prepare materials
	std::vector<std::string> materialsCalls;
	unsigned char nbMaterials = 0;
	std::string* materialsPath = nullptr;
	if (this->materialsPaths != nullptr) {
		nbMaterials = this->materialsPaths->GetNbMaterials();
		materialsPath = this->materialsPaths->GetMaterialsPaths();
	} else if (this->specificMaterialPath != nullptr) {
		nbMaterials = 1;
		materialsPath = this->specificMaterialPath;
	}

	// Search if there are tags in the source code
	// (To avoid tags in comment, only search after a new line,
	// the first one should always be ‘#version’)
	std::size_t insertPoint = content.find("\n@");
	std::size_t endTag, spaceTag;
	std::string tag, argument, newContent;
	while (insertPoint != std::string::npos) {
		insertPoint += 2;

		// Search for the end of the tag (either a new line or a space)
		endTag = content.find('\n', insertPoint);
		spaceTag = content.find(' ', insertPoint);
		if (spaceTag < endTag) {
			tag = content.substr(insertPoint, (spaceTag - insertPoint));
			argument = content.substr(spaceTag + 1, (endTag - spaceTag - 1));
		} else {
			tag = content.substr(insertPoint, (endTag - insertPoint));
			argument = "";
		}

		newContent = "";

		// Check the tag and prepare the new content
		if (!tag.compare(ST_DEFINE_MACROS)) {
			// Add preprocessor macros (#define)
			for (const std::pair<const std::string, std::string>& item:
					this->preprocessorMacros) {
				newContent += "#define " + item.first + '\t' + item.second
						+ '\n';
			}
		} else if (!tag.compare(ST_DEFINE_MATERIALS)) {
			// Add materials functions definitions
			std::string materialContent, materialCall;
			std::size_t beginCall, endCall;

			for (unsigned char i = 0; i < nbMaterials; i++) {
				materialContent = LoadTextFile(materialsPath[i]);
				if (materialContent.size()) {
					// Add the material content to the shader
					newContent += materialContent;

					// Create a function call from the material content
					materialCall =
							GetFunctionCallFromDeclaration(materialContent);

					// If the call was created, add it
					if (materialCall.size()) {
						materialsCalls.push_back(materialCall);
						continue;
					}
				}
				// Add a basic call to avoid compilation error if the file was
				// empty or the function declaration was not found
				materialsCalls.push_back("vec3(0, 0, 0)");
			}
		} else if (!tag.compare(ST_CALL_MATERIALS)) {
			// Add calls to materials functions added
			// (We consider the definitions are already done)

			// Check the number of calls prepared
			if ((nbMaterials == materialsCalls.size())
					&& (nbMaterials)) {
				std::size_t nextCall;
				if (nbMaterials == 1) {
					newContent += this->PrepareMaterialCallArgument(
							argument, materialsCalls[0]);
				} else {
					if (defineMaterialID) {
						newContent += "uint material = uint(texelFetch("
								"face_material, gl_PrimitiveID).r);\n";
						defineMaterialID = false;
					}
					for (unsigned char i = 0; i < nbMaterials; i++) {
						newContent += "if (material == "
								+ std::to_string((unsigned int)
										(i + this->materialsPaths
												->GetFirstMaterial()))
								+ ") { " + this->PrepareMaterialCallArgument(
										argument, materialsCalls[i])
								+ " }\nelse ";
					}
					newContent += "{ " + this->PrepareMaterialCallArgument(
							argument, "vec3(0, 0, 0)") + " }\n";
				}
			}
		}

		// Replace the tag by the new content in the source code
		content.replace(insertPoint - 1,
				(endTag - insertPoint + 2), newContent);

		// Search the next possible tag
		insertPoint = content.find("\n@", endTag);
	}

	return content;
}

std::string ShadersReader::PrepareMaterialCallArgument(const std::string& text,
		const std::string& materialCall) {
	std::string newText = text;
	std::size_t nextCall = newText.find(STCM_TAG);
	while (nextCall != std::string::npos) {
		newText.replace(nextCall, 4, materialCall);
		nextCall = newText.find(STCM_TAG, (nextCall + materialCall.size()));
	}
	return newText;
}
