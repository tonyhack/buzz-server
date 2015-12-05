
set(THRIFT_SCRIPT
    gateway_buff_protocol.thrift
    gateway_entity_protocol.thrift
    gateway_item_protocol.thrift
    gateway_gm_protocol.thrift
    gateway_protocol.thrift
    gateway_scene_protocol.thrift
    gateway_skill_protocol.thrift
    gateway_soul_protocol.thrift
    gateway_task_protocol.thrift)

if(UNIX)

foreach(script ${THRIFT_SCRIPT})
  exec_program(thrift ${PROJECT_SOURCE_DIR}/src/gateway_server/protocol/meta/
      ARGS "-I ${ENTITY_META_DIR} -I ${GLOBAL_META_DIR} -out ../ --gen cpp ${script}")
endforeach(script)

endif(UNIX)

