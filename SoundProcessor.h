#pragma once

#include <vector>
#include <memory>
#include <cassert>

class SoundProcessor {
public:
  class IOutputPort {
  public:
    IOutputPort(SoundProcessor& processor): processor_(processor) {
    }
    virtual ~IOutputPort() {}

    SoundProcessor& processor() const { return processor_; }

    virtual int bufferSize() const = 0;
    virtual int size() const = 0;
    virtual void setSize(int v) = 0;
    virtual const double* data() const = 0;
    virtual double* data() = 0;
  private:
    SoundProcessor& processor_;    
  };

  class OutputPort: public IOutputPort {
  public:
    OutputPort(SoundProcessor& processor, int bufferSize): IOutputPort(processor) {
      data_.resize(bufferSize);
    }

    virtual int bufferSize() const override { return data_.size(); }
    virtual int size() const override { return size_; }
    virtual void setSize(int v) override {
      assert(v <= bufferSize());
      size_ = v;
    }
    virtual const double* data() const override { return data_.data(); }
    virtual double* data() override { return data_.data(); }

  private:
    std::vector<double> data_;
    int size_;
  };

  class IInputPort {
  public:
    IInputPort(SoundProcessor& processor): processor_(processor) {}

    SoundProcessor& processor() const { return processor_; }

    virtual const IOutputPort* source() const = 0;
    virtual void setSource(const IOutputPort* v) = 0;

  private:
    SoundProcessor& processor_;
  };


  class InputPort: public IInputPort {
  public:
    InputPort(SoundProcessor& processor): IInputPort(processor) {}

    virtual const IOutputPort* source() const override { return source_; }
    virtual void setSource(const IOutputPort* v) override { source_ = v; }

  private:
    const IOutputPort* source_;
  };

  virtual ~SoundProcessor() {}

  virtual bool isFinished() const;
  
  int inputPortCount() const { return inputPorts_.size(); }
  IInputPort* inputPort(int i) const { return inputPorts_[i].get(); }

  int outputPortCount() const { return outputPorts_.size(); }
  IOutputPort* outputPort(int i) const { return outputPorts_[i].get(); }

  virtual void process() = 0;

  void run();
  //std::vector<double>* run(const std::vector<double>* inputs);

protected:
  std::vector<std::unique_ptr<IInputPort>> inputPorts_;
  std::vector<std::unique_ptr<IOutputPort>> outputPorts_;
};

