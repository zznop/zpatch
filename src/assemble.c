#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <limits.h>
#include "assemble.h"

extern char **environ;

static bool _touch_source_file(void)
{
    bool rv = false;
    FILE *file;
    int header_len;
    int n;
    const char *header = (
        ".section .text\n"
        ".global _start\n"
        "\n"
        "_start:\n"
    );

    file = fopen(PATCH_SOURCE, "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to create patch source file\n");
        return false;
    }

    header_len = strlen(header);
    n = fwrite(header, 1, header_len, file);
    if (n != header_len) {
        fprintf(stderr, "Failed to write to patch source file\n");
        goto out;
    }
    rv = true;
out:
    fclose(file);
    return rv;
}

static bool _select_editor(char *path)
{
    if (access("/usr/bin/vim", F_OK) != -1)
        strcpy(path, "/usr/bin/vim");
    else if (access("/usr/bin/vi", F_OK) != -1)
        strcpy(path, "/usr/bin/vi");
    else if (access("/usr/bin/nano", F_OK) != -1)
        strcpy(path, "/usr/bin/nano");
    else
        return false;
    return true;
}

bool write_patch_assembly(void)
{
    bool rv;
    int status;
    pid_t pid;
    char path[PATH_MAX];
    char *argv[3] = {0};

    /* Select a text editor to spawn */
    rv = _select_editor(path);
    if (rv == false) {
        fprintf(stderr, "Failed to find a text editor\n");
        return false;
    }
    argv[0] = path;
    argv[1] = PATCH_SOURCE;

    /* Create template patch.S source file */
    if (_touch_source_file() == false)
        return false;

    /* Fork and open patch.S file in editor in child */
    pid = fork();
    if (pid == 0) {
        execve(argv[0], argv, environ);
        _exit(1);
    }

    /* Parent waits for child to finish */
    waitpid(pid, &status, 0);
    return true;
}

bool assemble_patch(void)
{
    pid_t pid;
    int status;
    char *argv[] = { AS, "-m68000", PATCH_SOURCE, "-o", PATCH_OBJECT, NULL };

    pid = fork();
    if (pid == 0) {
        execve(argv[0], argv, environ);
        _exit(1);
    }

    waitpid(pid, &status, 0);
    return WEXITSTATUS(status) == 0;
}

bool extract_bin(uint32_t offset)
{
    pid_t pid;
    int status;
    char str_ofs[5];
    char *argv[] = { LD, "-Ttext", NULL, "--oformat", "binary", PATCH_OBJECT, "-o", PATCH_BIN, NULL };;

    sprintf(str_ofs, "%u", offset);
    argv[2] = str_ofs;

    pid = fork();
    if (pid == 0) {
        execve(argv[0], argv, environ);
        _exit(1);
    }

    waitpid(pid, &status, 0);
    return WEXITSTATUS(status) == 0;
}

