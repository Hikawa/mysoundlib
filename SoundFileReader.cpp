#include "SoundFileReader.h"

#include <cstring>

SoundFileReader::SoundFileReader(const std::filesystem::path& path, int step)
  : SoundFile(path)
{
  assert(channels() > 0);
  for (int ch = 0; ch < channels(); ++ch) {
    outputPorts_.push_back(std::make_unique<OutputPort>(*this, step));
  }
}

void SoundFileReader::process() {
  const int s = step();

  if (channels() == 1) {
    OutputPort* target = outputPort(0);
    double* output = target->data();
    memset(output, 0, s * sizeof(double));
    target->setSize(read(output, s));
    return;
  }

  std::unique_ptr<double[]> buffer {new double[channels() * s]};
  memset(buffer.get(), 0, s * channels() * sizeof(double));
  const int count = read(buffer.get(), s);
  for (int i = 0; i < s; ++i)
    for (int ch = 0; ch < channels(); ++ch)
      outputPort(ch)->data()[i] = buffer[i * channels() + ch];
  for (int ch = 0; ch < channels(); ++ch)
    outputPort(ch)->setSize(count);
}

