#include <QtCore/QTranslator>

#include <QtWidgets/QApplication>

#include "include/core/applicationlauncher.h"

int main(int argc, char *argv[])
{
    int exitCode = 0;
    try
    {
        QApplication app(argc, argv);
        QTranslator translator_app;
        bool res {translator_app.load(QLatin1String("DemoTest_ru.qm"), QLatin1String(":/i18n/res"))};
        if (res) app.installTranslator(&translator_app);

        QTranslator translator_s;
        res = translator_s.load(QLatin1String("qtbase_ru"), QLatin1String(":/i18n/res"));
        if (res) app.installTranslator(&translator_s);

        DEMO_NAMESPACE::ApplicationLauncher appLauncher;

        exitCode = app.exec();
    }
    catch (...)
    {
        qFatal("Unhandled exception");
    }
    return exitCode;
}
