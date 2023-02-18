#ifndef EXECUTEPROCESS_H
#define EXECUTEPROCESS_H

#include <QDebug>

#include <windows.h>
#include <string>
#include <iostream>

///
/// \brief  ExecuteProcessW
///
/// Execute a process with parameters
///
/// \param  fullpath - Full path to the executable
/// \param  param    - Parameters used in the command
/// \return size_t   - 0 for success
///
size_t ExecuteProcessW(std::wstring fullpath, std::wstring param);

///
/// \brief ExecuteProcess
///
/// Execute a process without parameters
///
/// \param  fullpath - Full path to the executable
/// \return size_t   - 0 for success
///
size_t ExecuteProcess(std::wstring fullpath);

#endif //EXECUTEPROCESS_H
