#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <QStringList>
#include <fstream>
#include <future>
#include <QMessageBox>

#include "utf8_parse.h"
#include "ExecuteProcess.h"
//#include "CommandForm.h"
enum CommandType
{
  CMD = 0,
  EXPLORER = 1,
  WEB = 2
};


struct Command
{
  CommandType type = CommandType::CMD;
  std::string icon = "";
  std::string name = "";
  std::wstring param = L"";
  std::string website= "";
  void save(std::ofstream& out);
  void run();
  //void edit();
};

#endif // COMMAND_H
