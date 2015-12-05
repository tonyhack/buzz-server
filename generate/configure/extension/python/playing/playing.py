import ccevent
import ccevent.playing.ttypes

import ccvariable
import ccvariable.ttypes

import variable_table
import idol
import plot
import slime
import playing_types
import slaughter_house
import team
import coliseum
import unreal_soul
import guild

import proxy

from playing_event import *

from thrift import TSerialization

def initialize():
  if playing_types.Config.parse(proxy.Function.get_playing_configure_file()) == False:
    proxy.Logging.error("parse playing failed.")
    return False

  if plot.initialize() == False:
    proxy.Logging.error("initialize plot failed.")
    return False

  if idol.initialize() == False:
    proxy.Logging.error("initialize idol failed.")
    return False

  if slime.initialize() == False:
    proxy.Logging.error("initialize slime failed.")
    return False

  if team.initialize() == False:
    proxy.Logging.error("initialize team failed.")
    return False

  if coliseum.initialize() == False:
    proxy.Logging.error("initialize coliseum failed.");
    return False

  if slaughter_house.initialize() == False:
    proxy.Logging.error("initialize slaughter_house failed.")
    return False

  if unreal_soul.initialize() == False:
    proxy.Logging.error("initialize unreal_soul failed.")
    return False

  if guild.initialize() == False:
    proxy.Logging.error("initialize guild failed.")

  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_REQUEST_ADD_AWARD, 0,\
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_on_event_playing_request_add_award")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_REQUEST_AWARD, 0,\
      ccevent.ttypes.ChannelType.CHANNEL_ACTOR, "playing_on_event_playing_actor_request_award")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_REQUEST_UPDATE_RECORD, 0,\
      ccevent.ttypes.ChannelType.CHANNEL_ACTOR, "playing_on_event_playing_request_update_record")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_REQUEST_AUTO_FINISH, 0,\
      ccevent.ttypes.ChannelType.CHANNEL_ACTOR, "playing_on_event_playing_request_auto_finish")

  return True


def finalize():
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_REQUEST_ADD_AWARD, 0,\
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_on_event_playing_request_add_award")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_REQUEST_AWARD, 0,\
      ccevent.ttypes.ChannelType.CHANNEL_ACTOR, "playing_on_event_playing_actor_request_award")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_REQUEST_UPDATE_RECORD, 0,\
      ccevent.ttypes.ChannelType.CHANNEL_ACTOR, "playing_on_event_playing_request_update_record")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_REQUEST_AUTO_FINISH, 0,\
      ccevent.ttypes.ChannelType.CHANNEL_ACTOR, "playing_on_event_playing_request_auto_finish")

  plot.finalize()
  idol.finalize()
  slime.finalize()
  team.finalize()
  coliseum.finalize()
  slaughter_house.finalize()
  unreal_soul.finalize()
  guild.finalize()

