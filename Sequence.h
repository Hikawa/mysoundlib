#pragma once

#include "SoundProcessor.h"

#include <list>
#include <memory>

class Sequence: public SoundProcessor {
public:
  Sequence() {}

  void pushBack(const std::shared_ptr<SoundProcessor>& v);

  virtual int inputCount() override;
  virtual int outputCount() override;

  virtual bool process(double** inputs, double** outputs) override;

  virtual int inputStep() const override;
  virtual int outputStep() const override;

private:
  std::list<std::shared_ptr<SoundProcessor>> children_;
};

