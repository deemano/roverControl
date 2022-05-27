#include <iostream>
#include "stdafx.h"
#include "FA_API.h"
#include <windows.h>

#define VK_KEY_W	0x57    //  ('W')	    W   Forward
#define VK_KEY_A	0x41    //  ('A')	    A   Left
#define VK_KEY_D	0x44    //  ('D')	    D   Right  
#define VK_KEY_S	0x53    //  ('S')	    S   Backward 

using namespace std;
int iPort = 5;

void first() {
	bool temp180 = false;
	if (GetAsyncKeyState(VK_KEY_W) < 0) { // Trigger event with 'T' keypress
		if (!temp180) {
			temp180 = true;
			FA_Right(iPort, 5);
			Sleep(250);
		}

	}
	else { // Detect key release & close event
		if (temp180) {
			temp180 = false;
			cout << "Executed 1111\n";
		}
	}
};

void second() {
	bool temp180 = false;
	if (GetAsyncKeyState(VK_KEY_S) < 0) { // Trigger event with 'T' keypress
		if (!temp180) {
			temp180 = true;
			temp180 = true;
			FA_Left(iPort, -5);
			Sleep(250);
		}

	}
	else { // Detect key release & close event
		if (temp180) {
			temp180 = false;
			cout << "Executed 2222\n";
		}
	}
};

int main()
{
	char PortNumber = 5;

	// Open Port
	FA_ComOpen(PortNumber);

	first();
	second();

	// Close Port
	FA_ComClose(PortNumber);

	return 0;
}


