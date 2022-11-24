struct job InitJob(char *cmd, int jid)
{
    struct job j;
    j.jid = jid;
    j.cmd = CreateCopy(cmd);
    j.stat = "waiting";
    j.start = j.stop = NULL;
    sprintf(j.fnout, "%d.%s", j.jid, out);
    sprintf(j.fnerr, "%d.%s", j.jid, err);
    return j;
}

void list_all_status(struct job *jobs, int n)
{
    printf("Job ID\tCommand\t\tstarttime\tendtime\tstatus\n");
    for (int i = 0; i < n;)
    {
        if (!strcmp(jobs[i].stat, "Success"))
        {
            printf("%d\t %s\t %s\t %s\t %s\n", jobs[i].jid, jobs[i].cmd, jobs[i].start, jobs[i].stop, "Success");
        }
        i++;
    }


void JobsList(struct job *jobs, int n, char *mode)
{
    int i;
    if (jobs != NULL && n != 0)
    {
        if (!strcmp(mode, "submithistory"))
        {
            list_all_status(jobs, n);
            return;
        }
        if (!strcmp(mode, "showjobs"))
        {
            list_status(jobs, n);
            return;
        }
    }

int Insert(struct queue *q, struct job *jp)
{
    if ((q == NULL) || (q->count == q->size))
        return -1;

    q->buffer[q->end % q->size] = jp;
    q->end = (q->end + 1) % q->size;

    return ++q->count;
}

struct queue *InitQueue(int n)
{
    struct queue *q = malloc(sizeof(struct queue));
    q->size = n;
    q->buffer = malloc(sizeof(struct job *) * n);
    q->start = 0;
    q->end = 0;
    q->count = 0;

    return q;
}


struct job *FreeQueue(struct queue *q)
{
    if ((q == NULL) || (q->count == 0))
        return NULL;

    struct job *j = q->buffer[q->start];
    q->start = (q->start + 1) % q->size;
    q->count--;

    return j;
}


void FreeEntireQueue(struct queue *q)
{
    for (int i = 0; i < q->count; i++)
    {
        free(q->buffer[i]);
    }
    free(q);
}
int CheckSpace(char c)
{
    return !(c != ' ' && c != '\t');
}


char **ParseArguments(char *line)
{
    char *copy = malloc((strlen(line) + 1));
    strcpy(copy, line);

    char *arg;
    char **args = malloc(sizeof(char *));
    int i = 0;
    while ((arg = strtok(copy, " \t")) != NULL)
    {
        args[i] = malloc((strlen(arg) + 1));
        strcpy(args[i], arg);
        args = realloc(args, sizeof(char *) * (++i + 1));
        copy = NULL;
    }
    args[i] = NULL;
    return args;
}

int main(int argc, char **argv)
{
    char *fnerr;
    pthread_t tid;

    if (argc != 2)
    {
        printf("Usage: %s Queue size\n", argv[0]);
        return 0;
    }

    ARGUMENT = atoi(argv[1]);

    CURRENT_JOBS = InitQueue(1000);

    pthread_create(&tid, NULL, ProcessAllJobs, NULL);

    ReadInput();

    return 0;
}

void ReadInput()
{
    int i;
    char line[1000];
    char *kw;
    char *cmd;

    i = 0;
    while (printf("Enter Command> ") && CountCharacters(line, 1000) != -1)
    {
        if ((kw = strtok(CreateCopy(line), " \t\n")) != NULL)
        {
            if (strcmp(kw, "submit") == 0)
            {
                cmd = RemoveSpace(strstr(line, "submit") + 6);
                cmd[strlen(cmd) - 1] = '\0';
                RUNNING_JOBS[i] = InitJob(cmd, i);
                Insert(CURRENT_JOBS, RUNNING_JOBS + i);
                printf("Added struct job %d to the struct job struct queue\n", i++);
            }
            else if (strcmp(kw, "showjobs") == 0 || strcmp(kw, "submithistory") == 0)
                JobsList(RUNNING_JOBS, i, kw);
            else if (strcmp(kw, "exit") == 0)
                exit(0);
        }
    }
    kill(0, SIGINT);
}

void *ProcessAllJobs(void *arg)
{
    struct job *jp;

    CURRENT = 0;
    for (int i = 0; i <= CURRENT;)
    {
        if (CURRENT_JOBS->count > 0 && CURRENT < ARGUMENT)
        {
            jp = FreeQueue(CURRENT_JOBS);

            pthread_create(&jp->tid, NULL, ProcessJob, jp);

            pthread_detach(jp->tid);
        }
        sleep(1);
    }
    return NULL;
}




