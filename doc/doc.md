# Introduction

PAPAS (PAramatrized PArticle Simulation)is a particle simulatation and reconstruction algorithm which is designed to allow users to test the performance of detector design. The approach is fast simulation/ reconstruction. Users must provide simple C++ classes to define key etector parameters eg tracker acceptance and calorimeter radius and length. An implementation of the CMS detector is provided as a starting point. 


PAPAS propagates stable generated particles through a simple detector model.

In the tracker, charged particles may be detected as tracks, taking into account the acceptance, efficiency, and momentum resolution of this detector.

In the calorimeters, particles are detected as energy deposits. The energy deposits are modelled by taking into account the following detector properties: energy resolution, acceptance, energy thresholds, and characteristic size. The latter is defined as the distance between two clusters below which the two clusters cannot be resolved and are considered as a single cluster.

A particle flow algorithm then runs over the simulated tracks and clusters to identify and reconstruct charged hadrons, photons, and neutral hadrons. These particles can then be used as an input to higher-level algorithms like jet clustering, or directly in the analysis.

TIP Electrons and muons are passed through PAPAS without any modification, and the user is responsible for applying is own efficiency and resolution models. The hadronic decay products of tau leptons are simulated just like other hadrons and photons.

A CMS-like detector model is provided as an example.

# Quick-start

An up-to-date installation and quick start guide for the impatient user can be found on the [PAPAS github page](https://github.com/alicerobson/papas).
