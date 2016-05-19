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




namespace papas {

  
class PFEvent;
class Detector;

//Wrapper for Python to allow graphs to be plotted
class PFApp {
public:
  PFApp();
  void display(const PFEvent& pfevent, const Detector& det);
  void run(){ m_theApp.Run();}
private:
  Int_t m_argc; //dummy arguments
  char* m_str[0];
  TApplication m_theApp;
  PFEventDisplay* m_display;
  std::shared_ptr<GDetector> m_gdetector;
};

// ROOT App to allow graphs to be plotted
PFApp::PFApp() :
  m_argc(0), m_theApp("App", &m_argc ,m_str) {
}
  
void PFApp::display(const PFEvent& pfEvent, const Detector& det) {

  m_display = new PFEventDisplay({ViewPane::Projection::xy, ViewPane::Projection::yz});
  m_gdetector =  std::make_shared<GDetector>(GDetector(det));
  m_display->addToRegister(m_gdetector, 0);
  m_display->drawPFEvent(pfEvent);
  std::cout <<"Hello";
}
} // end namespace papas


#endif /* AliceDisplay_hpp */
