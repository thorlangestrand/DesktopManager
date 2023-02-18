#include "warn.h"

void Warn(QString msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}
