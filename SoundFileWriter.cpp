#include "SoundFileWriter.h"

#include <cstring>

SoundFileWriter::SoundFileWriter(const std::filesystem::path& path, int channels, double sampleRate)
  : SoundFile(path, channels, sampleRate)
{
  for (int ch = 0; ch < channels; ++ch) {
    inputPorts_.push_back(std::make_unique<InputPort>(*this));
  }
}

void SoundFileWriter::process() {
  const int s = step();

  if (channels() == 1) {
    const IOutputPort* source = inputPort(0)->source();
    write(source->data(), s);
    return;
  }

  std::unique_ptr<double[]> buffer {new double[channels() * s]};
  for (int i = 0; i < s; ++i)
    for (int ch = 0; ch < channels(); ++ch)
      buffer[i * channels() + ch] = inputPort(ch)->source()->data()[i];
  write(buffer.get(), s);
}

