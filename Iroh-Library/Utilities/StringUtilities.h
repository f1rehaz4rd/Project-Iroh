#ifndef IROH_IROHLIBRARY_UTILITIES_STRINGUTILITIES_H
#define IROH_IROHLIBRARY_UTILITIES_STRINGUTILITIES_H	

#include <codecvt>
#include <locale>
#include <string>

namespace Utilities {

	// StringUtilities is a simple class to do some conversions
	// so that the code doesn't have to be repeatedly written in
	// multiple places.
	class StringUtilities {
	public:
		// Converts std::wstring to std::string
		static std::string wstrtostr(std::wstring wstr);

		// Converts std::string to std::wstring
		static std::wstring strtowstr(std::string str);
};
}

#endif // !IROH_IROHLIBRARY_UTILITIES_STRINGUTILITIES_H
