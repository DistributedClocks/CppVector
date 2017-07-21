#include <msgpack.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <map>

int main(void)
{
    
    std::map<std::string, int> vc;
    vc["abc"] = 15;

    msgpack::type::tuple<std::string, std::string, std::map<std::string, int> > src("char", "try it", vc);

    // serialize the object into the buffer.
    // any classes that implements write(const char*,size_t) can be a buffer.
    std::stringstream buffer;
    msgpack::pack(buffer, src);

    // send the buffer ...
    buffer.seekg(0);

    // deserialize the buffer into msgpack::object instance.
    std::string str(buffer.str());

    msgpack::object_handle oh =
        msgpack::unpack(str.data(), str.size());

    // deserialized object is valid during the msgpack::object_handle instance is alive.
    msgpack::object deserialized = oh.get();

    // msgpack::object supports ostream.
    std::cout << deserialized << std::endl;

    // convert msgpack::object instance into the original type.
    // if the type is mismatched, it throws msgpack::type_error exception.
    msgpack::type::tuple<std::string, std::string, std::map<std::string, int> > dst;
    deserialized.convert(dst);

    return 0;
}