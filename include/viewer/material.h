#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

class MaterialList
{
public:
	MaterialList(std::string defaultPath);
	MaterialList(std::string defaultPath, std::string* materialsPaths,
			unsigned char nbMaterials = 1, unsigned char firstMaterial = 0);
	MaterialList(MaterialList* materials);
	~MaterialList();

	std::string* GetDefaultPath();
	std::string* GetMaterialPath(unsigned char materialID);
	std::string* GetMaterialsPaths();
	unsigned char GetNbMaterials();
	unsigned char GetFirstMaterial();

	void SetDefaultPath(std::string path);
	void SetMaterialsPaths(std::string* materialsPaths,
			unsigned char nbMaterials = 1, unsigned char firstMaterial = 0);
	void SetMaterialPath(std::string materialPath, unsigned char materialID);

private:
	void CopyList(std::string* list, unsigned char size);
	void CleanList();

	std::string defaultPath;
	std::string* materialsPaths;
	unsigned char nbMaterials;
	unsigned char firstMaterial;
};

#endif // MATERIAL_H
