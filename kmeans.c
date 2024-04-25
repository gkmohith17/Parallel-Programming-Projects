#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define NUM_THREADS 4
#define MAX_ITER 100


void initialize_centroids(double *X, int n, int d, int k, double *centroids);
void assign_points_to_clusters(double *X, int n, int d, double *centroids, int *assignments, int k);
void update_centroids(double *X, int n, int d, double *centroids, int *assignments, int k);

int main() {
    //can be replaced with any dataset of our own
    int n = 1000; // Number of data points in our sample data
    int d = 2;   
    int k = 3;    // Number of clusters k-means
    double *X = (double *)malloc(n * d * sizeof(double)); // allocating memory
    double *centroids = (double *)malloc(k * d * sizeof(double));
    int *assignments = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n * d; i++) {
        X[i] = (double)rand() / RAND_MAX;  // Random values between 0 and 1
    }

    // Initialize centroids
    initialize_centroids(X, n, d, k, centroids);

    // K-means algorithm
    for (int iter = 0; iter < MAX_ITER; iter++) {
        assign_points_to_clusters(X, n, d, centroids, assignments, k);
        update_centroids(X, n, d, centroids, assignments, k);
    }

    printf("Final centroids:\n");  //printing the centroids
    for (int i = 0; i < k; i++) {
        printf("Cluster %d: ", i);
        for (int j = 0; j < d; j++) {
            printf("%.2f ", centroids[i * d + j]);
        }
        printf("\n");
    }

    // Free allocated memory
    free(X);
    free(centroids);
    free(assignments);

    return 0;
}

void initialize_centroids(double *X, int n, int d, int k, double *centroids) {
    int *selected_indices = (int *)malloc(k * sizeof(int)); //here k is randomly taken as the number of points
    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < k; i++) {
        selected_indices[i] = rand() % n;
    }

    #pragma omp parallel for num_threads(NUM_THREADS)  //copy the selected points as centriods to check
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < d; j++) {
            centroids[i * d + j] = X[selected_indices[i] * d + j];
        }
    }

    // Free allocated memory
    free(selected_indices);
}

void assign_points_to_clusters(double *X, int n, int d, double *centroids, int *assignments, int k) {
    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < n; i++) {
        double min_dist = INFINITY;
        int closest_centroid = -1;
        for (int j = 0; j < k; j++) {
            double dist = 0.0;
            for (int l = 0; l < d; l++) {
                dist += pow(X[i * d + l] - centroids[j * d + l], 2);
            }
            if (dist < min_dist) {
                min_dist = dist;
                closest_centroid = j;
            }
        }
        assignments[i] = closest_centroid;
    }
}

void update_centroids(double *X, int n, int d, double *centroids, int *assignments, int k) {

    double *cluster_sums = (double *)calloc(k * d, sizeof(double));
    int *cluster_counts = (int *)calloc(k, sizeof(int));

    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < n; i++) {
        int cluster = assignments[i];
        #pragma omp atomic
        cluster_counts[cluster]++;
        for (int j = 0; j < d; j++) {
            #pragma omp atomic
            cluster_sums[cluster * d + j] += X[i * d + j];
        }
    }

    #pragma omp parallel for num_threads(NUM_THREADS)  // Update centroids
    for (int i = 0; i < k; i++) {
        if (cluster_counts[i] > 0) {
            for (int j = 0; j < d; j++) {
                centroids[i * d + j] = cluster_sums[i * d + j] / cluster_counts[i];
            }
        }
    }

    // Free allocated memory
    free(cluster_sums);
    free(cluster_counts);
}

