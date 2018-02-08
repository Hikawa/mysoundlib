#include "Group.h"

#include <stdexcept>
#include <algorithm>

Group::Group(const std::list<SoundProcessor*>& children) {
  std::list<SoundProcessor*> acc;
  for (SoundProcessor* child: children) {
    auto lastSource = acc.end();
    for (auto it = acc.begin(); it != acc.end(); ++it) {
      for (int input = 0; input < child->inputPortCount(); ++input) {
        if (&child->inputPort(input)->source()->processor() == *it) {
          lastSource = it;
          break;
        }
      }
    }
    acc.insert(lastSource == acc.end()? acc.begin(): std::next(lastSource), child);
  }
  for (SoundProcessor* child: acc)
    children_.push_back(std::unique_ptr<SoundProcessor>(child));

  std::list<IInputPort*> inputs;
  std::list<IOutputPort*> outputs;

  for (const auto& child: children_) {
    for (int i = 0; i < child->inputPortCount(); ++i)
      inputs.push_back(child->inputPort(i));
    for (int i = 0; i < child->outputPortCount(); ++i)
      outputs.push_back(child->outputPort(i));
  }

  for (const auto& child: children_) {
    bool before = true;
    for (const auto& other: children_) {
      if (other == child) before = false;
      for (int i = 0; i < child->inputPortCount(); ++i) {
        if (&child->inputPort(i)->source()->processor() == other.get()) {
          if (!before) throw std::runtime_error("Dependency loop");
          inputs.remove(child->inputPort(i));
          outputs.remove(const_cast<IOutputPort*>(child->inputPort(i)->source()));
        }
      }
    }
  }

  for (IInputPort* input: inputs)
    inputPorts_.push_back(std::make_unique<ProxyInputPort>(*this, input));

  for (IOutputPort* output: outputs)
    outputPorts_.push_back(std::make_unique<ProxyOutputPort>(*this, output));
}

void Group::process() {
  for (const auto& child: children_)
    child->process();
}

bool Group::isFinished() const {
  for (const auto& child: children_)
    if (!child->isFinished()) return false;
  return true;
}

