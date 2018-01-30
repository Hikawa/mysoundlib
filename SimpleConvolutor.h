#pragma once

#include <vector>
#include <memory>

#include "SoundProcessor.h"

class SimpleConvolutor: public SoundProcessor {
public:
  SimpleConvolutor() {}
  SimpleConvolutor(const std::vector<double>& filter) {
    setFilter(filter);
  }
  SimpleConvolutor(std::vector<double>&& filter) {
    setFilter(filter);
  }

  std::vector<double>& filter() const { return filter_; }
  void setFilter(const std::vector<double>& v) { filter_ = v; }
  void setFilter(std::vector<double>&& v) { filter_.swap(v); }

  virtual int inputCount() override { return 1; }
  virtual int outputCount() override { return 1; }

  virtual int inputStep() const override { return inputStep_; }
  virtual void setInputStep(int v) override;

private:
  std::vector<double> filter_;
  int inputStep_;
  std::unique_ptr<double[]> buffer_;
};