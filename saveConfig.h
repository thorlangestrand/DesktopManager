#ifndef SAVECONFIG_H
#define SAVECONFIG_H

#include <filesystem>
#include <shlobj.h>
#include <combaseapi.h>
#include <fstream>

#include "warn.h"
#include "globals.h"
#include "utf8_parse.h"
/// Save config to disk
/// Returns 0 on success, 1 on failure
bool saveConfig();

#endif // SAVECONFIG_H
