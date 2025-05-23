/*
 *  TODO: 
 *  - handle empty clusters
 *  - use proper initalization (kmeans++)
 *  - use a real dataset
 *  - generalize for N-dimensional points?
 *
 * */

#include <math.h>
#include <stdio.h>
#include <string.h>

#define K 2
#define N 6

#define EPS 0.01f

typedef struct { double x, y; } point_2d;
typedef struct { double x, y; int count; } centroid;

static int labels[N];
static centroid centroids[K];
static centroid prev_centroids[K];
static point_2d data[N] = {
    {1.0,  1.0},
    {2.2,  2.1},
    {3.2,  3.3},
    {4.1,  4.2},

    {9.0,  9.0},
    {9.3,  8.7}
};

void init_centroids(centroid* centroids, int* labels) {
    centroids[0] = (centroid){ data[0].x, data[0].y, 0 }; // quick flexible hardcoding
    centroids[1] = (centroid){ data[1].x, data[1].y, 0 };
    for (int n = 0; n < N; n++)
        labels[n] = -1;
}

double calc_distance(centroid c, point_2d p) {
    return sqrt(
        pow((c.x-p.x), 2) + pow((c.y-p.y), 2)
    );
}

int centroids_moved(centroid prev[], centroid curr[]) {
    for (int k = 0; k < K; k++) {
        double dx = prev[k].x - curr[k].x;
        double dy = prev[k].y - curr[k].y;
        if (sqrt(dx*dx + dy*dy) > EPS) return 1;
    }
    return 0;
}

int main() {
    init_centroids(centroids, labels);
    int max_iters = 10; // til im sure it always terminates
    do {
        printf("---------------\n");
        memcpy(prev_centroids, centroids, sizeof(centroids));
        memset(centroids, 0, sizeof(centroids));

        for (int n = 0; n < N; n++) {
            double min_distance = INFINITY;
            double distance = -1;
            for (int k = 0; k < K; k++) {
                if ((distance = calc_distance(prev_centroids[k], data[n])) < min_distance) {
                    min_distance = distance;
                    labels[n] = k;
                }
            }
            centroids[labels[n]].count++;
            centroids[labels[n]].x += data[n].x;
            centroids[labels[n]].y += data[n].y;

            printf("n=%d, k=%d\n", n, labels[n]);
        }
        for (int k = 0; k < K; k++) {
            if (centroids[k].count > 0) {
                centroids[k].x /= centroids[k].count;
                centroids[k].y /= centroids[k].count;
            }
        }

        max_iters--;
        if (max_iters == 0) {
            printf("maximum iterations reached, shutting down...\n");
            return 0;
        }
    } while (centroids_moved(prev_centroids, centroids));
}
