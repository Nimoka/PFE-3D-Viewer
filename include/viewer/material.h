#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

class MaterialList
{
public:
	MaterialList(const std::string& defaultPath);
	MaterialList(const std::string& defaultPath, std::string* materialsPaths,
			unsigned char nbMaterials = 1, unsigned char firstMaterial = 0);
	MaterialList(MaterialList* materials);
	~MaterialList();

	std::string* GetDefaultPath();
	std::string* GetMaterialPath(unsigned char materialID);
	std::string* GetMaterialsPaths();
	unsigned char GetNbMaterials();
	unsigned char GetFirstMaterial();

	void SetDefaultPath(const std::string& path);
	void SetMaterialsPaths(std::string* materialsPaths,
			unsigned char nbMaterials = 1, unsigned char firstMaterial = 0);
	void SetMaterialPath(std::string materialPath, unsigned char materialID);

private:
	void CopyList(std::string* list, unsigned char size);
	void CleanList();

	std::string defaultPath;
	std::string* materialsPaths = nullptr;
	unsigned char nbMaterials = 0;
	unsigned char firstMaterial = 0;
};

#endif // MATERIAL_H
