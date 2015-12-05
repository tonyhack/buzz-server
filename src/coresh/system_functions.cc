#include "coresh/system_functions.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

namespace coresh {

bool CreatePidFile(const char *file) {
  FILE *pid_file = fopen(file, "w");
  if (NULL == pid_file) {
    return false;
  }

  fprintf(pid_file, "%d\n", getpid());
  fclose(pid_file);

  return true;
}

} // namespace coresh

