#include "db_server.h"
#include "../engine/include/hw_log.h"

namespace hw {

bool DBServer::OnStart() {
    HW_LOG_INFO("DBServer Start");
    return true;
}

void DBServer::OnClose() {
    HW_LOG_INFO("DBServer Close");
}

} // namespace hw
