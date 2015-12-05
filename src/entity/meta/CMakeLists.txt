
set(THRIFT_SCRIPT
    actor.thrift
    constants.thrift
    entity.thrift
    equipment.thrift
    direction.thrift
    item.thrift
    npc.thrift
    role.thrift
    task.thrift)

if(UNIX)

foreach(script ${THRIFT_SCRIPT})
  exec_program(thrift ${PROJECT_SOURCE_DIR}/src/entity/meta/
      ARGS "-out ../ --gen cpp ${script}")
  exec_program(thrift ${PROJECT_SOURCE_DIR}/src/entity/meta/
      ARGS "-out ${PROJECT_SOURCE_DIR}/generate/configure/extension/python/ --gen py ${script}")
endforeach(script)

endif(UNIX)

