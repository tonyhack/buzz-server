ps x | grep "pressure_client -configure" | awk '{print $1}' | xargs kill -TERM
ps x | grep "chat_server -configure" | awk '{print $1}' | xargs kill -TERM
