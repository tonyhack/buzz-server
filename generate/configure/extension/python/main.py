import sys
sys.path.append('ccentity')
sys.path.append('ccevent')
sys.path.append('ccrequest')
sys.path.append('ccvariable')
sys.path.append('playing')

import __main__

import variable_table

import playing
import playing.playing
import proxy
import timer_test

from playing.playing_event import *
from playing.idol_event import *
from playing.plot_event import *
from playing.slime_event import *
from playing.team_event import *
from playing.coliseum_event import *
from playing.slaughter_house_event import *
from playing.unreal_soul_event import *
from playing.guild_event import *
from timer_handler import *

def initialize():
  variable_table.initialize()
  proxy.initialize()
  if playing.playing.initialize() == False:
    return False
  timer_test.initialize()
  return True

def finalize():
  variable_table.finalize()
  playing.playing.finalize()
  timer_test.finalize()

