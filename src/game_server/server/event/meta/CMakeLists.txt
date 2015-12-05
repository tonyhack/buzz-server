
set(THRIFT_SCRIPT
    game_actor_event.thrift
    game_event.thrift
    game_npc_event.thrift
    game_role_event.thrift
    game_scene_event.thrift
    game_skill_event.thrift
    game_system_event.thrift)

if(UNIX)

foreach(script ${THRIFT_SCRIPT})
#  exec_program(thrift ${PROJECT_SOURCE_DIR}/src/game_server/server/event/meta/
#      ARGS "-I ${ENTITY_META_DIR} -out ../ --gen cpp ${script}")
  exec_program(thrift ${PROJECT_SOURCE_DIR}/src/game_server/server/event/meta/
      ARGS "-I ${ENTITY_META_DIR} -out ${PROJECT_SOURCE_DIR}/generate/configure/extension/python --gen py ${script}")
endforeach(script)

endif(UNIX)

