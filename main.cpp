#include "SoundFileReader.h"
#include "SoundFileWriter.h"
#include "FftConvolutor.h"
#include "FirDesign.h"
#include "DspWindows.h"
#include "Group.h"
//#include "Parallel.h"

int main(int argc, char* argv[]) {
  auto reader = new SoundFileReader(std::filesystem::path("test.wav"), 1024);
  std::vector<double> filters[2];
  filters[0].resize(1024 * 4);
  firLowpassCoefs(2000.0/reader->sampleRate(), &blackmanHarrisWindow, filters[0]);
  filters[1].resize(1024 * 4);
  firLowpassCoefs(4000.0/reader->sampleRate(), &blackmanHarrisWindow, filters[1]);
  for (int i = 0; i < 1024*4; ++i)
    filters[1][i] -= filters[0][i];
  FftConvolutor* convolutors[2] = { new FftConvolutor(1024, filters[0]), new FftConvolutor(1024, filters[1]) };
  auto writer = new SoundFileWriter(std::filesystem::path("output.wav"), 2, reader->sampleRate());
  for (int i = 0; i < 2; ++i) {
    convolutors[i]->inputPort(0)->setSource(reader->outputPort(0));
    writer->inputPort(i)->setSource(convolutors[i]->outputPort(0));
  }

  Group group(std::list<SoundProcessor*> {reader, convolutors[0], convolutors[1], writer});
  group.run();
}

