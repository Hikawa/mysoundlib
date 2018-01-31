#include "SoundProcessor.h"

#include <cassert>

void SoundProcessor::run() {
  assert(inputCount() == 0);
  double** outputs = nullptr;
  if (outputCount() > 0) {
    outputs = new double*[outputCount()];
    for (int i = 0; i < outputCount(); ++i)
      outputs[i] = new double[outputStep()];
  }
  while (process(nullptr, outputs));
}

