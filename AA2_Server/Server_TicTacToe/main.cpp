#include "Server.h"

int main() {
    Server myServer(53000);
    myServer.Run();
    return 0;
}