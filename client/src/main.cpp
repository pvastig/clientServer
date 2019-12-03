#include "client.h"

#include "../common/log.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    ERROR << "Incorrect arguments";
    return 1;
  }

  try {
    std::string const ip       = "127.0.0.1";
    unsigned short const port  = 1271;
    char const* const filePath = argv[1];
    Client client(filePath, ip, port);
    client.run();
    return 0;
  } catch (std::runtime_error& e) {
    ERROR << e.what();
  } catch (...) {
    ERROR << "Uknown error :(";
  }
  return 1;
}
