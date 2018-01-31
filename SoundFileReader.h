#pragma once

#include "SoundProcessor.h"
#include "SoundFile.h"

class SoundFileReader: public SoundProcessor, public SoundFile {
public:
  SoundFileReader(const std::filesystem::path& path): SoundFile(path) {}

  virtual int inputCount() override { return 0; }
  virtual int outputCount() override { return channels(); }

  virtual bool process(double** inputs, double** outputs) override;

  virtual int inputStep() const override { return step_; }
  void setStep(int v) { step_ = v; }

private:
  int step_;
};

