// Mot population gom nhieu solution
// 1 solution gom lich cac ky thi 1..n
// dieu kien rang buoc
// conflict[i,j] = so luong thi sinh tham gia ky thi i va j
// time[i] luu slot time to chuc cho ky thi i
// voi 2 ky thi i,j: neu conflict[i,j] != 0 => time[i] != time[j]

// du lieu input
// course.txt (id #students)
// 001 2
// 002 3

// student.txt
// 001 002 (sinh vien A)
// 002 (sinh vien B)
// 001 002 (sinh vien C)

#include <cstdio>
#include <algorithm>
#include <fstream> // ifstream
#include <iterator> //istream_iterator
#include <vector>
#include <iostream>
#include <sstream> //stringstream
#include <string>

#include <cstdlib> //file in C
#include <cstdio>
#include <map>
#include <time.h>

#define MAX_GENERATION 100
#define PROB_HC 0.75
#define PROB_MUTATION 0.05
#define TOURNAMENT_SIZE 3
#define POPSIZE 50
#define HC_REPEAT 100
#define MAX_RETRIES 30


using namespace std;

int countExam;
vector< vector<int> > conflict;

//string fp1 = "input/test1.crs";
string file = "input/test1.stu";

class solution {

	public:
		// variable
		vector<int> time; // gan timeslot cho moi ky thi
		vector<int> used_time; // luu timeslot da su dung
		map<int,int> exams_in_timeslot; // luu so exam trong 1 timeslot

		int fitness; // luu so timeslot duoc su dung trong mot solution, can toi thieu fitness

		// constructor
		solution() {
			// gan ngau nhien timeslot cho n exam
			for (int i=0; i<countExam; i++) {
				int random;

				while (1) {
					random = (int)(((float) countExam-1)*rand()/(RAND_MAX + 1.0));

					time.push_back(random);
					if (satisfyToNow(i))
					{
						//printf("exam=%d,time=%d\n",i,random);
						break;
					}
					else time.pop_back();

				}

				exams_in_timeslot[random]++;
			}
			// tinh fitness cho solution tren?
			// fitness = so luong used timeslot
			// used timeslot duoc tinh tu map<timeslot,#exams> exams_in_timeslot
			fitness = 0;
			calculate_timeslot();
			calculate_fitness();
		}

		// tinh so used timeslot trong solution
		void calculate_timeslot() {
			used_time.clear();
			map<int,int>::iterator it;
			for (it=exams_in_timeslot.begin(); it != exams_in_timeslot.end(); it++)
				if (it->second > 0) { // neu so luong exam tren 1 slottime >0, chon slottime ay
					used_time.push_back(it->first);
				}

		}

		// Tinh gia tri fitness cho solution
		void calculate_fitness() {
			fitness = used_time.size();
		}

		// gan ky thi i cho timeslot t
		void assignExam(int i, int t) {
			//update thong tin exams_in_timeslot
			// used timeslot va fitness se luon update dua vua exams_in_timeslot, vi the chi can
			// update exams_in_timeslot
			exams_in_timeslot[time[i]]--; // remove gia tri timeslot cu
			time[i]= t;
			exams_in_timeslot[time[i]]++; // update gia tri moi
		}

		// kiem tra gia tri timeslot duoc gan moi cho ky thi i co vi pham constraint
		bool feasiableAssigment(int i) {
			for (int j=0; j<countExam; j++)
				if ((i!=j) && (conflict[i][j] > 0) && (time[i]==time[j]))
					return false;
			return true;
		}


		// methods
		bool satisfyToNow(int i) {
			for (int j=0; j<i; j++) // kiem tra ky thi j
				if ((conflict[i][j] >0) && (time[i] == time[j]))
				return false;

			return true;

		}

		// mutation cho dan so moi
		// dua vao xac suat mutate tung solution, bang viec thay doi timeslot cho cac ky thi dua tren xac suat
		// timeslot moi phai thoa man khong vi pham constraint
		// voi moi exam, thu 3 lan de kiem tra timeslot moi
		void mutate() {
			for (int i=0; i< countExam; i++)
				if (((1.0)*rand()/(RAND_MAX + 1.0)) < PROB_MUTATION) {
					// chon 1 timeslot ngau nhien gan cho ky thi i, thu toi da 3 lan
					// dk gan la thoa man rang buoc

					int previous_timeslot = time[i]; // luu lai timeslot cu neu timeslot moi vi pham constraint
					for (int j=0; j<MAX_RETRIES; j++) {
						int random = (int)(( (float) countExam-1 )*rand()/(RAND_MAX + 1.0));
						assignExam(i,random);
						if (feasiableAssigment(i)) { // gan moi timeslot cho ky thi i thoam man constraint
							break;
						} else assignExam(i,previous_timeslot);
					}


				}

			// update used timeslot
			calculate_timeslot();
			// update new fitness
			calculate_fitness();
		}


		// Hillclimbing cho dan so moi
		// dua vao xac suat HC, hillclimbing cho tung solution
		// voi moi solution duoc chon HC
		// lap k lan, moi lan chon ngau nhien mot ky thi
		// gan lai timeslot cho ky thi duoc chon voi gtri trong tap used timeslot
		// dk thay doi timeslot moi la khong vi pham constraint va tao ra fitness tot hon

		// note: voi mutation, chon timeslot ngau nhien
		// voi hillclim chon timeslot tot nhat trong tap used_time da co

		void hillClim() {
			for (int i=0; i<HC_REPEAT; i++) {
				// chon ngau nhien 1 ky thi
				int exam = (int)(( (float) countExam-1 )*rand()/(RAND_MAX + 1.0));

				//int previous_timeslot = time[exam];
				//int currentFitness = fitness;
				for (vector<int>::iterator it=used_time.begin(); it != used_time.end(); it++) {
					// gan exam cho timeslot moi *it
#if 0
					// cach 1
					assignExam(exam, *it);
					calculate_timeslot();
					calculate_fitness();
					if (!feasiableAssigment(exam) || currentFitness < fitness) {
						//backup timeslot cu
						assignExam(exam, previous_timeslot);
						calculate_timeslot(); // update again used timeslot
						calculate_fitness(); // update again fitness value
					}
#endif

					// cach 2
					// copy current solution toi 1 solution khac
					// thay doi timeslot cho exam i tren solution moi
					// neu thay doi tot hon ve fitness, swap lai cho solution cu
					solution candi = *this;
					candi.assignExam(exam, *it);
					candi.calculate_timeslot();
					candi.calculate_fitness();

					if (candi.feasiableAssigment(exam) && candi.fitness < fitness) {
						printf("candi.fitness = %d\n",candi.fitness);
						assignExam(exam,*it);
						used_time = candi.used_time;
						fitness = candi.fitness;
						break; // voi ky thi exam duoc chon, neu tim duoc 1 timeslot tot hon thi break ngay,
						// TODO: test voi truong hop tim timeslot tot nhat trong tap used timeslot
					}

				}
			}
		}

		// print 1 solution
		void Printf() {
			printf("print the solution\n");
			// print cac timeslot cho exam
			for (int i=0; i< countExam; i++)
				printf("time[%d]=%d  ",i,time[i]);

			printf("\nfitness value:%d\n", fitness);
		}

};

vector<solution> population;


int countCourse() {

	//int cnt;

	// C++ style using STL count
#if 0
	ifstream fp("input/course.txt");

	fp.unsetf(ios_base::skipws); //don't skip newline '\n'

	istream_iterator<char> itt(fp);
	istream_iterator<char> end;

	cnt = count(itt,end,'\n');
	fp.close();
	printf("%d\n",cnt);
#endif

	// C style
#if 0
	printf("test\n");
	int line = 0;
	FILE *fp = fopen(fp1.c_str(),"r");
	printf("test2\n");
	int ch = 0;
	while (!feof(fp)) {
		ch = fgetc(fp);
		if (ch == '\n') line++;
	}
	printf("%d\n",line);
	fclose(fp);

	return line;
#endif //
}



void conflictMatrix() {
   #if 0
	// C++ style
	ifstream fp("input/student.txt");
	string line;

	while (getline(fp,line)) {
		stringstream st(line);
		istream_iterator<string> itt(st);
		istream_iterator<string> end;

		vector<string> vt(itt,end);

		for (vector<string>::iterator it = vt.begin(); it != vt.end(); it++)
			for (vector<string>::iterator nit = it+1; nit != vt.end(); nit++)
				{
					printf(" %s",(*it).c_str());
					conflict[atoi((*it).c_str())][atoi((*nit).c_str())]++;
					conflict[atoi((*nit).c_str())][atoi((*it).c_str())]++;
				}

		printf("\n");
	}
#endif

	// C style
	FILE *fp = fopen(file.c_str(),"r");
	char line[1024];

    fgets(line,sizeof line, fp);
    countExam = atoi(line);

    conflict.resize(countExam);
	for (int i=0; i< countExam; i++)
		conflict[i].resize(countExam);

    printf("So luong exam: %d \n",countExam);

	while (fgets(line,sizeof line, fp)!= NULL) {
		char *start = line;
		int vl, n;
		vector<int> vt; // Khong can khoi tao neu thuc hien push_back()?

		while (sscanf(start,"%d%n",&vl,&n) == 1) {
			// printf("%d<%d> ",vl,n);
			// %n: so ky tu duoc doc toi hien tai
			// sscanf tra lai so bien duoc gan thanh cong
			// http://stackoverflow.com/questions/13199693/what-does-the-n-stand-for-in-sscanfs-d-n-i-n
			vt.push_back(vl);
			start += n;
		}


		for (vector<int>::iterator it = vt.begin(); it != vt.end(); it++) {
			for (vector<int>::iterator nit = it+1; nit != vt.end(); nit++) {
				printf("(%d,%d) ",*it, *nit);

				conflict[(*it)-1][(*nit)-1]++;
				conflict[(*nit)-1][(*it)-1]++;
			}
			printf("\n");
		}

		printf("\n");
	}

	//debug
	for (int i=0; i< countExam; i++)
	{
		for (int j=i+1; j< countExam; j++) {
			printf("(%d,%d)=%d  ",i,j,conflict[i][j]);
		}
		printf("\n");
	}

}

void readinput() {


	// dem so course tu file course.txt
	// doc file va dem so ky tu xuong dong '\n'
	//countExam = countCourse();
	//printf("so luong course %d\n",countExam);

	// tinh ma tran conflict tu file student.txt
	conflictMatrix();
}

bool compare(solution a, solution b) {
	return (a.fitness <= b.fitness);
}

solution bestSolution() {
	//duyet best tu dau dan so hien tai va tim ra solution co fitness tot nhat
	solution *best = &(*population.begin());
	vector<solution>::iterator it;

	for (it=population.begin(); it != population.end(); it++) {
		if (best->fitness > (*it).fitness)
			best = &(*it);
	}

	return (*best);

}

void generateGeneration() {

	population.resize(POPSIZE);
	vector<solution>::iterator it;

	for (it=population.begin(); it!= population.end();it++)
		*it = solution();
}

void HybExamTabling() {
	// Tao dan so
	printf("start GA\n");

	generateGeneration();

	for (int i=0; i<MAX_GENERATION; i++) {
		// chon lua best solution tu dan so hien tai, solution nay chac chan duoc add vao dan so tiep theo ma khong can mutation
		solution best = bestSolution();
		// tao dan so moi tu dan so hien tai, tao ra (popsize-1) individuals moi
		// moi individua moi duoc chon nhu sau
		// lay ngau nhien 1 tap gom 3 individuals tu dan so hien tai
		// chon ra individual(solution) tot nhat trong 3 individual tren lam individual moi cho the he tiep theo
		vector<solution> newPopulation;
		vector<solution> selectedPopulation;

		printf("generation %d, best fitness=%d\n",i,best.fitness);

		for (int j=0; j<POPSIZE; j++) {
			//chon mot solution moi
			selectedPopulation.clear();

			for (int k=0; k<TOURNAMENT_SIZE; k++) {
				//selectedPopulation.push_back(population[(int) POPSIZE* rand()/(RAND_MAX + 1.0)]);
				selectedPopulation.push_back(population.at((int)(((float) POPSIZE - 1)*rand()/(RAND_MAX + 1.0))));
			}

			sort(selectedPopulation.begin(),selectedPopulation.end(),compare);

			newPopulation.push_back(selectedPopulation.front());
		}

		population = newPopulation;

		vector<solution> newPopu;

		for (int i=0; i<POPSIZE-1; i++) {
			// mutation cho dan so moi
			// dua vao xac suat mutate tung solution, bang viec thay doi timeslot cho cac ky thi dua tren xac suat
			// timeslot moi phai thoa man khong vi pham constraint
			// voi moi exam, thu 3 lan de kiem tra timeslot moi
			if (((1.0)*rand()/(RAND_MAX + 1.0)) < PROB_MUTATION) {
				//printf("mutate\n");
				population[i].mutate();
			}

			// hillclimbing cho dan so moi
			// dua vao xac suat HC, hillclimbing cho tung solution
			// voi moi solution duoc chon HC
			// lap k lan, moi lan chon ngau nhien mot ky thi
			// gan lai timeslot cho ky thi duoc chon voi gtri trong tap used timeslot
			// dk thay doi timeslot moi la khong vi pham constraint va tao ra fitness tot hon
			if ((1.0 *rand()/(RAND_MAX + 1.0)) < PROB_HC) {
				population[i].hillClim();
			}

			newPopu.push_back(population[i]);
		}

		// them best solution tu dan so cu vao dan so moi
		newPopu.push_back(best);

		population = newPopu;
	}

	// print best solution
	solution best = bestSolution();
	best.Printf();

}
int main() {
	srand(time(NULL));

	readinput();

	HybExamTabling();


	return 0;
}
