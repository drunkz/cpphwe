#include "include/hw_config.h"

namespace hw {

Config::Config() {
}

Config::~Config() {
}

bool Config::Load(const std::string& filename) {
    ini.SetUnicode();
    SI_Error rc = ini.LoadFile(filename.c_str());
    if (rc < 0) {
        return false;
    }
    FillData();
    ini.Reset();
    return true;
}

} // namespace hw