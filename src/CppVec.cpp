#include "CppVec.h"
#include <msgpack.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <tuple>

using namespace std;

string _pid;
VClock _currentVC;
bool _logging;
bool _debugmode;
string _logfilename;
mutex _mutex;

// Constructor
CppVec::CppVec() {
	
}

// Destructor
CppVec::~CppVec() {
	
}

VClock CppVec::getCurrentVC() {
	return _currentVC;
}

void CppVec::initCppVector(string pid, string logfilename) {
	_pid = pid;
	_logging = true;
	_debugmode = false;
	_logfilename = logfilename;

	// Initialize our local VC
	_currentVC = VClock();
	_currentVC.tick(_pid);

	ofstream logfile(_logfilename, ofstream::out | ofstream::trunc);
	logThis("Initialization Complete", _currentVC.getVCString());

	logLocalEvent("LLE");

}

bool CppVec::logThis(string message, string vcString) {

	ofstream logfile(_logfilename, ofstream::out | ofstream::app);
	logfile << _pid << " " << vcString << "\n" << message << endl;
	logfile.close();

	// TODO: Improve error checking
	return true; 
}

bool CppVec::logLocalEvent(string message) {

	_mutex.lock();

	// Tick own clock
	_currentVC.tick(_pid);
	
	bool ok;
	ok = logThis(message, _currentVC.getVCString());

	_mutex.unlock();

	return ok;
}

template <typename T> string CppVec::prepareSend(string message, T payload) {
   
	_mutex.lock();
	_currentVC.tick(_pid);

	// Define a new type for the clockpayload and create it
	typedef msgpack::type::tuple<string, T, map<string, int> > clockPayload;
    clockPayload data(_pid, payload, _currentVC.getClock());

    // write to local log
    logThis(message, _currentVC.getVCString());

    // serialize the object into the buffer.
    // any classes that implements write(const char*, size_t) can be a buffer.
    stringstream buffer;
    msgpack::pack(buffer, data);

    // store the buffer as a string, to send on the wire
    string str(buffer.str());

    _mutex.unlock();

    return str;

} 

template <typename T> void CppVec::unpackReceive(string message, string buffer, T* unpack) {

	_mutex.lock();
	
	// Define clock payload type
	typedef msgpack::type::tuple<string, T, map<string, int> > clockPayload;

	// Unroll the buffer into a msgpack::object
	msgpack::object_handle oh = msgpack::unpack(buffer.data(), buffer.size());
    msgpack::object deserialized = oh.get();

    // convert msgpack::object instance into the original type.
    // if the type is mismatched, it throws msgpack::type_error exception.
    clockPayload dst;
    deserialized.convert(dst);

    // Unroll the T type into interface
    *unpack = get<1>(dst);
    map<string, int> theirVCMap = get<2>(dst);
    VClock theirVC = VClock();
    theirVC.copyFromMap(theirVCMap);

    // Tick own clock, and merge with incoming
    _currentVC.tick(_pid);
    _currentVC.merge(theirVC);

    // write to local log
    logThis(message, _currentVC.getVCString());

	_mutex.unlock();

}

int main() {

	cout << "Hello World in C++!!!" << endl; // prints !!!Hello World!!!	

	// Testing of the VClock
	VClock abc = VClock();
	abc.set("a", 1);
	abc.set("b", 1);
	abc.set("c", 1);
	abc.set("d", 18);
	abc.set("e", 12);
	abc.tick("d");

	VClock abc2 = VClock();
	abc2.set("a", 3);
	abc2.set("b", 5);
	abc2.set("c", 1);
	abc2.set("d", 15);
	abc2.set("e", 11);

	cout << "abc" << endl;
	abc.printVCString();

	cout << "abc2" << endl;
	abc2.printVCString();

	abc.merge(abc2);

	cout << "merged" << endl;
	abc.printVCString();

	cout << "Done" << endl;
	
	// Testing of the pack and unpack
	CppVec cv;
	cv.initCppVector("myPID", "logFile");
	
	// Send a string
	string buffer;
	
	string s = "placeholder";
	buffer = cv.prepareSend("PrepareSend message", "myPayload");
	cv.unpackReceive("UnpackReceive message", buffer, &s);
	cout << s << endl;

	// Send an int
	int x = 100;
	buffer = cv.prepareSend("PrepareSend message", 123);
	cv.unpackReceive("UnpackReceive message", buffer, &x);
	cout << x << endl;

	return 0;

}
