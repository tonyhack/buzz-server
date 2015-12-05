
set(THRIFT_SCRIPT
    protocol.thrift)

if(UNIX)

foreach(script ${THRIFT_SCRIPT})
  exec_program(thrift ${PROJECT_SOURCE_DIR}/src/test/chat/protocol/meta/
      ARGS "-r -out ../ --gen cpp ${script}")
endforeach(script)

endif(UNIX)
