// Fedor Ratnikov
#include "CaloConfiguration.hh"

const double LHCbMmodule = 121.2 * mm;


CaloConfiguration::CaloConfiguration () :
  fIsShashlik (true),
  fModuleSize (LHCbMmodule),
  fAbsorberPV (0),
  fSensitivePV (0)

{}


CaloConfiguration::CaloConfiguration (const ShashlikCaloConfiguration& cfg) :
  fIsShashlik (true),
  fShashlik (cfg),
  fModuleSize (LHCbMmodule),
  fAbsorberPV (0),
  fSensitivePV (0)
{}

CaloConfiguration::CaloConfiguration (const SpacalCaloConfiguration& cfg) :
  fIsShashlik (false),
  fSpacal (cfg),
  fModuleSize (LHCbMmodule),
  fAbsorberPV (0),
  fSensitivePV (0)
  
{}
  
int CaloConfiguration::modules() const {
  return isShashlik() ? fShashlik.modules : fSpacal.modules;
} 

int CaloConfiguration::cells() const {
  return isShashlik() ? fShashlik.cells : fSpacal.cells;
}

double CaloConfiguration::calorLength () const {
  return isShashlik() ? (fShashlik.sciDepth + fShashlik.absorberDepth)*fShashlik.nLayers : fSpacal.length;
}

double CaloConfiguration::frontCellLength () const {
  return isShashlik() ? fShashlik.frontCellLength : fSpacal.frontCellLength;
}
