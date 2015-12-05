#!/bin/sh

script_path=`readlink -f $0`
script_dir=`dirname $script_path`

buzz_root=`readlink -f $script_dir/../../`
buzz_robot_root=`readlink -f $script_dir/../`

# gateway protocol
rm $buzz_robot_root/src/protocol/*.h \
   $buzz_robot_root/src/protocol/*.cpp

cp $buzz_root/src/entity/*.h \
   $buzz_root/src/entity/*.cpp \
   $buzz_robot_root/src/protocol/
cp $buzz_root/src/global/kickout_actor_* \
   $buzz_root/src/global/server_type_* \
   $buzz_robot_root/src/protocol/
cp $buzz_root/src/gateway_server/protocol/gateway_*.h \
   $buzz_root/src/gateway_server/protocol/gateway_*.cpp \
   $buzz_robot_root/src/protocol/
cp $buzz_root/src/login_server_informal/protocol/login_*.h \
   $buzz_root/src/login_server_informal/protocol/login_*.cpp \
   $buzz_robot_root/src/protocol/

# common protocol

cd $buzz_robot_root
for file in `ls src/protocol/*.cpp`; do
  file_list="${file_list} ${file}"
done

output_makefile=$buzz_robot_root/mak/libbuzzprotocol.mak
printf "TARGET= build/libbuzzprotocol\nSRCS = \\\\\n" >$output_makefile

for file in $file_list; do
  printf "$file \\\\\n" >>$output_makefile
done

printf "\

CPP_FLAG = -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H
LINK_TYPE = static
INCLUDE = -Isrc
BUILD_DIR = build

include mak/main.mak

" >>$output_makefile

exit 0

