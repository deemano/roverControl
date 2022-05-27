#pragma once
#include "stdafx.h"
#include "FA_API.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <math.h>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>

using namespace std;

// define symbolic constant names & hexadecimal values to identify keypresses
#define VK_KEY_W	0x57    //  ('W')	    W   forward
#define VK_KEY_A	0x41    //  ('A')	    A   left
#define VK_KEY_D	0x44    //  ('D')	    D   right  
#define VK_KEY_S	0x53    //  ('S')	    S   backward 
#define VK_KEY_Q	0x51    //  ('Q')	    Q   cancel reverse keys
#define VK_KEY_Y    0x59    //  ('Y')	    Y   Accelerometer data read
#define VK_KEY_O	0x4F    //  ('O')	    O   Buttom Line sensor read
#define VK_KEY_T	0x54    //  ('T')       T   Turn 180 degrees
#define VK_KEY_U	0x55    //  ('U')	    U   Compass bear data read
#define VK_KEY_I	0x49    //  ('I')	    I   Light sensor data read
#define VK_KEY_P	0x50    //  ('P')	    P   IR distance sensor read
#define VK_KEY_0	0x30    //  ('0')	    0   Turn all x8 LED On (This is zero not the letter 'O')
#define VK_KEY_9	0x39    //  ('9')	    9   Turn all x8 LED Off
#define VK_KEY_R	0x52    //  ('R')	    R   Reset safety stop
#define VK_KEY_X	0x58    //  ('X')	    X   Restart COM port autoscan
#define VK_KEY_L	0x4C    //  ('L')	    L   Read 3D accelerometer data
#define VK_KEY_K	0x4B    //  ('K')	    K   Read actuators encoder data
#define VK_KEY_J	0x4A    //  ('J')	    J   Resets motors encoders
#define VK_KEY_B	0x42    //  ('B')	    B	Space Speed booster
#define VK_KEY_M	0x4D    // ('M')	    M   Enter Self-Drive Mode
#define VK_KEY_Z	0x5A    //  ('Z')	    Z   Exit Self_drive Mode
#define VK_RMENU	0xA5	//  (Right ALT))    Cancel drivng back-right / back-left feature


// Base class - contains default COM-port number. Other device info to be added in the future.
class DeviceMain {
private:
    string series = "FA1037xx"; // Future dev: xx gets overridden when we can get device specific name from Windows
    string producer = "Formula AllCode";
    string model = "RB4420";
    string color = "Black";
    string softwareNameVer = "ACI - Automation Control Interface, v1.5f Beta";
    int yearReleased = 2019;
    int comPort = 1;            // default value
public:
    virtual int port();     // changes the private default comPort to user input 
    int port(int portGiven);// change the private default COM-port value using pointer
    int autoSet_comPort();  // carries all COM port selection process, including run-time polymorphism
};

// Derived class - Inheritance
class Device: public DeviceMain {
public:
    string nameToAdd = "";  // Future dev: need method to get port no. device name from Windows machines & initialize this + to DeviceMain name 
    int batteryCap = 0;     // Work in progress to get this detail from the rover. Needs more time.
    int temp{}; 
    int port();             // main COM port selection method
};

// Data class with all methods to gather information from device
class Data{
public:
    bool safety = true; // used for safety stops in control
    // data functions:
    void compass();
    void writeOnLCD();
    void lightSensor();
    void lineSensor(int tempHere);
    void lineSensor();
    void irSensor();
    double irFormula(double dataIn);
    void irProximityCheck();
    void irRadar();
    int accelerometer();
};

// Movement commands main class. Derived from Data class - Inheritance
class Control:public Data {
private:
    // keeping reference to deviceMain instead of copy - high efficency.
    // main() has to make sure deviceMain object out-lives Control object
    DeviceMain& deviceMain;
    // private basic control commands variables:
    int key_forward{}, key_backward{}, key_left{}, key_right{}, key_reverse{};

public:
    int speedVal = 20;
    int countBoost = 0;
    int turn;
    int reset = 0;
    int reverse = 0;
    bool down = false; // boolean variable to differentiate between key-pressed / key-released
    // IR reading variables:
    double frontIR{}, backIR{}, frontLeftIR{}, frontRightIR{}, backLeftIR{}, backRightIR{};

    // constructor, which only sets deviceMain and keeps the keys name at their default
    // deviceMain is initialized before constructor function body
    // as references have no default value
    Control(DeviceMain& gr) : deviceMain(gr)
    { // Default list:
        key_forward     = VK_UP;        // Drive forwards
        key_backward    = VK_DOWN;      // Drive backwards
        key_left        = VK_LEFT;      // Dive left
        key_right       = VK_RIGHT;     // Drive right
        key_reverse     = VK_RSHIFT;    // Activate / deactivate drive reverse left of right
        turn            = VK_KEY_T;     // Turn 180 on spot
        int comPort {};
        int iPort {};
    }
    void automation();
    void definedCommand();              // loop to define rules of the key-pressed commands
    void turn180();                     // turn device 180 degrees method
    // function overloading
    void sounds(int start);
    void sounds(int start, int honk);
    void sounds(int one, int two, int three);
    int boostSpeed();

    // function to change the key control values stored in the member variables
    // can be called by constructor or externally
    void changeKeys(int change_key_forward, int change_key_backward, int change_key_left, int change_key_right, int change_key_reverse)
    {
        key_forward = change_key_forward;
        key_backward = change_key_backward;
        key_left = change_key_left;
        key_right = change_key_right;
        key_reverse = change_key_reverse;
    }
};

