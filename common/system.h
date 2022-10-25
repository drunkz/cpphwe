#ifndef HW_SYSTEM_H
#define HW_SYSTEM_H

#include <string>

namespace hw {

// 文件是否存在
bool FileExists(const std::string& sFilename);

// 是否已运行
bool IsAlreadyRun(const std::string& lpName);

} // namespace hw

#endif // HW_SYSTEM_H