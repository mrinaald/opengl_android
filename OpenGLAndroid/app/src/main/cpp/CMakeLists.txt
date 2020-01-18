# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Standard Android dependencies
find_library(android-lib android)
find_library(EGL-lib EGL)
find_library(GLESv2-lib GLESv2)
find_library(log-lib log)


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

# Sources
file(GLOB native_srcs "*.cpp")
# Output binary
add_library(native-lib SHARED ${native_srcs})


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

# Build
target_link_libraries(native-lib
            ${android-lib}
            ${EGL-lib}
            ${GLESv2-lib}
            ${log-lib}
            # ${libs_dir}/jni/${ANDROID_ABI}/libcardboard_api.so
        )