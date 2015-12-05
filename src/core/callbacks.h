//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:35:03.
// File name: callbacks.h
//
// Description:
// Define callbacks.
//

#ifndef __CORE__CALLBACK__H
#define __CORE__CALLBACK__H

#include <boost/function.hpp>

namespace core {

class ByteBufferDynamic;
class InetAddress;
class TCPConnection;
class TimeTick;

typedef ByteBufferDynamic Buffer;

typedef boost::shared_ptr<TCPConnection> TCPConnectionPtr;

typedef boost::function<void ()> SignalCallback;
typedef boost::function<void ()> EventLoopCallback;
typedef boost::function<void (int, const InetAddress &)> NewConnectionCallback;

typedef boost::function<void ()> CommonEventCallback;
typedef boost::function<void (const TimeTick &)> ReadEventCallback;

typedef boost::function<void (const TimeTick &)> EventLoopTimeCallback;

typedef boost::function<void (const TCPConnectionPtr &)> ConnectionCallback;
typedef boost::function<void (const TCPConnectionPtr &)> CloseCallback;
typedef boost::function<void (const TCPConnectionPtr &, Buffer &, const TimeTick &)> MessageCallback;
typedef boost::function<void (const TCPConnectionPtr &)> WriteCompleteCallback;

}  // namespace core

#endif  // __CORE__CALLBACK__H

