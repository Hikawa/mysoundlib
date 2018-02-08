#pragma once

#include <vector>
#include <functional>

void firLowpassCoefs(
  double freq,
  const std::function<double(double)>& window,
  std::vector<double>& result);
