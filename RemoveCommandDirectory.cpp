#include "mainwindow.h"


/**
 * @brief MainWindow::removeCommandDirectory
 *
 * Removes a command and its containing directory given
 * by passed hash from disk
 *
 * @param hash      Hash of command to be removed from disk
 *
 * @return          Error based on outcome, default is NONE
 */
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



/**
 * @brief MainWindow::handleCommandDirectoryError
 *
 * Gives a brief explanation of the given error
 *
 * @param cmd       Command which caused the error
 * @param err       Error type
 *
 * @see Warn(...)
 */
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
