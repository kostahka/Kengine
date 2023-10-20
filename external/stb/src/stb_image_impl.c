#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO

#ifdef _WIN32 || _WIN64
#define STBI_WINDOWS_UTF8
#endif

#include "stb_image.h"