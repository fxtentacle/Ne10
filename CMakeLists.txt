#
#  Copyright 2011-14 ARM Limited
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#    * Neither the name of ARM Limited nor the
#      names of its contributors may be used to endorse or promote products
#      derived from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY ARM LIMITED AND CONTRIBUTORS "AS IS" AND
#  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
#  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
cmake_minimum_required(VERSION 2.6)

project(NE10 C ASM)

option(NE10_BUILD_SHARED "Build NE10 shared libraries" OFF)
option(NE10_BUILD_STATIC "Build NE10 static libraries" ON)
option(NE10_BUILD_EXAMPLES "Build NE10 examples" ON)

#hard float abi
option(NE10_ARM_HARD_FLOAT "Build NE10 for hard float abi" OFF)
option(NE10_ARM64 "Build NE10 for ARM64" OFF)

#unit test options
option(NE10_BUILD_UNIT_TEST "Build NE10 unit test" OFF)
if (NE10_BUILD_UNIT_TEST)
    #decide the test is smoke, regression or performance test, only one of
    #three options can be ON! And if you want to run performance test, you
    #should build the releave version, see the BUILD_DEBUG below.
    option(NE10_SMOKE_TEST "Run smoke test" OFF)
    option(NE10_REGRESSION_TEST "Run regression test" OFF)
    option(NE10_PERFORMANCE_TEST "Run performance test" OFF)

    option(NE10_DEBUG_TRACE "Print debug trace" OFF)
endif()

#check if proper platform is set.
if((NOT ANDROID_PLATFORM) AND (NOT GNULINUX_PLATFORM) AND (NOT IOS_PLATFORM) AND (NOT INTEL_MAC_PLATFORM))
    message(FATAL_ERROR "No platform is defined! see CMakeBuilding.txt under doc for details.")
endif()

#select functionalities to be compiled
option(NE10_ENABLE_MATH "Build math functionalities to NE10" ON)
option(NE10_ENABLE_DSP "Build dsp functionalities to NE10" ON)
option(NE10_ENABLE_IMGPROC "Build image processing functionalities to NE10" ON)
option(NE10_ENABLE_PHYSICS "Build physics functionalities to NE10" ON)

set(NE10_VERSION 10)

if(BUILD_DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -g -DDEBUG")
    message("-- Building type: DEBUG")
else(BUILD_DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -DNDEBUG")
    message("-- Building type: RELEASE")
endif(BUILD_DEBUG)

if(ANDROID_PLATFORM)
  if(NE10_ARM64)
    if(BUILD_DEBUG)
    else(BUILD_DEBUG)
      set(CMAKE_C_FLAGS " -Ofast -ftree-vectorize -flto -funsafe-math-optimizations")
    endif(BUILD_DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfix-cortex-a53-835769 --sysroot=${NDK_SYSROOT_PATH}")
    set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -mfix-cortex-a53-835769 --sysroot=${NDK_SYSROOT_PATH}")
  else(NE10_ARM64)
    if(NE10_ARM_HARD_FLOAT)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb-interwork -mthumb -march=armv7-a -mfloat-abi=hard -Wl,--no-warn-mismatch -mfpu=vfp3 --sysroot=${NDK_SYSROOT_PATH}")
        set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -mthumb-interwork -mthumb -march=armv7-a -mfloat-abi=hard -Wl,--no-warn-mismatch -mfpu=neon --sysroot=${NDK_SYSROOT_PATH}")
    else(NE10_ARM_HARD_FLOAT)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb-interwork -mthumb -march=armv7-a -mfloat-abi=softfp -mfpu=vfp3 --sysroot=${NDK_SYSROOT_PATH}")
        set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -mthumb-interwork -mthumb -march=armv7-a -mfloat-abi=softfp -mfpu=neon --sysroot=${NDK_SYSROOT_PATH}")
    endif(NE10_ARM_HARD_FLOAT)
  endif(NE10_ARM64)
    message("loaded toolchain:
    ${ANDROID_TOOLCHAIN_PATH}/${ANDROID_ABI}-gcc
    ${ANDROID_TOOLCHAIN_PATH}/${ANDROID_ABI}-g++
    ${ANDROID_TOOLCHAIN_PATH}/${ANDROID_ABI}-as
    ${ANDROID_TOOLCHAIN_PATH}/${ANDROID_ABI}-ar
    ${ANDROID_TOOLCHAIN_PATH}/${ANDROID_ABI}-ranlib")
    if(ANDROID_PLATFORM AND ANDROID_DEMO)
      add_subdirectory(android/NE10Demo/jni)
    endif()
elseif(GNULINUX_PLATFORM)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb-interwork -mthumb -march=armv7-a -mfpu=vfp3")
    set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -mthumb-interwork -mthumb -march=armv7-a -mfpu=neon")
elseif(INTEL_MAC_PLATFORM)
    if(BUILD_DEBUG)
    else(BUILD_DEBUG)
      set(CMAKE_C_FLAGS " -Ofast -fvectorize -flto")
    endif(BUILD_DEBUG)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DINTEL_MAC_PLATFORM -arch x86_64 -mmacosx-version-min=10.7 ")
    set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -DINTEL_MAC_PLATFORM -arch x86_64 -mmacosx-version-min=10.7 ")			
elseif(IOS_PLATFORM)
    #set minimal target ios version.If not provided this option, Xcode
    #5.0.2 would generate code for iOS 7.0, which would lead runtime
    #error when run on device with earlier iOS version.
    if(NOT MIN_IOS_VER)
      set(MIN_IOS_VER "5.0")
    endif()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch armv7 -arch armv7s -mfpu=vfp3 -miphoneos-version-min=" ${MIN_IOS_VER})
    set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -arch armv7 -arch armv7s -mfpu=neon -miphoneos-version-min=" ${MIN_IOS_VER})
    message("-- CFLAGS: ${CMAKE_C_FLAGS}")

    string(REPLACE ";" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
    string(REPLACE ";" "" CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS})
    if(IOS_DEMO)
        add_subdirectory(ios)
    endif()
endif()

# The NE10 library.
add_subdirectory(modules)

if(NE10_BUILD_EXAMPLES AND NE10_ENABLE_MATH)
    add_subdirectory(samples)
endif()

if(NE10_BUILD_UNIT_TEST)
    add_subdirectory(test)
endif()
