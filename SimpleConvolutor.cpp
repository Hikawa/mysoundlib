#include "SimpleConvolutor.h"

#include <cstring>

void SimpleConvolutor::init() {
  buffer_.resize(filter_.size() - 1);
  memset(buffer_.data(), 0, buffer_.size() * sizeof(double));
}

bool SimpleConvolutor::process(double** inputs, double** outputs) {
  for (int n = 0; n < inputStep_; ++n) {
    double s = (n < buffer_.size())? buffer_[n]: 0.0;
    for (int k = 0; (k < filter_.size()) && (k < n + 1); ++k) {
      s += filter_[k] * inputs[0][n - k];
    }
    outputs[0][n] = s;
  }

  for (int p = 0; p < filter_.size() - 1; ++p) {
    const int n = inputStep_ + p;
    double s = (n < buffer_.size())? buffer_[n]: 0.0;
    for (int k = p + 1; (k < filter_.size()) && (k < n + 1); ++k) {
      s += filter_[k] * inputs[0][n - k];
    }
    buffer_[p] = s;
  }
  return true;
}

