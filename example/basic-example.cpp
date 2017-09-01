#include "../src/CppVec.h"
#include <iostream>

#define MAXBUFLEN 100

using namespace std;

/*
Compile using:
g++ -W -Wall -Werror -std=c++11 basic-example.cpp ../src/CppVec.cpp ../src/VClock/VClock.cpp
*/

int main() {

	cout << "Hello CppVector!" << endl;

	// Testing of the VClock
	VClock clock1 = VClock();
	clock1.set("a", 1);
	clock1.set("b", 1);
	clock1.set("c", 1);
	clock1.set("d", 18);
	clock1.set("e", 12);
	clock1.tick("d");

	VClock clock2 = VClock();
	clock2.set("a", 3);
	clock2.set("b", 5);
	clock2.set("c", 1);
	clock2.set("d", 15);
	clock2.set("e", 11);

	cout << "clock1" << endl;
	clock1.printVCString();

	cout << "clock2" << endl;
	clock2.printVCString();

	clock1.merge(clock2);

	cout << "merged clock" << endl;
	clock1.printVCString();
	
	// Testing of the pack and unpack
	CppVec cv;
	cv.initCppVector("myPID", "logFile");
	
	// Create the buffer
	char* buffer;
	
	string s = "placeholder";
	buffer = cv.prepareSend("PrepareSend message", "myPayload");
	cv.unpackReceive("UnpackReceive message", buffer, &s, MAXBUFLEN);
	cout << "Unpacked: " << s << endl;

	// Send an int
	int x = 100;
	buffer = cv.prepareSend("PrepareSend message", 123);
	cv.unpackReceive("UnpackReceive message", buffer, &x, MAXBUFLEN);
	cout << "Unpacked: " << x << endl;

	return 0;

}