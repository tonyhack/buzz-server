
set(CORESH_SOURCES
    application_service.cc
    client_side.cc
    communicator.cc
    connection.cc
    connection_pool.cc
    connect_ready_queue.cc
    message_queue.cc
    packetizer.cc
    variable_table.cc
    reusable_buffers.cc
    lock_free_queue.cc
    network_service.cc
    packetizer_table.cc
    system_functions.cc)

set(CORESH_INCLUDES
    application_service.h
    atomic_counter.h
    blocking_queue.h
    client_side.h
    communicator.h
    compressor.h
    connection.h
    connection_pool.h
    connect_ready_queue.h
    encoder.h
    lock_free_queue.h
    message_queue.h
    network_service.h
    nonblocking_queue.h
    packetizer.h
    packetizer_table.h
    protocol_format.h
    reusable_buffers.h
    system_functions.h
    variable_table.h)


set(CORESH_COMPRESS_SOURCES
    compress/compress_null.cc)

set(CORESH_COMPRESS_INCLUDES
    compress/compress_null.h)


set(CORESH_ENCODE_SOURCES
    encode/encode_null.cc)

set(CORESH_ENCODE_INCLUDES
    encode/encode_null.h)



if(UNIX)

  set(ALL_FILES
      ${CORESH_SOURCES}
      ${CORESH_COMPRESS_SOURCES}
      ${CORESH_ENCODE_SOURCES}
      ${CORE_BASE_SOURCES}
      ${CORE_TINYXML_SOURCES}
      ${CORE_SOURCES})

elseif(MSVC)

  set(ALL_FILES
      ${CORESH_SOURCES}
      ${CORESH_INCLUDES}
      ${CORESH_COMPRESS_SOURCES}
      ${CORESH_COMPRESS_INCLUDES}
      ${CORESH_ENCODE_SOURCES}
      ${CORESH_ENCODE_INCLUDES}
      ${CORE_BASE_SOURCES}
      ${CORE_BASE_INCLUDES}
      ${CORE_TINYXML_SOURCES}
      ${CORE_TINYXML_INCLUDES}
      ${CORE_SOURCES}
      ${CORE_INCLUDES}
      )

  source_group_by_dir(ALL_FILES)

endif()


#Shared library
#add_library(coresh SHARED ${ALL_FILES})

#Static library
add_library(coresh_static STATIC ${ALL_FILES})

#Ensure build both share and static library using same name.
set_target_properties(coresh_static PROPERTIES OUTPUT_NAME "coresh")
#set_target_properties(core PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(coresh_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

#add_subdirectory(test)

