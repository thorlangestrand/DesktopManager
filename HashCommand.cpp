#include "HashCommand.h"

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
