#include "GatherInfo.h"

namespace Utilities {
	std::string GatherInfo::GetMachineGUID() {
		wchar_t buf[128];
		DWORD bufLen = sizeof(buf) / sizeof(wchar_t);

		std::wstring MachineGuid;

		LONG err;
		if ((err = RegGetValueW(
			HKEY_LOCAL_MACHINE,
			GUIDREGISTRYPATH,
			MACHINEGUID,
			RRF_RT_REG_SZ,
			NULL,
			&buf,
			&bufLen)) != ERROR_SUCCESS) {
			return "";
		}

		MachineGuid = buf;
		return StringUtilities::wstrtostr(MachineGuid);
	}

	std::string GatherInfo::GetPrimaryIP() {
		
		
		return std::string();
	}

	std::vector<std::string> GatherInfo::GetSystemIPs() {


		return std::vector<std::string>();
	}

	std::string GatherInfo::GetHostname() {


		return std::string();
	}

	std::string GatherInfo::GetMACAddr() {
		
		
		
		return std::string();
	}

	std::string GatherInfo::GetWinVer() {


		return std::string();
	}
}
