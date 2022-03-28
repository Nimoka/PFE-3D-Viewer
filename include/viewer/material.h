#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

/**
 * \brief List of materials.
 * 
 * List of materials, where a default material is returned when the index is
 * out-of-bound.
 */
class MaterialList
{
public:
	/**
	 * \brief Constructor.
	 * 
	 * `MaterialList` constructor.
	 * 
	 * \param defaultPath Default path returned for an out-of-bound index.
	 */
	MaterialList(const std::string& defaultPath);
	/**
	 * \brief Constructor with materials.
	 * 
	 * `MaterialList` constructor with an array of materials.
	 * The array will be duplicated.
	 * 
	 * \param defaultPath Default path returned for an out-of-bound index.
	 * \param materialsPaths Array of materials paths. Must be of size
	 *      `nbMaterials`.
	 * \param nbMaterials Number of paths in the array (default: 1).
	 * \param firstMaterial ID of the first material of the array (default: 0).
	 */
	MaterialList(const std::string& defaultPath, std::string* materialsPaths,
			unsigned char nbMaterials = 1, unsigned char firstMaterial = 0);
	/**
	 * \brief Constructor by duplication.
	 * 
	 * `MaterialList` constructor using an existing object that would be
	 * duplicated.
	 * 
	 * \param materials List to duplicate.
	 */
	MaterialList(MaterialList* materials);
	/**
	 * \brief Destructor.
	 * 
	 * `MaterialList` destructor.
	 */
	~MaterialList();

	/**
	 * \brief Getter of a pointer to `defaultPath`.
	 * 
	 * Return a pointer to the `defaultPath` field, corresponding to the default
	 * path returned for an out-of-bound index.
	 * 
	 * \return Value of the `defaultPath` field.
	 */
	std::string* GetDefaultPath();
	/**
	 * \brief Fake getter of an element of the list.
	 * 
	 * Return a pointer of the path found in the list for the index
	 * `materialID`.
	 * Will return a pointer to `defaultPath` if `materialID` is out-of-bound
	 * in the list.
	 * 
	 * \param materialID Index to look at in the list.
	 * \return Material path for the index `materialID`.
	 */
	std::string* GetMaterialPath(unsigned char materialID);
	/**
	 * \brief Getter of `materialsPaths`.
	 * 
	 * Return the value of the `materialsPaths` field, a pointer corresponding
	 * to the array containing all materials paths.
	 * The size of the array can be accessed with `GetNbMaterials()`.
	 * 
	 * \return Value of the `materialsPaths` field.
	 */
	std::string* GetMaterialsPaths();
	/**
	 * \brief Getter of `nbMaterials`.
	 * 
	 * Return the value of the `nbMaterials` field, corresponding to the number
	 * of materials in the array `materialsPaths`.
	 * Should be the size of the array `materialsPaths`.
	 * 
	 * \return Value of the `nbMaterials` field.
	 */
	unsigned char GetNbMaterials();
	/**
	 * \brief Getter of `firstMaterial`.
	 * 
	 * Return the value of the `firstMaterial` field, corresponding to the ID of
	 * the first material of the array `materialsPaths`.
	 * 
	 * \return Value of the `firstMaterial` field.
	 */
	unsigned char GetFirstMaterial();

	/**
	 * \brief Setter of `defaultPath`.
	 * 
	 * Set a new default path.
	 * If some paths were added before or after the list, leading to unknown
	 * path for certain indice, the old one was copied.
	 * This approach can easily be modified to always use the current default
	 * path insted, by adding setting these indice with an empty string in
	 * `SetMaterialPath()` and checking it in `GetMaterialPath()`.
	 * 
	 * \param path New default path.
	 */
	void SetDefaultPath(const std::string& path);
	/**
	 * \brief Setter of `materialsPaths`.
	 * 
	 * Reset the list of materials paths using an array of `nbMaterials`
	 * strings.
	 * 
	 * \param materialsPaths Array of materials paths. Must be of size
	 *      `nbMaterials`.
	 * \param nbMaterials Number of paths in the array (default: 1).
	 * \param firstMaterial ID of the first material of the array (default: 0).
	 */
	void SetMaterialsPaths(std::string* materialsPaths,
			unsigned char nbMaterials = 1, unsigned char firstMaterial = 0);
	/**
	 * \brief Fake setter of a specific material path.
	 * 
	 * Reset a specific material path, accessed using `materialID`.
	 * If `materialID` is out-of-bound, `materialsPaths` will be extended and
	 * all unknown paths will be set to `defaultPath`.
	 * 
	 * \param materialPath New material path.
	 * \param materialID ID of the material to replace.
	 */
	void SetMaterialPath(std::string materialPath, unsigned char materialID);

private:
	/**
	 * \brief Copy the array to `materialsPaths`.
	 * 
	 * Replace `materialsPaths` with a copy of the array `list` of size `size`.
	 * 
	 * \param list Array of strings that will replace `materialsPaths`.
	 * \param size Size of `list`.
	 */
	void CopyList(std::string* list, unsigned char size);
	/**
	 * \brief Delete `materialsPaths` and clean all its values.
	 * 
	 * Delete `materialsPaths` array and reset `nbMaterials` and
	 * `firstMaterial`.
	 * `defaultPath` will not be affected.
	 */
	void CleanList();

	/**
	 * \brief Default path.
	 * 
	 * Default path returned when trying to access an out-of-bound index.
	 */
	std::string defaultPath;
	/**
	 * \brief Array of materials paths.
	 * 
	 * Array of materialsPaths.
	 * Must be of size `nbMaterials`.
	 */
	std::string* materialsPaths = nullptr;
	/**
	 * \brief Number of paths in the array.
	 * 
	 * Number of materials paths stored in `materialsPaths` field.
	 * It is basically the size of this array.
	 */
	unsigned char nbMaterials = 0;
	/**
	 * \brief ID of the first material of the array.
	 * 
	 * ID of the first material of the array. Used to get the real ID of an
	 * index of `materialsPaths`.
	 */
	unsigned char firstMaterial = 0;
};

#endif // MATERIAL_H
