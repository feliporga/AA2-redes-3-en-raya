#include "Server.h"

short main() {
    Server myServer(55000);
    myServer.Run();
    return 0;
}