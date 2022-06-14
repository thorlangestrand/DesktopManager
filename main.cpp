#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    // Stylesheet
    QFile styleSheetFile(":/stylesheets/ManjaroMix.qss");
    styleSheetFile.open(QIODevice::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    QApplication a(argc, argv);
    a.setStyleSheet(styleSheet);
    //a.setWindowIcon(QIcon("C:\\Users\\admin\\Desktop\\PC\\code\\qt\\DesktopManager\\resources\\rinsmile.png"));
    MainWindow w;
    w.show();
    return a.exec();
}
