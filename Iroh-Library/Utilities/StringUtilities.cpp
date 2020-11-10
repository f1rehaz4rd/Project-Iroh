#include "StringUtilities.h"

namespace Utilities {
	
	std::string StringUtilities::wstrtostr(std::wstring wstr) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.to_bytes(wstr);
	}


	std::wstring StringUtilities::strtowstr(std::string str) {
		std::wstring wstr(str.begin(), str.end());
		return wstr;
	}
}
