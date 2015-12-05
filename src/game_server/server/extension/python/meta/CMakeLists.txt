set(THRIFT_SCRIPT
    python_variable.thrift)

if(UNIX)

  foreach(script ${THRIFT_SCRIPT})
    #exec_program(thrift ${PROJECT_SOURCE_DIR}/src/game_server/server/extension/python/meta/
    #    ARGS "-out ../ --gen cpp ${script}")
    exec_program(thrift ${PROJECT_SOURCE_DIR}/src/game_server/server/extension/python/meta/
        ARGS "-out ${PROJECT_SOURCE_DIR}/generate/configure/extension/python/ --gen py ${script}")
  endforeach(script)

endif(UNIX)

