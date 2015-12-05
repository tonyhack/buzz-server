
set(THRIFT_SCRIPT
    game_buff_request.thrift
    game_cooling_request.thrift
    game_effect_request.thrift
    game_item_request.thrift
    game_request.thrift
    game_scene_request.thrift
    game_skill_request.thrift
    game_soul_request.thrift)

if(UNIX)

foreach(script ${THRIFT_SCRIPT})
#  exec_program(thrift ${PROJECT_SOURCE_DIR}/src/game_server/server/request/meta/
#      ARGS "-I ${ENTITY_META_DIR} -out ../ --gen cpp ${script}")
  exec_program(thrift ${PROJECT_SOURCE_DIR}/src/game_server/server/request/meta/
      ARGS "-I ${ENTITY_META_DIR} -out ${PROJECT_SOURCE_DIR}/generate/configure/extension/python/ --gen py ${script}")
endforeach(script)

endif(UNIX)

