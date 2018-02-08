#include "Parallel.h"

namespace {

class ProxyOutputPort: public SoundProcessor::IOutputPort {
public:
  ProxyOutputPort(Parallel& processor, SoundProcessor::IOutputPort* target)
    : IOutputPort(processor)
    , target_(target)
  {}

  SoundProcessor::IOutputPort* target() const { return target_; }

  virtual int bufferSize() const override { return target()->bufferSize(); }
  virtual int size() const override { return target()->size(); }
  virtual void setSize(int v) override { target()->setSize(v); }
  virtual const double* data() const override { return target()->data(); }
  virtual double* data() override { return target()->data(); }

private:
  SoundProcessor::IOutputPort* target_;
};

class ProxyInputPort: public SoundProcessor::IInputPort {
public:
  ProxyInputPort(
    Parallel& processor,
    const std::list<SoundProcessor*>& children,
    int index)
    : IInputPort(processor)
  {
    for (SoundProcessor* child: children)
      if (index < child->inputPortCount())
        targets_.push_back(child->inputPort(index));
  }

  virtual const SoundProcessor::IOutputPort* source() const override { return source_; }
  virtual void setSource(const SoundProcessor::IOutputPort* v) override {
    source_ = v;
    for (SoundProcessor::IInputPort* target: targets_)
      target->setSource(v);
  }

private:
  const SoundProcessor::IOutputPort* source_;
  std::list<SoundProcessor::IInputPort*> targets_;
};

}

Parallel::Parallel(const std::list<SoundProcessor*>& children) {
  int maxInputs = 0;
  for (SoundProcessor* child: children) {
    children_.push_back(std::unique_ptr<SoundProcessor>(child));
    if (child->inputPortCount() > maxInputs)
      maxInputs = child->inputPortCount();
    for (int i = 0; i < child->outputPortCount(); ++i)
      outputPorts_.push_back(std::make_unique<ProxyOutputPort>(*this, child->outputPort(i)));
  }
  for (int i = 0; i < maxInputs; ++i)
    inputPorts_.push_back(std::make_unique<ProxyInputPort>(*this, children, i));
}

void Parallel::process() {
  for (const auto& child: children_)
    child->process();
}

