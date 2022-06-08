#include <stdio.h>
#include<stdlib.h>
#include<fstream>
#include<string>
#include<sstream>
#define _USE_MATH_DEFINES
#include <iostream>
#include<complex>
#include<math.h>
#include<vector>
#include<time.h>
#include"llgmn.h"
using namespace std;

int main() {
	vector<vector<double>>input_data((0, 0));
	vector<double>label(0, 0);
	vector<vector<double>>label_data((0, 0));
	vector<double>input(0, 0);

	string file_d1("patarn1.csv");
	string file_d2("patarn2.csv");
	string file_d3("patarn3.csv");
	string file_d4("patarn4.csv");
	string file_d("input.txt");
	string file_l1("label1.csv");
	string file_l2("label2.csv");
	string file_l3("label3.csv");
	string file_l4("label4.csv");
	string file_l("teacher.txt");
	//“ü—Íƒf[ƒ^‚Ì“Ç‚İ‚İ
	input_data = read_file4(file_d4);
	label_data = read_file4(file_l4);
	//input_data = read_file(file_d2);
	//label_data = read_file(file_l2);


	input_data = make_vector(input_data);
	int choose = 0;
	cout << "Which learning do you choose." << endl;
	cout << "0:online" << endl;
	cout << "1:patch" << endl;
	scanf_s("%d", &choose);
	if (choose == 0) {
		learn_online(input_data, label_data);
	}
	else if (choose == 1) {
		learn_patch(input_data, label_data);
	}
	else {
		//passs
	}

	return 0;
}