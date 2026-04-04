/*
 * Sequential BFS
 * Graph: randomly generated (adjacency list)
 * Usage: ./sequential -n <nodes> -d <avg_degree>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ---------- graph ---------- */
typedef struct {
    int  n;        /* number of nodes  */
    int  m;        /* number of edges  */
    int *row_ptr;  /* CSR row pointers */
    int *col_idx;  /* CSR column indices (neighbours) */
} Graph;

Graph *build_graph(int n, int deg) {
    Graph *g  = malloc(sizeof(Graph));
    g->n      = n;
    g->m      = n * deg;
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

/* ---------- BFS ---------- */
int *bfs(Graph *g, int src, double *elapsed) {
    int  n       = g->n;
    int *level   = malloc(n * sizeof(int));
    int *visited = calloc(n, sizeof(int));
    int *queue   = malloc(n * sizeof(int));

    memset(level, -1, n * sizeof(int));
    level[src] = visited[src] = 1;
    queue[0]   = src;
    int head = 0, tail = 1;

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    while (head < tail) {
        int u = queue[head++];
        for (int e = g->row_ptr[u]; e < g->row_ptr[u + 1]; e++) {
            int v = g->col_idx[e];
            if (!visited[v]) {
                visited[v]    = 1;
                level[v]      = level[u] + 1;
                queue[tail++] = v;
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);
    *elapsed = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9;

    free(visited);
    free(queue);
    return level;
}

/* ---------- main ---------- */
int main(int argc, char **argv) {
    int n   = 10000;   /* nodes        */
    int deg = 10;      /* avg degree   */
    int src = 0;       /* source node  */

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-n") && i + 1 < argc) n   = atoi(argv[++i]);
        if (!strcmp(argv[i], "-d") && i + 1 < argc) deg = atoi(argv[++i]);
        if (!strcmp(argv[i], "-s") && i + 1 < argc) src = atoi(argv[++i]);
    }

    printf("=== Sequential BFS ===\n");
    printf("Nodes: %d | Avg degree: %d | Source: %d\n\n", n, deg, src);

    Graph *g = build_graph(n, deg);

    double elapsed;
    int   *level = bfs(g, src, &elapsed);

    int reachable = 0, max_level = 0;
    for (int i = 0; i < n; i++) {
        if (level[i] >= 0) { reachable++; if (level[i] > max_level) max_level = level[i]; }
    }

    printf("Reachable nodes : %d / %d\n", reachable, n);
    printf("Max BFS depth   : %d\n",       max_level);
    printf("Time            : %.6f sec\n", elapsed);

    free(level);
    free_graph(g);
    return 0;
}