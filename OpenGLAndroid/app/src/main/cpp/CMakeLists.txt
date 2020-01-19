# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Standard Android dependencies
find_library(android_lib android)
find_library(EGL_lib EGL)
find_library(log_lib log)


if (${ANDROID_PLATFORM_LEVEL} LESS 18)
    set(OPENGL_LIB GLESv2)
else ()
    set(OPENGL_LIB GLESv3)
endif (${ANDROID_PLATFORM_LEVEL} LESS 18)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

# Sources
file(GLOB native_srcs "./*.cpp")
# Output binary
add_library(native-lib SHARED ${native_srcs})


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

# Build
target_link_libraries(native-lib
            ${android_lib}
            ${EGL_lib}
            ${OPENGL_LIB}
            ${log_lib}
            # ${libs_dir}/jni/${ANDROID_ABI}/libcardboard_api.so
        )