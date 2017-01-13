// 10.1.2017, Tai<taitrananhvn@gmail.com>

// Implement N-Queen problem using backtracking algorithm
// http://www.geeksforgeeks.org/backtracking-set-3-n-queen-problem/

#include <cstdio>
#include <cstring>
using namespace std;

#define N 20
// Kiem tra thoa man contraint khi dat hau tai
// hang row, cot col
// rang buoc duoc kiem tra voi cac con hau 
// da duoc dat tu cot 1->col-1
// 3 rang buoc
// 	1. tu cot 0 toi col-1, khong co con hau nau tren
// 		hang row
//  2. khong co con hau nao thuoc cheo tren, ben trai [row,col]
//  3. khong co con hau nao thuoc cheo duoi, ben trai [row,col]

bool satisfyContraint(int arr[N][N], int row, int col) {
	// dieu kien 1
	for (int i = 0; i<col; i++) 
		if (arr[row][i]) return false;

	// dieu kien 2
	for (int i=row, j=col; i>=0 && j>=0; i--,j--) 
		if (arr[i][j]) return false;

	// dieu kien 3
	for (int i=row, j=col; i<N && j>=0; i++,j--)
		if (arr[i][j]) return false;

	return true;
}

// Dat con hau tai cot thu col, gia su cac con hau 
// da duoc dat hoan thanh tu cot 1->col-1
bool solveNQueen(int arr[N][N], int col) {
	// duyet tu hang 1 toi hang N, de kiem tra vi tri 
	// dat hau. Tai hang i, kiem tra xem vi tri hau
	// co vi pham contraints voi cac hau da dat 
	// trong cac cot 1->col-1
	// Neu khong vi pham:
	// 1. dat hau tai vtri i cot col va de 
	//    qui tim vi tri dat hau tai cot tiep col+1
	// 2. quay lui. khoi phuc trang thai tai i ve trang
	//	  thai khoi tao, kiem tra hang hang tiep theo i+1
	// Neu vi pham, kiem tra hang tiep theo i+1

	if (col >= N) return true;

	for (int i=0; i<N; i++) {
		if (satisfyContraint(arr,i,col)) {
			arr[i][col] = 1;
			if (solveNQueen(arr,col+1)) return true;
			arr[i][col] = 0; //quay lui
		}
	}

	return false;
}

int main() {
	
	//freopen("input.txt","r",stdin);
	//scanf("%d",&N); //board with N queen

	//int arr[N][N];

	//memset(arr,sizeof(arr),0);
	int arr[N][N] = { {0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};

	if (solveNQueen(arr,0)) {
		for (int i =0; i<N; i++) {
			for (int j =0; j<N; j++) {
				if (arr[i][j]) printf("1 ");
				else printf("0 ");
			}
			printf("\n");
		}
	} else printf("No solution\n");


	return 0;
}