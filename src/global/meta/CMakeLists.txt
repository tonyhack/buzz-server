set(THRIFT_SCRIPT
    kickout_actor_type.thrift
    server_type.thrift)

if(UNIX)

foreach(script ${THRIFT_SCRIPT})
  exec_program(thrift ${GLOBAL_META_DIR}
      ARGS "-out ../ --gen cpp ${script}")
endforeach(script)

endif(UNIX)

