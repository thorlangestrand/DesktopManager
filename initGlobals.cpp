#include "initGlobals.h"

#include <QDebug>
bool initGlobals()
{

    LPWSTR path = NULL;

    HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path);

    if (!SUCCEEDED(hr))
    {
        Warn("Catastrophic failure failed to locate appdata");
        return false;
    }

    CoTaskMemFree(path);

    std::string workablePath = utf8_encode(path);

    workablePath += "\\ScuffManager";

    if (!std::filesystem::exists(workablePath))
    {
        if (!std::filesystem::create_directory(workablePath))
        {
            QString wMsg = "Failed to create directory at path: " + QString::fromStdString(workablePath);
            Warn(wMsg);
            return false;
        }
    }

    // If no config file found create one and initialize with default values
    if (!std::filesystem::exists(workablePath + "\\config.scuff"))
    {
        Warn("No config file detected, using default configuration");

        std::ofstream out(workablePath + "\\config.scuff");

        if (!out.is_open())
        {
            Warn("Failed to open config file");
            return false;
        }

        out << "C:\\flashplayer32.exe";
        out.close();
        return true;
    }


    std::ifstream in(workablePath + "\\config.scuff");

    if (!in.is_open())
    {
        Warn("Failed to open config file");
        return false;
    }

    std::getline(in, globals::swfPath);
    in.close();

    return true;
}

