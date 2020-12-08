#ifndef IROH_IROHLIBRARY_IROH_ENABLESERVICES_H
#define IROH_IROHLIBRARY_IROH_ENABLESERVICES_H

#include <Windows.h>

#define DEFAULT_ENABLERDP FALSE
#define DEFAULT_SLEEPTIMER 15000

namespace Iroh {

	// EnableServices class will ensure that the set services are enabled.
	// This class will halt code execution if the sleep is not set to zero and
	// should be called in a thread if the sleep time is not greater than zero.
	class EnableServices {
	public:
		EnableServices();

		// Set RDP to be enabled when Start is called.
		VOID SetEnableRDP(BOOL);

		// Sets the sleep time between checks.
		VOID SetSleepTime(INT);

		// Starts the execution, make sure to call in a thread if you want 
		// continued code execution.
		VOID Start();

	private:
		// Check if RDP should be enabled
		BOOL enableRDP;
		INT sleepTime;

		// Calls upon CheckServices and then sleeps for set time.
		VOID ServicesLoop();

		// Checks through all the services that have to be enabled.
		VOID CheckServices();
		
		// Checks to see if the RDP service is enabled, and if not
		// it enables it.
		DWORD CheckRDP();
	};

}

#endif // !IROH_IROHLIBRARY_IROH_ENABLESERVICES_H
