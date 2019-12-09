#include "client.h"

#include "../common/log.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    ERROR << "Client: incorrect arguments";
    return 1;
  }

  try {
    // TODO: make possability to set ip, port outside,
    // may be from command arguments or config via cmake
    std::string const ip       = "127.0.0.1";
    unsigned short const port  = 1271;
    char const* const filePath = argv[1];
    Client client(filePath, ip, port);
    client.run();
    return 0;
  } catch (std::exception& e) {
    ERROR << "Client: " << e.what();
  } catch (...) {
    ERROR << "Client: uknown error :(";
  }
  return 1;
}
