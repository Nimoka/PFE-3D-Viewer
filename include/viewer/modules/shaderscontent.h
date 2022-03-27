#ifndef MODULES_SHADERSCONTENT_H
#define MODULES_SHADERSCONTENT_H

#include "modules/module.h"
#include "shadersreader.h"

/**
 * \brief Shaders content module for _Dear ImGui_
 * 
 * Module that show some metadata and data about a list of pair of shaders
 * (vertex and fragment shaders).
 * If there are more than a single pair of shaders, all different pairs will be
 * displayed in different subwindows.
 */
class ShadersContentModule: public GUIModule
{
public:
	/**
	 * \brief Constructor.
	 * 
	 * `ShadersContentModule` constructor.
	 * 
	 * \param context Application context using this module. It will ask to
	 *      kill it when it is done.
	 * \param nbShaders Number of pairs of shaders passed as `shaders`.
	 *      Correspond to the size of the array passed as `shaders`.
	 * \param shaders Array of pointers of pairs of shaders (directly to the
	 *      `ShadersReader` objects). Some of data displayed are directly
	 *      accessed from theseobjects, so they must be alive while this module
	 *      is rendered.
	 * \param firstMaterial ID of the first material (default: `0`).
	 */
	ShadersContentModule(void* context,
			unsigned char nbShaders, ShadersReader** shaders,
			unsigned char firstMaterial = 0);
	/**
	 * \brief Constructor by duplication.
	 * 
	 * `ShadersContentModule` constructor using an existing object that would be
	 * duplicated.
	 * 
	 * \param module Module to duplicate.
	 */
	ShadersContentModule(ShadersContentModule* module);
	/**
	 * \brief Destructor.
	 * 
	 * `ShadersContentModule` destructor.
	 */
	~ShadersContentModule();

	/**
	 * \brief Render the module.
	 * 
	 * Call _Dear ImGui_ instructions to render the module.
	 */
	void Render();

	/**
	 * \brief Getter of `nbShaders`
	 * 
	 * Return the value of the `nbShaders` field, corresponding to the number of
	 * pairs of shaders whose content is displayed.
	 * 
	 * \return Value of the `nbShaders` field.
	 */
	unsigned char GetNbShaders();
	/**
	 * \brief Getter of `nbShaders`
	 * 
	 * Return the value of the `shaders` field, corresponding to an array of
	 * pointer of pairs of shaders (directly to the `ShadersReader` objects)
	 * whose content is displayed.
	 * The size of the array can be accessed with `GetNbShaders()`.
	 * 
	 * \return Value of the `nbShaders` field.
	 */
	ShadersReader** GetShaders();
	/**
	 * \brief Getter of `firstMaterial`
	 * 
	 * Return the value of the `firstMaterial` field, corresponding to the ID of
	 * the first material, used to tell for which material is used a pair of
	 * shaders.
	 * 
	 * \return Value of the `firstMaterial` field.
	 */
	unsigned char GetFirstMaterial();

	/**
	 * \brief Setter of `shaders`
	 * 
	 * Set the array of pointers of pairs of shaders (directly to the 
	 * `ShadersReader` objects) as passed as `shaders`. The size of the array
	 * should be `nbShaders`.
	 * Duplicate the array of pointers to avoid `SEGFAULT`.
	 * 
	 * \param nbShaders Number of pairs of shaders passed as `shaders`.
	 *      Correspond to the size of the array passed as `shaders`.
	 * \param shaders Array of pointers of pairs of shaders (directly to the
	 *      `ShadersReader` objects). Some of data displayed are directly
	 *      accessed from theseobjects, so they must be alive while this module
	 *      is rendered.
	 * \param firstMaterial ID of the first material (default: `0`).
	 */
	void SetShaders(unsigned char nbShaders, ShadersReader** shaders,
			unsigned char firstMaterial = 0);

private:
	/**
	 * \brief Delete the array `shaders`.
	 * 
	 * Delete the array `shaders` containing the pointers to pairs of shaders
	 * (directly to the `ShadersReader` objects) and reset the array pointer to
	 * `nullptr`.
	 * The corresponding objects will not be deleted.
	 */
	void CleanShaders();

	/**
	 * \brief Number of pairs of shaders.
	 * 
	 * Size of the `shaders` array.
	 */
	unsigned char nbShaders = 0;
	/**
	 * \brief Array of pairs of shaders.
	 * 
	 * Array of pointers of pairs of shaders (directly to `ShadersReader`
	 * objects).
	 * Its size must not be less than `nbShaders` value.
	 */
	ShadersReader** shaders = nullptr;
	/**
	 * \brief ID of the first material.
	 * 
	 * ID of the first material, used to tell for which material is used a pair
	 * of shaders.
	 */
	unsigned char firstMaterial = 0;
};

#endif // MODULES_SHADERSCONTENT_H
