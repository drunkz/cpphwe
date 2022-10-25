#include "../engine/include/hw_internals.h"
#include "../engine/include/hw_log.h"
#include "db_server.h"
#include <iostream>

#ifdef _WIN32
#include "client/windows/handler/exception_handler.h"
#endif

bool MinidumpCallback(const wchar_t* dump_path, const wchar_t* minidump_id, void* context, EXCEPTION_POINTERS* exinfo, MDRawAssertionInfo* assertion,
                      bool succeeded) {
    return true;
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    google_breakpad::ExceptionHandler eh(L"dmp", nullptr, MinidumpCallback, nullptr, google_breakpad::ExceptionHandler::HANDLER_ALL);
#endif
    using namespace hw;
    if (!DBServer::GetInstance().Run(kInitUV | kInitDB)) {
        return -1;
    }
    // DBServer::GetInstance().Close();
    return 0;
}
