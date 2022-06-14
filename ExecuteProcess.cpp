#include "ExecuteProcess.h"

/**
 * @brief ExecuteProcess
 * Execute a process
 * @param fullpath      Full path to the executable
 * @param param         Parameters used in the command
 * @return size_t       Return code, 0 for success
 */
size_t ExecuteProcessW(std::wstring fullpath, std::wstring param)
{
  // Return value, 0 on success
  size_t iReturnVal = 0;
  std::wstring sTempStr = L"";

  /**
   * Param needs to be in the form of
   * "C:\Windows\explorer.exe C:\Users\admin\Desktop\PC\art"
   * by the end, while by default it would be
   * "C:\Windows\explorer.exeC:\Users\admin\Desktop\PC\art"
   * hence the need for an extra space
   */
   // If param passed
  if (param.size() != 0)
  {
    // If no space already at beginning
    if (param[0] != L' ')
    {
      // Insert space at beginning of param
      param.insert(0, L" ");
    }
  }

  // First param of CreateProcessW has to be the executable on its own,
  // so need to copy data in order to build up full command
  sTempStr = fullpath;
  param = sTempStr.append(param);


  // Gotta allocate memory because CreateProcessW can modify param
  wchar_t* processParam = new wchar_t[param.size() + 1];
  // Check for failed alloc
  if (processParam == nullptr) return 1;

  // Copy data from param to buffer
  wcscpy_s(processParam, param.size() + 1, param.c_str());

  // CreateProcess API initialization
  STARTUPINFOW siStartupInfo;
  PROCESS_INFORMATION piProcessInfo;

  // Need to set memory for some reason
  memset(&siStartupInfo, 0, sizeof(STARTUPINFOW));
  memset(&piProcessInfo, 0, sizeof(PROCESS_INFORMATION));

  // Size of structure
  siStartupInfo.cb = sizeof(siStartupInfo);


  /**
   * NOTE:
   * Can also have the first parameter be NULL
   */
  // If failed
  if (CreateProcessW(const_cast<LPCWSTR>(fullpath.c_str()),
    processParam, 0, 0, false,
    CREATE_DEFAULT_ERROR_MODE, 0, 0,
    &siStartupInfo, &piProcessInfo) == false)
  {
    iReturnVal = GetLastError();
  }

  // Watch process for 1 second, if fail return 1
  DWORD dwExitCode;
  dwExitCode = WaitForSingleObject(piProcessInfo.hProcess, (1000));
  if (dwExitCode != 0) iReturnVal = 1;

  // Free memory
  delete[]processParam;

  // Release handles
  CloseHandle(piProcessInfo.hProcess);
  CloseHandle(piProcessInfo.hThread);

  return iReturnVal;
}

/**
 * @brief ExecuteProcess
 * Execute a process
 * Overload without param
 * @param fullpath      Full path to the executable
 * @return size_t       Return code, 0 for success
 */
size_t ExecuteProcess(std::wstring fullpath)
{
  // Return value, 0 on success
  size_t iReturnVal = 0;
  std::wstring sTempStr = L"";
  // First param of CreateProcessW has to be the executable on its own,
  // so need to copy data in order to build up full command
  sTempStr = fullpath;


  // CreateProcess API initialization
  STARTUPINFOW siStartupInfo;
  PROCESS_INFORMATION piProcessInfo;

  // Need to set memory for some reason
  memset(&siStartupInfo, 0, sizeof(STARTUPINFOW));
  memset(&piProcessInfo, 0, sizeof(PROCESS_INFORMATION));

  // Size of structure
  siStartupInfo.cb = sizeof(siStartupInfo);


  /**
   * NOTE:
   * Can also have the first parameter be NULL
   */
  // If failed
  if (CreateProcessW(const_cast<LPCWSTR>(fullpath.c_str()),
    NULL, 0, 0, false,
    CREATE_DEFAULT_ERROR_MODE, 0, 0,
    &siStartupInfo, &piProcessInfo) == false)
  {
    iReturnVal = GetLastError();
  }

  // Watch process for 1 second, if fail return 1
  DWORD dwExitCode;
  dwExitCode = WaitForSingleObject(piProcessInfo.hProcess, (1000));
  if (dwExitCode != 0) iReturnVal = 1;

  // Release handles
  CloseHandle(piProcessInfo.hProcess);
  CloseHandle(piProcessInfo.hThread);

  return iReturnVal;
}
//void ExecuteProcess(std::wstring fullpath)
//{
//  // Return value, 0 on success
//  std::wstring sTempStr = L"";
//  // First param of CreateProcessW has to be the executable on its own,
//  // so need to copy data in order to build up full command
//  sTempStr = fullpath;


//  // CreateProcess API initialization
//  STARTUPINFOW siStartupInfo;
//  PROCESS_INFORMATION piProcessInfo;

//  // Need to set memory for some reason
//  memset(&siStartupInfo, 0, sizeof(STARTUPINFOW));
//  memset(&piProcessInfo, 0, sizeof(PROCESS_INFORMATION));

//  // Size of structure
//  siStartupInfo.cb = sizeof(siStartupInfo);


//  /**
//   * NOTE:
//   * Can also have the first parameter be NULL
//   */
//  // If failed
//  if (CreateProcessW(const_cast<LPCWSTR>(fullpath.c_str()),
//    NULL, 0, 0, false,
//    CREATE_DEFAULT_ERROR_MODE, 0, 0,
//    &siStartupInfo, &piProcessInfo) == false)
//  {

//  }

//  // Watch process for 1 second, if fail return 1




//  // Release handles
//  CloseHandle(piProcessInfo.hProcess);
//  CloseHandle(piProcessInfo.hThread);
//}
