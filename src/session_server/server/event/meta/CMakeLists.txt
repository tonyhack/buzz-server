
set(THRIFT_SCRIPT
    session_actor_event.thrift
    session_event.thrift
    session_system_event.thrift)

if(UNIX)

foreach(script ${THRIFT_SCRIPT})
  exec_program(thrift ${PROJECT_SOURCE_DIR}/src/session_server/server/event/meta/
      ARGS "-I ${GLOBAL_META_DIR} -out ../ --gen cpp ${script}")
endforeach(script)

endif(UNIX)

