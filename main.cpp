#include "SoundFileReader.h"
#include "SoundFileWriter.h"
#include "SimpleConvolutor.h"
#include "FirDesign.h"
#include "DspWindows.h"
#include "Group.h"

int main(int argc, char* argv[]) {
  auto reader = new SoundFileReader(std::filesystem::path("test.wav"), 1024);
  std::vector<double> filter;
  filter.resize(1024 * 4);
  firLowpassCoefs(300.0/reader->sampleRate(), &blackmanHarrisWindow, filter);
  auto convolutor = new SimpleConvolutor(1024, filter);
  auto writer = new SoundFileWriter(std::filesystem::path("output.wav"), 1, reader->sampleRate());
  convolutor->inputPort(0)->setSource(reader->outputPort(0));
  writer->inputPort(0)->setSource(convolutor->outputPort(0));

  Group group(std::list<SoundProcessor*> {reader, convolutor, writer});
  group.run();
}

