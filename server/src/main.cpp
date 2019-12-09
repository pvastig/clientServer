#include "server.h"

#include "../common/log.h"

int main(int, char**) {
  try {
    // TODO: make possability to set ip, port outside
    // may be from command arguments or config via cmake
    std::string const ip      = "127.0.0.1";
    unsigned short const port = 1271;
    Server server(ip, port);
    server.run();
  } catch (std::exception& e) {
    ERROR << "Server: " << e.what() << std::endl;
  } catch (...) {
    ERROR << "Client: Uknown error :(";
  }

  return 1;
}
