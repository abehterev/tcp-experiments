#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class Config{

public:
    Config(int argc, char *argv[]);
    QString GetAddress();
    int GetPort();
    void SetAddress(QString addr);
    void SetPort(int port);

private:
    QString address;
    int port;

};

#endif // CONFIG_H
