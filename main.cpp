#include "SoundFileReader.h"
#include "SoundFileWriter.h"
//#include "SimpleConvolutor.h"
#include "Group.h"

int main(int argc, char* argv[]) {
  auto reader = new SoundFileReader(std::filesystem::path("test.wav"), 1024);
  //SimpleConvolutor convolutor();
  auto writer = new SoundFileWriter(std::filesystem::path("output.wav"), 1, reader->sampleRate());
  writer->inputPort(0)->setSource(reader->outputPort(0));

  Group group(std::list<SoundProcessor*> {reader, writer});
  group.run();
}

