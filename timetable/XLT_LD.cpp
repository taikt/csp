// Thuat toan HET_LD, chi su dung leo doi, khong su dung dot bien

// Mot DanSo gom nhieu CaThe
// 1 CaThe gom lich cac ky thi 1..n
// dieu kien rang buoc
// XungDot[i,j] = so luong thi sinh tham gia ky thi i va j
// time[i] luu slot time to chuc cho ky thi i
// voi 2 ky thi i,j: neu XungDot[i,j] != 0 => time[i] != time[j]

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

#define SoTheHe 50
#define XacSuatLeoDoi 0.75
#define XacSuatDotBien 0.05
#define KichThuocTapChonLoc 3
#define SoCaTheDanSo 50
#define SoBuocLapLeoDoi 100
#define SoLanKiemTra 30
//#define DocDuLieuTu2File

using namespace std;

int SoKyThi;
vector< vector<int> > XungDot;

string fp1 = "input/car-f-92.crs";
string fp2 = "input/car-f-92.stu";
string file = "input/input1.txt";

class CaThe {

	public:
		// variable
		vector<int> time; // gan timeslot cho moi ky thi
		vector<int> used_time; // luu timeslot da su dung
		map<int,int> SoKyThiTrongTimeSlot; // luu so exam trong 1 timeslot

		int fitness; // luu so timeslot duoc su dung trong mot CaThe, can toi thieu fitness

		// constructor
		CaThe() {
			// gan ngau nhien timeslot cho n exam
			for (int i=0; i<SoKyThi; i++) {
				int random;

				while (1) {
					random = (int)(((float) SoKyThi-1)*rand()/(RAND_MAX + 1.0));

					time.push_back(random);
					if (ThoaManToiKyThi(i))
					{
						//printf("exam=%d,time=%d\n",i,random);
						break;
					}
					else time.pop_back();

				}

				SoKyThiTrongTimeSlot[random]++;
			}
			// tinh fitness cho CaThe tren?
			// fitness = so luong used timeslot
			// used timeslot duoc tinh tu map<timeslot,#exams> SoKyThiTrongTimeSlot
			fitness = 0;
			TinhTimeSlot();
			TinhFitness();
		}

		// tinh so used timeslot trong CaThe
		void TinhTimeSlot() {
			used_time.clear();
			map<int,int>::iterator it;
			for (it=SoKyThiTrongTimeSlot.begin(); it != SoKyThiTrongTimeSlot.end(); it++)
				if (it->second > 0) { // neu so luong exam tren 1 slottime >0, chon slottime ay
					used_time.push_back(it->first);
				}

		}

		// Tinh gia tri fitness cho CaThe
		void TinhFitness() {
			fitness = used_time.size();
		}

		// gan ky thi i cho timeslot t
		void GanKyThi(int i, int t) {
			//update thong tin SoKyThiTrongTimeSlot
			// used timeslot va fitness se luon update dua vua SoKyThiTrongTimeSlot, vi the chi can
			// update SoKyThiTrongTimeSlot
			SoKyThiTrongTimeSlot[time[i]]--; // remove gia tri timeslot cu
			time[i]= t;
			SoKyThiTrongTimeSlot[time[i]]++; // update gia tri moi
		}

		// kiem tra gia tri timeslot duoc gan moi cho ky thi i co vi pham constraint
		bool ThoaManRangBuoc(int i) {
			for (int j=0; j<SoKyThi; j++)
				if ((i!=j) && (XungDot[i][j] > 0) && (time[i]==time[j]))
					return false;
			return true;
		}


		// methods
		bool ThoaManToiKyThi(int i) {
			for (int j=0; j<i; j++) // kiem tra ky thi j
				if ((XungDot[i][j] >0) && (time[i] == time[j]))
				return false;

			return true;

		}

		// mutation cho dan so moi
		// dua vao xac suat DotBien tung CaThe, bang viec thay doi timeslot cho cac ky thi dua tren xac suat
		// timeslot moi phai thoa man khong vi pham constraint
		// voi moi exam, thu 3 lan de kiem tra timeslot moi
		void DotBien() {
			for (int i=0; i< SoKyThi; i++)
				if (((1.0)*rand()/(RAND_MAX + 1.0)) < XacSuatDotBien) {
					// chon 1 timeslot ngau nhien gan cho ky thi i, thu toi da 3 lan
					// dk gan la thoa man rang buoc

					int oldTimeSlot = time[i]; // luu lai timeslot cu neu timeslot moi vi pham constraint
					for (int j=0; j<SoLanKiemTra; j++) {
						int random = (int)(( (float) SoKyThi-1 )*rand()/(RAND_MAX + 1.0));
						GanKyThi(i,random);
						if (ThoaManRangBuoc(i)) { // gan moi timeslot cho ky thi i thoam man constraint
							break;
						} else GanKyThi(i,oldTimeSlot);
					}


				}

			// update used timeslot
			TinhTimeSlot();
			// update new fitness
			TinhFitness();
		}


		// LeoDoibing cho dan so moi
		// dua vao xac suat HC, LeoDoibing cho tung CaThe
		// voi moi CaThe duoc chon HC
		// lap k lan, moi lan chon ngau nhien mot ky thi
		// gan lai timeslot cho ky thi duoc chon voi gtri trong tap used timeslot
		// dk thay doi timeslot moi la khong vi pham constraint va tao ra fitness tot hon

		// note: voi mutation, chon timeslot ngau nhien
		// voi LeoDoi chon timeslot tot nhat trong tap used_time da co

		void LeoDoi() {
			for (int i=0; i<SoBuocLapLeoDoi; i++) {
				// chon ngau nhien 1 ky thi
				int exam = (int)(( (float) SoKyThi-1 )*rand()/(RAND_MAX + 1.0));

				//int oldTimeSlot = time[exam];
				//int currentFitness = fitness;
				for (vector<int>::iterator it=used_time.begin(); it != used_time.end(); it++) {
					// gan exam cho timeslot moi *it
#if 0
					// cach 1
					GanKyThi(exam, *it);
					TinhTimeSlot();
					TinhFitness();
					if (!ThoaManRangBuoc(exam) || currentFitness < fitness) {
						//backup timeslot cu
						GanKyThi(exam, oldTimeSlot);
						TinhTimeSlot(); // update again used timeslot
						TinhFitness(); // update again fitness value
					}
#endif

					// cach 2
					// copy current CaThe toi 1 CaThe khac
					// thay doi timeslot cho exam i tren CaThe moi
					// neu thay doi tot hon ve fitness, swap lai cho CaThe cu
					CaThe ungvien = *this;
					ungvien.GanKyThi(exam, *it);
					ungvien.TinhTimeSlot();
					ungvien.TinhFitness();

					if (ungvien.ThoaManRangBuoc(exam) && ungvien.fitness < fitness) {
						//printf("ungvien.fitness = %d\n",ungvien.fitness);
						GanKyThi(exam,*it);
						used_time = ungvien.used_time;
						fitness = ungvien.fitness;
						break; // voi ky thi exam duoc chon, neu tim duoc 1 timeslot tot hon thi break ngay,
						// TODO: test voi truong hop tim timeslot tot nhat trong tap used timeslot
					}

				}
			}
		}

		// print 1 CaThe
		void Printf() {
			printf("\n In ra slot thoi gian cho ca the tot nhat:\n");
			// print cac timeslot cho exam
			for (int i=0; i< SoKyThi; i++)
				printf("time[%d]=%d  ",i+1,time[i]);

			printf("\nGia tri fitness:%d\n", fitness);
		}

};

vector<CaThe> DanSo;


int DemSoKyThi() {

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
#if 1
	int line = 0;
	FILE *fp = fopen(fp1.c_str(),"r");

	int ch = 0;
	while (!feof(fp)) {
		ch = fgetc(fp);
		if (ch == '\n') line++;
	}

	fclose(fp);

	return line;
#endif //
}



void MaTranXungDot() {
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
					XungDot[atoi((*it).c_str())][atoi((*nit).c_str())]++;
					XungDot[atoi((*nit).c_str())][atoi((*it).c_str())]++;
				}

		printf("\n");
	}
#endif

	// C style
#ifdef DocDuLieuTu2File
	FILE *fp = fopen(fp2.c_str(),"r");
	char line[1024];

    //fgets(line,sizeof line, fp);
    //SoKyThi = atoi(line);

    XungDot.resize(SoKyThi);
	for (int i=0; i< SoKyThi; i++)
		XungDot[i].resize(SoKyThi);

    //printf("So luong exam: %d \n",SoKyThi);

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
				//printf("(%d,%d) ",*it, *nit);

				XungDot[(*it)-1][(*nit)-1]++;
				XungDot[(*nit)-1][(*it)-1]++;
			}
			//printf("\n");
		}

		//printf("\n");
	}
#else // Chi doc du lieu tu 1 file.txt
    FILE *fp = fopen(file.c_str(),"r");
	char line[1024];

    fgets(line,sizeof line, fp);
    SoKyThi = atoi(line);

    XungDot.resize(SoKyThi);
	for (int i=0; i< SoKyThi; i++)
		XungDot[i].resize(SoKyThi);

    printf("So luong ky thi: %d \n",SoKyThi);

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
				//printf("(%d,%d) ",*it, *nit);

				XungDot[(*it)-1][(*nit)-1]++;
				XungDot[(*nit)-1][(*it)-1]++;
			}
			//printf("\n");
		}

		//printf("\n");
	}
#endif // DocDuLieuTu2File
	//debug
#if 0
	for (int i=0; i< SoKyThi; i++)
	{
		for (int j=i+1; j< SoKyThi; j++) {
			printf("(%d,%d)=%d  ",i,j,XungDot[i][j]);
		}
		printf("\n");
	}
#endif
}

void DocDauVao() {


	// dem so course tu file course.txt
	// doc file va dem so ky tu xuong dong '\n'
#ifdef DocDuLieuTu2File
	SoKyThi = DemSoKyThi();
	printf("so luong ky thi %d\n",SoKyThi);

	// tinh ma tran XungDot tu file student.txt
	MaTranXungDot();
#else
    MaTranXungDot();
#endif
}

bool Sosanh(CaThe a, CaThe b) {
	return (a.fitness <= b.fitness);
}

CaThe bestCaThe() {
	//duyet best tu dau dan so hien tai va tim ra CaThe co fitness tot nhat
	CaThe *best = &(*DanSo.begin());
	vector<CaThe>::iterator it;

	for (it=DanSo.begin(); it != DanSo.end(); it++) {
		if (best->fitness > (*it).fitness)
			best = &(*it);
	}

	return (*best);

}

void TaoDanSo() {

	DanSo.resize(SoCaTheDanSo);
	vector<CaThe>::iterator it;

	for (it=DanSo.begin(); it!= DanSo.end();it++)
		*it = CaThe();
}

void XepLichThi() {
    const clock_t thoigianBatdau = clock();
    printf("\nStart running thuat toan HET_LD\n\n");
	// Tao dan so
	TaoDanSo();


	for (int i=0; i<SoTheHe; i++) {
		// chon lua best CaThe tu dan so hien tai, CaThe nay chac chan duoc add vao dan so tiep theo ma khong can mutation
		CaThe best = bestCaThe();
		// tao dan so moi tu dan so hien tai, tao ra (SoCaTheDanSo-1) individuals moi
		// moi individua moi duoc chon nhu sau
		// lay ngau nhien 1 tap gom 3 individuals tu dan so hien tai
		// chon ra individual(CaThe) tot nhat trong 3 individual tren lam individual moi cho the he tiep theo
		vector<CaThe> newDanSo;
		vector<CaThe> DanSoDuocChon;

		printf("The he %d, gia tri fitness tot nhat=%d\n",i,best.fitness);
		printf("Thoi gian da chay: %.3f sec\n",(float (clock()-thoigianBatdau)/(CLOCKS_PER_SEC)));

		for (int j=0; j<SoCaTheDanSo; j++) {
			//chon mot CaThe moi
			DanSoDuocChon.clear();

			for (int k=0; k<KichThuocTapChonLoc; k++) {
				//DanSoDuocChon.push_back(DanSo[(int) SoCaTheDanSo* rand()/(RAND_MAX + 1.0)]);
				DanSoDuocChon.push_back(DanSo.at((int)(((float) SoCaTheDanSo - 1)*rand()/(RAND_MAX + 1.0))));
			}

			sort(DanSoDuocChon.begin(),DanSoDuocChon.end(),Sosanh);

			newDanSo.push_back(DanSoDuocChon.front());
		}

		DanSo = newDanSo;

		vector<CaThe> DanSoMoi;

		for (int i=0; i<SoCaTheDanSo-1; i++) {
			// mutation cho dan so moi
			// dua vao xac suat DotBien tung CaThe, bang viec thay doi timeslot cho cac ky thi dua tren xac suat
			// timeslot moi phai thoa man khong vi pham constraint
			// voi moi exam, thu 3 lan de kiem tra timeslot moi
#if 0
			if (((1.0)*rand()/(RAND_MAX + 1.0)) < XacSuatDotBien) {
				//printf("DotBien\n");
				DanSo[i].DotBien();
			}
#endif
			// Thuc hien thuat toan leo doi cho dan so moi
			// dua vao xac suat HC, Leo doi cho tung CaThe
			// voi moi CaThe duoc chon HC
			// lap k lan, moi lan chon ngau nhien mot ky thi
			// gan lai timeslot cho ky thi duoc chon voi gtri trong tap used timeslot
			// dk thay doi timeslot moi la khong vi pham constraint va tao ra fitness tot hon
			if ((1.0 *rand()/(RAND_MAX + 1.0)) < XacSuatLeoDoi) {
				DanSo[i].LeoDoi();
			}

			DanSoMoi.push_back(DanSo[i]);
		}

		// them best CaThe tu dan so cu vao dan so moi
		DanSoMoi.push_back(best);

		DanSo = DanSoMoi;
	}

	// print best CaThe
	CaThe best = bestCaThe();
	best.Printf();
	printf("Thoi gian da chay: %.3f sec\n",(float (clock()-thoigianBatdau)/(CLOCKS_PER_SEC)));

}
int main() {
	srand(time(NULL));

	DocDauVao();

	XepLichThi();


	return 0;
}
