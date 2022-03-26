#ifndef MODULES_MESSAGE_H
#define MODULES_MESSAGE_H

#include "modules/module.h"

/**
 * \brief Message module for _Dear ImGui_.
 * 
 * Corresponds to a subwindow that only contains a message with no extra
 * button.
 * Can be derivated to add content like buttons, images, title and more.
 */
class MessageModule: public GUIModule
{
public:
	/**
	 * \brief Constructor.
	 * 
	 * `MessageModule` constructor.
	 * 
	 * \param context Application context using this module. It will ask to
	 *      kill it when it is done.
	 * \param message Text to display.
	 */
	MessageModule(void* context, std::string message);
	/**
	 * \brief Destructor.
	 * 
	 * `MessageModule` destructor.
	 */
	~MessageModule();

	/**
	 * \brief Render the module.
	 * 
	 * Call _Dear ImGui_ instructions to render the module.
	 */
	void Render();

	/**
	 * \brief Getter of `message`.
	 * 
	 * Return the value of the `message` attribute, corresponding to the text
	 * displayed as content of the subwindow.
	 */
	const std::string& GetMessage();

protected:
	/**
	 * \brief Text to display.
	 * 
	 * Text displayed as content of the subwindow.
	 */
	std::string message;
	/**
	 * \brief Flags describing style of the _ImGui_ subwindow.
	 * 
	 * Flags describing style of the _ImGui_ subwindow.
	 * Initialized during `Init()`.
	 */
	ImGuiWindowFlags flags;

private:
	/**
	 * \brief Initialize the module.
	 * 
	 * Initialize the module: set _ImGui_ flags for subwindow style.
	 * Will be automatically called during object construction.
	 */
	void Init();
};

/**
 * \brief Loading message module for _Dear ImGui_.
 * 
 * Corresponds to a subwindow that contains a message with a loading animation
 * with no extra button.
 * 
 * Originally designed to be used during file loading.
 */
class LoadingMessageModule: public MessageModule
{
public:
	/**
	 * \brief Constructor.
	 * 
	 * `LoadingMessageModule` constructor.
	 * 
	 * \param context Application context using this module. It will ask to
	 *      kill it when it is done.
	 * \param message Text to display.
	 */
	LoadingMessageModule(void* context, std::string message);
	/**
	 * \brief Destructor.
	 * 
	 * `LoadingMessageModule` destructor.
	 */
	~LoadingMessageModule();

	/**
	 * \brief Render the module.
	 * 
	 * Call _Dear ImGui_ instructions to render the module.
	 */
	void Render();

private:
	/**
	 * \brief Accent color of the loading animation.
	 * 
	 * Accent color of the spinner used as loading animation.
	 * Initialized during object construction.
	 */
	ImU32 accentColor;
	/**
	 * \brief Background color of the loading animation.
	 * 
	 * Background color of the spinner used as loading animation.
	 * Initialized during object construction.
	 */
	ImU32 backgroundColor;
};

/**
 * \brief Processing message module for _Dear ImGui_.
 * 
 * Corresponds to a subwindow that contains a message with a processing
 * animation with no extra button.
 * 
 * Works by using two pointers to integer values, one for current value and
 * another for the expected maximum value. At rendering, these values will be
 * directly read from where they are updated: at each render, the progression
 * is the real progression.
 * 
 * Originally designed to be used during mesh processing.
 */
class ProcessingMessageModule: public MessageModule
{
public:
	/**
	 * \brief Constructor.
	 * 
	 * `ProcessingMessageModule` constructor.
	 * 
	 * \param context Application context using this module. It will ask to
	 *      kill it when it is done.
	 * \param message Text to display.
	 * \param currentValue Pointer to the still-updating current integer value.
	 * \param expectedValue Pointer to the possibly-updating maximum expected
	 *      integer value.
	 */
	ProcessingMessageModule(void* context, std::string message,
			int* currentValue, int* expectedValue);
	/**
	 * \brief Destructor.
	 * 
	 * `ProcessingMessageModule` destructor.
	 */
	~ProcessingMessageModule();

	/**
	 * \brief Render the module.
	 * 
	 * Call _Dear ImGui_ instructions to render the module.
	 */
	void Render();

private:
	/**
	 * \brief Pointer to the current integer value.
	 * 
	 * Pointer to the still-updating current integer value.
	 */
	int* currentValue = nullptr;
	/**
	 * \brief Pointer to the maximum expected integer value.
	 * 
	 * Pointer to the possibly-updating maximum expected integer value.
	 */
	int* expectedValue = nullptr;

	/**
	 * \brief Accent color of the progressing animation.
	 * 
	 * Accent color of the progression bar used as progressing animation.
	 * Initialized during object construction.
	 */
	ImU32 accentColor;
	/**
	 * \brief Background color of the progressing animation.
	 * 
	 * Background color of the progression bar used as progressing animation.
	 * Initialized during object construction.
	 */
	ImU32 backgroundColor;
};

/**
 * \brief Alert message module for _Dear ImGui_.
 * 
 * Corresponds to a subwindow that contains a message with an `OK` button.
 * 
 * Used for example when there are errors during PLY file loading or shaders
 * compilation.
 */
class AlertMessageModule: public MessageModule
{
public:
	/**
	 * \brief Constructor.
	 * 
	 * `AlertMessageModule` constructor.
	 * 
	 * \param context Application context using this module. It will ask to
	 *      kill it when it is done.
	 * \param message Text to display.
	 */
	AlertMessageModule(void* context, std::string message);
	/**
	 * \brief Destructor.
	 * 
	 * `AlertMessageModule` destructor.
	 */
	~AlertMessageModule();

	/**
	 * \brief Render the module.
	 * 
	 * Call _Dear ImGui_ instructions to render the module.
	 */
	void Render();
};

#endif // MODULES_MESSAGE_H
