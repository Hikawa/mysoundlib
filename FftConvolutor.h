#pragma once

#include "SoundProcessor.h"

class FftConvolutor: public SoundProcessor {
public:
  FftConvolutor(int step, const std::vector<double>& filter);
  FftConvolutor(int step, std::vector<double>&& filter);

  ~FftConvolutor();

  int step() const { return outputPort(0)->bufferSize(); }
  virtual void process() override;

private:
  void init(int step);
  class Impl;
  std::unique_ptr<Impl> impl_;
};

