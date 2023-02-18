#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <QStringList>
#include <fstream>
#include <future>
#include <QMessageBox>
#include <QGridLayout>
#include <QLabel>
#include <QFile>
#include <QStringList>
#include "ScuffButton.h"
#include "utf8_parse.h"
#include "ExecuteProcess.h"
#include "ScuffButton.h"
#include "globals.h"
// Command types
enum CommandType
{
  CMD = 0,
  EXPLORER = 1,
  WEB = 2,
  SWF = 3
};

/**
 * @brief The parentData struct
 *
 * Hosts information about the "physical" structure
 * that represents the command in the ui
 *
 * For now only used for editing style but this could
 * also be used for editing content and deletion
 * (if it also referenced the qgridlayout that houses all these objects)
 */
struct parentData
{
    ScuffButton* pushB = nullptr;
    ScuffButton* editB = nullptr;
    QLabel*      label = nullptr;

    parentData(ScuffButton* _b1, ScuffButton* _b2, QLabel* _l1)
    {
        pushB = _b1; editB = _b2; label = _l1;
    }

    ~parentData(){};
};


/**
 * @brief The Command struct
 * Has all the data required for execution of command
 */
struct Command
{
  CommandType type = CommandType::CMD;
  std::string icon = "";
  std::string name = "";
  std::wstring param = L"";
  std::string website= "";

  parentData container = { nullptr, nullptr, nullptr };

  Command(){};

  void save(std::ofstream& out);
  void run();
  bool nameMatch(std::string _name);
  void updateUi(const QString& unselectedStyleSheet);
};

#endif // COMMAND_H
