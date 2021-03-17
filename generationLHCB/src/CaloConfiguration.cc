// Fedor Ratnikov
#include "CaloConfiguration.hh"

const double LHCbMmodule = 121.2 * mm;


CaloConfiguration::CaloConfiguration () :
  isShashlik (true),
  moduleSize (LHCbMmodule),
  absorberPV (0),
  sensitivePV (0)

{}


CaloConfiguration::CaloConfiguration (const ShashlikCaloConfiguration& cfg) :
  isShashlik (true),
  shaslik (cfg),
  moduleSize (LHCbMmodule),
  absorberPV (0),
  sensitivePV (0)
{}

CaloConfiguration::CaloConfiguration (const SpacalCaloConfiguration& cfg);
  isShashlik (false),
  spacal (cfg),
  moduleSize (LHCbMmodule),
  absorberPV (0),
  sensitivePV (0)

  {}
  
int CaloConfiguration::modules() const {
  return isShashlik ? shashlik.modules : spacal.modules;
} 

int CaloConfiguration::cells() const {
  return isShashlik ? shashlik.cells : spacal.cells;
}

double CaloConfiguration::calorLength () const {
  return isShashlik ? ((shashlik.sciDepth + shashlik.absorberDepth)*shashlik.nLayers : spacal.length;}
}

double CaloConfiguration::frontCellLength () const {
  return isShashlik ? shashlik.frontCellLength : spacal.frontCellLength;
}
