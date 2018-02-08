#pragma once

#include "SoundProcessor.h"

#include <list>

class Parallel: public SoundProcessor {
public:
  Parallel(const std::list<SoundProcessor*>& children);

  virtual void process() override;

private:

  std::vector<std::unique_ptr<SoundProcessor>> children_;
};

