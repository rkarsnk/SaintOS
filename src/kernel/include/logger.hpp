#pragma once

#include <cstdio>

#include <console.hpp>

enum LogLevel {
  kError = 3,
  kWarn  = 4,
  kInfo  = 6,
  kDebug = 7,
};

void SetLogLevel(LogLevel level);
int Log(LogLevel level, const char* format, ...);