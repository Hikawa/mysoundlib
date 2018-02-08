#include "SoundFileReader.h"
#include "SoundFileWriter.h"
#include "FftConvolutor.h"
#include "FirDesign.h"
#include "DspWindows.h"
#include "Group.h"
#include "Parallel.h"

int main(int argc, char* argv[]) {
  auto reader = new SoundFileReader(std::filesystem::path("test.wav"), 1024);
  std::vector<double> filters[2];
  filters[0].resize(1024 * 4);
  firLowpassCoefs(2000.0/reader->sampleRate(), &blackmanHarrisWindow, filters[0]);
  filters[1].resize(1024 * 4);
  firLowpassCoefs(4000.0/reader->sampleRate(), &blackmanHarrisWindow, filters[1]);
  for (int i = 0; i < 1024*4; ++i)
    filters[1][i] -= filters[0][i];
  auto convolutors = new Parallel(std::list<SoundProcessor*> { new FftConvolutor(1024, filters[0]), new FftConvolutor(1024, filters[1]) });

  auto writer = new SoundFileWriter(std::filesystem::path("output.wav"), 2, reader->sampleRate());
  convolutors->inputPort(0)->setSource(reader->outputPort(0));
  writer->setSources(convolutors);

  Group group(std::list<SoundProcessor*> {reader, convolutors, writer});
  group.run();
}

