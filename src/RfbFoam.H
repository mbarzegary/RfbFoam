#include <sys/stat.h>
#include "fvCFD.H"
#include "singlePhaseTransportModel.H"
#include "turbulentTransportModel.H"
#include "simpleControl.H"
#include "fvOptions.H"
#include "IFstream.H"
#include <map>
#include <vector>
#include <algorithm>

#include "createStructures.H"
struct dataTransfer modelParams;

void useInterpInData();
void computeSherwoodCoeff();
void setSimpleMTCoeff();
void solveGoverningEqns();
void printOutput();

// Solver params, should be moved to external dictionary later
int maxFlowIters = 2500;
int maxScalarIters = 2000;
double flowResTol = 1.0e-5;
double scalarResTol = 1.0e-5;
bool onlyU = false;
bool onlyScalar = false;
