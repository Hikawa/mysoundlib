#include "SoundProcessor.h"

#include <cassert>

bool SoundProcessor::isFinished() const {
  for (const auto& p: outputPorts_)
    if (p->size() > 0) return false;
  return true;
}

void SoundProcessor::run() {
  do {
    process();
  } while (!isFinished());
}

