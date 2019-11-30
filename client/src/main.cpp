#include "client.h"

#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Incorrect arguments" << std::endl;
    return 1;
  }

  try {
    Client client(argv[1]);
    std::string const ip      = "127.0.0.1";
    unsigned short const port = 1271;
    client.run(ip, port);
    return 0;
  } catch (std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Uknown error :(" << std::endl;
  }
  return 1;
}
