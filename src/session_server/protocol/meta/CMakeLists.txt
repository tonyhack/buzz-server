
set(THRIFT_SCRIPT
    game_actor_protocol.thrift
    game_scene_protocol.thrift
    session_game_protocol.thrift
    session_gateway_protocol.thrift
    session_protocol.thrift)

if(UNIX)

foreach(script ${THRIFT_SCRIPT})
  exec_program(thrift ${PROJECT_SOURCE_DIR}/src/session_server/protocol/meta/
      ARGS "-I ${GLOBAL_META_DIR} -out ../ --gen cpp ${script}")
endforeach(script)

endif(UNIX)

