#include "DspWindows.h"

#include <cmath>

double blackmanHarrisWindow(double x) {
  return 0.35875 - 0.48829*cos(2*M_PI*x) + 0.14128*cos(4*M_PI*x) - 0.01168*cos(6*M_PI*x);
}

