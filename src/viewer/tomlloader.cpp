#include "tomlloader.h"

#include "context.h"

bool TOMLLoader::LoadContext(void *c, std::string filepath) {
	Context *context = (Context*)c;
	bool errorEncountered = false;
	std::string errorMessage = "";
	try {
		/* Parse TOML file */

		auto data = toml::parse(filepath);

		/* Process options found */

		// Window
		{
			if (data.contains("window")) {
				auto& window = toml::find(data, "window");
				if (window.contains("title")) {
					std::string title = toml::find_or<std::string>(window,
																   "title", "");
					if (!title.empty())
						context->SetForcedWindowTitle(title);
					else {
						errorMessage += "  - Bad value found for the field ‘title’.\n";
						errorEncountered = true;
					}
				}

				int width = toml::find_or<int>(window, "width", 0);
				int height = toml::find_or<int>(window, "height", 0);
				if (!width && window.contains("width")) {
					errorMessage += "  - Bad value found for the field ‘width’.\n";
					errorEncountered = true;
				}
				if (!height && window.contains("height")) {
					errorMessage += "  - Bad value found for the field ‘height’.\n";
					errorEncountered = true;
				}
				if (width && height)
					context->SetWindowSize(width, height);
				else if (width)
					context->SetWindowSize(width, DEFAULT_WINDOW_HEIGHT);
				else if (height)
					context->SetWindowSize(DEFAULT_WINDOW_WIDTH, height);
			}
		}
	} catch (const std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		return false;
	} catch (const toml::syntax_error &e) {
		std::cerr << "Syntax error found in file ‘" << filepath << "’: "
				  << e.what() << "." << std::endl;
		return false;
	} catch (const std::out_of_range &e) {
		std::cerr << e.what() << std::endl;
		return false;
	} catch (...) {
		std::cerr << "Unknown error encoutered while loading file ‘"
				  << filepath << "’" << std::endl;
		return false;
	}

	if (errorEncountered) {
		std::cerr << "One or several errors have been encountered while "
				  << "loading the file ‘" << filepath << "’:" << std::endl
				  << errorMessage;
	}

	return !errorEncountered;
}