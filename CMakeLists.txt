cmake_minimum_required(VERSION 2.6)
project(mysoundlib)

find_package(PkgConfig)
pkg_check_modules(SNDFILE REQUIRED sndfile)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
  ${SNDFILE_INCLUDE_DIRS})

SET(HEADERS
  SoundProcessor.h
  SimpleConvolutor.h
  SoundFile.h
  SoundFileReader.h
  SoundFileWriter.h
  Group.h
  FirDesign.h
  DspWindows.h)
  
SET(SOURCES
  SoundProcessor.cpp
  SimpleConvolutor.cpp
  SoundFile.cpp
  SoundFileReader.cpp
  SoundFileWriter.cpp
  Group.cpp
  FirDesign.cpp
  DspWindows.cpp
  main.cpp)


add_executable(mysoundlib ${HEADERS} ${SOURCES})
target_link_libraries(mysoundlib ${SNDFILE_LIBRARIES} stdc++fs)  

install(TARGETS mysoundlib RUNTIME DESTINATION bin)

