#pragma once

#include <string>
#include <vector>

namespace HE
{
	namespace Utils
	{
		// String utils
		std::string GetFilename(const std::string& filepath);
		std::string GetExtension(const std::string& filename);
		std::string RemoveExtension(const std::string& filename);
		bool StartsWith(const std::string& string, const std::string& start);
		std::vector<std::string> SplitString(const std::string& string, const std::string& delimiters);
		std::vector<std::string> SplitString(const std::string& string, const char delimiter);
		void ReplaceSlash(std::string& filepath);
	}

	class FileDialog
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}