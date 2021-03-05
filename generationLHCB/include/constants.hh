#ifndef constants_h
#define constants_h 1

#include "G4SystemOfUnits.hh"

const int modulesX = 5;
const int modulesY = 5;
const int moduleCells = 6;  // assume square modules
//const int moduleCells = 60;  // assume square modules
const int calGranularityX = modulesX * moduleCells;
const int calGranularityY = modulesY * moduleCells;
//const G4double moduleSize = 121.2 * mm; // regular setup
const G4double moduleSize = 121.2 * mm / 5; // old setup
const G4double calorSizeX = moduleSize * modulesX;
const G4double calorSizeY = moduleSize * modulesY;
const G4double cellSize = moduleSize / moduleCells;

const G4double scintillatorThickness = 4. * mm;
const G4double absorberThickness = 2. * mm;
const int nLayers = 66;

const G4double layerThickness  = scintillatorThickness + absorberThickness;
const G4double totalLength = nLayers * layerThickness;
const int aggregateLayers = 66;
//const int aggregateLayers = 1;
const int nLogLayers = nLayers/aggregateLayers;



#endif
