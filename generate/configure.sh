#!/bin/sh

usage() {
  echo "usage: $0 <server_ip> [mysql_ip] <offset>"
  echo "example: $0 192.168.1.128 0"
  echo "         $0 192.168.1.128 192.168.1.129 0"
  echo
  echo "server_ip: 服务器ip"
  echo "mysql_ip:  数据库ip(不填默认为服务器ip)"
  echo "offset:    端口(数据库)偏移值"
  exit 1
}

if [ $# -eq 2 ] ; then
  ip=$1
  mysql_ip=$1
  offset=$2
elif [ $# -eq 3 ]; then
  ip=$1
  mysql_ip=$2
  offset=$3
else
  usage
fi

dir=`pwd $0`

substitute_var() {
awk \
-v dir=$dir \
-v ip=$ip \
-v mysql_ip=$mysql_ip \
-v offset=$offset '
/\[server_host\]/ {
  gsub(/\[server_host\]/, ip);
}
/\[mysql_host\]/ {
  gsub(/\[mysql_host\]/, mysql_ip);
}
/\[generate_directory\]/ {
  gsub(/\[generate_directory\]/, dir);
}
/.*<(host|admin_host|gateway).*port="[0-9]+"/ {
  port = gensub(/.*port="([0-9]+)".*/, "\\1", "");
  port += offset;
  gsub(/"[0-9]+"/, "\"" port "\"");
}
/db="buzz-/ {
  if (offset != 0) {
    gsub(/buzz-game/, "buzz-game-" offset);
    gsub(/buzz-center/, "buzz-center-" offset);
  }
}
{
  print $0;
}
' $1 >$2
}

substitute_var ./configure/server_example.xml ./configure/server.xml
substitute_var ./configure/login_example.xml ./configure/login.xml

echo "configure server.xml login.xml complete!!"

exit 0
