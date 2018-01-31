#include "SoundFileWriter.h"

#include <cstring>

bool SoundFileWriter::process(double** inputs, double** outputs) {
  if (channels() == 1) {
    write(inputs[0], step_);
    return true;
  }
  std::unique_ptr<double[]> buffer {new double[channels() * step_]};
  for (int i = 0; i < step_; ++i)
    for (int ch = 0; ch < channels(); ++ch)
      buffer[i * channels() + ch] = inputs[ch][i];
  write(buffer.get(), step_);
  return true;
}

