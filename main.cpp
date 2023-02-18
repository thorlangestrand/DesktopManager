#include <QApplication>
#include <QFile>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    // Stylesheet
    QFile styleSheetFile(":/stylesheets/ManjaroMix.qss");
    styleSheetFile.open(QIODevice::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());






    // Icon is set in DesktopManager.pro (project file) by
    // RC_ICONS = C:\Path\To\Icon\icon.ico

    QApplication a(argc, argv);

    if (!initGlobals())
    {
        Warn("Failed to initialize globals");
        return 1;
    }

    a.setStyleSheet(styleSheet);
    MainWindow w;
    w.show();
    return a.exec();
}
