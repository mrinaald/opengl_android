//
// Created by mrinaal on 1/19/20.
//

#include "utils.h"

#include <time.h>


namespace ndk_opengl_app {

long GetMonotonicTimeMilliSecs() {
  struct timespec res{};
  clock_gettime(CLOCK_MONOTONIC, &res);
  return (res.tv_sec * 1000) + (res.tv_nsec / 1000000);
}

}   // namespace ndk_opengl_app

