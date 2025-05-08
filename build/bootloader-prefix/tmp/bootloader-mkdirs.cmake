# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/acg/esp/esp-idf/components/bootloader/subproject"
  "/home/acg/Programming/Drone/build/bootloader"
  "/home/acg/Programming/Drone/build/bootloader-prefix"
  "/home/acg/Programming/Drone/build/bootloader-prefix/tmp"
  "/home/acg/Programming/Drone/build/bootloader-prefix/src/bootloader-stamp"
  "/home/acg/Programming/Drone/build/bootloader-prefix/src"
  "/home/acg/Programming/Drone/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/acg/Programming/Drone/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/acg/Programming/Drone/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
