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
	_logfilename = logfilename + "-Log.txt";

	// Initialize our local VC
	_currentVC = VClock();
	_currentVC.tick(_pid);

	ofstream logfile(_logfilename, ofstream::out | ofstream::trunc);
	logThis("Initialization Complete", _currentVC.getVCString());
	
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



