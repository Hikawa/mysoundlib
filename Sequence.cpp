#include "Sequence.h"

#include <stdexcept>
#include <cstring>

void Sequence::pushBack(const std::shared_ptr<SoundProcessor>& v) {
  if (!children_.empty()) {
    if (children_.back()->outputCount() < v->inputCount())
      throw std::runtime_error("Sequence: not enough inputs");
    if (children_.back()->outputStep() != v->inputStep())
      throw std::runtime_error("Sequence: last->outputStep() != next->inputStep()");
  }
  children_.push_back(v);
}

int Sequence::inputCount() {
  if (children_.empty()) return 0;
  return children_.front()->inputCount();
}

int Sequence::outputCount() {
  if (children_.empty()) return 0;
  return children_.back()->outputCount();
}

bool Sequence::process(double** inputs, double** outputs) {
  if (children_.empty()) return true;
  double** buffer = inputs;
  for (const auto& it: children_) {
    double** target;
    if (it == children_.back())
      target = outputs;
    else {
      target = new double*[it->outputCount()];
      for (int i = 0; i < it->outputCount(); ++i)
        target[i] = new double[it->outputStep()];
    }
    const bool result = it->process(buffer, target);
    if (target != outputs) {
      for (int i = 0; i < it->outputCount(); ++i)
        delete [] target;
      delete [] target;
    }
    if (!result) return false;
  }
  return true;
}

int Sequence::inputStep() const {
  if (children_.empty()) return 0;
  return children_.front()->inputStep();
}

int Sequence::outputStep() const {
  if (children_.empty()) return 0;
  return children_.back()->outputStep();
}

