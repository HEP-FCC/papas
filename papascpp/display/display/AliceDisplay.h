//
//  AliceDisplay.hpp
//  papas
//
//  Created by Alice Robson on 19/05/16.
//
//

#ifndef AliceDisplay_hpp
#define AliceDisplay_hpp

#include "TApplication.h"
#include "displaycore.h"
#include <iostream>
#include <stdio.h>

#include "DefinitionsCollections.h"
#include "PFEventDisplay.h"
#include "displaypfobjects.h"
#include "TSystem.h"
#include "Tpad.h"
namespace papas {

class PFEvent;
class Detector;

// Experimental Wrapper for Python to allow graphs to be plotted
class PFApp {
public:
  PFApp();
  void display(const Simulator& simulator, const PFEvent& pfevent, const Particles& particles, const Detector& det);
  void display2(const Simulator& simulator, const PFEvent& pfEvent, const Particles& particles,
                const Detector& det);  // under development for exprimental purposes.
  void run() {gPad->Modified(); gPad->Update(); gSystem->ProcessEvents(); m_theApp.Run(); }

private:
  Int_t m_argc;  // dummy arguments
  char* m_str[1];
  TApplication m_theApp;
  PFEventDisplay* m_display; //make shared
  std::shared_ptr<GDetector> m_gdetector;
};

}  // end namespace papas

#endif /* AliceDisplay_hpp */
