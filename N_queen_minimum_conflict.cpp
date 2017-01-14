// 12.1.2017, Tai<taitrananhvn@gmail.com>
// N-Queen problem using minimum confict algorithm

// Ky hieu
// row[i] la vi tri con hau tai hang i, tuc cot q[i]
// can tim vi tri cac con hau(i,row[i]), i=1->n 

// Conflict duoc tinh nhu sau:

// Gia su 1 con hau tai hang i, cot j
// neu co 1 con hau khac tren hang i=> confict doi voi i = +1
// => neu co n con hau khac tren hang i => anh huong conflict
// doi voi i la n
// Tong quat, gia su hang i co tat ca n con hau
// khi do tong xung dot tren hang i la n*(n-1)/2 
// (chia 2 vi neu i xung dot j thi j cung xung dot i)
// Tuong tu, tai 1 duong cheo tren bat ky (i+row[i]=const) co k quan hau
// thi tong xung dot tren do la k*(k-1)/2
// tren tai cheo duoi bat ky (i+N-row[i]=const) co l con hau,
// thi tong xung dot tren duong cheo do la l*(l-1)/2

// Thuat toan
// 1. Khoi tao n con hau 
// con hau hang 0 chon ngau nhien
// con hau hang i chon thoa man conflict nho nhat
// 2. Tai moi buoc lap (Lap den khi nao conflict tong = 0 hoac vuot qua gioi han cho phep)
// chon hang k co tong conflict tren no lon nhat trong so n hang
// Tren hang nay, chon vi tri moi cua con hau de dat duoc min-confict trong tat ca lua chon 
// (muc dich la tim duoc 1 assigment de minimize conflict)
// Tinh lai confict tong
// (neu qua 2 buoc lap ma tong conflict khong doi, thi doi ngau nhien 1 vi tri hau
// tren 1 hang bat ky)

#include <cstdio>
#include <limits>
#include <cstdlib> //rand,srand
#include <ctime>
#include <vector>
#include <cstring>

using namespace std;
#define N 20
#define INF 10000

int row[N];
int col[N];
int lowerDiag[2*N-1];
int upperDiag[2*N-1];
int count = 0;

// Find a queen position on given row to get smallest confict
int selectQueen(int row) {
	vector<int> conflictIndex;
	int minConflict = INF;
	for (int j=0; j<N; j++) {
		// new position is queen(row,j)
		int tempConflict = 0;
		tempConflict = (col[j]*(col[j]+1))/2;
		tempConflict += (upperDiag[row+j]*(upperDiag[row+j]+1))/2;
		tempConflict += (lowerDiag[row+N-1-j]*(lowerDiag[row+N-1-j]+1))/2;

		if (tempConflict < minConflict) {
			minConflict = tempConflict;
			conflictIndex.clear();
			conflictIndex.push_back(j);
			
		} else if (tempConflict == minConflict) {
			conflictIndex.push_back(j);
		}
	}

	return conflictIndex[rand() % conflictIndex.size()];
}

// Find row which has smallest conflict
int highestConflictRow() {
	vector<int> conflictIndex;

	int minConflict = 0;

	for (int i=0; i<N; i++) {
		int j = row[i];
		int tempConflict = 0;

		tempConflict = (col[j]*(col[j]+1))/2;
		tempConflict += (upperDiag[i+j]*(upperDiag[i+j]+1))/2;
		tempConflict += (lowerDiag[i+N-1-j]*(lowerDiag[i+N-1-j]+1))/2;

		if (tempConflict > minConflict) {
			minConflict = tempConflict;
			conflictIndex.clear();
			conflictIndex.push_back(i);
			
		} else if (tempConflict == minConflict) {
			conflictIndex.push_back(i);
		}
	}

	return conflictIndex[rand() % conflictIndex.size()];
}

void resetCount() {
	memset(col,0,sizeof(col));
	memset(lowerDiag,0,sizeof(lowerDiag));
	memset(upperDiag,0,sizeof(upperDiag));
}

void init() {
	vector<int> conflictIndex;

	srand(time(NULL)); //init random seed
	row[0] = rand()%N;
	int j = row[0];

	resetCount();

	col[j]++;
	upperDiag[0+j]++;
	lowerDiag[0+N-1-j]++;

	//find Queen at row[i]
	for (int i=1; i<N; i++) {		
		j= row[i] = selectQueen(i);

		col[j]++;
		upperDiag[i+j]++;
		lowerDiag[i+N-1-j]++;
	}

}

// calcualte Conflict on board with queen(i,row[i]), i=0 -> N-1
int calculateConflict() {
	int confl = 0;
	int j;

	resetCount();

	// queen(i, row[i])
	// increase number of queens on column j=row[i], upper and lower diagnogals
	for (int i = 0; i<N; i++) { 
		j = row[i];
		col[j]++;
		upperDiag[i+j]++;
		lowerDiag[i+N-1-j]++;
	}

	for (int i=0; i < 2*N-1; i++) {
		if (i < N) {
			confl += col[i]*(col[i]-1)/2; 
		}
		confl += upperDiag[i]*(upperDiag[i]-1)/2;
		confl += lowerDiag[i]*(lowerDiag[i]-1)/2;
	}

	return confl;
}



// Main behavior
void minConfict() {
	int n, newConflict, preConflict;
	int cnt = 0;
	while (preConflict != 0) {
		count++;
		//n = rand() % N; // TODO: find row with highest confict among rows from 1->N
		n = highestConflictRow();

		row[n] = selectQueen(n);
		newConflict = calculateConflict();
		if (newConflict == preConflict) {
			cnt++;
			if (cnt>= 2) {
				row[rand()%N] = rand()%N;
				cnt=0;
			}
		}
		preConflict = newConflict;
	}

	//printf("count=%d\n",count);
}

void printAssigment() {
	for (int i = 0; i<N; i++) {
		for (int j =0; j< row[i]; j++) printf("0 ");
		printf("1 ");
		for (int j=row[i]+1; j<N; j++) printf("0 ");
		printf(" row[%d]=%d\n",i,row[i]);
	}
}

int main() {
	printf("First assigment\n");
	init();
	printAssigment();
	printf("Initial conflict value=%d\n\n",calculateConflict());

	

	minConfict();
	printf("Final assigment\n");
	printAssigment();
	printf("Final conflict value=%d, number of assigments=%d\n",calculateConflict(),count);

	return 0;
}