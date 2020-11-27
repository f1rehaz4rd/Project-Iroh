#ifndef IROH_IROHLIBRARY_IROH_ENABLESERVICES_H
#define IROH_IROHLIBRARY_IROH_ENABLESERVICES_H

#include <Windows.h>

#define DEFAULT_ENABLERDP FALSE
#define DEFAULT_SLEEPTIMER 15000

namespace Iroh {

	class EnableServices {
	public:
		BOOL enableRDP;
		INT sleepTime;

		EnableServices();
		VOID SetEnableRDP(BOOL);
		VOID SetSleepTime(INT);
		VOID Start();

	private:
		VOID ServicesLoop();
		VOID CheckServices();
		DWORD CheckRDP();
	};


}

#endif // !IROH_IROHLIBRARY_IROH_ENABLESERVICES_H
