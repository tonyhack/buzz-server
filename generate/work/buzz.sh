#!/bin/bash

script_name=`basename $0`

idx_schedule=0
idx_database=1
idx_session=2
idx_game=3
idx_gateway=4
idx_login=5

server_name_list[$idx_schedule]='schedule'
pidfile_list[$idx_schedule]='./schedule_server.pid'
bin_list[$idx_schedule]="../build/bin/schedule_server/schedule_server \
-conf ../configure/server.xml -pidfile ${pidfile_list[$idx_schedule]}"

server_name_list[$idx_database]='database'
pidfile_list[$idx_database]='./database_server.pid'
bin_list[$idx_database]="../build/bin/database_server/database_server \
-conf ../configure/server.xml -id 1 -pidfile ${pidfile_list[$idx_database]}"

server_name_list[$idx_session]='session'
pidfile_list[$idx_session]='./session_server.pid'
bin_list[$idx_session]="../build/bin/session_server/session_server \
-conf ../configure/server.xml -pidfile ${pidfile_list[$idx_session]}"

server_name_list[$idx_game]='game'
pidfile_list[$idx_game]='./game_server.pid'
bin_list[$idx_game]="../build/bin/game_server/game_server \
-conf ../configure/server.xml -id 1 -pidfile ${pidfile_list[$idx_game]}"

server_name_list[$idx_gateway]='gateway'
pidfile_list[$idx_gateway]="./gateway_server.pid"
bin_list[$idx_gateway]="../build/bin/gateway_server/gateway_server \
-conf ../configure/server.xml -id 1 -pidfile ${pidfile_list[$idx_gateway]}"

server_name_list[$idx_login]='login'
pidfile_list[$idx_login]="./login_server.pid"
bin_list[$idx_login]="../build/bin/login_server/login_server \
-conf ../configure/login.xml -pidfile ${pidfile_list[$idx_login]}"

usage() {
  printf "usage: $script_name [server_name] <operation>\n"
  printf "server_name: all|schedule|database|session|game|gateway|login\n"
  printf "operation: status|start|stop|restart\n"
  exit 1
}

show_error() {
  local message=$1
  printf "\033[;31m%s\033[0m\n" "$message"
}

show_success() {
  local message=$1
  printf "\033[;32m%s\033[0m\n" "$message"
}

# server status operation
op_status() {
  local server_name=$1
  local bin=$2
  local pidfile=$3

  pid=`cat $pidfile 2>/dev/null`
  if [ $? -ne 0 ]; then
    status='stopped'
  else
    kill -0 $pid 2>/dev/null
    if [ $? -eq 0 ]; then
      status='running'
    else
      status='stopped'
    fi
  fi
 
  if [ $status == "running" ]; then
    printf "%-9.9s %-8d" "$server_name" $pid
    show_success "$status"
  else
    printf "%-9.9s %-8s" "$server_name" ""
    show_error "$status"
  fi
}

# start server operation
op_start() {
  local server_name=$1
  local bin=$2
  local pidfile=$3
  local i

  printf "[starting %-8.8s server]" "$server_name"

  pid=`cat $pidfile 2>/dev/null`
  if [ $? -eq 0 ]; then
    kill -0 $pid 2>/dev/null
    if [ $? -eq 0 ]; then
      show_error '[already started]'
      return 1
    fi
  fi

  $bin 2>&1 >>err.log

  for (( i = 0; i < 50; ++i )); do
    pid=`cat $pidfile 2>/dev/null`
    if [ $? -eq 0 ]; then
      kill -0 $pid 2>/dev/null
      if [ $? -eq 0 ]; then
        show_success '[success]'
        return 0
      fi
    fi
    sleep 0.1
  done

  show_error '[failed]'
  return 1
}

# stop server operation
op_stop() {
  local server_name=$1
  local bin=$2
  local pidfile=$3
  local i

  printf "[stopping %-8.8s server]" "$server_name"

  pid=`cat $pidfile 2>/dev/null`
  if [ $? -ne 0 ]; then
    show_error '[not started]'
    return 1
  fi

  kill -0 $pid 2>/dev/null
  if [ $? -ne 0 ]; then
    show_error '[not started]'
    return 1
  fi

  kill $pid

  for (( i = 0; i < 500; ++i )); do
    kill -0 $pid 2>/dev/null
    if [ $? -ne 0 ]; then
      show_success '[success]'
      return 0
    fi
    sleep 0.1
  done

  show_error '[failed]'
  return 1
}

# run operation
run() {
  local server_name=$1
  local op=$2

  if [ $server_name == "all" ]; then
    for (( i = 0; i < ${#bin_list[@]}; ++i )); do
      $op "${server_name_list[$i]}" "${bin_list[$i]}" "${pidfile_list[$i]}"
    done
  elif [ $server_name == "all_reverse" ]; then
    for (( i = ${#bin_list[@]} - 1; i >= 0; --i )); do
      $op "${server_name_list[$i]}" "${bin_list[$i]}" "${pidfile_list[$i]}"
    done
  else
    var="\$idx_${server_name}"
    idx=`eval echo $var`
    if [ -z "$idx" ]; then
      echo "server_name is invalid"
      exit 1
    fi
    var="\${bin_list[\$idx_${server_name}]}"
    bin=`eval echo $var`
    var="\${pidfile_list[\$idx_${server_name}]}"
    pidfile=`eval echo $var`
    $op $server_name "$bin" "$pidfile"
  fi
}

run_reverse() {
  local server_name=$1
  local op=$2

  if [ $server_name == "all" ]; then
    run all_reverse $op
  else
    run $server_name $op
  fi
}

if [ $# -eq 1 ]; then
  server_name=all
  operation=$1
elif [ $# -eq 2 ]; then
  server_name=$1
  operation=$2
else
  usage
fi

case $operation in
  status)
    run $server_name "op_status"
    ;;
  start)
    run $server_name "op_start"
    ;;
  stop)
    run_reverse $server_name "op_stop"
    ;;
  restart)
    run_reverse $server_name "op_stop"
    run $server_name "op_start"
    ;;
  *)
    echo "operation is invalid"
    ;;
esac

exit 0
