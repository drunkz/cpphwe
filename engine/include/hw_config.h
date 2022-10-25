#ifndef HW_CONFIG_H
#define HW_CONFIG_H

#include "SimpleIni.h"
#include <string>

namespace hw {
class Config {
  public:
    Config();
    virtual ~Config();
    bool Load(const std::string& filename);

  protected:
    virtual void FillData() = 0;

  protected:
    CSimpleIniA ini;
};

} // namespace hw

#endif // HW_CONFIG_H
