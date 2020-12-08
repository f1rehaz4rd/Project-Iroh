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

	// Firewall class loops through the vector of firewall rules given and
	// ensure that they are on the system to always allow traffic through.
	// This class will halt code execution if the sleep is not set to zero and
	// should be called in a thread if the sleep time is not greater than zero.
	class Firewall {

	// TODO: Make a CreateFirewallRule function for the struct.
	public:
		// Struct to hold all the firewall rule data.
		struct FirewallRule {
			std::wstring ruleName;
			std::wstring ruleDescription;
			BOOL enabled = VARIANT_TRUE;
			std::wstring localPorts;
			NET_FW_IP_PROTOCOL protocol;
			NET_FW_RULE_DIRECTION direction;
		};

		Firewall();

		// Sets the sleep time between checks.
		VOID SetSleepTime(INT);

		// Adds a firewall rule to the list
		VOID AddRule(FirewallRule);

		// Starts the execution, make sure to call in a thread if you want 
		// continued code execution.
		VOID Start();

	private:
		// List of firewall rules that need to be checked.
		std::vector<FirewallRule> listOfRules;
		INT sleepTime;

		// Calls upon the InsertRules function and then 
		// sleeps for the set time.
		VOID FirewallLoop();

		// Loops through the list of firewall rules and calls 
		// InsertFirewallRule to handle checking and inserting.
		VOID InsertRules();

		// Initializes the WFCOM object
		HRESULT WFCOMInitialize(INetFwPolicy2**);

		// Checks the INetFwRule against the FirewallRule that are passed.
		// If the rule is there but, no the same as the FirewallRule->enabled
		// the function sets it to whatever value FirewallRule->enabled is.
		// Return: 1 for true, 0 for false
		DWORD FWRuleCompare(INetFwRule*, FirewallRule*);
		
		// Checks the firewall rules that are currently on the system
		// against the passed rule.
		// Return: 1 for true, 0 for false
		DWORD CheckFirewall(FirewallRule*);

		// Inserts the passed firewall rule in the firewall. It checks to
		// see if the rule is there already before inserting.
		DWORD InsertFirewallRule(FirewallRule*);
	};

}

#endif // !IROH_IROHLIBRARY_IROH_FIREWALL_H
