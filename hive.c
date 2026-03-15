/*
 * hive.c
 *
 * The program:
 * - spawns L larvae (processes that allocate memory periodically)
 * - spawns Z husks (children that exit immediately and become zombies)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <errno.h>

/* Write message and exit on fatal error */
static void fatal(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/* Create directory if not present (ignore EEXIST) */
static void ensure_dir(const char *path) {
    if (mkdir(path, 0755) != 0 && errno != EEXIST) {
        fatal("mkdir");
    }
}

/* Write hive PID to /tmp/lab_<SRN>/hive.pid */
static void write_pidfile(const char *lab_dir, pid_t pid) {
    char buf[128];
    snprintf(buf, sizeof(buf), "%s/hive.pid", lab_dir);
    FILE *f = fopen(buf, "w");
    if (!f) {
        fprintf(stderr, "Warning: cannot write pid file %s: %s\n", buf, strerror(errno));
        return;
    }
    fprintf(f, "%d\n", (int)pid);
    fclose(f);
}

/* Larva process: allocate 1 MB repeatedly and log progress */
static void larva_process(int srn, const char *lab_dir) {
    pid_t mypid = getpid();
    int M = (srn % 20) + 50; /* per-larva memory limit in MB */

    struct rlimit rl;
    rl.rlim_cur = (rlim_t)M * 1024 * 1024;
    rl.rlim_max = (rlim_t)M * 1024 * 1024;
    (void)setrlimit(RLIMIT_AS, &rl); /* try to set limit; ignore failure */

    char logpath[256];
    snprintf(logpath, sizeof(logpath), "%s/larva_%d.log", lab_dir, (int)mypid);
    FILE *log = fopen(logpath, "w");
    if (!log) {
        while (1) pause();
    }

    fprintf(log, "[Larva %d] started (M=%d MB)\n", (int)mypid, M);
    fflush(log);

    int total_mb = 0;
    for (;;) {
        void *block = malloc(1024 * 1024); /* 1 MB */
        if (block == NULL) {
            fprintf(log, "[Larva %d] malloc failed at %d MB; going dormant\n", (int)mypid, total_mb + 1);
            fflush(log);
            fclose(log);
            while (1) pause(); /* stay dormant */
        }

        memset(block, 0xAB, 1024 * 1024); /* touch memory */
        total_mb++;
        fprintf(log, "[Larva %d] Allocated 1 MB (Total: %d MB)\n", (int)mypid, total_mb);
        fflush(log);
        sleep(2);
    }
}

/* Spawn L larvae */
static void spawn_larvae(int L, int srn, const char *lab_dir, pid_t *larva_pids) {
    for (int i = 0; i < L; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork larva");
            continue;
        }
        if (pid == 0) {
            larva_process(srn, lab_dir);
            _exit(EXIT_SUCCESS); /* unreachable */
        } else {
            larva_pids[i] = pid;
        }
    }
}

/* Spawn Z husks: children exit immediately (become zombies until reaped) */
static void spawn_husks(int Z) {
    for (int i = 0; i < Z; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork husk");
            continue;
        }
        if (pid == 0) {
            _exit(EXIT_SUCCESS);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <SRN>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int len = (int)strlen(argv[1]);
    int srn = (len >= 3) ? atoi(argv[1] + len - 3) : atoi(argv[1]);

    int L = (srn % 5) + 3;
    int Z = (srn % 5) + 4;
    int M = (srn % 20) + 50;

    pid_t hive_pid = getpid();

    printf("[Hive %d] Starting. SRN=%d, L=%d, Z=%d, M=%d\n", (int)hive_pid, srn, L, Z, M);
    fflush(stdout);

    char *home = getenv("HOME");
    printf("[Hive %d] Home Directory: %s\n", (int)hive_pid, home ? home : "Unknown");
    
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("[Hive %d] Current Directory: %s\n", (int)hive_pid, cwd);
    } else {
        printf("[Hive %d] Current Directory: Unknown\n", (int)hive_pid);
    }
    fflush(stdout);

    char lab_dir[64];
    snprintf(lab_dir, sizeof(lab_dir), "/tmp/lab_%d", srn);
    ensure_dir(lab_dir);

    pid_t *larva_pids = (pid_t *)calloc((size_t)L, sizeof(pid_t));
    if (!larva_pids) {
        fatal("calloc");
    }

    spawn_larvae(L, srn, lab_dir, larva_pids);
    spawn_husks(Z);

    printf("[Hive %d] Spawned %d larvae and %d husks.\n", (int)hive_pid, L, Z);
    fflush(stdout);

    write_pidfile(lab_dir, hive_pid);

    printf("[Hive %d] Control commands: type 'reap' to reap zombies, 'quit' to shutdown.\n", (int)hive_pid);
    fflush(stdout);

    char cmd[64];
    for (;;) {
        printf("[Hive %d] > ", (int)hive_pid);
        fflush(stdout);

        if (!fgets(cmd, sizeof(cmd), stdin)) {
            /* If stdin is closed, stay alive so the process can still be observed. */
            sleep(1);
            continue;
        }

        cmd[strcspn(cmd, "\r\n")] = '\0';

        if (strcmp(cmd, "reap") == 0) {
            int reaped = 0;
            printf("[Hive %d] Command received: reap (reaping zombies)...\n", (int)hive_pid);
            fflush(stdout);
            while (waitpid(-1, NULL, WNOHANG) > 0) {
                reaped++;
            }
            printf("[Hive %d] Reaped %d zombie(s).\n", (int)hive_pid, reaped);
            fflush(stdout);
        } else if (strcmp(cmd, "quit") == 0) {
            break;
        } else if (cmd[0] == '\0') {
            continue;
        } else {
            printf("[Hive %d] Unknown command '%s'. Try: reap | quit\n", (int)hive_pid, cmd);
            fflush(stdout);
        }
    }

    /* Shutdown */
    printf("[Hive %d] Command received: quit (shutting down)...\n", (int)hive_pid);
    fflush(stdout);

    /* Terminate larvae */
    for (int i = 0; i < L; ++i) {
        if (larva_pids[i] > 0) {
            (void)kill(larva_pids[i], SIGTERM);
        }
    }

    /* Reap any remaining children */
    while (waitpid(-1, NULL, WNOHANG) > 0) { }

    free(larva_pids);

    printf("[Hive %d] Shutdown complete.\n", (int)hive_pid);
    fflush(stdout);

    return EXIT_SUCCESS;
}
