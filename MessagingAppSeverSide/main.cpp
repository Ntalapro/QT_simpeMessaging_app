#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    server* myserver = new server();
    myserver->startServer();

    return a.exec();
}
