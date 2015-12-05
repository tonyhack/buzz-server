import sys
sys.path.append('ccentity')
sys.path.append('ccevent')
sys.path.append('ccrequest')
sys.path.append('ccvariable')

import __main__

import ccevent
import ccevent.ttypes
import ccevent.actor.ttypes

import ccvariable
import ccvariable.ttypes

import variable_table

from thrift import TSerialization

def test_initialize():
  variable_table.initialize();
  #__main__.CC_COMMUNICATOR_PROXY.follow(ccevent.ttypes.EventType.EVENT_ACTOR_LOGIN, 0, ccevent.ttypes.ChannelType.CHANNEL_ACTOR, "test_on_actor_login")
  communicator_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.COMMUNICATOR_PROXY)
  communicator_proxy.follow(ccevent.ttypes.EventType.EVENT_ACTOR_LOGIN, 0, ccevent.ttypes.ChannelType.CHANNEL_ACTOR, "test_on_actor_login")

def test_on_actor_login(message_type, channel, channel_type, serialize):
  message = ccevent.actor.ttypes.EventActorLogin()
  TSerialization.deserialize(message, serialize)
  print "actor(%d) login" % (message.id_)

#def test_initialize():
#  print "test_initialize"

