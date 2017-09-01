#define CATCH_CONFIG_MAIN
#define MAXBUFLEN 100

#include "catch.hpp"
#include "../src/CppVec.h"

TEST_CASE( "CppVector Test") {

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

TEST_CASE( "VClock test") {

    VClock clock1 = VClock();
    clock1.set("a", 1);
    clock1.set("b", 8);
    clock1.set("c", 2);
    
    SECTION ( "VClock set and tick ") {

        REQUIRE( clock1.findTicks("a") == 1 );
        REQUIRE( clock1.findTicks("b") == 8 );
        REQUIRE( clock1.findTicks("c") == 2 );

        clock1.tick("a");

        REQUIRE( clock1.findTicks("a") == 2 );
        REQUIRE( clock1.findTicks("b") == 8 );
        REQUIRE( clock1.findTicks("c") == 2 );

        clock1.tick("d");

        REQUIRE( clock1.findTicks("a") == 2 );
        REQUIRE( clock1.findTicks("b") == 8 );
        REQUIRE( clock1.findTicks("c") == 2 );
        REQUIRE( clock1.findTicks("d") == 1 );

        REQUIRE( clock1.lastUpdate() == 8 );

    }

    // State does not persist from test.
    clock1.tick("d");

    VClock clock2 = VClock();
    clock2.set("a", 3);
    clock2.set("b", 5);
    clock2.set("c", 1);
    clock2.set("e", 2);

    SECTION (" VClock merge ") {

        clock1.merge(clock2);

        REQUIRE( clock1.findTicks("a") == 3 );
        REQUIRE( clock1.findTicks("b") == 8 );
        REQUIRE( clock1.findTicks("c") == 2 );
        REQUIRE( clock1.findTicks("d") == 1 );
        REQUIRE( clock1.findTicks("e") == 2 ); 

        REQUIRE( clock1.lastUpdate() == 8 );

    }

}

