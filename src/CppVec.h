#include <string>
#include <map>
#include <mutex>
#include "./VClock/VClock.h"

class CppVec {

public:
	
	CppVec();
	virtual ~CppVec();

	void initCppVector(std::string, std::string);
	VClock getCurrentVC();
	bool logThis(std::string, std::string);
	bool logLocalEvent(std::string);
	template <typename T> std::string prepareSend(std::string message, T payload);
	template <typename T> void unpackReceive(std::string message, std::string buffer, T* unpack);

private:

	std::string _pid;
	VClock _currentVC;
	bool _logging;
	bool _debugmode;
	std::string _logfilename;
	std::mutex _mutex;

};
