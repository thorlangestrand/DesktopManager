#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    // Stylesheet
    QFile styleSheetFile(":/stylesheets/ManjaroMix.qss");
    styleSheetFile.open(QIODevice::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());

    // Icon is set in DesktopManager.pro (project file) by
    // RC_ICONS = C:\Path\To\Icon\icon.ico

    QApplication a(argc, argv);
    a.setStyleSheet(styleSheet);
    MainWindow w;
    w.show();
    return a.exec();
}
