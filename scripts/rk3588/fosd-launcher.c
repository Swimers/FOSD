/* A static entry point for the relocatable RK3588 FOSD bundle. */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int path_in_bundle(char *out, size_t size, const char *dir,
                          const char *relative)
{
    int length = snprintf(out, size, "%s/%s", dir, relative);
    return length >= 0 && (size_t)length < size;
}

int main(int argc, char **argv)
{
    char dir[PATH_MAX];
    char loader[PATH_MAX];
    char program[PATH_MAX];
    char runtime_lib[PATH_MAX];
    char vtk_lib[PATH_MAX];
    char library_path[PATH_MAX * 2 + 32];
    char **loader_argv;
    char *slash;
    ssize_t length = readlink("/proc/self/exe", dir, sizeof(dir) - 1);

    if (length < 0 || (size_t)length >= sizeof(dir) - 1) {
        perror("FOSD: locate executable");
        return 127;
    }
    dir[length] = '\0';
    slash = strrchr(dir, '/');
    if (slash == NULL) {
        fputs("FOSD: invalid executable path\n", stderr);
        return 127;
    }
    *slash = '\0';

    if (!path_in_bundle(loader, sizeof(loader), dir,
                        "lib/runtime/ld-linux-aarch64.so.1") ||
        !path_in_bundle(program, sizeof(program), dir, "FOSD-Pro.bin") ||
        !path_in_bundle(runtime_lib, sizeof(runtime_lib), dir, "lib/runtime") ||
        !path_in_bundle(vtk_lib, sizeof(vtk_lib), dir, "lib/VTK") ||
        snprintf(library_path, sizeof(library_path), "%s:%s:/lib:/usr/lib",
                 runtime_lib, vtk_lib) >= (int)sizeof(library_path)) {
        fputs("FOSD: bundle path is too long\n", stderr);
        return 127;
    }
    if (access(loader, X_OK) != 0 || access(program, X_OK) != 0) {
        fprintf(stderr, "FOSD: missing bundled loader or program in %s\n", dir);
        return 127;
    }
    if (chdir(dir) != 0) {
        perror("FOSD: enter bundle directory");
        return 127;
    }
    if (setenv("FOSD_DATA_ROOT", dir, 1) != 0) {
        perror("FOSD: set data directory");
        return 127;
    }

    loader_argv = calloc((size_t)argc + 4, sizeof(*loader_argv));
    if (loader_argv == NULL) {
        perror("FOSD: allocate arguments");
        return 127;
    }
    loader_argv[0] = loader;
    loader_argv[1] = "--library-path";
    loader_argv[2] = library_path;
    loader_argv[3] = program;
    for (int i = 1; i < argc; ++i)
        loader_argv[i + 3] = argv[i];

    execv(loader, loader_argv);
    fprintf(stderr, "FOSD: execute bundled loader: %s\n", strerror(errno));
    free(loader_argv);
    return 127;
}
