#pragma once

#include "SoundProcessor.h"

#include <list>
#include <vector>
#include <memory>

class Group: public SoundProcessor {
public:
  class ProxyOutputPort: public IOutputPort {
  public:
    ProxyOutputPort(Group& processor, IOutputPort* target)
      : IOutputPort(processor)
      , target_(target)
    {}

    IOutputPort* target() const { return target_; }

    virtual int bufferSize() const override { return target()->bufferSize(); }
    virtual int size() const override { return target()->size(); }
    virtual void setSize(int v) override { target()->setSize(v); }
    virtual const double* data() const override { return target()->data(); }
    virtual double* data() override { return target()->data(); }

  private:
    IOutputPort* target_;
  };

  class ProxyInputPort: public IInputPort {
  public:
    ProxyInputPort(Group& processor, IInputPort* target)
      : IInputPort(processor)
      , target_(target)
    {}

    IInputPort* target() const { return target_; }

    virtual const IOutputPort* source() const override { return target()->source(); }
    virtual void setSource(const IOutputPort* v) override { target()->setSource(v); }

  private:
    IInputPort* target_;
  };

  Group(const std::list<SoundProcessor*>& children);

  virtual void process() override;

private:
  std::vector<std::unique_ptr<SoundProcessor>> children_;
};

