#pragma once
#include "calculation/PlateIFE.h"
namespace fosd {
namespace simulation {
// Compatibility include for the standalone validation tools.
using PlateIFE = Calculation::PlateIFE;
using Calculation::createPlateIFE;
} // namespace simulation
} // namespace fosd
