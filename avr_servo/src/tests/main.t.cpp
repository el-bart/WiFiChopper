/*
 * main.t.cpp
 *
 * TUT main test file for library/application.
 * this is little reworked exmaple testapp main
 * from TUT home page.
 *
 */
#include <tut/tut.hpp>
#include <tut/tut_reporter.hpp>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cassert>


using namespace std;


namespace tut
{
test_runner_singleton runner;
}


int main(const int argc, const char *const * const argv)
{
  int returnCode=255;
  try
  {
    tut::reporter reporter;
    tut::runner.get().set_callback(&reporter);

    switch(argc)
    {
      case 1:   // this is the default case
        tut::runner.get().run_tests();
        break;

      case 2:   // run specific test group
        tut::test_runner_singleton::get().run_tests( argv[1] );
        break;

      case 3:   // run specific test from specific group
        tut::test_runner_singleton::get().run_test( argv[1], ::atoi(argv[2]) );
        break;

      default:
        assert(!"this code is never reached");
    } // switch(argc)

    returnCode=(reporter.all_ok())?0:1;
  }
  catch(const std::exception &ex)
  {
    cerr<<argv[0]<<": TUT execption cought: "<< ex.what() <<endl;
    return 1;
  }
  catch(...)
  {
    cerr<<argv[0]<<": unknown exception cought"<<endl;
    return 2;
  };

  return returnCode;
}

