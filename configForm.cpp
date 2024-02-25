#include "mainwindow.h"
#include "configForm.h"

ConfigForm::ConfigForm(MainWindow* parent) :
    QDialog(parent),
    mw(parent)
{
//    QLabel* flashPlayerPathLabel = nullptr;
//    QLabel* iconsFolderPathLabel = nullptr;

//    // Input
//    QLineEdit* flashPlayerPath =   nullptr;
//    QLineEdit* iconsFolderPath =   nullptr;

//    // Accept/reject
//    QDialogButtonBox* buttonBox =  nullptr;

//    // Grid layout for labels/input
//    QGridLayout* mainLayout =      nullptr;


    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &ConfigForm::verify);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ConfigForm::reject);

    flashPlayerPathLabel = new QLabel("Flash Player path");
    iconsFolderPathLabel = new QLabel("Icons folder");

    flashPlayerPath = new QLineEdit(globals::swfPath.c_str());
    iconsFolderPath = new QLineEdit(globals::iconsFolder.c_str());

    mainLayout = new QGridLayout;
    mainLayout->setHorizontalSpacing(10);
    mainLayout->setVerticalSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(flashPlayerPathLabel,0, 0);  mainLayout->addWidget(flashPlayerPath,0, 1);
    mainLayout->addWidget(iconsFolderPathLabel,1, 0);  mainLayout->addWidget(iconsFolderPath,1, 1);
    mainLayout->addWidget(buttonBox, 2, 0, 1, 2);


    ConfigForm::setLayout(mainLayout);
    ConfigForm::setWindowTitle("Configure Settings");


}

ConfigForm::~ConfigForm()
{

}


void ConfigForm::verify()
{
    accept();
    mw->acceptConfigForm(this);
}

void ConfigForm::reject()
{
    mw->rejectConfigForm(this);
}

