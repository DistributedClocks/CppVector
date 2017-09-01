#define CATCH_CONFIG_MAIN
#define MAXBUFLEN 100

#include "catch.hpp"
#include "../src/CppVec.h"

struct SampleStruct {
    int X;
    std::string Y;
};

TEST_CASE( " CppVector Test") {

    CppVec cv;
    cv.initCppVector("test", "testfile");

    // Create the buffer
    char* buffer;
    
    SECTION( "CppVec Init ") {

        REQUIRE( cv.getCurrentVC().findTicks("test") == 1);

        cv.logLocalEvent("Some event");

        REQUIRE( cv.getCurrentVC().findTicks("test") == 2);

    }

    SECTION( "Sending and receiving a string" ) {
        
        // start the string
        std::string s = "placeholder";
        buffer = cv.prepareSend("PrepareSend message", "myPayload");
        cv.unpackReceive("UnpackReceive message", buffer, &s, MAXBUFLEN);
        
        REQUIRE( s == "myPayload" );
    }

    SECTION( "Sending and receiving an int" ) {
        // Send an int
        int x = 100;
        buffer = cv.prepareSend("PrepareSend message", 123);
        cv.unpackReceive("UnpackReceive message", buffer, &x, MAXBUFLEN);
        
        REQUIRE( x == 123 );
    }

    SECTION( "Sending and receiving a map" ) {
        
        std::map<std::string, int> vclock;
        vclock["distclock"] = 15;

        buffer = cv.prepareSend("PrepareSend message", vclock);
        
        std::map<std::string, int> vclock2;
        cv.unpackReceive("UnpackReceive message", buffer, &vclock2, MAXBUFLEN);
        
        REQUIRE( vclock2["distclock"] == 15 );

    }

}

TEST_CASE( "Example test" ) {

    std::vector<int> v( 5 );
    
    REQUIRE( v.size() == 5 );
    REQUIRE( v.capacity() >= 5 );
    
    SECTION( "resizing bigger changes size and capacity" ) {
        v.resize( 10 );
        
        REQUIRE( v.size() == 10 );
        REQUIRE( v.capacity() >= 10 );
    }
    SECTION( "resizing smaller changes size but not capacity" ) {
        v.resize( 0 );
        
        REQUIRE( v.size() == 0 );
        REQUIRE( v.capacity() >= 5 );
    }
    SECTION( "reserving bigger changes capacity but not size" ) {
        v.reserve( 10 );
        
        REQUIRE( v.size() == 5 );
        REQUIRE( v.capacity() >= 10 );
    }
    SECTION( "reserving smaller does not change size or capacity" ) {
        v.reserve( 0 );
        
        REQUIRE( v.size() == 5 );
        REQUIRE( v.capacity() >= 5 );
    }
}