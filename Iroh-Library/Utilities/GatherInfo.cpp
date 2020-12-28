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
		char hostname[256];
		WSADATA wsaData;

		PDNS_RECORD pDnsRecord;
		DNS_STATUS dnsStatus;

		IN_ADDR ipAddr;
		std::string strIP;

		if (WSAStartup(0x101, &wsaData) != 0) 
			return "";

		gethostname(hostname, 256);
		
		dnsStatus = DnsQuery_A(
			(PCSTR)hostname,
			DNS_TYPE_A,
			DNS_QUERY_STANDARD,
			NULL,
			&pDnsRecord,
			NULL);

		if (dnsStatus) {
			printf("Failed %d \n", dnsStatus);
			return "";
		}
		
		ipAddr.S_un.S_addr = (pDnsRecord->Data.A.IpAddress);
		strIP = inet_ntoa(ipAddr);

		WSACleanup();
		DnsRecordListFree(pDnsRecord, DnsFreeRecordList);

		return strIP;
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
