# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/vorbis-src"
  "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/vorbis-build"
  "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/vorbis-subbuild/vorbis-populate-prefix"
  "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/vorbis-subbuild/vorbis-populate-prefix/tmp"
  "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/vorbis-subbuild/vorbis-populate-prefix/src/vorbis-populate-stamp"
  "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/vorbis-subbuild/vorbis-populate-prefix/src"
  "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/vorbis-subbuild/vorbis-populate-prefix/src/vorbis-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/vorbis-subbuild/vorbis-populate-prefix/src/vorbis-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/vorbis-subbuild/vorbis-populate-prefix/src/vorbis-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
