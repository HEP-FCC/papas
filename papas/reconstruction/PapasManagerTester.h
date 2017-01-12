//
//  PapasManagerTester.h
//  papas
//
//  Created by Alice Robson on 12/01/17.
//
//

#ifndef PapasManagerTester_h
#define PapasManagerTester_h
#include "papas/reconstruction/PapasManager.h"
#include "papas/simulation/Simulator.h"
namespace papas {
  
  class PapasManagerTester: public PapasManager {
  public:
    PapasManagerTester(Detector& detector);
    Simulator setSimulator(const Particles& particles);
    //private:
    //;
    //Simulator m_simulator;
    // bool operator()(IdType i, IdType j);//todo reinstate was used for sorting ids
  };
}


#endif /* PapasManagerTester_h */
