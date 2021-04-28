#include "Firewall.h"

namespace Iroh {

	Firewall::Firewall() : sleepTime(DEFAULT_SLEEPTIMER), listOfRules()
	{ /* No functionality here */ }

	VOID Firewall::SetSleepTime(INT timeInMilli) {
		this->sleepTime = timeInMilli;
	}

	VOID Firewall::AddRule(FirewallRule newRule)
	{
		this->listOfRules.push_back(newRule);
	}

	VOID Firewall::Start() {

		if (this->listOfRules.size() > 0) {
			if (this->sleepTime == 0)
				this->InsertRules();
			else
				this->FirewallLoop();
		}

	}

	VOID Firewall::FirewallLoop() {

		while (TRUE) {
			this->InsertRules();
			Sleep(this->sleepTime);
		}

	}

	VOID Firewall::InsertRules() {
		
		for (size_t i = 0; i < this->listOfRules.size(); i++) {
			this->InsertFirewallRule(&(this->listOfRules.at(i)));
		}

	}

	HRESULT Firewall::WFCOMInitialize(INetFwPolicy2** ppNetFwPolicy2) {

		HRESULT hr = S_OK;

		hr = CoCreateInstance(
			__uuidof(NetFwPolicy2),
			NULL,
			CLSCTX_INPROC_SERVER,
			__uuidof(INetFwPolicy2),
			(void**)ppNetFwPolicy2);

		if (FAILED(hr))
		{
			goto Cleanup;
		}

	Cleanup:
		return hr;

	}

	DWORD Firewall::FWRuleCompare(INetFwRule* FwRule, FirewallRule* firewallRule) {
		
		variant_t InterfaceArray;
		variant_t InterfaceString;

		VARIANT_BOOL bEnabled;
		BSTR bstrVal;

		long lVal = 0;
		long lProfileBitmask = 0;

		NET_FW_RULE_DIRECTION fwDirection;
		NET_FW_ACTION fwAction;
		long protocol;
		VARIANT_BOOL isEnabled;

		struct ProfileMapElement
		{
			NET_FW_PROFILE_TYPE2 Id;
			LPCWSTR Name;
		};

		ProfileMapElement ProfileMap[3];
		ProfileMap[0].Id = NET_FW_PROFILE2_DOMAIN;
		ProfileMap[0].Name = L"Domain";
		ProfileMap[1].Id = NET_FW_PROFILE2_PRIVATE;
		ProfileMap[1].Name = L"Private";
		ProfileMap[2].Id = NET_FW_PROFILE2_PUBLIC;
		ProfileMap[2].Name = L"Public";

		INT ctr = 0;
		 
		if (SUCCEEDED(FwRule->get_Name(&bstrVal)))
		{
			if (wcscmp(bstrVal, (const wchar_t*)firewallRule->ruleName.c_str()) != 0)
				return 0;
		}

		if (SUCCEEDED(FwRule->get_Description(&bstrVal))) {
			if (bstrVal != NULL) {
				if (wcscmp(bstrVal, (const wchar_t*)firewallRule->ruleDescription.c_str()) != 0)
					return 0;
			}
		}

		if (SUCCEEDED(FwRule->get_Direction(&fwDirection)))
		{
			if (fwDirection != firewallRule->direction)
				return 0;
		}

		if (SUCCEEDED(FwRule->get_Action(&fwAction)))
		{
			switch (fwAction)
			{
			case NET_FW_ACTION_BLOCK:
				return 0;
			case NET_FW_ACTION_ALLOW:
				break;
			default:
				break;
			}
		}

		if (SUCCEEDED(FwRule->get_LocalPorts(&bstrVal))) {
			if (bstrVal != NULL) {
				if (wcscmp(bstrVal, (const wchar_t*)firewallRule->localPorts.c_str()) != 0)
					return 0;
			}
		}

		if (SUCCEEDED(FwRule->get_Protocol(&protocol))) {
			if (protocol != firewallRule->protocol)
				return 0;
		}

		if (SUCCEEDED(FwRule->get_Enabled(&isEnabled))) {
			if (isEnabled != firewallRule->enabled)
				FwRule->put_Enabled(firewallRule->enabled);
		}

		return 1;
	}

	DWORD Firewall::CheckFirewall(FirewallRule* firewallRule) {
	
		DWORD result = 0;
		
		HRESULT hrComInit = S_OK;
		HRESULT hr = S_OK;

		ULONG cFetched = 0;
		CComVariant var;

		IUnknown* pEnumerator;
		IEnumVARIANT* pVariant = NULL;

		INetFwPolicy2* pNetFwPolicy2 = NULL;
		INetFwRules* pFwRules = NULL;
		INetFwRule* pFwRule = NULL;

		long fwRuleCount;

		// Initialize COM.
		hrComInit = CoInitializeEx(
			0,
			COINIT_APARTMENTTHREADED
			);

		// Ignore RPC_E_CHANGED_MODE; this just means that COM has already been
		// initialized with a different mode. Since we don't care what the mode is,
		// we'll just use the existing mode.
		if (hrComInit != RPC_E_CHANGED_MODE)
		{
			if (FAILED(hrComInit))
			{
				wprintf(L"CoInitializeEx failed: 0x%08lx\n", hrComInit);
				goto Cleanup;
			}
		}

		// Retrieve INetFwPolicy2
		hr = WFCOMInitialize(&pNetFwPolicy2);
		if (FAILED(hr))
		{
			goto Cleanup;
		}

		// Retrieve INetFwRules
		hr = pNetFwPolicy2->get_Rules(&pFwRules);
		if (FAILED(hr))
		{
			wprintf(L"get_Rules failed: 0x%08lx\n", hr);
			goto Cleanup;
		}

		// Obtain the number of Firewall rules
		hr = pFwRules->get_Count(&fwRuleCount);
		if (FAILED(hr))
		{
			wprintf(L"get_Count failed: 0x%08lx\n", hr);
			goto Cleanup;
		}

		// Iterate through all of the rules in pFwRules
		pFwRules->get__NewEnum(&pEnumerator);

		if (pEnumerator)
		{
			hr = pEnumerator->QueryInterface(__uuidof(IEnumVARIANT), (void**)&pVariant);
		}

		while (SUCCEEDED(hr) && hr != S_FALSE)
		{
			var.Clear();
			hr = pVariant->Next(1, &var, &cFetched);

			if (S_FALSE != hr)
			{
				if (SUCCEEDED(hr))
				{
					hr = var.ChangeType(VT_DISPATCH);
				}
				if (SUCCEEDED(hr))
				{
					hr = (V_DISPATCH(&var))->QueryInterface(__uuidof(INetFwRule), reinterpret_cast<void**>(&pFwRule));
				}

				if (SUCCEEDED(hr))
				{
					// Output the properties of this rule
					result = FWRuleCompare(pFwRule, firewallRule);
					if (result == 1)
						goto Cleanup;
				}
			}
		}

	Cleanup:
		var.Clear();

		// Release pFwRule
		if (pFwRule != NULL)
		{
			pFwRule->Release();
		}

		// Release INetFwPolicy2
		if (pNetFwPolicy2 != NULL)
		{
			pNetFwPolicy2->Release();
		}

		// Uninitialize COM.
		if (SUCCEEDED(hrComInit))
		{
			CoUninitialize();
		}

		return result;
	}

	DWORD Firewall::InsertFirewallRule(FirewallRule* firewallRule) {
		
		HRESULT hrComInit = S_OK;
		HRESULT hr = S_OK;
		INetFwPolicy2* pNetFwPolicy2 = NULL;
		INetFwRules* pFwRules = NULL;
		INetFwRule* pFwRule = NULL;
		long CurrentProfilesBitMask = 0;

		BSTR bstrRuleName = SysAllocString(firewallRule->ruleName.c_str());
		BSTR bstrRuleDescription = SysAllocString(firewallRule->ruleDescription.c_str());
		BSTR bstrRuleLPorts = SysAllocString(firewallRule->localPorts.c_str());
		
		if (CheckFirewall(firewallRule) == 1)
			goto Cleanup;

		// Initialize COM.
		hrComInit = CoInitializeEx(
			0,
			COINIT_APARTMENTTHREADED
			);

		// Ignore RPC_E_CHANGED_MODE; this just means that COM has already been
		// initialized with a different mode. Since we don't care what the mode is,
		// we'll just use the existing mode.
		if (hrComInit != RPC_E_CHANGED_MODE)
		{
			if (FAILED(hrComInit))
				goto Cleanup;
		}

		// Retrieve INetFwPolicy2
		hr = WFCOMInitialize(&pNetFwPolicy2);
		if (FAILED(hr))
			goto Cleanup;

		// Retrieve INetFwRules
		hr = pNetFwPolicy2->get_Rules(&pFwRules);
		if (FAILED(hr))
		{
			printf("get_Rules failed: 0x%08lx\n", hr);
			goto Cleanup;
		}

		// Retrieve Current Profiles bitmask
		hr = pNetFwPolicy2->get_CurrentProfileTypes(&CurrentProfilesBitMask);
		if (FAILED(hr))
		{
			printf("get_CurrentProfileTypes failed: 0x%08lx\n", hr);
			goto Cleanup;
		}

		// Create a new Firewall Rule object.
		hr = CoCreateInstance(
			__uuidof(NetFwRule),
			NULL,
			CLSCTX_INPROC_SERVER,
			__uuidof(INetFwRule),
			(void**)&pFwRule);
		if (FAILED(hr))
		{
			printf("CoCreateInstance for Firewall Rule failed: 0x%08lx\n", hr);
			goto Cleanup;
		}
		
		// Populate the Firewall Rule object
		pFwRule->put_Name(bstrRuleName);
		pFwRule->put_Description(bstrRuleDescription);
		pFwRule->put_Protocol(firewallRule->protocol);
		pFwRule->put_LocalPorts(bstrRuleLPorts);
		pFwRule->put_Direction(firewallRule->direction);
		pFwRule->put_Profiles(CurrentProfilesBitMask);
		pFwRule->put_Action(NET_FW_ACTION_ALLOW);
		pFwRule->put_Enabled(firewallRule->enabled);

		// Add the Firewall Rule
		hr = pFwRules->Add(pFwRule);
		if (FAILED(hr))
		{
			printf("Firewall Rule Add failed: 0x%08lx\n", hr);
			goto Cleanup;
		}

	Cleanup:

		// Free BSTR's
		SysFreeString(bstrRuleName);
		SysFreeString(bstrRuleDescription);
		SysFreeString(bstrRuleLPorts);

		// Release the INetFwRule object
		if (pFwRule != NULL)
			pFwRule->Release();

		// Release the INetFwRules object
		if (pFwRules != NULL)
			pFwRules->Release();

		// Release the INetFwPolicy2 object
		if (pNetFwPolicy2 != NULL)
			pNetFwPolicy2->Release();

		// Uninitialize COM.
		if (SUCCEEDED(hrComInit))
			CoUninitialize();

		return 0;

	}

}