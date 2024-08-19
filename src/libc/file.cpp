
internal char *
load_file(char *path, Arena *a)
{
	FILE *f = fopen(path, "r");
	if (!f) {
		print_error("failed to open shader path");
		return false;
	}

	U32 flength = 0;
	fseek(f, 0, SEEK_END);
	flength = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *fbuffer = (char *)calloc(flength + 1, sizeof(char));
	if (!fbuffer) {
		print_error("failed to allocate memory for shader");
		return false;
	}
	U32 bytesread = fread(fbuffer, sizeof(char), flength, f);
	if (bytesread < flength) {
		print_error("reading file failed");
		free(fbuffer);
		return false;
	}

}
