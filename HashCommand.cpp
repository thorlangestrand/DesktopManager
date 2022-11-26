#include "HashCommand.h"

/**
 * @brief hashCommand
 *
 * Converts all data from a command to a hash
 * which is then used to ensure no exact duplicate
 * commands
 * The hash is also used as the save folder where
 * the command is stored
 *
 * @param cmd       Command to be hashed
 * @return          MD5 hash of command
 */
QString hashCommand(Command* cmd)
{
return QString(
            QCryptographicHash::hash(
                QByteArray::fromStdString(
                                      (char)static_cast<int>(cmd->type) +
                                       cmd->name +
                                       cmd->icon +
                                       utf8_encode(cmd->param) +
                                       cmd->website)
                ,QCryptographicHash::Md5).toHex());
}
