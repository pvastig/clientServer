#include "../common/log.h"

#include "server.h"

#include <iostream>

int main(int, char**) {
  try {
    Server server("127.0.0.1", 1271);
    server.run();
  } catch (std::exception& e) {
    ERROR << e.what() << std::endl;
  }
  return 1;
}
