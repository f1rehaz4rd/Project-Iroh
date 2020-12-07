#ifndef IROH_IROHLIBRARY_IROH_FIREWALL_H
#define IROH_IROHLIBRARY_IROH_FIREWALL_H

#include <Windows.h>
#include <atlcomcli.h>
#include <comutil.h>
#include <netfw.h>

#include <string>
#include <vector>

#define NET_FW_IP_PROTOCOL_TCP_NAME L"TCP"
#define NET_FW_IP_PROTOCOL_UDP_NAME L"UDP"

#define NET_FW_RULE_DIR_IN_NAME L"In"
#define NET_FW_RULE_DIR_OUT_NAME L"Out"

#define NET_FW_RULE_ACTION_BLOCK_NAME L"Block"
#define NET_FW_RULE_ACTION_ALLOW_NAME L"Allow"

#define NET_FW_RULE_ENABLE_IN_NAME L"TRUE"
#define NET_FW_RULE_DISABLE_IN_NAME L"FALSE"

#define DEFAULT_SLEEPTIMER 15000

namespace Iroh {

	class Firewall {

	public:
		//
		// Struct to hold all the firewall rule data.
		//
		struct FirewallRule {
			std::wstring ruleName;
			std::wstring ruleDescription;
			BOOL enabled = VARIANT_TRUE;
			std::wstring localPorts;
			NET_FW_IP_PROTOCOL protocol;
			NET_FW_RULE_DIRECTION direction;
		};
		std::vector<FirewallRule> listOfRules;

		Firewall();
		VOID SetSleepTime(INT);
		VOID AddRule(FirewallRule);
		VOID Start();

	private:
		// List of firewall rules that need to be checked.
		INT sleepTime;

		VOID FirewallLoop();
		VOID InsertRules();
		HRESULT WFCOMInitialize(INetFwPolicy2**);
		DWORD DumpFWRulesInCollection(INetFwRule*, FirewallRule*);
		DWORD CheckFirewall(FirewallRule*);
		DWORD InsertFirewallRule(FirewallRule*);
	};

}

#endif // !IROH_IROHLIBRARY_IROH_FIREWALL_H
