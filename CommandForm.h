#ifndef COMMANDFORM_H
#define COMMANDFORM_H
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QMessageBox>
#include <vector>
#include <QIcon>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <string.h>
#include "Command.h"
#include "CommandFormUiStorage.h"
#include "HashCommand.h"
#include "Warn.h"

class MainWindow;
class CommandForm : public QDialog
{

// Constructor
public:
    CommandForm(const QString &title, MainWindow* parent, Command* cmd);
    ~CommandForm();
public slots:
    void verify();
    void command_type_index_changed(int index);
    void reject();

// Data we need to construct commands
public:
    // Enum, type of command
    QString iconPath;
    QString commandName;
    //QString executable;
    QString parameters;

private:
    // Is it an existing command being edited
    bool editing = false;

    // Labels
    QLabel* typeLabel;
    QLabel* iconLabel;
    QLabel* cmdNameLabel;
    //QLabel* executableLabel;
    QLabel* paramLabel;
    QLabel* webLabel;

    // Input
    QComboBox* type;
    QLineEdit* iconEdit;
    QLineEdit* cmdNameEdit;
    //QLineEdit* executableEdit;
    QLineEdit* paramEdit;
    QLineEdit* webEdit;

    // Buttons for file explorer
    QPushButton* iconButton;
    QPushButton* paramButton;
   //QPushButton* cmdNameButton;

    // Delete button, used for edit prompt
    QPushButton* deleteButton;

    // Ok, Cancel
    QDialogButtonBox* buttonBox;

    // Grid layout
    QGridLayout* mainLayout = nullptr;

    // Input, output command
    Command* cmd = nullptr;

    // Commandtype, default CMD
    CommandType cmdType = CommandType::CMD;

    // Pointer to main window which stores global data
    MainWindow* mw = nullptr;

    // This quickly got out of hand, creating a function to deal with scalability
    //std::vector<std::pair<QLabel*, QLineEdit*>> uiStorage;
    std::vector<InputBox> uiStorage;

    //std::vector<SCUFF> UICUM;

private:
    void hideOthers(InputBox input);
    bool acceptedOutcome();
    void iconButtonFile();
    void paramButtonFile();
    void deleteCommand();
    void deleteButtonHandle();
    void openFolderHandle();
};






#endif // COMMANDFORM_H
