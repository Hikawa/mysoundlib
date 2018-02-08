#pragma once

#include <vector>
#include <memory>
#include <cassert>

class SoundProcessor {
public:
  class OutputPort {
  public:
    OutputPort(SoundProcessor& processor, int bufferSize): processor_(processor) {
      data_.resize(bufferSize);
    }

    SoundProcessor& processor() const { return processor_; }

    int bufferSize() const { return data_.size(); }
    int size() const { return size_; }
    void setSize(int v) {
      assert(v <= bufferSize());
      size_ = v;
    }
    const double* data() const { return data_.data(); }
    double* data() { return data_.data(); }

  private:
    SoundProcessor& processor_;
    std::vector<double> data_;
    int size_;
  };

  class InputPort {
  public:
    InputPort(SoundProcessor& processor): processor_(processor) {}

    SoundProcessor& processor() const { return processor_; }

    const OutputPort* source() const { return source_; }
    void setSource(const OutputPort* v) { source_ = v; }

  private:
    SoundProcessor& processor_;
    const OutputPort* source_;
  };

  virtual ~SoundProcessor() {}

  bool isFinished() const;
  
  int inputPortCount() const { return inputPorts_.size(); }
  InputPort* inputPort(int i) const { return inputPorts_[i].get(); }

  int outputPortCount() const { return outputPorts_.size(); }
  OutputPort* outputPort(int i) const { return outputPorts_[i].get(); }

  virtual void process() = 0;

  void run();

protected:
  std::vector<std::unique_ptr<InputPort>> inputPorts_;
  std::vector<std::unique_ptr<OutputPort>> outputPorts_;
};

