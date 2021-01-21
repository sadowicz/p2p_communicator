#include "tcp_server.h"

tcp_server::tcp_server() {
    this->server = new QTcpServer(this);
}
