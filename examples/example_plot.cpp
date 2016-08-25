
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

#include <TApplication.h>

using namespace papas;


int main(int argc, char* argv[]) {
  
  if (argc != 2) {
    std::cerr << "Usage: ./mainexe filename" << std::endl;
    return 1;
  }
  const char* fname = argv[1];
  auto pythiaConnector = PythiaConnector(fname);
  
  // Create CMS detector and PapasManager
  CMS CMSDetector;
  papas::PapasManager papasManager{CMSDetector};
  
  unsigned int eventNo = 0;
  pythiaConnector.processEvent(eventNo, papasManager);
  TApplication tApp("theApp", &argc, argv) ;
  papasManager.display(true);
  tApp.Run();
  return EXIT_SUCCESS;
}
