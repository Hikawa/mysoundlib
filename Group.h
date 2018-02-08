#pragma once

#include "SoundProcessor.h"

#include <list>
#include <vector>
#include <memory>

class Group: public SoundProcessor {
public:
  Group(const std::list<SoundProcessor*>& children);

  virtual void process() override;

private:
  std::vector<std::unique_ptr<SoundProcessor>> children_;
};

