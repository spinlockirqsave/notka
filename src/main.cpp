#include "../inc/notka.h"
#include <QApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>


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
