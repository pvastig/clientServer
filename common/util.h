#pragma once

#include <string>

namespace util {

template <class T>
class SocketRAII {
 public:
  explicit SocketRAII(T& socket) : m_socket(socket) {}
  ~SocketRAII() {
    if (m_socket.is_open())
      m_socket.close();
  }

 private:
  T& m_socket;
};

size_t hashStr(std::string const& str);
}  // namespace util
