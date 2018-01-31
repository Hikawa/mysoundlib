#include "SoundFileReader.h"
#include "SoundFileWriter.h"
//#include "SimpleConvolutor.h"
#include "Sequence.h"

int main(int argc, char* argv[]) {
  auto reader = std::make_shared<SoundFileReader>(std::filesystem::path("test.wav"));
  //SimpleConvolutor convolutor();
  auto writer = std::make_shared<SoundFileWriter>(std::filesystem::path("output.wav"), 1, reader->sampleRate());

  auto sequence = std::make_unique<Sequence>();
  sequence->pushBack(reader);
  sequence->pushBack(writer);
  sequence->run();
}

