#include "SoundFileReader.h"

#include <cstring>

bool SoundFileReader::process(double** inputs, double** outputs) {
  if (channels() == 1) {
    memset(outputs[0], 0, step_ * sizeof(double));
    return (read(outputs[0], step_) > 0);
  }
  std::unique_ptr<double[]> buffer {new double[channels() * step_]};
  memset(buffer.get(), 0, step_ * channels() * sizeof(double));
  const int count = read(buffer.get(), step_);
  for (int i = 0; i < step_; ++i)
    for (int ch = 0; ch < channels(); ++ch)
      outputs[ch][i] = buffer[i * channels() + ch];
  return count > 0;
}

