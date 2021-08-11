#include <logger.hpp>

namespace {
  LogLevel log_level = kWarn;
}

void SetLogLevel(LogLevel level) {
  log_level = level;
}

extern Console* console;

int Log(LogLevel level, const char* format, ...) {
  if (level > log_level) {
    return 0;
  }

  va_list ap;
  int result;
  char s[1024];

  va_start(ap, format);
  result = vsprintf(s, format, ap);
  va_end(ap);

  console->PutString(s);
  return result;
}
