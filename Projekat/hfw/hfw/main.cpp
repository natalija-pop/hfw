#include <iostream>
#include  "rule_enumerations.h"
#include  "general_fw_settings.h"
#include "rule_settings.h"

using namespace std;

#define APP_TITLE "Host-based firewall administration"

int __cdecl main()
{
	cout << APP_TITLE << "\n\n\n";

	int menuItem = -1;

	while(menuItem != 0)
	{
		cout << "Menu\n"
			<< "1. Enumerate inbound rules\n"
			<< "2. Enumerate outbound rules\n"
			<< "3. Enable profile\n"
			<< "4. Disable profile\n"
			<< "5. Create new rule\n"
			<< "6. Disable firewall\n"
			<< "7. Get firewall settings\n"
			<< "0. Exit\n";

		cin >> menuItem;
		switch (menuItem)
		{
			case 0: cout << "Program is shutting down...\n"; break;

			case 1: EnumerateFWRules(NET_FW_RULE_DIR_IN); break;

			case 2: EnumerateFWRules(NET_FW_RULE_DIR_OUT); break;

			case 3: ManageProfileFwStateMenu(true); break;

			case 4: ManageProfileFwStateMenu(false); break;

			case 5: AddNewRule(); break;

			case 6: DisableWindowsFirewall(); break;

			case 7: GetFirewallSettings(); break;

			default: cout << "Invalid menu item selected. Try again.\n"; break;
		}
	}
	return 0;
}