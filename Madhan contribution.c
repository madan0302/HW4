void list_status(struct job *jobs, int n)
{
    printf("jobid\tcommand\t\tstatus\n");
    for (int i = 0; i < n;)
    {
        if (strcmp(jobs[i].stat, "Success") != 0)
        {
            printf("%d\t %s\t %s\n", jobs[i].jid, jobs[i].cmd, jobs[i].stat);
        }
        i++;
    }
}


int CheckSpace(char c)
{
    return !(c != ' ' && c != '\t');
}


char *RemoveSpace(char *s)
{
    while (CheckSpace(*s))
    {
        s++;
    }
    return s;
}


int CountCharacters(char *s, int n)
{
    int c, i = 0;
    while (--n > 0 && (c = getchar()) != EOF && c != '\n')
    {
        s[i++] = c;
    }
    if (c == '\n')
    {
        s[i++] = c;
    }
    s[i] = '\0';
    return i;
}

char *CreateCopy(char *s)
{
    char *copy = malloc(strlen(s) + 1);
    strcpy(copy, s);
    copy[strlen(s)] = '\0';
    return copy;
}

char *DateTime()
{
    time_t tim = time(NULL);
    return CopyLine(ctime(&tim));
}


char *CopyLine(char *s)
{
    char *copy = malloc(strlen(s) + 1);
    int i = 0;
    while (s[i] != '\n')
    {
        copy[i] = s[i];
        i++;
    }
    copy[i] = '\0';
    return copy;
}

int CreateFile(char *fn)
{
    int fd = open(fn, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        fprintf(stderr, "Error: failed to open \"%s\"\n", fn);
        perror("open");
        return -1;
    }
    return fd;
}


void *ProcessJob(void *arg)
{
    struct job *jp;
    char **args;
    pid_t pid;

    jp = (struct job *)arg;

    jp->stat = "Working";
    jp->start = DateTime();

    ++CURRENT;
    pid = fork();
    if (pid == 0)
    {
        args = ParseArguments(jp->cmd);
        dup2(CreateFile(jp->fnout), STDOUT_FILENO);
        dup2(CreateFile(jp->fnerr), STDERR_FILENO);
        execvp(args[0], args);
        return 0;
    }
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, WUNTRACED);
        jp->stop = DateTime();
        jp->stat = "Success";
    }

    --CURRENT;
    return 0;
}







