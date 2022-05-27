
#include "stdafx.h"
#include <iostream>
#include "FA_API.h"
#include <windows.h>

#define VK_KEY_W	0x57    //  ('W')	    W   Forward
#define VK_KEY_A	0x41    //  ('A')	    A   Left
#define VK_KEY_D	0x44    //  ('D')	    D   Right  
#define VK_KEY_S	0x53    //  ('S')	    S   Backward 

using namespace std;

int iPort = 5;
bool down = false; // boolean variable to differentiate between key-pressed / key-released
bool good2go = true;
void first() {
	bool temp180 = false;
	if (GetAsyncKeyState(VK_KEY_W) < 0) { // Trigger event with 'T' keypress
		if (!temp180) {
			temp180 = true;
			int initial = 0;
			while (!down) {

				int front = FA_ReadIR(iPort, 2);
				int left = FA_ReadIR(iPort, 0);
				int back = FA_ReadIR(iPort, 6);
				int frontLeft = FA_ReadIR(iPort, 1);
				int frontRight = FA_ReadIR(iPort, 3);
				int backLeft = FA_ReadIR(iPort, 7);
				int backRight = FA_ReadIR(iPort, 5);
				//cout << "Front IR: " << front << "\n";
				//cout << "Left: " << left << "\n";
				//cout << "frontLeft: " << frontLeft << "\n";
				//cout << "frontRight: " << frontRight << "\n";
				//cout << "backLeft: " << backLeft << "\n";
				int check = (front + frontLeft + frontRight) / 3;
				//cout << "\n\ncheck: " << check << "\n\n";
				if (front < 500 && frontLeft < 500 && initial == 0) { // if no object in front and front left - drive
					cout << "Looking for an object...\n";

					FA_SetMotors(iPort, 10, 10);
					if (front > 500) {
						initial++;
					}
				}
				if (front > 500) { // if object in front
					good2go = false;
					cout << "Object found. Stopping to evaluate ...\n";
					FA_SetMotors(iPort, 0, 0);
					FA_Backwards(iPort, 10);
					cout << "Turning around object.\n";
					FA_SetMotors(iPort, 10, -10);
					///FA_Right(iPort, 5); // turn til no more object in front
					Sleep(500);
					FA_SetMotors(iPort, 0, 0);
				}
				if (frontLeft > 500) { // if object in front-left
					cout << "Object located in the front-left ...\n";
					FA_Right(iPort, 4); // turn right a bit
					Sleep(500);
				}

				if (frontLeft < 500 && left > 400) { // if no object in front-left but object in left
					cout << "Object detected in the left side ... ";
					FA_Left(iPort, 2);
					FA_Forwards(iPort, 30);
					Sleep(500);
				}
				if (left > 600 && frontLeft > 600) {
					cout << "\ngoing a bit right to follow the object\n";
					FA_Right(iPort, 5);
					FA_SetMotors(iPort, 10, 10);
				}
				if (left < 250 && frontLeft < 250 && front < 500 && good2go == false) {
					cout << "\nTurning left arround the object.\n";
					FA_Left(iPort, 8);
					Sleep(500);
					FA_SetMotors(iPort, 10, 10);
					cout << "\nScanning ...\n";
				}
				else if (left > 600 && frontLeft > 600 && left < 250) {
					cout << "\n HERE 66666\n";
					FA_SetMotors(iPort, 10, 10);
				}

				if (GetAsyncKeyState(VK_KEY_S) < 0) {
					if (!down) {
						cout << "Self-drive mode disactivated.\nNormal drive mode active." << "\n";
						down = true;
						good2go = false;
						FA_SetMotors(iPort, 0, 0);
						return; // maybe ?
					}
				}
			}
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
			//FA_Left(iPort, -5);
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
	FA_ComOpen(PortNumber);              // COM port stream function (external)
	FA_PlayNote(PortNumber, 500, 100);   // sound confirmation for connection established
	Sleep(150);
	while (true) {
		first();
		second();
	}


	// Close Port
	FA_ComClose(PortNumber);

	return 0;
}


