#include "CommandForm.h"
#include "mainwindow.h"


/**
 * @brief CommandForm::iconButtonFile
 *
 * Get icon path and set to icon field
 */
void CommandForm::iconButtonFile()
{
    // The tr function is a translate function. This means that depending on language environment the word Icons should be
    // translated to the appropriate word in whatever language
    QString fileName = QFileDialog::getOpenFileName(this, "Icon", "C:\\Users\\admin\\Desktop\\PC\\art\\Icons", tr("Icons (*.png *.ico *.jpg"));
    iconEdit->setText(fileName);
}

/**
 * @brief CommandForm::paramButtonFile
 *
 * Get file or folder path and set to param field
 */
void CommandForm::paramButtonFile()
{
    LPWSTR path = NULL;

    // Result of folder path, 0 if no err
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_Profile, 0, NULL, &path);
    if (!SUCCEEDED(hr))
    {
        QMessageBox msg;
        msg.setText("Error, user directory could not be located");
        msg.exec();
        return;
    }

    CoTaskMemFree(path);
    std::string workablePath = utf8_encode(path);




    QString fileName;
    switch(cmdType)
    {
    case CommandType::CMD : {
        fileName = QFileDialog::getOpenFileName(this, "Command", QString::fromStdString(workablePath), tr("Executables (*.exe *.msi *.bat *.cmd *.ps1)"));
        break;
    }
    case CommandType::EXPLORER: {
        fileName = QFileDialog::getExistingDirectory(this, "Directory", QString::fromStdString(workablePath));
        break;
    }
    default: {
        break;
    }
    }

    paramEdit->setText(fileName);
}

/**
 * @brief CommandForm::deleteButtonHandle
 * Called upon DELETE button pressed
 * Sends signal to main
 * @see MainWindow::delteCommand(...)
 */
void CommandForm::deleteButtonHandle()
{
    accept();
    mw->deleteCommand(cmd, this);
}


CommandForm::CommandForm(const QString& title, MainWindow* parent, Command* _cmd)
    : QDialog(parent),
    mw(parent)
{

    // List of commands
    QStringList CommandList =
    {
        "Cmd",
        "Explorer",
        "Web"
    };

    // Set ptr to command so it can be accessed throughout the class
    cmd = _cmd;

    // If command already has name, set editing to true
    if (!cmd->name.empty()) editing = true;

    // Labels for input
    iconLabel = new QLabel(tr("Icon location: "));
    cmdNameLabel = new QLabel(tr("Name: "));
    paramLabel = new QLabel(tr("Params: "));
    webLabel = new QLabel(tr("Website: "));

    // Type input (dropdown)
    type = new QComboBox;
    type->addItems(CommandList);

    // String inputs
    iconEdit = new QLineEdit;
    cmdNameEdit = new QLineEdit;
    paramEdit = new QLineEdit;
    webEdit = new QLineEdit;

    // Set data based on passed command, used for edits
    // There is no danger here except i guess if new fails and we get a nullptr
    // but there's no way that would happen right haha
    cmdType = cmd->type;
    type->setCurrentIndex(cmdType);
    iconEdit->setText(QString::fromStdString(cmd->icon));
    cmdNameEdit->setText(QString::fromStdString(cmd->name));
    paramEdit->setText(QString::fromStdString(utf8_encode(cmd->param)));
    webEdit->setText(QString::fromStdString(cmd->website));

    // Buttons to search for items
    iconButton = new QPushButton;
    iconButton->setIcon(QIcon(":/resources/Fatcow-Farm-Fresh-Folder-black.ico"));
    paramButton = new QPushButton;
    paramButton->setIcon(QIcon(":/resources/Fatcow-Farm-Fresh-Folder-black.ico"));
    deleteButton = new QPushButton;
    deleteButton->setText("Delete");

    // Connect file reading buttons
    connect(iconButton, &QPushButton::pressed, this, &CommandForm::iconButtonFile);
    connect(paramButton, &QPushButton::pressed, this, &CommandForm::paramButtonFile);

    // Connect delete button
    connect(deleteButton, &QPushButton::pressed, this, &CommandForm::deleteButtonHandle);




    uiStorage.push_back({ paramLabel, paramEdit, paramButton });
    //uiStorage.push_back({cmdNameLabel, cmdNameEdit, cmdNameButton});
    //uiStorage.push_back({ executableLabel, executableEdit });
    uiStorage.push_back({ webLabel, webEdit });

    // Ok, Cancel
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    // When press Ok, Cancel, handle these
    // Verify calls CommandForm::verify which we have implemented ourselves
    connect(buttonBox, &QDialogButtonBox::accepted, this, &CommandForm::verify);
    // Reject closes the window immediately by default
    connect(buttonBox, &QDialogButtonBox::rejected, this, &CommandForm::reject);

    // Set up layout, may god have mercy on my soul
    mainLayout = new QGridLayout;
    mainLayout->addWidget(type, 0, 0);
    mainLayout->addWidget(iconLabel, 1, 0);
    mainLayout->addWidget(iconEdit, 1, 1);
    mainLayout->addWidget(iconButton, 1, 2);
    mainLayout->addWidget(cmdNameLabel, 2, 0);
    mainLayout->addWidget(cmdNameEdit, 2, 1);
    mainLayout->addWidget(paramLabel, 3, 0);
    mainLayout->addWidget(paramEdit, 3, 1);
    mainLayout->addWidget(paramButton, 3, 2);
    mainLayout->addWidget(webLabel, 4, 0);
    mainLayout->addWidget(webEdit, 4, 1);
    mainLayout->addWidget(buttonBox, 5, 0, 1, 2);
    if (editing) mainLayout->addWidget(deleteButton, 5, 0, 1, 1);
    setLayout(mainLayout);
    setWindowTitle(title);

    // QObject, Static member of QObject being watched, current object, member function to be called when static member triggered
    connect(type, &QComboBox::currentIndexChanged, this, &CommandForm::command_type_index_changed);

    CommandForm::command_type_index_changed(cmdType);

}

/**
 * @brief CommandForm::~CommandForm
 */
CommandForm::~CommandForm()
{
    delete type;
    delete iconLabel;
    delete iconEdit;
    delete cmdNameLabel;
    delete cmdNameEdit;
    delete paramLabel;
    delete paramEdit;
    delete webLabel;
    delete webEdit;
    delete buttonBox;
    delete mainLayout;
    delete iconButton;
    delete paramButton;
}


/**
 * @brief
 * Based on selected command, determine whether
 * all required information is present
 * @return
 */
bool CommandForm::acceptedOutcome()
{
    namespace fs = std::filesystem;

    // Does name exist, universal
    if (cmdNameEdit->text().isEmpty())
    {
        Warn("No name given for command");
        return false;
    }
    // Check requirements for given command type
    switch(cmdType)
    {
    case CommandType::CMD : {
        // Given param
        if (paramEdit->text().isEmpty())
        {
            Warn("No parameters given");
            return false;
        }
        // Does executable exist
        if (!fs::exists(paramEdit->text().toStdString()))
        {
            Warn(QString::fromStdString(paramEdit->text().toStdString() + " is not a valid file path"));
            return false;
        }
        return true;
        //return !(paramEdit->text().isEmpty() || !fs::exists(paramEdit->text().toStdString()));
    }
    case CommandType::EXPLORER: {
        // Given param
        if (paramEdit->text().isEmpty())
        {
            Warn("No parameters given");
            return false;
        }
        // Does folder exist
        if (!fs::exists(paramEdit->text().toStdString()))
        {
            Warn(QString::fromStdString(paramEdit->text().toStdString() + " is not a valid folder"));
            return false;
        }
        return true;
    }
    case CommandType::WEB : {
        // Does web exist
        if (webEdit->text().isEmpty())
        {
            Warn("No webpage given");
            return false;
        }
        return true;
    }
    default: {
        return false;
    }
    }
    return false;
}


/**
 * @brief
 * Called upon OK button pressed
 * Checks if conditions are good, if so writes data
 * to command
 *
 * @see MainWindow::addCommand(...)
 */
void CommandForm::verify()
{
    // If successful
    if (CommandForm::acceptedOutcome())
    {
        QString tmpP = paramEdit->text();

        tmpP.replace("/", "\\");

        // Send accept signal, this closes the window
        accept();
        // Set params of passed pointer
        cmd->type = cmdType;
        if (!iconEdit->text().isEmpty())
        {
            cmd->icon = iconEdit->text().toStdString();
        }
        else {
            cmd->icon = ":/resources/default.ico";
        }
        cmd->name = cmdNameEdit->text().toStdString();
        cmd->param = utf8_decode(tmpP.toStdString());
        cmd->website = webEdit->text().toStdString();

        // I don't know why this works
        // Thanks random stackoverflow guy with 1 upvote
        mw->addCommand(cmd, this);


    }
}

void CommandForm::reject()
{
    mw->rejectForm(this);
}


/**
 * @brief
 * Hide other togglable elements than the ones passed
 * as arguments
 * @param input      List of elements
 */
void CommandForm::hideOthers(InputBox input)
{
    //  The verbose way of doing it, including for memes, is now deprecated due to changes
    //  for(std::vector<std::pair<QLabel*,QLineEdit*>>::iterator it = uiStorage.begin(); it != uiStorage.end(); it++)
    //  {
    //      if ((*it).first != show.first)
    //      {
    //          (*it).first->hide();
    //          (*it).second->hide();
    //      }
    //  }
    for (InputBox& p : uiStorage)
    {
        if(p.first != input.first)
        {
            p.first->hide();
            p.second->hide();
            if (p.third != nullptr) p.third->hide();
        }
    }

    input.first->show();
    input.second->show();
    if (input.third != nullptr) input.third->show();
}

/**
 * @brief CommandForm::command_type_index_changed
 *
 * Called when the dropdown menu for command type
 * is updated
 * Sets current command type and shows relevant ui fields
 * @param index         Index of command
 * @see CommandForm::hideOthers(...)
 */
void CommandForm::command_type_index_changed(int index)
{
    // Cast to enum
    cmdType = static_cast<CommandType>(index);

    switch (cmdType)
    {
    case CommandType::CMD: {
        hideOthers({ paramLabel, paramEdit, paramButton});
        break;
    }
    case CommandType::EXPLORER: {
        hideOthers({ paramLabel, paramEdit, paramButton});
        break;
    }
    case CommandType::WEB: {
        hideOthers({ webLabel, webEdit });
        break;
    }
    default: {
        break;
    }
    }
}