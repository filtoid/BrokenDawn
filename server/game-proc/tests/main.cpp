#include "./CouchDBTests.h"
#include "./UserTests.h"
#include "./GameProcTests.h"

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <iostream>

using namespace std;

int main (int argc, char **argv)
{
	cout << "Start Tests\n";

	// As curlpp is the main way of communicating with
	// the db then it is important to catch any exceptions
	// thrown by the global instance.
	try
        {
		// Global cleanup object for curlpp
                curlpp::Cleanup myCleanup;

		CppUnit::TestSuite* suite = new CppUnit::TestSuite();
		CppUnit::TestResult result;
	
		// CouchDBTests
		CouchDBTests::setup( suite );
		// User Tests
		UserTests::setup( suite );
		// Game-proc Tests
		GameProcTests::setup( suite );

		CppUnit::TextUi::TestRunner runner;
		
		// runner now owns this pointer (and will destroy it)
 		runner.addTest( suite );
 		runner.setOutputter( CppUnit::CompilerOutputter::defaultOutputter( 
                          &runner.result(),
                          std::cerr ) );
 		runner.run( "", true );    // Run all tests and wait

	}catch( curlpp::RuntimeError &e )
        {
                std::cout << e.what() << std::endl;
		return 1;
        }catch( curlpp::LogicError &e )
        {
                std::cout << e.what() << std::endl;
		return 1;
        }

	cout << "End Tests\n";

	return 0;
}
