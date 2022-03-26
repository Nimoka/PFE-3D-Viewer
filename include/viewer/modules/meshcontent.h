#ifndef MODULES_MESHCONTENT_H
#define MODULES_MESHCONTENT_H

#include "mesh.h"
#include "modules/module.h"

/**
 * \brief Mesh content module for _Dear ImGui_.
 * 
 * Module that show some metadata and data about the mesh currently loaded.
 */
class MeshContentModule: public GUIModule
{
public:
	/**
	 * \brief Constructor.
	 * 
	 * `MeshContentModule` constructor.
	 * 
	 * \param context Application context using this module. It will ask to
	 *      kill it when it is done.
	 * \param filename Filename of the mesh, displayed as a metadata.
	 *      _Was used originally for the title of the subwindow (for first
	 *      PLY reader demo)._
	 * \param mesh Mesh object where are stored data. Some of data displayed are
	 *      directly accessed from it, so it must be alive while this module
	 *      is rendered.
	 */
	MeshContentModule(void* context, std::string filename, Mesh* mesh);
	/**
	 * \brief Constructor by duplication.
	 * 
	 * `MeshContentModule` constructor using an existing object that would be
	 * duplicated.
	 * 
	 * \param module Module to duplicate.
	 */
	MeshContentModule(MeshContentModule* module);
	/**
	 * \brief Destructor.
	 * 
	 * `MeshContentModule` destructor.
	 */
	~MeshContentModule();

	/**
	 * \brief Initialize the module.
	 * 
	 * Initialize the module: set _ImGui_ flags for table style.
	 * Will be automatically called during object construction.
	 */
	void Init();
	/**
	 * \brief Render the module.
	 * 
	 * Call _Dear ImGui_ instructions to render the module.
	 */
	void Render();

	/**
	 * \brief Getter of `mesh`.
	 * 
	 * Return the value of the `mesh` field, a pointer corresponding to the mesh
	 * whose content is displayed.
	 * 
	 * \return Value of the `mesh` field.
	 */
	Mesh* GetMesh();
	/**
	 * \brief Getter of `filename`.
	 * 
	 * Return the value of the `filename` field, corresponding to the filename
	 * of the mesh as it was passed during construction.
	 */
	const std::string& GetFilename();

private:
	/**
	 * \brief Mesh whose content is displayed.
	 * 
	 * Pointer to the mesh whose content is displayed.
	 * The object will not be deleted during destruction.
	 */
	Mesh* mesh = nullptr;
	/**
	 * \brief Filename of the mesh.
	 * 
	 * Filename of the mesh as it was passed during construction.
	 */
	std::string filename;

	/**
	 * \brief Flags describing style of the _ImGui_ tables.
	 * 
	 * Flags describing style of the _ImGui_ tables.
	 * Initialized during `Init()`.
	 */
	ImGuiTableFlags tableFlags;
};

#endif // MODULES_MESHCONTENT_H
