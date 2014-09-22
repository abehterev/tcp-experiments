#include <QCoreApplication>
#include <iostream>
#include "config.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Config *C = new Config(argc, argv);

    cout << "Address: " << C->GetAddress().toStdString() << endl;
    cout << "Port: " << C->GetPort() << endl;

    //return 0;
    return app.exec();
}
