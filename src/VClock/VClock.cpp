#include "VClock.h"
#include <iostream>
#include <map>
#include <string>
#include <sstream>

using namespace std;

// The local vector clock state
map<string, int> _vc;

// Constructor
VClock::VClock() {
	
}

// Destructor
VClock::~VClock() {
	
}

map<string, int> VClock::getClock() {
	return _vc;
}

void VClock::set(string id, int ticks) {
	_vc[id] = ticks;
}

void VClock::tick(string id) {
	_vc[id] = _vc[id] + 1;
}

int VClock::findTicks(string id) {
	int ticks = _vc[id];
	return ticks;
}

int VClock::lastUpdate() {

	typedef map<string, int>::iterator clock_iterator; 
	int last = 0;

	// Find the highest VClock value
	for (clock_iterator c = _vc.begin() ; c != _vc.end(); ++c) {
		cout << last << endl;
		if (c -> second > last) {
			last = c -> second;
		}
	}

	return last;
}

void VClock::merge(VClock other) {

	map<string, int> otherMap = other.getClock();
	typedef map<string, int>::iterator other_iterator; 

	for (other_iterator o = otherMap.begin() ; o != otherMap.end(); ++o) {
	
		string theirKey = o -> first;
		int theirVal = o -> second;

		// Take the higher of my value or other value
		if (_vc[theirKey] < theirVal) {
			_vc[theirKey] = theirVal;
		}
	}
}

void VClock::printVCString() {
	cout << getVCString() << endl;
}

string VClock::getVCString() {

	stringstream ss;
	typedef map<string, int>::iterator clock_iterator; 

	ss << "{";
	for (clock_iterator c = _vc.begin() ; c != _vc.end(); ++c) {
		ss << c -> first << ": " << c -> second;
		
		// In C++, hasNext is only done by 
		// copying the iterator and advancing it
		clock_iterator dupe = c;
		++dupe;

		if (dupe != _vc.end()) {
			ss << ", ";
		}

	}
	
	ss << "}";
	return ss.str();
}

void VClock::copyFromMap(map<string, int> vcMap) {

	map<string, int> emptyMap;
	_vc = emptyMap;

	typedef map<string, int>::iterator clock_iterator; 
	for (clock_iterator c = vcMap.begin(); c != vcMap.end(); ++c) {
		set(c -> first, c -> second);
	}

}