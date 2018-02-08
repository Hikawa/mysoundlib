#include "FirDesign.h"

#include <cmath>

void firLowpassCoefs(
  double freq,
  const std::function<double(double)>& window,
  std::vector<double>& result)
{
  const double w = freq * 2.0 * M_PI;
  for (int i = 0; i < result.size(); ++i) {
    double v;
    const double n = i - 0.5*(result.size()-1);
    if ((result.size() % 2) && (i == result.size()/2))
      v = w/M_PI;
    else v = sin(w*n)/(M_PI*n);
    result[i] = v*window(i/(result.size()-1.0));
  }
}

