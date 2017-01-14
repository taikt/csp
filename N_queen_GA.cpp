// 12.1.2017, Tai<taitrananhvn@gmail.com>
// N-Queen problem using Genetic algorithm


#include <cstdio>
#include <limits>
#include <cstdlib> //rand,srand
#include <ctime>
#include <vector>
#include <cstring>
#include <math.h>
using namespace std;

#define N 8
#define popSize 512
#define SurvivalRate 0.05

int count = 0;

class board {
public:
	//int row[N]; // to store queen at each row
	int row[N];

	board() {
		// select randomly position of queens
		//srand(time(0));
		//row = new int[N];
		srand((unsigned)time(0));
		for (int i=0; i<N; i++)
			row[i] = (rand()%N);
	}

	void printAssigment() {
		for (int i = 0; i<N; i++) {
			for (int j =0; j< row[i]; j++) printf("0 ");
				printf("1 ");
			for (int j=row[i]+1; j<N; j++) printf("0 ");
				printf(" row[%d]=%d\n",i,row[i]);
		}
	}

	// calcualte Conflict on board with queen(i,row[i]), i=0 -> N-1
	int calculateConflict() {
		int confl = 0;
		int j;
		int col[N];
		int lowerDiag[2*N-1];
		int upperDiag[2*N-1];

		memset(col,0,sizeof(col));
		memset(lowerDiag,0,sizeof(lowerDiag));
		memset(upperDiag,0,sizeof(upperDiag));

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

	void mutate() {
		int count;
		count = rand()% N;
		for (int i=0; i<1; i++) { // 1 or count, which is better?
			int j = rand()%N;
			row[j]=rand()%N;
		}
	}
/*
	board Mate(board Parent) {
		board Child;
		// random pos between 1 and N-1
		int r=(rand()%(N-1))+1;
		for(int i=0; i<r; i++) {
			Child.row[i]=row[i];
		}
		for(int i=r; i<N; i++) {
			Child.row[i]=Parent.row[i];
		}
		return Child;
	}
*/
};

board* population;

void swap(int i, int j) {
	board tmp;
	tmp = population[i];
	population[i] = population[j];
	population[j] = tmp;
}

int partition(int left, int right) {
	int index = left;
	board pilot = population[right];

	for (int i=left; i<right; i++)
		if (population[i].calculateConflict() < pilot.calculateConflict()) {
			swap(index,i);
			//printf("swap %d and %d\n",index,i);
			index++;
		}
	swap(index,right);

	return index;
}

// use quicksort 
void sortPopulation(int left, int right) {
	int index;
	if (left < right) {
		index = partition(left,right);
		sortPopulation(left,index-1);
		sortPopulation(index+1,right);
	}
}

void init() {
	population = new board[popSize];
	
	//TODO: trouble if just init population without specifying rand() for each population 
	// as below. In first case, all population has same queen position

	for (int i=0; i< popSize; i++) {
		for (int j = 0; j< N; j++)
			population[i].row[i] = rand()%N;
	}
	sortPopulation(0,popSize-1);

	printf("Init conflict %d\n",population[0].calculateConflict());
}

board reproduce(int index1, int index2) {
	int k = rand() % N;
	board child;

	for (int i=0; i<k; i++)
		child.row[i] = population[index1].row[i];
	for (int i=k; i<N; i++)
		child.row[i] = population[index2].row[i];

	return child;
}


// Main behavior
void geneticAlgorithm (board* population) {
	board child;
	int p1, p2;

	// Not changing 'cut' best elements in old population
	// Change for remaining elements for new population

	while (population[0].calculateConflict() != 0) {
		//printf("Best population conflict %d\n", population[0].calculateConflict());
		/*
		// this is very bad case
		for (int i = 0; i<popSize; i++) {
			// create new child from two randomly good parents
			p1 = rand() % (int)floor(surviveRate*popSize);
			p2 = rand() % (int)floor(surviveRate*popSize);
			printf("p1=%d, p2=%d\n",p1,p2);
			child = reproduce(p1,p2);
			child.mutate();

			population[i] = child;			
		}
		*/
		int cut = min(popSize - 1, max((int)floor(popSize*SurvivalRate), 1));
		int numberOfKids = floor(1/SurvivalRate)-1;
		/*
		// this is fine
		printf("PopSize=%d, cut=%d, num.kids=%d\n",popSize,cut,numberOfKids);
		for(int i=cut, j=0; i<popSize; i+=numberOfKids, j++) {
			for(int k=0; k<numberOfKids; k++) {
				population[i+k]=population[j].Mate(population[(j+k)%cut]);
			}
		}
		*/
		for (int i=cut, j=0; i<popSize; i+=numberOfKids, j++)
		{	
			for (int k=0; k<numberOfKids; k++) {				
				population[i+k] = reproduce(j,(j+k)%cut);
			}
		}

		// mutate every board with the mutation rate
		for(int i=0; i<popSize; i++) {
			population[i].mutate();
		}

		sortPopulation(0,popSize-1);
		count++;
	}

	printf("Final conflict %d, number of moves %d\n", population[0].calculateConflict(),count);
	population[0].printAssigment();
}


int main() {
	
	init();
	geneticAlgorithm(population);

	return 0;
}