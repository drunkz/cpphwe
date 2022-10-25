#include "system.h"
#include <filesystem>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace hw {
namespace fs = std::filesystem;

bool FileExists(const std::string& sFilename) {
    return fs::exists(fs::path(sFilename));
}

bool IsAlreadyRun(const std::string& lpName) {
    HANDLE hMutex = NULL;
    hMutex = CreateMutex(NULL, FALSE, lpName.c_str());
    if (hMutex != NULL) {
        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            CloseHandle(hMutex);
            return true;
        }
    }
    return false;
}

} // namespace hw