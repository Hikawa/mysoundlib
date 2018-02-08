#pragma once

#include <vector>
#include <memory>

#include "SoundProcessor.h"

class SimpleConvolutor: public SoundProcessor {
public:
  SimpleConvolutor(int step, const std::vector<double>& filter)
    : filter_(filter)
  {
    init(step);
  }

  SimpleConvolutor(int step, std::vector<double>&& filter)
    : filter_(filter)
  {
    init(step);
  }

  int step() const { return outputPort(0)->bufferSize(); }

  virtual void process() override;

private:
  void init(int step);

  std::vector<double> filter_;
  std::vector<double> buffer_; // filter_.size() - 1
  int bufferFilled_;
};
