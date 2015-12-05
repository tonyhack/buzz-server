
set(THRIFT_SCRIPT
    session_request.thrift
    session_scene_request.thrift)

if(UNIX)

foreach(script ${THRIFT_SCRIPT})
  exec_program(thrift ${PROJECT_SOURCE_DIR}/src/session_server/server/request/meta/
      ARGS "-out ../ --gen cpp ${script}")
endforeach(script)

endif(UNIX)

