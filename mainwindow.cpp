#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Desktop Manager");
    //this->statusBar()->setSizeGripEnabled(false);
    // Read commands from file
    MainWindow::getCommands();
    // Setup buttons from commands
    MainWindow::setupButtons();
    // Prevent buttons from being affected, should change to deal with buttons later
    //ui->centralwidget->grabKeyboard();
    //ui->searchBox->setReadOnly(true);
    // Alternative stylesheet used for unselected commands
    QFile styleSheetFile(":/stylesheets/Unselected.qss");
    styleSheetFile.open(QIODevice::ReadOnly);
    unselectedStyleSheet = QLatin1String(styleSheetFile.readAll());

    MainWindow::handleMatch();


    // Unsure where to set up shortcuts, just gonna do it in setup
        //MainWindow::ui->pushButton->click();

    // Create new command on ctrl+n
    QShortcut* newCommand = new QShortcut(QKeySequence("Ctrl+N"), this);
    QObject::connect(newCommand, &QShortcut::activated, this, [this](){ this->ui->pushButton->click(); });

    // If command selected unique, edit command, else nothing
    QShortcut* editCommand = new QShortcut(QKeySequence("Ctrl+E"), this);
    QObject::connect(editCommand, &QShortcut::activated, this, [this](){ this->buttonResponseEditCommand();});


}

MainWindow::~MainWindow()
{
    delete ui;
    for (Command* c: commands)
    {
        delete c;
    }
    commands.clear();
    matchingCommands.clear();
}




// TODO

// CTRL+N, NEW COMMAND
// CTRL+E, EDIT COMMAND SAME SEARCH LOGIC AS LAUNCH
// CREATE SCRIPT TO CONVERT PNGS TO ICONS, FEED




Command* MainWindow::currentMatch()
{
    if (matchingCommands.size() == 1) return matchingCommands[0];
    return nullptr;
}








void MainWindow::handleMatch()
{
    matchingCommands.clear();
    for (Command*& c :commands)
    {
        if (c->nameMatch(ui->searchBox->toPlainText().toStdString()))
        {
            matchingCommands.push_back(c);
            c->updateUi("");
        }
        else {
            c->updateUi(unselectedStyleSheet);
        }
    }

}



/**
 * @brief MainWindow::keyPressEvent
 * Handle searching
 *
 * @param k
 */
void MainWindow::keyPressEvent(QKeyEvent* k)
{
    int tmpKeyCode = k->key();

    // Remove focus from search, no update to search, return
    if (tmpKeyCode == Qt::Key::Key_Escape)
    {
        ui->searchBox->clearFocus();
        return;
    }

    if (k->key() == Qt::Key::Key_Backspace)
    {
        // If control pressed, delete last word until space
        if (k->modifiers() == Qt::KeyboardModifier::ControlModifier)
        {
            // This can be done much more compact but readability > compactness
            QString tmpContent = ui->searchBox->toPlainText();
            int lastSpace = tmpContent.lastIndexOf(" ");

            // Remove spaces
            while(tmpContent.at(lastSpace).isSpace())
                lastSpace--;

            // If no spaces
            if (lastSpace == -1)
                ui->searchBox->setText("");

            ui->searchBox->setText(tmpContent.left(lastSpace + 1));

            //ui->searchBox->setText(ui->searchBox->toPlainText().left(ui->searchBox->toPlainText().lastIndexOf(" ")));
        }
        else {
            // Remove last char
            ui->searchBox->setText(ui->searchBox->toPlainText().left(ui->searchBox->toPlainText().size()-1));
        }

        // Have to reset matches and possibly find new ones by rolling back

        //matchingCommands.push_back(commands[0]);

    }
    else {
        if (k->modifiers() != Qt::KeyboardModifier::ShiftModifier && k->key() > 64 && k->key() < 91)
        {
            tmpKeyCode += 32;
        }
        if (tmpKeyCode > 19 && tmpKeyCode < 127)
        {
            ui->searchBox->setText(ui->searchBox->toPlainText() + (QChar)tmpKeyCode);
        }





    }
    handleMatch();
    // If uppercase and shift not held convert to lowercase (bug with qt input)


    if (k->key() == Qt::Key::Key_Return || k->key() == Qt::Key::Key_Enter)
    {
        // CHECK IF ONLY ONE MATCH; ELSE DO NOTHING;
        if (matchingCommands.size() == 1)
        {
            matchingCommands[0]->run();
            ui->searchBox->setText("");
            MainWindow::handleMatch();
        }
    }

}



/**
 * @brief clearWidgets
 * Clear all widgets out of a layout
 *
 * @param layout
 */
void clearWidgets(QLayout * layout) {
   if (!layout) return;
   while (auto item = layout->takeAt(0)) {
      delete item->widget();
      clearWidgets(item->layout());
   }
}

/**
 * @brief MainWindow::setupButtons
 * Create buttons
 */
void MainWindow::setupButtons()
{
    // Clear out grid layout
    clearWidgets(ui->gridLayout);

    for (size_t i = 0; i < commands.size(); i++)
    {
        ui->gridLayout->setRowStretch(i, 1);
        QPixmap icon(QString::fromStdString(commands[i]->icon));
        QIcon buttonIcon(icon);
        // Launch button
        ScuffButton* pb = new ScuffButton(commands[i]);



        //pb->resize(1, pb->height());
        // On pb emit QPushButton::pressed execute lambda function
        // given param pb where pb->cmd->run is called
        // Holy shit i get it
        // Connect, object, emit from object, action
        connect(pb, &QPushButton::pressed, [pb]() {(pb->cmd->run());});
        pb->setIcon(buttonIcon);
        pb->setIconSize({40,40});

        //pb->setMaximumSize(42,42);

        // Edit
        ScuffButton* editB = new ScuffButton(commands[i]);
        editB->setText("Edit");
        //QGridLayout* lol = new QGridLayout;
        connect(editB, &QPushButton::pressed, this, [i,this]() { MainWindow::buttonResponseEditCommand(i); } );
        QGridLayout* qg = new QGridLayout;

        // Name / description
        QLabel* cmdName = new QLabel(tr(commands[i]->name.c_str()));

        qg->addWidget(pb, 0, 1, 1, 4);
        qg->addWidget(cmdName, 1, 1, 1 ,3);
        qg->addWidget(editB, 1, 4, 1, 1);
        //commands[i]->parentData = {pb, cmdName, editB};
//        commands[i]->parentPushButton = pb;
//        commands[i]->parentEditButton = editB;
//        commands[i]->parentLabel = cmdName;

        commands[i]->container = { pb, editB, cmdName };


        //QWidget* wat = new QWidget;
        //wat->show();
        //qg->setParent(wat);
        // 5x5 grid should suffice
        ui->gridLayout->addLayout(qg, (size_t)(i/5), i % 5);
        //ui->gridLayout->addWidget(wat, (size_t)(i/5), i%5);
    }
}


/**
 * @brief MainWindow::buttonResponseEditCommand
 * @param i
 */
void MainWindow::buttonResponseEditCommand(int i)
{
    currentEditHash = hashCommand(commands[i]);
    QString t = "Edit Command";
    CommandForm* cmdFrm = new CommandForm(t, this, commands[i]);
    cmdFrm->resize({400,120});
    cmdFrm->show();

}

void MainWindow::buttonResponseEditCommand()
{
    Command* cmd = MainWindow::currentMatch();
    if (cmd == nullptr) return;
    ui->searchBox->setText("");
    MainWindow::handleMatch();
    currentEditHash = hashCommand(cmd);
    QString t = "Edit Command";
    CommandForm* cmdFrm = new CommandForm(t, this, cmd);
    cmdFrm->resize({400,120});
    cmdFrm->show();
}



/**
 * @brief MainWindow::addCommand
 * Called upon succesful completion of command add
 *
 * @param cmd
 * @param cmdFrm
 */
void MainWindow::addCommand(Command* cmd, CommandForm* cmdFrm)
{
    // Called after command form exits, safe to delete at that point
    delete cmdFrm;
    // Add to main
    commands.push_back(cmd);
    // Add to ui
    MainWindow::setupButtons();
    // Save new to disk
    MainWindow::saveCommand(cmd);

}

void MainWindow::editCommand(Command* cmd, CommandForm* cmdFrm)
{
    // Free memory from form
    delete cmdFrm;

    // Free space on disk from previous state of edited command
    DIRERR retVal = MainWindow::removeCommandDirectory(currentEditHash);

    if (retVal != DIRERR::NONE)
    {
        MainWindow::handleCommandDirectoryError(cmd, retVal);
        return;
    }

    MainWindow::setupButtons();
    MainWindow::saveCommand(cmd);
}




/**
 * @brief rejectForm
 * Called upon commandform abort
 * @param cmdFrm
 */
void MainWindow::rejectForm(CommandForm* cmdFrm)
{
    delete cmdFrm;
}



/**
 * @brief MainWindow::deleteCommand
 * Free up memory from command form, delete selected
 * command from both memory and disk and re-render
 * commands
 * @param cmd       Command to be deleted
 * @param cmdFrm    Pointer to the command form
 */
void MainWindow::deleteCommand(Command* cmd, CommandForm* cmdFrm)
{
    // Delete form
    delete cmdFrm;

    DIRERR retVal = MainWindow::removeCommandDirectory(hashCommand(cmd));
    if (retVal != DIRERR::NONE)
    {
        MainWindow::handleCommandDirectoryError(cmd, retVal);
        return;
    }

    std::vector<Command*> tmp;
    for (size_t i = 0; i < commands.size(); i++)
    {
        if (commands[i] == cmd)
        {
            delete cmd;
        }
        else {
            tmp.push_back(commands[i]);
        }
    }

    commands.clear();
    for (Command* c : tmp)
    {
        commands.push_back(c);
    }

    //qDebug()<<QString::fromStdString(cmd->icon);
    MainWindow::setupButtons();
}

/**
 * @brief MainWindow::saveCommand
 * Save command to disk
 * @param cmd
 */
void MainWindow::saveCommand(Command* cmd)
{
    // Create folder name based on hash of all info, guarantee no exact replicas
    // NOTE this might be dangerous if commandtypes more or equal to 10
    QString folderName = hashCommand(cmd);
    namespace fs = std::filesystem;

    // If this fails there are real issues so can't continue
    LPWSTR path = nullptr;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path);
    if (!SUCCEEDED(hr))
    {
        Warn("Catastrophic failure failed to locate appdata");
        return;
    }

    CoTaskMemFree(path);
    std::string workablePath = utf8_encode(path);
    workablePath += "\\ScuffManager";

    // If base path no exist
    if (!std::filesystem::exists(workablePath))
    {
        mkdir(workablePath.c_str());
    }
    // Concat base with hash
    std::string folderStr = folderName.toStdString();
    std::transform(folderStr.begin(), folderStr.end(), folderStr.begin(), ::toupper);
    workablePath += "\\" + folderStr;

    // If sub path exist means identical command exists
    if (std::filesystem::exists(workablePath))
    {
        Warn("Folder " + folderName + " already exists, which means this command has an exact duplicate");
        return;
    }
    workablePath += "\\";
    mkdir(workablePath.c_str());
    // Build up to data file location
    QString fullPath = QDir(QString::fromStdString(workablePath)).filePath("data.scuff");
    std::ofstream output(fullPath.toStdString());
    // Save cmd data to stream
    cmd->save(output);
    // Always remember to close your streams
    output.close();
}



void MainWindow::on_pushButton_pressed()
{
    QString t = "New Command";
    Command* cmd = new Command;
    CommandForm* cmdFrm = new CommandForm(t, this, cmd);
    cmdFrm->resize({400,120});
    cmdFrm->show();

    //delete cmdFrm;
    return;










    //system("explorer C:\\Users\\admin\\Desktop\\PC\\art");
    //int ret = WinExec("explorer.exe", SW_HIDE);

    //STARTUPINFO si;
    //PROCESS_INFORMATION pi;
    //CreateProcess("explorer.exe", "C:\\Users\\admin\\Desktop\\PC\\art", NULL, NULL, FALSE, 0, NULL, NULL, &si ,&pi);
    //CreateProcess(L"explorer.exe", L"C:\\Users\\admin\\Desktop\\PC\\art",0,0,false,CREATE_DEFAULT_ERROR_MODE,0,0,&si,&pi);
//    if (CreateProcess(const_cast<LPCWSTR>("explorer.exe"),(LPCWSTR)"" , NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
//    {
//        WaitForSingleObject(pi.hProcess, INFINITE);
//        CloseHandle(pi.hProcess);
//        CloseHandle(pi.hThread);
//    }

    //ExecuteProcess(L"C:\\Windows\\explorer.exe", L"C:\\Users\\admin\\Desktop\\PC\\art");

    //qDebug() << ret;
//    std::wstring cum = ;
//    ExecuteProcess(cum, L"C:\\users\\admin\\desktop\\pc\\art");
//    return;



}


//void MainWindow::on_pushButton_2_pressed()
//{
//    MainWindow::getCommands();
//    for (Command* c : commands)
//    {
////      qDebug() << c->type;
////      qDebug() << QString::fromStdString(c->icon);
////      qDebug() << QString::fromStdString(c->name);
////      qDebug() << QString::fromStdString(utf8_encode(c->param));
////      qDebug() << QString::fromStdString(c->website);
////      qDebug() << "END COMMAND";
//      switch (c->type)
//      {
//      case CommandType::CMD: {
//        //c->param = L"\"" + c->param + L"\"";
//          // ASYNC CANNOT DIFFERENTIATE BETWEEN OVERLOADS
//          qDebug() << c->param;
//          qDebug() << QString::fromStdString(c->name);
//          std::future<size_t> d = std::async(std::launch::async, ExecuteProcess, c->param);

//          //d.wait();
//        //std::system(utf8_encode(c->param).c_str());
////        size_t check = ExecuteProcess(c->param);
////        std::vector<std::future<size_t>> h;
////        he(c);
////        //h.push_back(std::async(std::launch::async, ExecuteProcess, c->param,this));

////        if (check!= 0)
////        {
////            qDebug() << "Err executing process";
////            qDebug() << GetLastError();
////        }
//        break;
//      }
//      case CommandType::EXPLORER: {
////        size_t check = ExecuteProcessW(L"C:\\Windows\\explorer.exe", c->param);
////        if (check != 0)
////        {
////          qDebug() << "Err executing process";
////          qDebug() << GetLastError();
////        }
//        break;
//      }
//      case CommandType::WEB: {
//          std::string tmp = "rundll32 url.dll, FileProtocolHandler ";
//          // If not begin with www., add
//          if(c->website.rfind("www.", 0) != 0)
//          {
//              tmp += "www.";
//          }
//          tmp += c->website;

//          std::future<int> d = std::async(std::launch::async, system, tmp.c_str());
//          //d.wait();




//          //system(tmp.c_str()); //WORKS
//          //qDebug() << QString::fromStdString(tmp);

////          size_t check = ExecuteProcess(utf8_decode(tmp));
////          if (check != 0)
////          {
////              qDebug() << GetLastError();
////          }
//          break;
//      }
//      default: {
//        qDebug() << "Unexpected command type detected, exiting...";;
//        exit(1);
//      }
//      }
//    }

//}


void MainWindow::on_searchBox_textChanged()
{
    // This is neat, annoying
//    if (ui->searchBox->toPlainText().isEmpty() && ui->searchBox->isVisible())
//    {
//        ui->searchBox->hide();
//    }
//    else {
//        ui->searchBox->show();
//    }
}

