#pragma once

class SoundProcessor {
public:
  virtual ~SoundProcessor() {}

  virtual int inputCount() = 0;
  virtual int outputCount() = 0;

  virtual void process(double** inputs, double** outputs) = 0;

  virtual int inputStep() const = 0;
  virtual void setInputStep(int v) = 0;

  virtual int outputStep() const { return inputStep(); }
};
