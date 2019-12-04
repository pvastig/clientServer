#include "util.h"

#include <functional>

namespace util {

size_t hashStr(std::string const& str) {
  std::hash<std::string> hash_fn;
  return hash_fn(str);
}
}  // namespace util
