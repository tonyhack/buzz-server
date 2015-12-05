import ccvariable
import ccvariable.ttypes

import variable_table

def log_info(log):
  log_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.LOG_PROXY)
  log_proxy.log_info("[Python] %s" % log)

def log_error(log):
  log_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.LOG_PROXY)
  log_proxy.log_info("[Python] %s" % log)

def log_debug(log):
  log_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.LOG_PROXY)
  log_proxy.log_info("[Python] %s" % log)

