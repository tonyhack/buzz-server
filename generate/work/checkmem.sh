#!/bin/sh
valgrind --tool=memcheck --leak-check=yes --log-file=./memcheck/schedule_server.memcheck ../build/bin/schedule_server/schedule_server -conf ../configure/server.xml -daemon false &

valgrind --tool=memcheck --leak-check=yes --log-file=./memcheck/database_server.memcheck ../build/bin/database_server/database_server -conf ../configure/server.xml -daemon false -id 1 &

valgrind --tool=memcheck --leak-check=yes --log-file=./memcheck/session_server.memcheck ../build/bin/session_server/session_server -conf ../configure/server.xml -daemon false &

valgrind --tool=memcheck --leak-check=yes --log-file=./memcheck/game_server.memcheck ../build/bin/game_server/game_server -conf ../configure/server.xml -id 1 -daemon false &

valgrind --tool=memcheck --leak-check=yes --log-file=./memcheck/gateway_server.memcheck ../build/bin/gateway_server/gateway_server -conf ../configure/server.xml  -id 1 -daemon false &


