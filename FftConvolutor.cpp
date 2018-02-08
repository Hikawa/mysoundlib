#include "FftConvolutor.h"

#include <cmath>
#include <fftw3.h>
#include <cstring>

class FftConvolutor::Impl {
public:
  Impl(FftConvolutor& parent, int step, const std::vector<double>& filter)
    : parent_(parent)
    , filter_(filter)
  {
    init(step);
  }

  Impl(FftConvolutor& parent, int step, std::vector<double>&& filter)
    : parent_(parent)
    , filter_(filter)
  {
     init(step);
  }

  void init(int step) {
    parent_.inputPorts_.push_back(std::make_unique<InputPort>(parent_));
    parent_.outputPorts_.push_back(std::make_unique<OutputPort>(parent_, step));

    fftSize_ = step + filter_.size() - 1;
    fftSize_ = 1 << static_cast<int>(ceil(log2(fftSize_)));
    
    input_.reset(new double[fftSize_]);
    memset(input_.get(), 0, fftSize_ * sizeof(double));
    spectrum_.reset(new fftw_complex[(fftSize_ >> 1) + 1]);
    output_.reset(new double[fftSize_]);
    buffer_.reset(new double[fftSize_ - step]);
    memset(buffer_.get(), 0, (fftSize_ - step)* sizeof(double));
    bufferFilled_ = fftSize_ - step;

    analPlan_ = fftw_plan_dft_r2c_1d(fftSize_, input_.get(), spectrum_.get(), FFTW_ESTIMATE);
    synthPlan_ = fftw_plan_dft_c2r_1d(fftSize_, spectrum_.get(), output_.get(), FFTW_ESTIMATE);

    filterSpectrum_.reset(new fftw_complex[(fftSize_ >> 1) + 1]);
    { // filter spectrum
      std::unique_ptr<double[]> input {new double[fftSize_]};
      memset(input.get(), 0, fftSize_ * sizeof(double));
      memcpy(input.get(), filter_.data(), filter_.size() * sizeof(double));
      fftw_plan anal = fftw_plan_dft_r2c_1d(fftSize_, input.get(), filterSpectrum_.get(), FFTW_ESTIMATE);
      fftw_execute(anal);
      fftw_destroy_plan(anal);
    }
  }

  void process() {
    const int s = step();
    const IOutputPort* source = parent_.inputPort(0)->source();
    assert(source);
    assert(source->bufferSize() == s);
    IOutputPort* target = parent_.outputPort(0);

    memcpy(input_.get(), source->data(), source->size() * sizeof(double));
    memset(&input_[source->size()], 0, (fftSize_ - source->size()) * sizeof(double));
    fftw_execute(analPlan_);
    for (int i = 0; i < (fftSize_ >> 1) + 1; ++i) {
      const double a = spectrum_[i][0]*filterSpectrum_[i][0] - spectrum_[i][1]*filterSpectrum_[i][1];
      const double b = spectrum_[i][0]*filterSpectrum_[i][1] + spectrum_[i][1]*filterSpectrum_[i][0];
      spectrum_[i][0] = a / fftSize_;
      spectrum_[i][1] = b / fftSize_;
    }
    fftw_execute(synthPlan_);

    memcpy(target->data(), output_.get(), s * sizeof(double));
    for (int i = 0; i < std::min(bufferFilled_, s); ++i) {
      target->data()[i] += buffer_[i];
    }

    bufferFilled_ += (- s + source->size());
    int outputSize = s;
    if (bufferFilled_ < 0) outputSize += bufferFilled_;
    if (outputSize < 0) outputSize = 0;

    for (int i = 0; i < bufferFilled_; ++i) {
      const int n = s + i;
      double acc = output_[n];
      if (n < fftSize_ - s) acc += buffer_[n];
      buffer_[i] = acc;
    }

    target->setSize(outputSize);
  }

  int step() const { return parent_.step(); }

  FftConvolutor& parent_;
  int fftSize_;
  std::vector<double> filter_;
  std::unique_ptr<fftw_complex[]> filterSpectrum_;
  std::unique_ptr<double[]> input_;
  fftw_plan analPlan_;
  std::unique_ptr<fftw_complex[]> spectrum_;
  fftw_plan synthPlan_;
  std::unique_ptr<double[]> output_;
  std::unique_ptr<double[]> buffer_;
  int bufferFilled_;
};

FftConvolutor::FftConvolutor(int step, const std::vector<double>& filter)
{
  impl_.reset(new Impl(*this, step, filter));
}

FftConvolutor::FftConvolutor(int step, std::vector<double>&& filter)
{
  impl_.reset(new Impl(*this, step, filter));
}


FftConvolutor::~FftConvolutor() {}

void FftConvolutor::process() {
  impl_->process();
}

