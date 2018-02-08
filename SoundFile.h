#pragma once

#include <experimental/filesystem>
namespace std {
namespace filesystem = experimental::filesystem;
}
#include <memory>

class SoundFile {
public:
  SoundFile(const std::filesystem::path& path); // read
  SoundFile(const std::filesystem::path& path, int channels, double sampleRate); // write
  ~SoundFile();

  int channels() const;
  double sampleRate() const;
  int read(double* buffer, int items);
  int write(const double* buffer, int items);

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
  bool isWriting_;
};

