#include "mainwindow.h"

DIRERR MainWindow::removeCommandDirectory(QString hash)
{
    // Delete from disk
    LPWSTR path = NULL;

    // Result of folder path, 0 if no err
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path);

    CoTaskMemFree(path);
    if (!SUCCEEDED(hr)) return DIRERR::LOOKUPFAILED;


    std::string workablePath = utf8_encode(path);
    workablePath += "\\ScuffManager";

    QString folderId = hash;
    namespace fs = std::filesystem;

    std::string dirId = workablePath + "\\" + folderId.toStdString();

    fs::path file("data.scuff");
    fs::path fullPath = dirId / file;

    bool removed = fs::remove_all(dirId);

    if (!removed) return DIRERR::REMOVEFAILED;

    return DIRERR::NONE;
}

void MainWindow::handleCommandDirectoryError(Command* cmd, DIRERR err)
{
    if (err == DIRERR::LOOKUPFAILED)
    {
        Warn("Catastrophic failure failed to locate appdata");
        return;
    }
    else if (err == DIRERR::REMOVEFAILED)
    {
        Warn("Error, unable to remove file for command " + QString::fromStdString(cmd->name) +" !");
        return;
    }
}
