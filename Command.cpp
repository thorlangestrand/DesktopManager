#include "Command.h"

void Command::save(std::ofstream& output)
{
    output << type << "\n";
    output << icon << "\n";
    output << name << "\n";
    output << utf8_encode(param) << "\n";
    output << website;
}

void Command::run()
{
    switch(type)
    {
    case CommandType::CMD: {
        std::future<size_t> d = std::async(std::launch::async, ExecuteProcess, param);
        break;
    }
    case CommandType::EXPLORER: {
        std::future<size_t> d = std::async(std::launch::async, ExecuteProcessW, L"C:\\Windows\\explorer.exe", param);
        break;
    }
    case CommandType::WEB: {
        std::string tmp = "rundll32 url.dll, FileProtocolHandler ";
        // If not begin with www., add
        if(website.rfind("www.", 0) != 0)
        {
            tmp += "www.";
        }
        tmp += website;

        std::future<int> d = std::async(std::launch::async, system, tmp.c_str());
        break;
    }
    default: {
        break;
    }
    }
}

//void Command::edit()
//{

//}
