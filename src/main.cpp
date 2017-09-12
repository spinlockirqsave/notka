/*
 * main.cpp
 *
 * Copyright(C) 2017, Piotr Gregor <piotrgregor@rsyncme.org>
 *
 * Notka Online Clipboard
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#include "../inc/notka.h"
#include <QApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "../inc/globals.h"


namespace Database {
        QMutex mutex;
}

int main(int argc, char *argv[])
{
        QApplication a(argc, argv);
        Notka main_window;

        QCommandLineParser parser;
        parser.setApplicationDescription(APP_NAME);

        parser.addHelpOption();

        QCommandLineOption no_gui_opt(QStringList() << "n" << "nogui",
                                      "Run without GUI.");

        parser.addOption(no_gui_opt);

        parser.process(a);

        if (!parser.isSet(no_gui_opt))
                        main_window.show();

        return a.exec();
}
