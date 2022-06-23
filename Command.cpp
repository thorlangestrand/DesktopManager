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
    case CommandType::SWF : {
        std::future<size_t> d = std::async(std::launch::async, ExecuteProcessW, L"C:\\Users\\admin\\Desktop\\PC\\not_hentai\\flashplayer32.exe", param);
        break;
    }
    // To run steam shortcuts, use WEB
    // Example: rundll32 url.dll, FileProtocolHandler steam://rungameid/1983000
    // Runs HAYAI
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

/**
 * @brief Command::nameMatch
 * @param _name     Name being checked
 * Returns whether or not substring matches name of command
 * @return      Whether match or not
 */
bool Command::nameMatch(std::string _name)
{
    std::string tmp = name;
    for (size_t i = 0; i < tmp.size(); i++)
        tmp[i] = std::toupper(tmp[i]);
    for (size_t i = 0; i < _name.size(); i++)
        _name[i] = std::toupper(_name[i]);
    return (tmp.compare(0, _name.size(), _name) == 0);
}


/**
 * @brief Command::updateUi
 * Updates command UI based on whether or not the command
 * matches the current search
 *
 * @param unselectedStyleSheet      Either an empty string in which case
 *                                  Qt defaults to using the original stylesheet,
 *                                  or a string representation of an alternate
 *                                  stylesheet
 */
void Command::updateUi(const QString& unselectedStyleSheet)
{

    // Update based on passed stylesheet
    container.pushB->setStyleSheet(unselectedStyleSheet);
    container.editB->setStyleSheet(unselectedStyleSheet);
    container.label->setStyleSheet(unselectedStyleSheet);
}


