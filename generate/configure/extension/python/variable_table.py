import sys
sys.path.append('ccvariable')

import __main__

import ccvariable
import ccvariable.ttypes

variables = {}

def initialize():
  variables[ccvariable.ttypes.Variable.ENTITY_MANAGER] = __main__.CC_ENTITY_MANAGER
  variables[ccvariable.ttypes.Variable.CURRENT_ACTOR] = __main__.CC_CURRENT_ACTOR
  variables[ccvariable.ttypes.Variable.CURRENT_NPC] = __main__.CC_CURRENT_NPC
  variables[ccvariable.ttypes.Variable.COMMUNICATOR_PROXY] = __main__.CC_COMMUNICATOR_PROXY
  variables[ccvariable.ttypes.Variable.REQUEST_PROXY] = __main__.CC_REQUEST_PROXY
  variables[ccvariable.ttypes.Variable.LOG_PROXY] = __main__.CC_LOG_PROXY
  variables[ccvariable.ttypes.Variable.TIMER_PROXY] = __main__.CC_TIMER_PROXY
  variables[ccvariable.ttypes.Variable.FUNCTION_PROXY] = __main__.CC_FUNCTION_PROXY

def finalize():
  return None

def get_variable(type):
  if type == ccvariable.ttypes.Variable.CURRENT_ACTOR:
    return __main__.CC_CURRENT_ACTOR
  elif type == ccvariable.ttypes.Variable.CURRENT_NPC:
    return __main__.CC_CURRENT_NPC
  else:
    return variables[type]

