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

#include "ui_mainwindow.h"
#include "ExecuteProcess.h"
#include "utf8_parse.h"
#include "CommandForm.h"
#include "ExecuteProcess.h"
#include "ScuffButton.h"
#include "HashCommand.h"
#include "Warn.h"
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

private slots:
    void on_pushButton_pressed();
    void on_pushButton_2_pressed();

public slots:
    void addCommand(Command* cmd, CommandForm* cmdFrm);
    void deleteCommand(Command* cmd, CommandForm* cmdFrm);

private:
    Ui::MainWindow *ui;
    std::vector<Command*> commands;
    void saveCommand(Command* cmd);
    void getCommands();
    void setupButtons();
    // Index
    void editCommand(int i);
public:
    void rejectForm(CommandForm* cmdFrm);
};
#endif // MAINWINDOW_H
