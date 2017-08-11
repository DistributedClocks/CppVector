#ifndef VCLOCK_H
#define VCLOCK_H

#include <string>
#include <map>

class VClock {

public:
	
	VClock();
	virtual ~VClock();

	int findTicks(std::string);
	void set(std::string, int);
	void tick(std::string);
	int lastUpdate();
	void merge(VClock);
	void printVCString();
	std::string getVCString();
	std::map<std::string, int> getClock();

	// A temporary replacement for "FromBytes"
	void copyFromMap(std::map<std::string, int>);

private:
	
	std::map<std::string, int> _vc;

};

#endif // VCLOCK_H 