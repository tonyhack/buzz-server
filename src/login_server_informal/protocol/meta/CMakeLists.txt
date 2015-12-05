
set(THRIFT_SCRIPT
    database_storage.thrift
    login_protocol.thrift)

if(UNIX)

foreach(script ${THRIFT_SCRIPT})
  exec_program(thrift ${PROJECT_SOURCE_DIR}/src/login_server_informal/protocol/meta/
      ARGS "-I ${GLOBAL_META_DIR} -out ../ --gen cpp ${script}")
endforeach(script)

endif(UNIX)

