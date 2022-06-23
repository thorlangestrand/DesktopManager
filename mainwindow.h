#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <QDir>
#include <iostream>
#include <initguid.h>
#include <shlobj.h>
#include <knownfolders.h>
#include <io.h>
#include <filesystem>
#include <fstream>
#include <future>
#include <QKeyEvent>
#include <QShortcut>

#include "ui_mainwindow.h"
#include "ExecuteProcess.h"
#include "utf8_parse.h"
#include "CommandForm.h"
#include "ExecuteProcess.h"
#include "ScuffButton.h"
#include "HashCommand.h"
#include "Warn.h"
#include "DirectoryErrorEnum.h"
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    // Need to keep track of the hash of the current edit
    // It will obviously change as the command is edited,
    // so we need to store the hash of the previous state
    // in order to delete the previous command from disk
    QString currentEditHash = "";

private slots:
    void on_pushButton_pressed();
    //void on_pushButton_2_pressed();

    void on_searchBox_textChanged();

public slots:
    void addCommand(Command* cmd, CommandForm* cmdFrm);
    void deleteCommand(Command* cmd, CommandForm* cmdFrm);
    void editCommand(Command* cmd, CommandForm* cmdFrm);

private:
    Ui::MainWindow *ui;
    std::vector<Command*> commands;
    // Used for search functionality
    std::vector<Command*> matchingCommands;
    // String representation of stylesheet
    // used for buttons not selected by search
    QString unselectedStyleSheet;
private:
    void saveCommand(Command* cmd);
    void getCommands();
    void setupButtons();
    void buttonResponseEditCommand(int i);
    void buttonResponseEditCommand();
    DIRERR removeCommandDirectory(QString hash);
    void handleCommandDirectoryError(Command* cmd, DIRERR err);
    // Keypress, searching
    void keyPressEvent(QKeyEvent* k);
    Command* currentMatch();
    // Handle matching (whether command matches search)
    void handleMatch();

public:
    void rejectForm(CommandForm* cmdFrm);
};
#endif // MAINWINDOW_H
