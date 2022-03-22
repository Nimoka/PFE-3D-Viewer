#include <material.h>

MaterialList::MaterialList(std::string defaultPath) {
	this->defaultPath = defaultPath;
}

MaterialList::MaterialList(std::string defaultPath,
		std::string* materialsPaths, unsigned char nbMaterials,
		unsigned char firstMaterial)
		: nbMaterials(nbMaterials)
		, firstMaterial(firstMaterial) {
	this->defaultPath = defaultPath;
	this->CopyList(materialsPaths, nbMaterials);
}

MaterialList::MaterialList(MaterialList* materials)
		: nbMaterials(materials->GetNbMaterials())
		, firstMaterial(materials->GetFirstMaterial()) {
	this->defaultPath = *materials->GetDefaultPath();
	this->CopyList(materials->GetMaterialsPaths(), this->nbMaterials);
}

MaterialList::~MaterialList() {
	this->CleanList();
}

std::string* MaterialList::GetDefaultPath() {
	return &this->defaultPath;
}

std::string* MaterialList::GetMaterialPath(unsigned char materialID) {
	// Check if the material asked is in the list
	if ((materialID < this->firstMaterial)
			|| (materialID >= (this->firstMaterial + this->nbMaterials))) {
		// If not, return the default material
		return this->GetDefaultPath();
	}

	// Else, return the material asked
	return (this->materialsPaths + materialID - this->firstMaterial);
}

std::string* MaterialList::GetMaterialsPaths() {
	return this->materialsPaths;
}

unsigned char MaterialList::GetNbMaterials() {
	return this->nbMaterials;
}

unsigned char MaterialList::GetFirstMaterial() {
	return this->firstMaterial;
}

void MaterialList::SetDefaultPath(std::string path) {
	this->defaultPath = path;
}

void MaterialList::SetMaterialsPaths(std::string* materialsPaths,
		unsigned char nbMaterials, unsigned char firstMaterial) {
	// Clean the old list and copy the new one
	this->CopyList(materialsPaths, nbMaterials);

	// Copy additional metadata
	this->firstMaterial = firstMaterial;
}

void MaterialList::SetMaterialPath(std::string materialPath,
		unsigned char materialID) {
	// Check if the material asked is in the list
	if ((materialID >= this->firstMaterial)
			&& (materialID < (this->firstMaterial + this->nbMaterials))) {
		// If so, modify it directly
		this->materialsPaths[materialID] = materialPath;
	} else {
		// If not, recreate the list to add it

		std::string* newList;
		unsigned char newSize;

		if (materialID < this->firstMaterial) {
			unsigned char diffSize = this->firstMaterial - materialID;
			newSize = this->nbMaterials + diffSize;
			newList = new std::string[newSize];

			// Copy the new material in first place
			newList[0] = materialPath;

			// Set default path for all new unknown paths
			unsigned char current;
			for (current = 1; current < diffSize; current++)
				newList[current] = this->defaultPath;

			// Copy the list at the end
			for (unsigned char i = 0; i < this->nbMaterials; i++)
				newList[current++] = this->materialsPaths[i];

			// Update the first material
			this->firstMaterial = materialID;
		} else {
			newSize = materialID - this->firstMaterial;	// +1 added at the end
			newList = new std::string[newSize + 1];

			// Copy the list at the beginning
			unsigned char current;
			for (current = 0; current < this->nbMaterials; current++)
				newList[current] = this->materialsPaths[current];

			// Set default path for all new unknown paths
			for (; current < newSize; current++)
				newList[current] = this->defaultPath;

			// Copy the new material at the end of the list
			newList[current] = materialPath;

			newSize++;
		}

		// Replace the old list with the new one
		this->CleanList();
		this->materialsPaths = newList;
		this->nbMaterials = newSize;
	}
}

void MaterialList::CopyList(std::string* list, unsigned char size) {
	// Clean the old list if needed
	this->CleanList();

	// Check the list
	if (list == nullptr)
		return;

	// Allocate memory for the new list
	this->materialsPaths = new std::string[size];

	// Copy data
	for (unsigned char i = 0; i < size; i++)
		this->materialsPaths[i] = list[i];

	// Copy metedata
	this->nbMaterials = size;
}

void MaterialList::CleanList() {
	if (this->materialsPaths == nullptr)
		return;
	delete [] this->materialsPaths;
	this->materialsPaths = nullptr;
	this->nbMaterials = 0;
	this->firstMaterial = 0;
}
