#include "Filesystem.hpp"

#include <stdio.h>
#include <stdlib.h>

namespace Engine {
namespace fs {
	const char* readFile(const char* filePath) {
		FILE *f = fopen(filePath, "rb");
		if (f == NULL) {
			fprintf(stderr, "failed to open file %s\n", filePath);
			return NULL;
		};

		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET); 

		char* string = (char*)malloc(fsize + 1);

		fread(string, fsize, 1, f);
		fclose(f);

		string[fsize] = '\0';

		return string;
	}
}
}

