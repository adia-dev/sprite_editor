#pragma once

#include "Maths.h"

namespace se {

	class File {
	  public:
		File() {}
		~File() {}

		static std::string
		Dialog(const std::string& label = "Select a file: ") {
			FILE*       fp;
			int         status;
			char        c_path[PATH_MAX];
			std::string osaCmd =
			    "osascript -e 'set fileAlias to choose file with "
			    "prompt \"" +
			    label +
			    "\"' -e 'set filePath to "
			    "POSIX path of fileAlias'";

			fp = popen(osaCmd.c_str(), "r");
			if (fp == NULL) {
				std::cerr << "Could not execute the command.\n";
				return "";
			}
			/* Handle error */;

			if (fgets(c_path, PATH_MAX, fp) != NULL) {
				c_path[strcspn(c_path, "\n")] = '\0';
			} else {
				std::cerr << "Could not get the file path.\n";
				return "";
			}

			status = pclose(fp);
			if (status == -1) std::cerr << "Error closing the process\n";

			return c_path;
		}

	  private:
	};

} // namespace se
