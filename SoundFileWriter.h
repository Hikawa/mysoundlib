#pragma once

#include "SoundProcessor.h"
#include "SoundFile.h"

class SoundFileWriter: public SoundProcessor, public SoundFile {
public:
  SoundFileWriter(const std::filesystem::path& path, int channels, double sampleRate):
    SoundFile(path, channels, sampleRate)
  {}

  virtual int inputCount() override { return channels(); }
  virtual int outputCount() override { return 0; }

  virtual bool process(double** inputs, double** outputs) override;

  virtual int inputStep() const override { return step_; }
  void setStep(int v) { step_ = v; }

private:
  int step_;
};

