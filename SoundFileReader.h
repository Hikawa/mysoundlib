#pragma once

#include "SoundProcessor.h"
#include "SoundFile.h"

class SoundFileReader: public SoundProcessor, public SoundFile {
public:
  SoundFileReader(const std::filesystem::path& path, int step);

  int step() const { return outputPort(0)->bufferSize(); }

  virtual void process() override;

private:
};

