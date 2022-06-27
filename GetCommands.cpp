#include "mainwindow.h"

/**
 * @brief
 * Read data to a command that uses cmd.exe as launch vector
 *
 * Remaining data to read
 * [STRING]         PARAM
 * [STRING]         WEBSITE - IGNORED
 *
 * @param data      Stream of data being read
 * @param c         Cmd command
 */
void readCommandData(std::ifstream& data, Command* c, std::string& buf)
{
    std::getline(data, buf);
    c->param = utf8_decode(buf);
}

/**
 * @brief
 * Read data to a command that launches a website
 *
 * Remaining data to read
 *
 * [STRING]         PARAM - IGNORED
 * [STRING]         WEBSITE
 *
 * @param data      Stream of data being read
 * @param c         Command
 */
void readWebData(std::ifstream& data, Command* c, std::string& buf)
{
    data.ignore(); // IGNORE PARAM
    std::getline(data, buf);
    c->website = buf; // WEBSITE
}


/**
 * @brief
 * Returns a list of all available commands
 *
 * FILE STRUCTURE
 *
 * [INT]        COMMAND TYPE
 * [STRING]     ICON
 * [STRING]     NAME
 * [STRING]     PARAM
 * [STRING]     WEBSITE
 *
 * Each command requires different information
 *
 * @return std::vector<Command>     List of commands
 */
void MainWindow::getCommands()
{
  namespace fs = std::filesystem;


  LPWSTR path = NULL;

  // Result of folder path, 0 if no err
  HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path);

  if (!SUCCEEDED(hr))
  {
      QMessageBox warn;
      warn.setText("Catastrophic failure failed to locate appdata");
      warn.exec();
      return;
  }

  CoTaskMemFree(path);

  std::string workablePath = utf8_encode(path);
  workablePath += "\\ScuffManager";

  if (!std::filesystem::exists(workablePath))
  {
    mkdir(workablePath.c_str());
  }



  for (const fs::directory_entry &dirEntry : fs::directory_iterator(workablePath))
  {
    fs::path dir(dirEntry);
    fs::path file("data.scuff");
    fs::path fullPath = dir / file;

    std::ifstream data(fullPath);

    if (data.fail())
    {
      Warn("Error, failure to open file at " + QString::fromStdString(fullPath.generic_string()));
    }
    else {
      Command* c = new Command;
      size_t type = 0;
      data >> type; data.ignore();
      c->type = static_cast<CommandType>(type); // Set type of command

      std::string buf = "";
      // Use getline instead of >> / ignore where data structure is unknown
      // (e.g. undefined spacing)
      std::getline(data, buf);
      c->icon = buf; // ICON
      buf = "";

      std::getline(data, buf);
      c->name = buf; // NAME
      buf = "";

      switch (type)
      {
      case CommandType::CMD: {
        readCommandData(data, c, buf);
        break;
      }
      case CommandType::EXPLORER: {  
        readCommandData(data, c, buf);
        break;
      }
      case CommandType::WEB: {
        readWebData(data, c, buf);
        break;
      }
      case CommandType::SWF: {
        readCommandData(data, c, buf);
        break;
      }
      default: {
        Warn("Unexpected command type detected, exiting...");
        exit(1);
      }
      }
      // Always remember to close up your data streams lads
      data.close();
      commands.push_back(c);
    }
  }
}
