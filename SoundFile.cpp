#include "SoundFile.h"

#include <sndfile.h>
#include <stdexcept>
#include <cstring>
#include <boost/format.hpp>

struct SoundFile::Impl {
  SNDFILE* handle;
  SF_INFO info;
};

SoundFile::SoundFile(const std::filesystem::path& path): isWriting_(false) {
  impl_.reset(new Impl());
  memset(&impl_->info, 0, sizeof(SF_INFO));
  impl_->handle = sf_open(path.c_str(), SFM_READ, &impl_->info);
  if (!impl_->handle)
    throw std::runtime_error((boost::format("Can't open file %1%: %2%") % path % sf_strerror(NULL)).str());
}

SoundFile::SoundFile(const std::filesystem::path& path, int channels, double sampleRate): isWriting_(true) {
  impl_.reset(new Impl());
  memset(&impl_->info, 0, sizeof(SF_INFO));
  impl_->info.samplerate = static_cast<int>(sampleRate);
  impl_->info.channels = channels;
  impl_->info.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT | SF_ENDIAN_CPU;
  impl_->handle = sf_open(path.c_str(), SFM_WRITE, &impl_->info);
  if (!impl_->handle)
    throw std::runtime_error((boost::format("Can't open file %1%: %2%") % path % sf_strerror(NULL)).str());
}

SoundFile::~SoundFile() {
  if (isWriting_) sf_write_sync(impl_->handle);
  sf_close(impl_->handle);
}

int SoundFile::channels() const {
  return impl_->info.channels;
}

double SoundFile::sampleRate() const {
  return impl_->info.samplerate;
}

int SoundFile::read(double* buffer, int items) {
  return sf_readf_double(impl_->handle, buffer, items);
}

int SoundFile::write(const double* buffer, int items) {
  return sf_writef_double(impl_->handle, buffer, items);
}

