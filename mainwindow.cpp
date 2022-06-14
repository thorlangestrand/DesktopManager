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
}

MainWindow::~MainWindow()
{
    delete ui;
    for (Command* c: commands)
    {
        delete c;
    }
    commands.clear();
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
        connect(editB, &QPushButton::pressed, this, [i,this]() {MainWindow::buttonResponseEditCommand(i);} );
        QGridLayout* qg = new QGridLayout;

        // Name / description
        QLabel* cmdName = new QLabel(tr(commands[i]->name.c_str()));

        qg->addWidget(pb, 0, 1, 1, 4);
        qg->addWidget(cmdName, 1, 1, 1 ,3);
        qg->addWidget(editB, 1, 4, 1, 1);
        // 6x6 grid should suffice
        ui->gridLayout->addLayout(qg, (size_t)(i/6), i % 6);
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
    std::cout << "he" << std::endl;
    QString t = "New Command";
    Command* cmd = new Command;
    CommandForm* cmdFrm = new CommandForm(t, this, cmd);
    cmdFrm->resize({400,120});
    cmdFrm->show();

    //delete cmdFrm;

    qDebug() << QString::fromStdString(cmd->name);
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


void MainWindow::on_pushButton_2_pressed()
{
    MainWindow::getCommands();
    for (Command* c : commands)
    {
//      qDebug() << c->type;
//      qDebug() << QString::fromStdString(c->icon);
//      qDebug() << QString::fromStdString(c->name);
//      qDebug() << QString::fromStdString(utf8_encode(c->param));
//      qDebug() << QString::fromStdString(c->website);
//      qDebug() << "END COMMAND";
      switch (c->type)
      {
      case CommandType::CMD: {
        //c->param = L"\"" + c->param + L"\"";
          // ASYNC CANNOT DIFFERENTIATE BETWEEN OVERLOADS
          qDebug() << c->param;
          qDebug() << QString::fromStdString(c->name);
          std::future<size_t> d = std::async(std::launch::async, ExecuteProcess, c->param);

          //d.wait();
        //std::system(utf8_encode(c->param).c_str());
//        size_t check = ExecuteProcess(c->param);
//        std::vector<std::future<size_t>> h;
//        he(c);
//        //h.push_back(std::async(std::launch::async, ExecuteProcess, c->param,this));

//        if (check!= 0)
//        {
//            qDebug() << "Err executing process";
//            qDebug() << GetLastError();
//        }
        break;
      }
      case CommandType::EXPLORER: {
//        size_t check = ExecuteProcessW(L"C:\\Windows\\explorer.exe", c->param);
//        if (check != 0)
//        {
//          qDebug() << "Err executing process";
//          qDebug() << GetLastError();
//        }
        break;
      }
      case CommandType::WEB: {
          std::string tmp = "rundll32 url.dll, FileProtocolHandler ";
          // If not begin with www., add
          if(c->website.rfind("www.", 0) != 0)
          {
              tmp += "www.";
          }
          tmp += c->website;

          std::future<int> d = std::async(std::launch::async, system, tmp.c_str());
          //d.wait();




          //system(tmp.c_str()); //WORKS
          //qDebug() << QString::fromStdString(tmp);

//          size_t check = ExecuteProcess(utf8_decode(tmp));
//          if (check != 0)
//          {
//              qDebug() << GetLastError();
//          }
          break;
      }
      default: {
        qDebug() << "Unexpected command type detected, exiting...";;
        exit(1);
      }
      }
    }

}

