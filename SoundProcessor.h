#pragma once

class SoundProcessor {
public:
  virtual ~SoundProcessor() {}

  virtual int inputCount() = 0;
  virtual int outputCount() = 0;

  virtual bool process(double** inputs, double** outputs) = 0; // return continue?

  virtual int inputStep() const = 0;
  virtual int outputStep() const { return inputStep(); }

  void run();

  static const int UNDEFINED_STEP = -1;
};

