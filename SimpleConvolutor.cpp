#include "SimpleConvolutor.h"

#include <cstring>
#include <cassert>

void SimpleConvolutor::init(int step) {
  inputPorts_.push_back(std::make_unique<InputPort>(*this));
  outputPorts_.push_back(std::make_unique<OutputPort>(*this, step));

  buffer_.resize(filter_.size() - 1);
  memset(buffer_.data(), 0, buffer_.size() * sizeof(double));
  bufferFilled_ = buffer_.size();
}

void SimpleConvolutor::process() {
  const int s = step();
  const IOutputPort* source = inputPort(0)->source();
  assert(source);
  assert(source->bufferSize() == s);
  const double* input = source->data();
  double* output = outputPort(0)->data();
  
  for (int n = 0; n < s; ++n) {
    double acc = 0.0;
    if (n < bufferFilled_) acc += buffer_[n];
    for (int k = 0; (k < filter_.size()) && (k < n + 1); ++k) {
      acc += filter_[k] * input[n - k];
    }
    output[n] = acc;
  }

  bufferFilled_ += (- s + source->size());

  for (int p = 0; p < bufferFilled_; ++p) {
    const int n = s + p;
    double acc = 0.0;
    if (n < buffer_.size()) acc += buffer_[n];
    for (int k = p + 1; (k < filter_.size()) && (k < n + 1); ++k) {
      acc += filter_[k] * input[n - k];
    }
    buffer_[p] = acc;
  }

  int outputSize = s;
  if (bufferFilled_ < 0) outputSize += bufferFilled_;
  if (outputSize < 0) outputSize = 0;

  outputPort(0)->setSize(outputSize);
}

