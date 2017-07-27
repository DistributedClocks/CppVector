CppVector (Work in progress)
===========

This library can be added to a c++ project to generate a ShiViz-compatible vector-clock timestamped log of events in a concurrent or distributed system. CppVector is compatible with C++11.

### Usage

Not ready yet.
Compile the project using:

```g++ -W -Wall -Werror -std=c++11 CppVec.cpp ./VClock/VClock.cpp```

### Index

void initCppVector(string pid, string logfilename)
VClock getCurrentVC()
string prepareSend(string message, T payload)
void UnpackReceive(string message, string buffer, T* unpack)
bool LogLocalEvent(string message)

#### initCppVector
```
  void initCppVector(string pid, string logfilename)
```

pid (string): local process name; must be unique in your distributed system.
logfilename (string) : name of the log file that will store info. Any old log with the same name will be truncated

#### getCurrentVC
```
  VClock getCurrentVC()
```

Returns the current process vector clock.

#### PrepareSend
```
  string prepareSend(string message, T payload)
```

message (string): message to be logged in log file
payload (T): object to be encoded

This function is meant to be used immediately before sending. Message will be logged along with the time of the send buf is encode-able data (structure or basic type) Returned is an encoded string with logging information

This function is meant to be called before sending a packet. It updates the Vector Clock for its own process, packages with the clock using msgpack support and returns the new byte array that should be sent onwards using the Send Command

#### UnpackReceive
```
void UnpackReceive(string message, string buffer, T* unpack)
```
message (string): message to be logged in log file
buffer (string): encoded string message
unpack (T*): pointer to object to be decoded into

UnpackReceive is used to unmarshall network data into local structures. Message will be logged along with the vector time the receive happened. buffer is the network data, previously packaged by PrepareSend. unpack is a pointer to a structure, the same as was packed by PrepareSend

This function is meant to be called immediately after receiving a packet. It unpacks the data by the program, the vector clock. It updates vector clock and logs it. and returns the user data

#### LogLocalEvent
```
bool LogLocalEvent(string message)
```
message (string): message to be logged in log file

Increments current vector timestamp and logs it into Log File, along with the message.
