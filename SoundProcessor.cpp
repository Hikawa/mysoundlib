#include "SoundProcessor.h"

#include <cassert>
#include <cstring>

bool SoundProcessor::isFinished() const {
  for (const auto& p: outputPorts_)
    if (p->size() > 0) return false;
  return true;
}
/*
std::vector<double>* SoundProcessor::run(const std::vector<double>* inputs) {
  std::vector<double>* result = new std::vector<double>[outputPortCount()];
  struct Position {
    const double* current;
    const double* end;
    std::unique_ptr<OutputPort> port;
  }
  std::unique_ptr<Position[]> inputPositions { new Position[inputPortCount()] };
  for (int i = 0; i < inputPortCount(); ++i) {
    inputPositions[i].current = inputs[i].data();
    inputPositions[i].end = inputs[i].data() + inputs[i].size();
    inputPositions[i].port.reset(new OutputPort(*this, inputPort(i)->));
  }
  do {
    for (int i = 0; i < inputPortCount(); ++i) {
      IInputPort* source = inputPort(i);
      mem
      int copy = inputPositions[i].end - inputPositions[i].current;
      if (copy > 0)
        memcpy(source->data(), inputPositions[i]., copy * sizeof(double));
    }
  } while (!isFinished());
  return result;
}
*/

void SoundProcessor::run() {
  assert(inputPortCount() == 0);
  do {
    process();
  } while (!isFinished());
}

void SoundProcessor::setSources(const SoundProcessor* sources) {
  assert(sources->outputPortCount() >= inputPortCount());
  for (int i = 0; i < inputPortCount(); ++i)
    inputPort(i)->setSource(sources->outputPort(i));
}

