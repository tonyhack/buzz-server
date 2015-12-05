//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-05 13:37:07.
// File name: http_response.cc
//
// Description:
// Define class HttpResponse.
//

#include "login_server_informal/server/http_response.h"

namespace login {

namespace server {

const char *HttpResponse::krn_str = "\r\n";
const size_t HttpResponse::krn_size = 2;
const char *HttpResponse::kclose_connection_str = "Connection: close\r\n";
const size_t HttpResponse::kclose_connection_size = 19;
const char *HttpResponse::kkeep_alive_str = "Connection: Keep-Alive\r\n";
const size_t HttpResponse::kkeep_alive_size = 24;

}  // namespace server

}  // namespace login

