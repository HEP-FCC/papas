//
//  rand.cpp
//  papas
//
//  Created by Alice Robson on 26/04/16.
//
//

#include "papas/utility/Random.h"

#include <random>

namespace randomgen {

std::random_device rdevice;

// swap these two for reproducable random vs fully random
// std::default_random_engine engine(m_rd());
std::default_random_engine engine(1000);

void setEngineSeed(double seed) { engine.seed(seed); }
RandUniform::RandUniform(double from, double to) : m_dist(from, to) {}
double RandUniform::next() { return m_dist(engine); }
void RandUniform::setSeed(double seed) { setEngineSeed(seed); }
RandNormal::RandNormal(double mean, double sd) : m_dist(mean, sd) {}
double RandNormal::next() { return m_dist(engine); }
void RandNormal::setSeed(double seed) { setEngineSeed(seed); }
RandExponential::RandExponential(double mean) : m_dist(mean) {}
double RandExponential::next() {
  double n = m_dist(engine);
  return n;
}
void RandExponential::setSeed(double seed) { setEngineSeed(seed); }
}  // end namespace randomgen
