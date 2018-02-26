# Copyright 2018, mike at github.com/iskinmike
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required ( VERSION 2.8.12 )

project ( fingerprint_for_wilton CXX )

include_directories ( ${PROJECT_NAME}
        ${CMAKE_SOURCE_DIR}/include
) # futronic api to get images from FS80

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -std=c++11 -O2")

link_directories("/usr/lib/i386-linux-gnu/" "/lib/i386-linux-gnu/")

link_libraries ( 
        ${CMAKE_SOURCE_DIR}/libs/libScanAPI-intel.so # added libscan library for FS80
        popt
        /lib/i386-linux-gnu/libpng12.so.0
        -lpthread)

add_executable(${PROJECT_NAME} src/main.cpp src/ftrScan.cpp src/bitmap_to_png.cpp)
