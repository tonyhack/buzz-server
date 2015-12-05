
add_subdirectory(protocol/meta/)

SET(META_SOURCE
    ${PROJECT_SOURCE_DIR}/src/test/chat/protocol/protocol_constants.cpp
    ${PROJECT_SOURCE_DIR}/src/test/chat/protocol/protocol_types.cpp)


add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(pressure_client)

