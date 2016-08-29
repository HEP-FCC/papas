
//
//  example_simple.cpp
//
//  Created by Alice Robson on 14/01/16.
//
//
// C++
#include <iostream>
#include <stdio.h>

#include "CMS.h"
#include "PDebug.h"
#include "PapasManager.h"
#include "PythiaConnector.h"

// STL
#include <chrono>
#include <iostream>

#include <TApplication.h>



using namespace papas;


int main(int argc, char* argv[]) {

 
  
  randomgen::setEngineSeed(0xdeadbeef);
  
  if (argc < 2) {
    std::cerr << "Usage: ./example_debug filename [logname]" << std::endl;
    return 1;
  }
  const char* fname = argv[1];
  auto pythiaConnector = PythiaConnector(fname);
  
  if (argc == 3) {
    const char* lname = argv[2];
    PDebug::On(lname);  // physics debug output
  }

  
  // Create CMS detector and PapasManager
  CMS CMSDetector;
  papas::PapasManager papasManager{CMSDetector};
  unsigned int eventNo = 0;
  unsigned int nEvents = 1000;
  
  auto start = std::chrono::steady_clock::now();
  
  // TApplication tapp("papas", &argc, argv );
  for (unsigned i = eventNo; i < eventNo + nEvents; ++i) {
    
    PDebug::write("Event: {}", i);
    if (i % 10 == 0) {
      std::cout << "reading event " << i << std::endl;
    }
    if (i == eventNo)
      start = std::chrono::steady_clock::now();
    else {
      papasManager.clear();
    }
    
    pythiaConnector.processEvent(i, papasManager);
    /*if (i==0)
      papasManager.display();
    gSystem->ProcessEvents();*/
  }
  
  auto end = std::chrono::steady_clock::now();
  auto diff = end - start;
  auto times = std::chrono::duration<double, std::milli>(diff).count();
  std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms" << std::endl;
  std::cout << 1000 * nEvents / times << " Evs/s" << std::endl;
  return EXIT_SUCCESS;
}
