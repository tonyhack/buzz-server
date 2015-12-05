#!/bin/sh

script_path=`readlink -f $0`
script_dir=`dirname $script_path`

buzz_root=`readlink -f $script_dir/../../`
buzz_robot_root=`readlink -f $script_dir/../`

cp $buzz_root/generate/configure/misc.xml \
   $buzz_robot_root/etc/

rm -f $buzz_robot_root/etc/extension/scenes/*.xml

cp $buzz_root/generate/configure/extension/scenes/*.xml \
   $buzz_robot_root/etc/extension/scenes/

cp $buzz_root/generate/configure/extension/skills/skill.xml \
   $buzz_robot_root/etc/extension/skills/

exit 0

