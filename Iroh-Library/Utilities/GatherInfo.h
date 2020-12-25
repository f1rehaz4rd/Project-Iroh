#ifndef IROH_IROHLIBRARY_UTILITIES_GATHERINFO_H
#define IROH_IROHLIBRARY_UTILITIES_GATHERINFO_H	

#include <Windows.h>

#include <string>
#include <vector>

#include "StringUtilities.h"

#define GUIDREGISTRYPATH L"SOFTWARE\\Microsoft\\Cryptography"
#define MACHINEGUID L"MachineGuid"

namespace Utilities {

	class GatherInfo {
	public:
		static std::string GetMachineGUID();
		static std::string GetPrimaryIP();
		static std::vector<std::string> GetSystemIPs();
		static std::string GetHostname();
		static std::string GetMACAddr();
		static std::string GetWinVer();
	};
}

#endif // !IROH_IROHLIBRARY_UTILITIES_GATHERINFO_H
