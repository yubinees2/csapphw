/* 2015142069 안유빈*/

/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);
/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	/* s=5, E=1, b=5 -> direct mapped cache with 32set, 32bytes/block=8int/block */

	int block_size;

	int row, col, tmp, i, j, idx;
	if (N==32){
		block_size = 8;
		for (col=0; col<M; col+=block_size){
			for (row=0; row<N; row+=block_size){
				for (i=row; i<row+block_size; i++){
					for (j=col; j<col+block_size; j++){
						if (i!=j) B[j][i] = A[i][j];
						else{
							tmp = A[i][j];
							idx = i;
						}
					}
					if (row==col) B[idx][idx] = tmp;
				}
			}
		}
	}
	else if (N==64){ // 64*64 case
		block_size = 4;
		for (col=0; col<M; col+=block_size){
			for (row=0; row<N; row+=block_size){
				for(i=row; i<row+block_size; i++){
					for(j=col; j<col+block_size; j++){
						if (i!=j){
						       B[j][i]=A[i][j];
						       
						}
						else{
							tmp = A[i][j];
							idx = i;
						}
					}
					if (row==col){
						B[idx][idx] = tmp;
					}
				}
			}
		}
			
	}
	
	else {	//61*67 CASE
		block_size = 8;		
		for (col=0; col<M; col+=block_size){
			for (row=0; row<N; row+=block_size){
				for(i=row; i<row+block_size && i<N; i++){
					for(j=col; j<col+block_size && j<M; j++){
						B[j][i]=A[i][j];
					}
				}
			}
		}

	}
}




/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
//    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

