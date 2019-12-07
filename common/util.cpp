#include "util.h"

#include <functional>

namespace util {

size_t hashStr(std::string const& str) {
  std::hash<std::string> hashFn;
  return hashFn(str);
}
}  // namespace util
