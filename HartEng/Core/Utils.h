#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace HE
{
	class FileDialog
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}

#endif UTILS_H 