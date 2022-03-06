#include "filewithextension.h"

FileWithExtension::FileWithExtension(std::string extension,
		const std::string &validator_name) : Validator(validator_name) {
	std::string capitalized = std::string{};
	std::stringstream out;
	for (int i = 0; i < extension.length(); i++)
		out << (char)toupper(extension[i]);
	capitalized = out.str();

	if (validator_name.empty())
		description(capitalized + "_EXTENSION");

	func_ = [extension, capitalized](std::string &input) {
		std::string newExtension = "." + extension;
		// TRICKY: If there's a difference between the extension and the end of
		// the file, compare will return a number greater than 0, which is seen
		// as the boolean true, while 0 is seen as the boolean false
		if (input.compare(input.length() - newExtension.length(),
				newExtension.length(), newExtension)) {
			return "The file '" + input + "' is not a " + capitalized
					+ " file!";
		}
		return std::string();
	};
}
