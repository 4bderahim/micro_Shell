#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_vars
{
    pid_t pids[1000];
    int pr_fd;
} t_vars;