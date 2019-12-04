#pragma once

template <class T>
class RAII {
 public:
  explicit RAII(T& socket) : m_socket(socket) {}
  ~RAII() {
    if (m_socket.is_open())
      m_socket.close();
  }

 private:
  T& m_socket;
};
