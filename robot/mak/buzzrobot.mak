TARGET = bin/buzzrobot
SRCS = src/robot/actor.cc \
       src/robot/actor_manager.cc \
       src/robot/actor_sight.cc \
       src/robot/aoi_actor.cc \
       src/robot/aoi_entity.cc \
       src/robot/aoi_npc.cc \
       src/robot/aoi_role.cc \
       src/robot/aoi_scene_item.cc \
       src/robot/client_app.cc \
       src/robot/config.cc \
       src/robot/debug.cc \
       src/robot/event_dispatcher.cc \
       src/robot/logging.cc \
       src/robot/main.cc \
       src/robot/map.cc \
       src/robot/map_manager.cc \
       src/robot/position.cc \
       src/robot/thrift_codec.cc \
       src/robot/user_command_handler.cc \
       src/robot/action/action.cc \
       src/robot/action/action_approach_target.cc \
       src/robot/action/action_attack.cc \
       src/robot/action/action_automata.cc \
       src/robot/action/action_automata_manager.cc \
       src/robot/action/action_check_in_area.cc \
       src/robot/action/action_check_variable.cc \
       src/robot/action/action_gm_command.cc \
       src/robot/action/action_modify_variable.cc \
       src/robot/action/action_move.cc \
       src/robot/action/action_rand_move.cc \
       src/robot/action/action_rand_move_in_area.cc \
       src/robot/action/action_set_variable.cc \
       src/robot/action/action_sleep.cc \
       src/robot/ai/melee_skill_selector.cc \
       src/robot/ai/monster_selector.cc \
       src/robot/ai/target_selector.cc \
       src/robot/ai/skill_selector.cc
CPP_FLAG = -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H
LINK_TYPE = exec
INCLUDE = -Isrc
LIB = -Lbuild -lbuzzprotocol -lbrickredcore -lthrift -ltinyxml2 -pthread -lrt
DEPFILE = build/libbuzzprotocol.a build/libbrickredcore.a build/libtinyxml2.a
BUILD_DIR = build

include mak/main.mak

