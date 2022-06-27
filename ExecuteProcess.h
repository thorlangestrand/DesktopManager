#ifndef EXECUTEPROCESS_H
#define EXECUTEPROCESS_H

#include <windows.h>
#include <string>
#include <iostream>
#include <QDebug>
size_t ExecuteProcessW(std::wstring fullpath, std::wstring param);
size_t ExecuteProcess(std::wstring fullpath);
//void ExecuteProcess(std::wstring fullpath);
#endif //EXECUTEPROCESS_H
