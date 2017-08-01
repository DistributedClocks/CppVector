#include "./VClock/VClock.h"

#include <mutex>
#include <msgpack.hpp>
#include <sstream>
#include <tuple>

class CppVec {

public:
	
	CppVec();
	virtual ~CppVec();

	void initCppVector(std::string, std::string);
	VClock getCurrentVC();
	bool logThis(std::string, std::string);
	bool logLocalEvent(std::string);
	template <typename T> const char* prepareSend(std::string message, T payload);
	template <typename T> void unpackReceive(std::string message, const char* buffer, 
		T* unpack, int numBytes);

private:

	std::string _pid;
	VClock _currentVC;
	bool _logging;
	bool _debugmode;
	std::string _logfilename;
	std::mutex _mutex;

};

template <typename T> 
const char* CppVec::prepareSend(std::string message, T payload) {
   
	_mutex.lock();
	_currentVC.tick(_pid);

	// Define a new type for the clockpayload and create it
	typedef msgpack::type::tuple<std::string, T, std::map<std::string, int> > clockPayload;
    clockPayload data(_pid, payload, _currentVC.getClock());

    // write to local log
    logThis(message, _currentVC.getVCString());

    // serialize the object into the buffer.
    // any classes that implements write(const char*, size_t) can be a buffer.
    std::stringstream buffer;
    msgpack::pack(buffer, data);

    // copy the buffer to a const char* to send on the wire
    const std::string tmp = buffer.str();
    const char* cstr = tmp.c_str();

    _mutex.unlock();

    return cstr;

} 

template <typename T> 
void CppVec::unpackReceive(std::string message, const char* buffer, T* unpack, int numBytes) {

	_mutex.lock();
	
	// Define clock payload type
	typedef msgpack::type::tuple<std::string, T, std::map<std::string, int> > clockPayload;

	// Unroll the buffer into a msgpack::object
	msgpack::object_handle oh = msgpack::unpack(buffer, numBytes);
    msgpack::object deserialized = oh.get();

    // convert msgpack::object instance into the original type.
    // if the type is mismatched, it throws msgpack::type_error exception.
    clockPayload dst;
    deserialized.convert(dst);

    // Unroll the T type into interface
    *unpack = std::get<1>(dst);
    std::map<std::string, int> theirVCMap = std::get<2>(dst);
    VClock theirVC = VClock();
    theirVC.copyFromMap(theirVCMap);

    // Tick own clock, and merge with incoming
    _currentVC.tick(_pid);
    _currentVC.merge(theirVC);

    // write to local log
    logThis(message, _currentVC.getVCString());

	_mutex.unlock();

}