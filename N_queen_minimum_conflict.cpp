// 12.1.2017, Tai<taitrananhvn@gmail.com>
// N-Queen problem using minimum confict algorithm

// q[i] la vi tri con hau tai hang i, tuc cot q[i]
// (i,q[i]), i=1->n
// Conflict duoc tinh nhu sau:
// Gia su 1 con hau tai hang i, cot j
// neu co 1 con hau khac tren hang i=> confict doi voi i = +1
// => neu co n con hau khac tren hang i => anh huong conflict
// doi voi i la n
// Tong quat, gia su hang i co tat ca n con hau
// khi do tong xung dot tren hang i la n*(n-1)/2 
// (chia 2 vi neu i xung dot j thi j cung xung dot i)
// tuong tu, tai 1 duong cheo tren bat ky (i+q[i]=const) co k quan hau
// thi tong xung dot tren do la k*(k-1)/2
// tren tai cheo duoi bat ky (i+N-q[i]=const) co l con hau,
// thi tong xung dot tren duong cheo do la l*(l-1)/2

// Thuat toan
// 1. Khoi tao n con hau 
// con hau hang 0 chon ngau nhien
// con hau hang i chon thoa man tong conflict hien tai la nho nhat
// 2. Tai moi buoc lap (Lap den khi nao conflict tong = 0 hoac vuot qua gioi han cho phep)
// Chon hang k co tong conflict tren no lon nhat trong so n hang
// tren hang k nay, chon vi tri moi cua con hau de dat duoc min-confict trong tat ca lua chon
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
#define N 4
#define INF 10000

int row[N];
int col[N];
int lowerDiag[2*N-1];
int upperDiag[2*N-1];

void init() {
	vector<int> conflictIndex;

	srand(time(NULL)); //init random seed
	row[0] = rand()%N;
	int j = row[0];

	// khoi tao mang trong
	memset(col,0,sizeof(int)*sizeof(col));
	memset(lowerDiag,0,sizeof(int)*sizeof(lowerDiag));
	memset(upperDiag,0,sizeof(int)*sizeof(upperDiag));

	col[j]++;
	upperDiag[0+j]++;
	lowerDiag[0+N-1-j]++;

	//find Queen at row[i]
	for (int i=1; i<N; i++) {
		int minConflict = INF;
		conflictIndex.clear();

		for (j=0; j<N; j++) {
			
			//assume put Queen at column j, let's calculate conflict 
			int tempConflict = 0;
			tempConflict = (col[j]*(col[j]+1))/2;
			tempConflict += (upperDiag[i+j]*(upperDiag[i+j]+1))/2;
			tempConflict += (lowerDiag[i+N-1-j]*(lowerDiag[i+N-1-j]+1))/2;

			if (tempConflict < minConflict) {
				minConflict = tempConflict;
				conflictIndex.clear();
				conflictIndex.push_back(j);
				
			} else if (tempConflict == minConflict) {
				conflictIndex.push_back(j);
			}

		}

		row[i] = conflictIndex[rand() % conflictIndex.size()];
		
		j = row[i];
		col[j]++;
		upperDiag[i+j]++;
		lowerDiag[i+N-1-j]++;
	}

}

// vi tri Queen (i,row[i]), i=0 -> N-1
int calculateConflict() {
	int confl = 0;
	int j;

	// khoi tao mang trong
	memset(col,0,sizeof(int)*sizeof(col));
	memset(lowerDiag,0,sizeof(int)*sizeof(lowerDiag));
	memset(upperDiag,0,sizeof(int)*sizeof(upperDiag));

	// Co N hau (i, row[i])
	// tinh tong hau tang len boi moi hau tai cot doc, duong cheo tren, duong cheo duoi
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

void printAssigment() {
	for (int i = 0; i<N; i++) {
		for (int j =0; j< row[i]; j++) printf("0 ");
		printf("1 ");
		for (int j=row[i]+1; j<N; j++) printf("0 ");
		printf(" row[%d]=%d\n",i,row[i]);
	}
}

int main() {
	init();
	printAssigment();
	printf("Conflict value=%d\n",calculateConflict());

	return 0;
}