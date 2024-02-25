#ifndef CONFIGFORM_H
#define CONFIGFORM_H
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QMessageBox>
#include <QIcon>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QMainWindow>
#include <QPushButton>

#include <vector>
#include <string.h>

#include "globals.h"

class MainWindow;

class ConfigForm : public QDialog
{
public:
    ConfigForm(MainWindow* parent);
    ~ConfigForm();

    // Labels
    QLabel* flashPlayerPathLabel = nullptr;
    QLabel* iconsFolderPathLabel = nullptr;

    // Input
    QLineEdit* flashPlayerPath =   nullptr;
    QLineEdit* iconsFolderPath =   nullptr;

    // Accept/reject
    QDialogButtonBox* buttonBox =  nullptr;

    // Grid layout for labels/input
    QGridLayout* mainLayout =      nullptr;

    // Reference to main window
    MainWindow* mw =               nullptr;


public slots:
    void verify();
    void reject();
};



#endif // CONFIGFORM_H
