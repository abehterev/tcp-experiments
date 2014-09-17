/* Based on:
***************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
**
****************************************************************************/

#include <QApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Client client;
    client.show();
    return app.exec();
}
