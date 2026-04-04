/*
 * Parallel BFS using MPI Collectives
 *
 * Strategy (simple and correct):
 *   - Every process builds the same graph (same seed).
 *   - visited[] and level[] arrays are kept in sync across ALL processes
 *     using MPI_Allreduce after every BFS level.
 *   - Each process only EXPANDS nodes it owns (its partition).
 *   - But it can DISCOVER any node (no ownership restriction on neighbours).
 *   - After Allreduce, every process has the full updated visited[]/level[].
 *   - Next frontier = newly discovered nodes owned by this process.
 *
 * Usage: mpirun -np <P> ./parallel -n <nodes> -d <avg_degree>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

/* ---------- graph ---------- */
typedef struct {
    int  n;
    int  m;
    int *row_ptr;
    int *col_idx;
} Graph;

Graph *build_graph(int n, int deg) {
    Graph *g   = malloc(sizeof(Graph));
    g->n       = n;
    g->m       = n * deg;
    g->row_ptr = malloc((n + 1) * sizeof(int));
    g->col_idx = malloc(g->m     * sizeof(int));

    srand(42);
    for (int i = 0; i <= n; i++) g->row_ptr[i] = i * deg;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < deg; j++)
            g->col_idx[i * deg + j] = rand() % n;

    return g;
}

void free_graph(Graph *g) {
    free(g->row_ptr);
    free(g->col_idx);
    free(g);
}

/* ---------- main ---------- */
int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* --- parse arguments --- */
    int n   = 10000;
    int deg = 10;
    int src = 0;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-n") && i + 1 < argc) n   = atoi(argv[++i]);
        if (!strcmp(argv[i], "-d") && i + 1 < argc) deg = atoi(argv[++i]);
        if (!strcmp(argv[i], "-s") && i + 1 < argc) src = atoi(argv[++i]);
    }

    /* --- build graph (identical on every process) --- */
    Graph *g = build_graph(n, deg);

    /* --- partition: process p owns nodes [start, end) --- */
    int chunk = n / size;
    int start = rank * chunk;
    int end   = (rank == size - 1) ? n : start + chunk;

    /* --- shared BFS state (all processes keep full arrays) --- */
    int *level       = malloc(n * sizeof(int));
    int *visited     = calloc(n, sizeof(int));
    int *local_vis   = calloc(n, sizeof(int));
    int *local_level = malloc(n * sizeof(int));

    memset(level,       -1, n * sizeof(int));
    memset(local_level, -1, n * sizeof(int));

    int *frontier = malloc(n * sizeof(int));
    int  f_size   = 0;

    double t_start = MPI_Wtime();

    /* --- initialise source --- */
    visited[src]     = 1;
    local_vis[src]   = 1;
    level[src]       = 0;
    local_level[src] = 0;

    if (src >= start && src < end)
        frontier[f_size++] = src;

    int current_level = 0;

    /* --- BFS level-by-level --- */
    while (1) {
        /* 1. Expand local frontier — discover ANY neighbour */
        for (int i = 0; i < f_size; i++) {
            int u = frontier[i];
            for (int e = g->row_ptr[u]; e < g->row_ptr[u + 1]; e++) {
                int v = g->col_idx[e];
                if (!visited[v] && !local_vis[v]) {
                    local_vis[v]   = 1;
                    local_level[v] = current_level + 1;
                }
            }
        }

        /* 2. Sync visited[] and level[] across all processes */
        MPI_Allreduce(local_vis,   visited, n, MPI_INT, MPI_LOR, MPI_COMM_WORLD);
        MPI_Allreduce(local_level, level,   n, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

        memcpy(local_vis,   visited, n * sizeof(int));
        memcpy(local_level, level,   n * sizeof(int));

        /* 3. Next frontier = newly discovered nodes this process owns */
        f_size = 0;
        for (int v = start; v < end; v++)
            if (visited[v] && level[v] == current_level + 1)
                frontier[f_size++] = v;

        /* 4. Stop if no process has work left */
        int local_work = f_size, global_work = 0;
        MPI_Allreduce(&local_work, &global_work, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        if (global_work == 0) break;

        current_level++;
    }

    double t_end = MPI_Wtime();

    /* --- rank 0 prints results --- */
    if (rank == 0) {
        int reachable = 0, max_level = 0;
        for (int i = 0; i < n; i++) {
            if (level[i] >= 0) {
                reachable++;
                if (level[i] > max_level) max_level = level[i];
            }
        }
        printf("=== Parallel BFS ===\n");
        printf("Nodes: %d | Avg degree: %d | Source: %d | Processes: %d\n\n",
               n, deg, src, size);
        printf("Reachable nodes : %d / %d\n", reachable, n);
        printf("Max BFS depth   : %d\n",       max_level);
        printf("Time            : %.6f sec\n", t_end - t_start);
    }

    free(level); free(visited);
    free(local_vis); free(local_level);
    free(frontier);
    free_graph(g);

    MPI_Finalize();
    return 0;
}