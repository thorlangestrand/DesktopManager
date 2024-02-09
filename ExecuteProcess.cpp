#include "ExecuteProcess.h"

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
  if (processParam == nullptr) {
      return 1;
  }

  // Copy data from param to buffer
  wcscpy_s(processParam, param.size() + 1, param.c_str());

  STARTUPINFOW siStartupInfo = {};
  PROCESS_INFORMATION piProcessInfo = {};

  /**
   * Zero initialize all values. This can also be done by writing
   * STARTUPINFOW siStartupInfo = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   * and
   * PROCESS_INFORMATION piProcessInfo = {0,0,0,0};
   * or
   * STARTUPINFOW = {};
   * and
   * PROCESS_INFORMATION = {};
   * respectively
   *
   *
   * The reason memset works here is because STARTUPINFOW and PROCESS_INFORMATION
   * are POD (Plain Old Data) structures
   *
   * A POD struct or class is a struct without any user defined constructors or
   * destructors or copy assignment operators. It also may not have any non-static
   * members that are not POD structures. An example of a non-POD structure would be
   * the std::string class
   *
   * There are other things that also determine whether a struct is a POD, however I
   * do not understand them well enough to explain it here
   *
   * Example
   *
   * Given structs
   * struct example_one {
   *    int a;
   *    int b;
   * };
   *
   * and
   * struct example_two {
   *    int a;
   *    std::string b;
   * }
   *
   * both
   * example_one e1 = {};
   * and
   * example_two e2 = {};
   * would be valid initializers
   * However, only for example_one can the memset method be used
   *
   * example_one e1;
   * memset(&e1, 0, sizeof(example_one));
   *
   * The above is valid as example_one only has POD (plain old data) members
   *
   * example_two e2;
   * memset(&e2, 0, sizeof(example_two);
   *
   * If we now try to something like set
   * e2.b = "";
   * the program will crash
   *
   * The program does not crash if e2.b is set to a strng of length 1 or greater
   * Why this happens is beyond me, and it appears to be completely undefined
   * behaviour
   */

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
  if (dwExitCode != 0) {
      iReturnVal = 1;
  }

  // Free memory
  delete[] processParam;

  // Release handles
  CloseHandle(piProcessInfo.hProcess);
  CloseHandle(piProcessInfo.hThread);

  return iReturnVal;
}

size_t ExecuteProcess(std::wstring fullpath)
{
  // Return value, 0 on success
  size_t iReturnVal = 0;
  std::wstring sTempStr = L"";
  // First param of CreateProcessW has to be the executable on its own,
  // so need to copy data in order to build up full command
  sTempStr = fullpath;

  HINSTANCE sHandle = ShellExecuteW(
              NULL,
              L"open",
              const_cast<LPCWSTR>(fullpath.c_str()),
              NULL,
              NULL,
              SW_SHOWNORMAL
  );

  if (CloseHandle(sHandle) == 0) {
      iReturnVal = 1;
  }

  qDebug() << sHandle;
  return iReturnVal;
}
