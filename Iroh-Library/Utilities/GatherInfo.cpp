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
		struct in_addr addr;
		std::string strIP;

		MIB_TCPTABLE_OWNER_PID *pTCPInfo;
		MIB_TCPROW_OWNER_PID* owner;
		DWORD size;

		SIZE_T firstSectionSize;
		std::string firstSection;
		SIZE_T secondSectionSize;
		std::string secondSection;

		if (GetExtendedTcpTable(
			NULL,
			&size,
			false,
			AF_INET,
			TCP_TABLE_BASIC_CONNECTIONS,
			0) != ERROR_INSUFFICIENT_BUFFER)
			return "";

		pTCPInfo = (MIB_TCPTABLE_OWNER_PID*)malloc(size);

		if (GetExtendedTcpTable(
			pTCPInfo,
			&size,
			false,
			AF_INET,
			TCP_TABLE_BASIC_CONNECTIONS,
			0) != NO_ERROR)
			return "";

		for (DWORD i = 0; i < pTCPInfo->dwNumEntries; i++) {
			//
			// Get the IP Address
			//
			owner = &pTCPInfo->table[i];

			addr.s_addr = (long)owner->dwLocalAddr;
			strIP = inet_ntoa(addr);

			//
			// Check if its a valid local address
			//
			firstSectionSize = strIP.find(".", 0);
			firstSection = strIP.substr(0, firstSectionSize);

			if (firstSection == "192") {
				secondSectionSize = strIP.find(".", firstSectionSize + 1);
				secondSection = strIP.substr(firstSectionSize + 1,
					secondSectionSize - firstSectionSize - 1);

				if (secondSection == "168")
					break;

			} else if (firstSection == "172" || firstSection == "10")
				break;
		}

		return strIP;
	}

	std::vector<std::string> GatherInfo::GetSystemIPs() {

		std::vector<std::string> ipList;
		PIP_ADAPTER_INFO adapterInfo;
		ULONG adapterInfoSize, status;

		adapterInfo = NULL;

		if ((status = GetAdaptersInfo(NULL, &adapterInfoSize))
			!= ERROR_BUFFER_OVERFLOW)
			goto Cleanup;

		adapterInfo = reinterpret_cast<PIP_ADAPTER_INFO>(malloc(adapterInfoSize));
		if (adapterInfo == NULL)
			goto Cleanup;

		if ((status = GetAdaptersInfo(adapterInfo, &adapterInfoSize)) 
			!= ERROR_SUCCESS)
			goto Cleanup;

		while (adapterInfo) {
			ipList.push_back(adapterInfo->IpAddressList.IpAddress.String);
			adapterInfo = adapterInfo->Next;
		}
	
	Cleanup:
		if (adapterInfo)
			free(adapterInfo);

		return ipList;
	}

	std::string GatherInfo::GetHostname() {
		TCHAR hostname[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD hostnameSize = sizeof(hostname) / sizeof(hostname[0]);

		if (!GetComputerName(hostname, &hostnameSize))
			return "";

		std::wstring hostnameStr = hostname;
		return StringUtilities::wstrtostr(hostnameStr);
	}

	std::string GatherInfo::GetMACAddr() {
		std::string primaryIP = GatherInfo::GetPrimaryIP();
		std::string MACAddr;
		CHAR* MACChar = (char*)malloc(20);

		PIP_ADAPTER_INFO adapterInfo;
		ULONG adapterInfoSize, status;

		adapterInfo = NULL;

		if ((status = GetAdaptersInfo(NULL, &adapterInfoSize))
			!= ERROR_BUFFER_OVERFLOW)
			goto Cleanup;

		adapterInfo = reinterpret_cast<PIP_ADAPTER_INFO>(malloc(adapterInfoSize));
		if (adapterInfo == NULL)
			goto Cleanup;

		if ((status = GetAdaptersInfo(adapterInfo, &adapterInfoSize))
			!= ERROR_SUCCESS)
			goto Cleanup;

		while (adapterInfo) {
			if (adapterInfo->IpAddressList.IpAddress.String == primaryIP) {
				sprintf_s(MACChar, 20, "%02X:%02X:%02X:%02X:%02X:%02X", 
					adapterInfo->Address[0], adapterInfo->Address[1],
					adapterInfo->Address[2], adapterInfo->Address[3],
					adapterInfo->Address[4], adapterInfo->Address[5]);

				MACAddr = MACChar;
			
				goto Cleanup;
			}

			adapterInfo = adapterInfo->Next;
		}

	Cleanup:
		free(MACChar);

		if (adapterInfo)
			free(adapterInfo);

		return MACAddr;
	}

	std::string GatherInfo::GetWinVer() {


		return std::string();
	}
}
