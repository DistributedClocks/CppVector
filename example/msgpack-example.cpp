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

void print_bytes(const void *object, size_t size)
{
  // This is for C++; in C just drop the static_cast<>() and assign.
  const unsigned char * const bytes = static_cast<const unsigned char *>(object);
  size_t i;

  printf("[ ");
  for(i = 0; i < size; i++)
  {
    printf("%02x ", bytes[i]);
  }
  printf("]\n");

}

int main(void)
{

    std::map<std::string, int> vc;
    vc["client"] = 2;

    msgpack::sbuffer buffer;
    msgpack::packer<msgpack::sbuffer> pk(buffer);

    std::string c = "client";
    int a = 0;

    pk.pack(c);
    pk.pack(a);
    pk.pack(vc);
    
    std::cout << "Size: " << buffer.size() << std::endl;
    
    print_bytes(buffer.data(), buffer.size());

    std::size_t len = buffer.size();

    // copy the buffer to a char[] to send on the wire
    char cstr[len];
    const char* bufferdata = buffer.data();
    memcpy(cstr, bufferdata, len);
    
    print_bytes(bufferdata, len);
    print_bytes(cstr, len);

    // SEND, GET

    std::string newPid;
    int newA;
    std::map<std::string, int> newVc;
    
    std::size_t off = 0;

    msgpack::object_handle result;    
    msgpack::unpack(result, cstr, len, off);
    msgpack::object obj1(result.get());

    obj1.convert(newPid);
    std::cout << newPid << std::endl;

    msgpack::unpack(result, cstr, len, off);
    msgpack::object obj2(result.get());

    obj2.convert(newA);
    std::cout << newA << std::endl;

    msgpack::unpack(result, cstr, len, off);
    msgpack::object obj3(result.get());

    obj3.convert(newVc);
    std::cout << newVc["client"] << std::endl;
    std::cout << newVc["server"] << std::endl;

    return 0;
}