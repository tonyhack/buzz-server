#!/bin/sh

if [ $# -ne 1 ]; then
  echo "usage: `basename $0` <count>"
  exit 1
fi

script_path=`readlink -f $0`
script_dir=`dirname $script_path`

buzz_robot_root=`readlink -f $script_dir/../`

output_makefile=$buzz_robot_root/etc/actor.xml
prefix=robot
robot_count=$1

printf "\
<?xml version=\"1.0\" encoding=\"utf-8\"?>
<config>
" >$output_makefile

for i in `seq 1 $robot_count`; do
  printf "<actor no=\"$i\" open_id=\"$prefix$i\" zone_id=\"1\"/>\n" >>$output_makefile
done

printf "\
</config>

" >>$output_makefile

exit 0

