/*
    This is a sample program that I use to learn msgpack with;
    it is not a part of the main library.
*/

#include <msgpack.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <map>

template <typename T>
struct ClockPayload {
    std::string Pid;
    T Payload;
    std::map<std::string, uint64_t> VcMap;
};

int main(void)
{
 
    msgpack::type::tuple<int, bool, std::string> src(0, true, "example");

    // serialize the object into the buffer.
    // any classes that implements write(const char*,size_t) can be a buffer.
    std::stringstream ssbuffer;
    msgpack::pack(ssbuffer, src);

    // send the buffer ...
    ssbuffer.seekg(0);

    // deserialize the buffer into msgpack::object instance.
    std::string str(ssbuffer.str());

    std::cout << str << std::endl;

    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());

    // deserialized object is valid during the msgpack::object_handle instance is alive.
    msgpack::object deserialized = oh.get();

    // msgpack::object supports ostream.
    std::cout << deserialized << std::endl;

    // convert msgpack::object instance into the original type.
    // if the type is mismatched, it throws msgpack::type_error exception.
    msgpack::type::tuple<int, bool, std::string> dst;
    deserialized.convert(dst);

    //////////////////////////////////////////////////////

    std::map<std::string, uint64_t> vc;
    vc["abc"] = 15;
    vc["omg"] = 1888;

    ClockPayload<int64_t> cp;
    cp.Pid = "myPid";
    cp.Payload = 1;
    cp.VcMap = vc;

    msgpack::sbuffer buffer;

    msgpack::packer<msgpack::sbuffer> pk(buffer);
    
    pk.pack(cp.Pid);
    pk.pack_int64(13);
    pk.pack(cp.VcMap);
    
    std::cout << buffer.size() << std::endl;

    // copy the buffer to a const char* to send on the wire
    const char* cstr = buffer.data();

    std::cout << cstr << std::endl;

    // Convert c string to a stringstream
    std::stringstream sBuffer;
    sBuffer << cstr;

    std::cout << buffer.size() << std::endl;

    // unpack
    //ClockPayload<std::string> cp2;

    std::size_t off = 0;

    while (off != sBuffer.str().size()) {
        msgpack::object_handle result;
        msgpack::unpack(result, sBuffer.str().data(), sBuffer.str().size(), off);
        msgpack::object obj;
        obj = result.get();
        std::cout << obj << std::endl;      
    }

    // Unpack one piece at a time
    /*msgpack::unpack(result, sBuffer.str().data(), sBuffer.str().size(), off);
    obj = result.get();
    obj.convert(cp2.Pid);

    msgpack::unpack(result, sBuffer.str().data(), sBuffer.str().size(), off);
    obj = result.get();
    obj.convert(cp2.Payload);

    msgpack::unpack(result, sBuffer.str().data(), sBuffer.str().size(), off);
    obj = result.get();
    obj.convert(cp2.VcMap);

    std::cout << cp2.Pid << std::endl;
    std::cout << cp2.Payload << std::endl;
    std::cout << cp2.VcMap["omg"] << std::endl;*/

    return 0;
}