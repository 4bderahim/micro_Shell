
#include "mic.h"

void exec(t_vars *vars,char **argv, char **env, int ii, int i, int *pr_fd, int pippe)
{
    int pipes[2];
    int next_cmd = 0;
    char **new_arg;
    int j = 0;
    printf("%s\n\n\n", argv[0]);
    if (argv[0][0] == 'c' && argv[0][1] == 'd')
        {
            if (!argv[1])
                write(2, "error: cd: bad arguments\n", 25);
            if (chdir(argv[1]) < 0)
                {
                    write(2, "error: cd: cannot change directory to ", 38);
                    write(2, argv[1], strlen(argv[1]));
                    write(2, "\n", 1);
                }
            else
                exit(0);
            exit(1);
        }
    pipe(pipes);
    vars->pids[i] = fork();
    if (vars->pids[i] == 0)
    {
        close(pipes[0]);
        if (vars->pids[1])
            dup2(*pr_fd, STDIN_FILENO);
        if (pippe)
            dup2(pipes[1], STDOUT_FILENO);
        if (execve(*argv, argv, env) == -1)
            write(2, "fatal", 5);
        exit(1);
    }
    close(pipes[1]);
    if (i != 0)
        close(*pr_fd);
    *pr_fd = dup(pipes[0]);
    close(pipes[0]);
}
void wait_em(t_vars *vars, int t)
{
    int i;
    i = 0;
    int *st;
    while (i < t)
    {
        waitpid(vars->pids[i], st, 0);
        i++;
    }
}
int main(int argc, char **argv, char **env)
{
    argv += 1 ;
    pid_t p;
    int t = 0;
    int i;
    int pippe = 1;
    
    t_vars *vars;
    int pr_fd;
    pr_fd = 0;
    vars = (t_vars *) malloc(sizeof(t_vars));
    if (!vars)
        return (0);
    i = 0;
    while (argv[i])
    {
        if (argv[i][0] == '|' || argv[i][0] == ';' || !argv[i+1])
            {
                if (!argv[i+1])
                    {
                        i++;
                        pippe = 0;
                    }
                if (argv[i] && argv[i][0] == ';')
                    pippe = 0;
                char *r = argv[i];
                argv[i] = NULL;
                exec(vars, argv, env, i, t, &pr_fd, pippe);
                if (!r)
                    break;
                t++;
                if (r && *r == ';')
                    wait_em(vars, t);
                argv += i+1;
                i = -1;
            }
        i++;
    }
    free(vars);
    close(pr_fd);
    wait_em(vars, t);
    return (0);
}
