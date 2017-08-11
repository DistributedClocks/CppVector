#ifndef CPPVEC_H
#define CPPVEC_H

#include "./VClock/VClock.h"

#include <iostream>
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
	template <typename T> char* prepareSend(std::string message, T payload);
	template <typename T> void unpackReceive(std::string message, char* buffer, 
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
char* CppVec::prepareSend(std::string message, T payload) {
   
	_mutex.lock();
	_currentVC.tick(_pid);

	msgpack::sbuffer buffer;
	msgpack::packer<msgpack::sbuffer> pk(buffer);

	pk.pack(_pid);
	pk.pack(payload);
	pk.pack(_currentVC.getClock());

    // write to local log
    logThis(message, _currentVC.getVCString());

    size_t len = buffer.size();

    // copy the buffer to a char* to send on the wire
    char* outBuf = (char*)malloc(len);
    memcpy(outBuf, buffer.data(), len);

    _mutex.unlock();

    return outBuf;

} 

template <typename T> 
void CppVec::unpackReceive(std::string message, char* buffer, T* unpack, int numBytes) {

	_mutex.lock();
	
	std::string pid;
	T payload;
    std::map<std::string, int> theirVCMap;
    
    std::size_t off = 0;

    // unpack the PID first as string
    msgpack::object_handle result;    
    msgpack::unpack(result, buffer, numBytes, off);
    msgpack::object obj1(result.get());

    obj1.convert(pid);

    // Unpack the payload as second piece
    msgpack::unpack(result, buffer, numBytes, off);
    msgpack::object obj2(result.get());

    obj2.convert(payload);

    msgpack::unpack(result, buffer, numBytes, off);
    msgpack::object obj3(result.get());

    obj3.convert(theirVCMap);

    // Unroll the T type into interface
    *unpack = payload;
  
    VClock theirVC = VClock();
    theirVC.copyFromMap(theirVCMap);

    // Tick own clock, and merge with incoming
    _currentVC.tick(_pid);
    _currentVC.merge(theirVC);

    // write to local log
    logThis(message, _currentVC.getVCString());

	_mutex.unlock();

}

#endif /* CPPVEC_H */