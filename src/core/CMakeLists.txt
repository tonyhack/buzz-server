#CMakeLists for src directory.

set(CORE_BASE_SOURCES
  base/bound_queue.cc
  base/boundless_queue.cc
  base/byte_buffer_dynamic.cc
  base/common.cc
  base/logging.cc
  base/thread.cc)

set(CORE_BASE_INCLUDES
  base/auto_mutex.h
  base/bound_queue.h
  base/boundless_queue.h
  base/bound_queue_common.h
  base/byte_buffer_dynamic.h
  base/byte_buffer_static.h
  base/common.h
  base/condition_variable.h
  base/logging.h
  base/mutex.h
  base/noncopyable.h
  base/scoped_mutex.h
  base/thread.h
  base/time_tick.h            
  base/types.h                
  base/variable_parameter.h)


set(CORE_TINYXML_SOURCES
  tinyxml/tinystr.cpp
  tinyxml/tinyxml.cpp
  tinyxml/tinyxmlerror.cpp
  tinyxml/tinyxmlparser.cpp)

set(CORE_TINYXML_INCLUDES
  tinyxml/tinystr.h
  tinyxml/tinyxml.h)


set(CORE_SOURCES
  acceptor.cc
  async_logging.cc
  async_logging_loop.cc
  async_logging_singleton.cc
  channel.cc
  configure.cc
  connector.cc
  epoller.cc
  event_control.cc
  event_loop.cc
  event_loop_thread_group.cc
  inet_address.cc
  loop_thread.cc
  signal_set.cc
  tcp_client.cc
  tcp_connection.cc
  tcp_server.cc
  tcp_socket.cc
  timing_base.cc
  timing_group.cc
  timing_wheel.cc)

set(CORE_INCLUDES
  acceptor.h
  async_logging.h
  async_logging_loop.h
  async_logging_singleton.h
  base_loop.h
  callbacks.h
  channel.h
  configure.h
  connector.h
  event_loop.h
  signaller.h
  timer.h
  epoller.h
  event_control.h
  event_handler.h
  event_loop_thread_group.h
  event_pipe.h
  inet_address.h
  logging_level.h
  loop_thread.h
  signal_set.h
  tcp_client.h
  tcp_connection.h
  tcp_server.h
  tcp_socket.h
  timer_handler.h
  timing_base.h
  timing_group.h
  timing_wheel.h
  version.h)


if(UNIX)

  set(ALL_FILES
      ${CORE_BASE_SOURCES}
      ${CORE_TINYXML_SOURCES}
      ${CORE_SOURCES})

elseif(MSVC)

  set(ALL_FILES
      ${CORE_BASE_INCLUDES}
      ${CORE_BASE_SOURCES}
      ${CORE_TINYXML_INCLUDES}
      ${CORE_TINYXML_SOURCES}
      ${CORE_INCLUDES}
      ${CORE_SOURCES})

  source_group_by_dir(ALL_FILES)

endif()


#Shared library
#add_library(core SHARED
#  ${ALL_FILES})

#Static library
add_library(core_static STATIC
  ${ALL_FILES})

#Ensure build both share and static library using same name.
set_target_properties(core_static PROPERTIES OUTPUT_NAME "core")
#set_target_properties(core PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(core_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

