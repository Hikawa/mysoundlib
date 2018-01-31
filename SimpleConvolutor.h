#pragma once

#include <vector>
#include <memory>

#include "SoundProcessor.h"

class SimpleConvolutor: public SoundProcessor {
public:
  SimpleConvolutor(int inputStep, const std::vector<double>& filter)
    : filter_(filter)
    , inputStep_(inputStep)
  {
    init();
  }

  SimpleConvolutor(int inputStep, std::vector<double>&& filter)
    : filter_(filter)
    , inputStep_(inputStep)
  {
    init();
  }

  virtual int inputCount() override { return 1; }
  virtual int outputCount() override { return 1; }

  virtual bool process(double** inputs, double** outputs) override;

  virtual int inputStep() const override { return inputStep_; }

private:
  void init();

  std::vector<double> filter_;
  int inputStep_ = UNDEFINED_STEP;
  std::vector<double> buffer_; // filter_.size() - 1
};
