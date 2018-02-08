#pragma once

#include "SoundProcessor.h"
#include "SoundFile.h"

class SoundFileWriter: public SoundProcessor, public SoundFile {
public:
  SoundFileWriter(const std::filesystem::path& path, int channels, double sampleRate);

  virtual void process() override;
  int step() const { return inputPort(0)->source()->bufferSize(); }

  virtual bool isFinished() const override { return inputPort(0)->source()->size() <= 0; }

private:
};

