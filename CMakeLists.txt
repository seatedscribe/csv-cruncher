#  Copyright (C) 2015 Enrico Mariotti <enricomariotti@yahoo.it>
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 or (at your option)
#  version 3 of the License.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

project(csvcruncher)
cmake_minimum_required(VERSION 2.8)
#aux_source_directory(. SRC_LIST)

#add_executable(${PROJECT_NAME} ${SRC_LIST})

macro(add_gcc_compiler_cxxflags FLAGS)
  if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")
  endif()
endmacro(add_gcc_compiler_cxxflags)

macro(add_gcc_compiler_cflags FLAGS)
  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAGS}")
  endif()
endmacro(add_gcc_compiler_cflags)

macro(add_gcc_compiler_flags FLAGS)
  add_gcc_compiler_cxxflags("${FLAGS}")
  add_gcc_compiler_cflags("${FLAGS}")
endmacro(add_gcc_compiler_flags)

add_gcc_compiler_flags("-O0 -g -Wall -Wextra -Wundef -Wpointer-arith -Wno-long-long")
add_gcc_compiler_cxxflags("-Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual")


add_subdirectory(src)
add_subdirectory(examples)



