
set(THRIFT_SCRIPT
    async_request.thirft
    database_protocol.thrift
    database_storage.thrift
    storage_actor_login.thrift
    storage_actor_logout_center.thrift
    storage_actor_online.thrift
    storage_actor_save.thrift
    storage_buff_login.thrift
    storage_buff_save.thrift
    storage_check_session.thrift
    storage_cooling_login.thrift
    storage_cooling_save.thrift
    storage_item_login.thrift
    storage_item_save.thrift
    storage_skill_login.thrift
    storage_skill_save.thrift
    storage_soul_login.thrift
    storage_soul_save.thrift
    storage_soul_setting_login.thrift
    storage_soul_setting_save.thrift
    storage_task_login.thrift
    storage_task_save.thrift)



if(UNIX)

foreach(script ${THRIFT_SCRIPT})
  exec_program(thrift ${PROJECT_SOURCE_DIR}/src/database_server/protocol/meta/
      ARGS "-I ${GLOBAL_META_DIR} -out ../ --gen cpp ${script}")
endforeach(script)

endif(UNIX)

