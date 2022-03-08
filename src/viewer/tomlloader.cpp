#include "tomlloader.h"

#include "context.h"

bool TOMLLoader::LoadContext(void *c, std::string filepath) {
	try {
		/* Parse TOML file */

		auto data = toml::parse(filepath);

		/* Process options found */

		// Window
		{
			auto& window = toml::find(data, "window");

			std::string title = toml::find_or<std::string>(window, "title", "");
			if (!title.empty())
				this->SetForcedWindowTitle(title);

			int width = toml::find_or<int>(window, "width", 0);
			int height = toml::find_or<int>(window, "height", 0);
			if (width && height) {
				this->SetWindowSize(width, height);
			} else if (width) {
				this->SetWindowSize(width, DEFAULT_WINDOW_HEIGHT);
			} else if (height) {
				this->SetWindowSize(DEFAULT_WINDOW_WIDTH, height);
			}
		}
	} catch (const std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		return false;
	} catch (const toml::syntax_error &e) {
		std::cerr << "Syntax error found in file ‘" << filepath << "’.";
		return false;
	} catch (...) {
		return false;
	}

	return true;
}