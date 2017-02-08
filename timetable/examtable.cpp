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

using namespace std;

int countExam;

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
				while (1) {
					int random = (int)(((float) countExam-1)*rand()/(RAND_MAX + 1.0));
					time.push_back(random);
					if satisfyToNow(i) break;
					else time.pop_back(i);

				}
			}
		}

		// methods

};

vector<solution> population;


int countCourse() {

	int cnt;

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
	int line = 0;
	FILE *fp = fopen("input/course.txt","r");
	int ch = 0;
	while (!feof(fp)) {
		ch = fgetc(fp);
		if (ch == '\n') line++;
	}
	printf("%d\n",line);
	fclose(fp);

	return line;
}

vector< vector<int> > conflict;

void conflictMatrix() {

	conflict.resize(countExam);
	for (int i=0; i< countExam; i++)
		conflict[i].resize(countExam);

#if 0	
	// C++ style
	ifstream fp("input/student.txt");
	string line;
	printf("%d\n",countExam);
	

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
	FILE *fp = fopen("input/student.txt","r");
	char line[1024];

	while (fgets(line,sizeof line, fp)!= NULL) {
		char *start = line;
		//printf("%s\n",line);
		
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

}

void readinput() {
	string filename1 = "input/course.txt";
	string filename2 = "input/student.txt";

	// dem so course tu file course.txt
	// doc file va dem so ky tu xuong dong '\n'
	countExam = countCourse();

	// tinh ma tran conflict tu file student.txt
	conflictMatrix();
}

int main() {
	srand(time(NULL));
	

	readinput();

	int random = (int)(((float) countExam-1)*rand()/(RAND_MAX + 1.0));
	printf("%d %d\n",random, rand());
	
	return 0;
}