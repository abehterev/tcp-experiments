#include "config.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <QString>


Config::Config(int argc, char *argv[])
{
    int opt;
    QString addr = "127.0.0.1";
    int port = 4444;
    while ((opt = getopt(argc, argv, "a:p:h")) != -1) {
                   switch (opt) {
                   case 'a':
                       addr = optarg;
                       break;
                   case 'p':
                       port = atoi(optarg);
                       break;
                   default: /* '?' */
                       fprintf(stderr, "Usage: %s [-a ip-address] [-p port]\n", argv[0]);
                       exit(EXIT_FAILURE);
                   }
               }
    this->address = addr;
    this->port = port;
}

void Config::SetAddress(QString addr)
{
   this->address = addr;
}

void Config::SetPort(int port)
{
    this->port = port;
}

QString Config::GetAddress()
{
    return this->address;
}

int Config::GetPort()
{
   return this->port;
}
