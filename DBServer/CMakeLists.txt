project(DBServer VERSION 1.0.2)

include(build)

set(source_files
    main.cpp
    db_server.cpp
    ../engine/hw_config.cpp
    ../engine/hw_server_base.cpp
    ../common/system.cpp
    ../engine/hw_net_mgr.cpp
    ../engine/hw_conn_mgr.cpp
    ../engine/hw_log.cpp
    ../engine/cfg_server_base.cpp
)

add_executable(${PROJECT_NAME} ${source_files})
target_link_libraries(${PROJECT_NAME} ${target_link_lib})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/config.ini ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}/config.ini
    ${mariadbcpp_dll} ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}
)