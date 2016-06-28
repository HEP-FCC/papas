//
//  AliceDisplay.hpp
//  papas
//
//  Created by Alice Robson on 19/05/16.
//
//

#ifndef AliceDisplay_hpp
#define AliceDisplay_hpp

#include <stdio.h>
#include <iostream>
#include "displaycore.h"
#include "TApplication.h"

#include "displaypfobjects.h"
#include "PFEventDisplay.h"
#include "DefinitionsCollections.h"




namespace papas {

  
class PFEvent;
class Detector;

//Wrapper for Python to allow graphs to be plotted
class PFApp {
public:
  PFApp();
  void display(const Simulator& simulator,const PFEvent& pfevent, const Particles& particles, const Detector& det);
  void display2(const Simulator& simulator, const PFEvent& pfEvent,const Particles& particles, const Detector& det); //under development
    
  
  void run(){ m_theApp.Run();}
private:
  Int_t m_argc; //dummy arguments
  char* m_str[1];
  TApplication m_theApp;
  PFEventDisplay* m_display;
  std::shared_ptr<GDetector> m_gdetector;
};


} // end namespace papas


#endif /* AliceDisplay_hpp */
