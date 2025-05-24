# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/ogg-src"
  "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/ogg-build"
  "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/ogg-subbuild/ogg-populate-prefix"
  "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/ogg-subbuild/ogg-populate-prefix/tmp"
  "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/ogg-subbuild/ogg-populate-prefix/src/ogg-populate-stamp"
  "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/ogg-subbuild/ogg-populate-prefix/src"
  "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/ogg-subbuild/ogg-populate-prefix/src/ogg-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/ogg-subbuild/ogg-populate-prefix/src/ogg-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/eli98/Downloads/SFML-3.0.1/SFML-3.0.1/build/_deps/ogg-subbuild/ogg-populate-prefix/src/ogg-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
