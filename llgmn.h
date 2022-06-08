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
using namespace std;

//ファイル入力の「,」を除去する関数
vector<string> split(string& input, char delimiter) {
	istringstream stream(input);
	string field;
	vector<string> result;
	while (getline(stream, field, delimiter)) {
		result.push_back(field);
	}
	return result;
}

//データをファイルから読み込む関数
vector<vector<double>> read_file(string file_name) {
	vector<vector<double>>input_data((0, 0));
	vector<double>input(0, 0);
	ifstream file(file_name);
	string line;
	vector<string>strvec;
	while (getline(file, line)) {
		input.resize(0);
		strvec = split(line, ',');
		for (int i = 0; i < strvec.size(); i++) {
			input.push_back(stod(strvec.at(i)));
		}
		input.erase(input.begin());
		input_data.push_back(input);
	}
	return input_data;
}

//データをファイルから読み込む関数(file4専用)
vector<vector<double>> read_file4(string file_name) {
	vector<vector<double>>input_data((0, 0));
	vector<double>input(0, 0);
	ifstream file(file_name);
	string line;
	vector<string>strvec;
	while (getline(file, line)) {
		input.resize(0);
		strvec = split(line, ',');
		for (int i = 0; i < strvec.size(); i++) {
			input.push_back(stod(strvec.at(i)));
		}
		input_data.push_back(input);
	}
	return input_data;
}

//入力を表示する関数
void display_allinput(vector<vector<double>>input_data) {
	cout << "input" << endl;
	for (int i = 0; i < input_data.size(); i++) {
		for (int j = 0; j < input_data[i].size(); j++) {
			cout << input_data[i][j] << " ";
		}
		cout << endl;
	}
}

void display_input(vector<double> input_data) {
	for (int i = 0; i < input_data.size(); i++) {
		cout << input_data[i] << " ";
	}
	cout << endl;
}

//各層の値を表示する関数
void display_layer(vector <vector<double>> middle, vector<double> output) {
	cout << "middle" << endl;
	for (int i = 0; i < middle.size(); i++) {
		for (int j = 0; j < middle[i].size(); j++) {
			cout << middle[i][j] << " ";
		}
		cout << endl;
	}
	cout << "output" << endl;
	for (int i = 0; i < output.size(); i++) {
		cout << output[i] << " ";
	}
	cout << endl;
}

//重みを表示する関数
void display_weight(vector<vector<vector<double>>>weights) {
	cout << "weight" << endl;
	for (int i = 0; i < weights.size(); i++) {
		cout << i << endl;
		for (int j = 0; j < weights[i].size(); j++) {
			for (int k = 0; k < weights[i][j].size(); k++) {
				cout << weights[i][j][k] << " ";
			}
			cout << endl;
		}
	}
}

//第一層への入力ベクトルを生成する関数
vector<vector<double>> make_vector(vector<vector<double>>input) {
	vector<vector<double>>input_data((0, 0));
	vector<double>one_element(0, 0);
	for (int i = 0; i < input.size(); i++) {
		one_element.resize(0);
		one_element.push_back(1);
		for (int j = 0; j < input[i].size(); j++) {
			one_element.push_back(input[i][j]);
		}
		for (int j = 0; j < input[i].size(); j++) {
			for (int k = j; k < input[i].size(); k++) {
				one_element.push_back(input[i][j] * input[i][k]);
			}
		}
		input_data.push_back(one_element);
	}
	return input_data;
}

//重みを保存するための箱を生成する関数
void make_weight(vector<vector<vector<double>>>& weights, vector<vector<vector<double>>>& update, double input_element,int label,int middle_element) {
	srand(time(NULL));
	vector<double> input(0, 0);
	vector<vector<double>>weight((0, 0));
	vector<double>layers(0, 0);
	//入力層から中間層の一入力への重みの生成
	for (int i = 0; i < input_element; i++) {
		input.push_back((double)rand() / RAND_MAX);
	}
	for (int i = 0; i < middle_element; i++) {
		weight.push_back(input);
	}
	for (int i = 0; i < label-1; i++) {
		weights.push_back(weight);
		update.push_back(weight);
	}
	vector<double>zero_vec(input_element, 0);
	weight.pop_back();
	//update.push_back(weight);
	weight.push_back(zero_vec);
	weights.push_back(weight);
	update.push_back(weight);
}

//中間層と出力層を生成する関数
void make_box(vector<vector<double>>& middle, vector<double>& output,double input_element, int label, int middle_element) {
	vector<double>unit(0, 0);
	//中間層の生成
	for (int i = 0; i < middle_element; i++) {
		unit.push_back(0);
	}
	for (int j = 0; j < label; j++) {
		middle.push_back(unit);
	}
	//出力層の生成
	for (int i = 0; i < label; i++) {
		output.push_back(0);
	}

}

//事後確率を計算する関数
vector<vector<double>> post_probable(vector<vector<double>>middle) {
	vector<vector<double>> output=middle;
	double sum = 0;
	for (int i = 0; i < middle.size(); i++) {
		for (int j = 0; j < middle[i].size(); j++) {
			sum += exp(middle[i][j]);
		}
	}
	for (int i = 0; i < middle.size(); i++) {
		for (int j = 0; j < middle[i].size(); j++) {
			output[i][j] = exp(middle[i][j]) / sum;
		}
	}
	return output;
}

//順方向を計算する関数
void forward_caliculation(vector<double> input, vector<vector<double>>& middle, vector<double>& output, vector<vector<vector<double>>>weights) {
	int layer = middle.size();
	int weights_size = weights.size();
	double sum = 0;
	//入力層から中間層への順方向計算
	for (int i = 0; i < weights.size(); i++) {
		for (int j = 0; j <weights[i].size(); j++) {
			for (int k = 0; k < weights[i][j].size(); k++) {
				sum += input[k] * weights[i][j][k];
			}
			middle[i][j] = sum;
			sum = 0;
		}
	}
	middle = post_probable(middle);
	//中間層から出力層への順方向計算
	for (int i = 0; i < middle.size(); i++) {
		for (int j = 0; j < middle[i].size(); j++) {
			sum += middle[i][j];
		}
		output[i] = sum;
		sum = 0;
	}
}

//誤差逆伝搬の更新値を計算する関数
vector<vector<vector<double>>> backward(vector<double>input, vector<vector<double>>middle, vector<double>output, vector<vector<vector<double>>>update, vector<double>label) {
	int update_size = update.size();
	double unit_loss = 0;
	//出力層への重みの誤差逆伝搬
	for (int i = 0; i < update_size; i++) {
		for (int j = 0; j < update[i].size(); j++) {
			unit_loss = (output[i] - label[i]) * middle[i][j] / output[i];
			for (int k = 0; k < update[i][j].size(); k++) {
				update[i][j][k] = unit_loss * input[k];
			}
			unit_loss = 0;
		}
	}
	return update;
}

//重みを更新する関数
void change_weight(vector<vector<vector<double>>>& weights, vector<vector<vector<double>>>update, double rate, double size) {
	for (int i = 0; i < update.size(); i++) {
		for (int j = 0; j < update[i].size(); j++) {
			for (int k = 0; k < update[i][j].size(); k++) {
				weights[i][j][k] -= rate * update[i][j][k] / size;
			}
		}
	}
}

//識別率を計算する関数
void answer_rate(string file_dn, string file_ln, vector<vector<vector<double>>>weights, vector<vector<double>>middle, vector<double>output) {
	vector<vector<double>>input_data((0, 0));
	vector<vector<double>>input_label((0, 0));
	vector<double>error = output;

	//入力データの読み込み
	input_data = read_file(file_dn);
	input_data = make_vector(input_data);
	//ラベルデータの読み込み
	input_label = read_file(file_ln);

	int size = input_data[0].size();
	int input_size = input_data.size();
	double answer_data = 0;
	int answer_label, count = 0;
	for (int i = 0; i < input_size; i++) {
		forward_caliculation(input_data[i], middle, output, weights);
		answer_data = output[0];
		answer_label = input_label[i][0];
		for (int j = 0; j < output.size(); j++) {
			if (answer_data < output[j]) {
				answer_data = output[j];
				answer_label = input_label[i][j];
			}
			else {
				//pass
			}
		}
		if (answer_label == 1) {
			count++;
		}
	}
	cout <<  file_dn << endl;
	cout << "識別率：" << (double)count / input_size << endl;
}

//識別率を計算する関数(データ4専用)
void answer_rated4(string file_dn, string file_ln, vector<vector<vector<double>>>weights, vector<vector<double>>middle, vector<double>output) {
	vector<vector<double>>input_data((0, 0));
	vector<vector<double>>input_label((0, 0));
	vector<double>error = output;

	//入力データの読み込み
	input_data = read_file4(file_dn);
	input_data = make_vector(input_data);
	//ラベルデータの読み込み
	input_label = read_file4(file_ln);

	int size = input_data[0].size();
	int input_size = input_data.size();
	double answer_data = 0;
	int answer_label, count = 0;
	for (int i = 0; i < input_size; i++) {
		forward_caliculation(input_data[i], middle, output, weights);
		answer_data = output[0];
		answer_label = input_label[i][0];
		for (int j = 0; j < output.size(); j++) {
			if (answer_data < output[j]) {
				answer_data = output[j];
				answer_label = input_label[i][j];
			}
			else {
				//pass
			}
		}
		if (answer_label == 1) {
			count++;
		}
	}
	cout << file_dn << endl;
	cout << "識別率：" << (double)count / input_size << endl;
}

//逐次学習を行う関数
void learn_online(vector<vector<double>>& input_data, vector<vector<double>>& input_label) {
	int input_element = input_data[0].size();
	int output_element = input_label[0].size();
	int label_size ,middle_element;
	int learning_times = 0;
	double rate = 0;
	double sum_error = 1;
	vector<vector<double>>input = input_data;
	vector<vector<double>>label = input_label;
	vector<vector<double>> middle((0, 0));
	vector<vector<vector<double>>> weights((0, 0));
	vector<vector<vector<double>>> update((0, 0));
	vector<double> output(0, 0);

	label_size = output_element;
	cout << "The number of component=";
	scanf_s("%d", &middle_element);
	cout << "Learning rate=";
	scanf_s("%lf", &rate);
	cout << "Learning times=";
	scanf_s("%d", &learning_times);

	make_weight(weights, update,input_element, label_size, middle_element);
	make_box(middle, output, input_element, label_size, middle_element);

	int input_size = input_data.size();
	int count = 0;
	//入力の順番をランダムに一回決め、以降はその順番で入力した場合
	srand(time(NULL));
	for (int i = 0; i < input_size; i++) {
		int unit = (int)rand() % input.size();
		input_data[i] = input[unit];
		input_label[i] = label[unit];
		input.erase(input.begin() + unit);
		label.erase(label.begin() + unit);
	}
	while (learning_times > count) {
		sum_error = 0;
		//逐次学習の1エポック
		for (int i = 0; i < input_size; i++) {
			forward_caliculation(input_data[i], middle, output, weights);
			update=backward(input_data[i], middle, output, update, input_label[i]);
			change_weight(weights, update, rate, 1);
			for (int j = 0; j < input_label[i].size(); j++) {
				sum_error += -input_label[i][j] * log(output[j]);
			}
		}
		cout << count << ":" << sum_error/input_size << endl;
		count++;

	}
	/*
	//入力の順番を毎回ランダムにした場合
	while (learning_times>count) {
		input = input_data;
		label = input_label;
		sum_error = 0;
		//逐次学習の1エポック
		for (int i = 0; i < input_size; i++) {
			int unit = (int)rand() % input.size();
			forward_caliculation(input[unit], middle, output, weights, label[unit], error);

			backward(input[unit], middle, output, weights,update, error);
			change_weight(weights, update,rate);
			input.erase(input.begin() + unit);
			label.erase(label.begin() + unit);
		}
		for (int j = 0; j < error.size(); j++) {
			sum_error += error[j] * error[j];
		}
		cout << sum_error << endl;
		count++;
	}
	*/
	cout << "試行回数：" << count << "," << "学習率:" << rate <<",コンポネント数:"<<middle_element<< endl;
	cout << "データ数" << input_data.size() << endl;
	
	string file_d1("patarn1.csv");
	string file_d2("patarn2.csv");
	string file_d3("patarn3.csv");
	string file_d4("patarn4.csv");
	//ifstream file_d("input.txt");
	string file_l1("label1.csv");
	string file_l2("label2.csv");
	string file_l3("label3.csv");
	string file_l4("label4.csv");
	//ifstream file_l("teacher.txt");
	answer_rate(file_d1, file_l1, weights, middle, output);
	answer_rate(file_d2, file_l2, weights, middle, output);
	answer_rate(file_d3, file_l3, weights, middle, output);
	answer_rated4(file_d4, file_l4, weights, middle, output);
}

vector<vector<vector<double>>> sum_update(vector<vector<vector<vector<double>>>>all_update, vector<vector<vector<double>>>update) {
	int input_size = all_update.size();
	int update_size = update.size();
	for (int i = 0; i < input_size; i++) {
		for (int j = 0; j < update_size; j++) {
			for (int k = 0; k < update[j].size(); k++) {
				for (int l = 0; l < update[j][k].size(); l++) {
					update[j][k][l] += all_update[i][j][k][l];
				}
			}
		}
	}
	return update;
}

//一括学習を行う関数
void learn_patch(vector<vector<double>>& input_data, vector<vector<double>>& input_label) {
	int input_element = input_data[0].size();
	int input_size = input_data.size();
	int output_element = input_label[0].size();
	int label_size, middle_element;
	int learning_times = 0;
	double rate = 0;
	double sum_error = 1;
	vector<vector<double>> middle((0, 0));
	vector<vector<vector<double>>> weights((0, 0));
	vector<vector<vector<double>>> update((0, 0));
	vector<double> output(0, 0);

	label_size = output_element;
	cout << "The number of component=";
	scanf_s("%d", &middle_element);
	cout << "Learning rate=";
	scanf_s("%lf", &rate);
	cout << "Learning times=";
	scanf_s("%d", &learning_times);

	make_weight(weights, update,input_element, label_size, middle_element);
	make_box(middle, output, input_element, label_size, middle_element);
	vector<vector<vector<double>>>all_middle(input_size, middle);
	vector<vector<double>>all_output(input_size, output);
	vector<vector<vector<vector<double>>>>all_update(input_size, update);
	vector<vector<vector<double>>> copy = update;
	vector<vector<vector<double>>> reset = update;

	//更新値を保存する箱の初期化
	for (int p = 0; p < copy.size(); p++) {
		for (int q = 0; q < copy[p].size(); q++) {
			for (int r = 0; r < copy[p][q].size(); r++) {
				reset[p][q][r] = 0;
			}
		}
	}

	int count = 0;
	//指定した学習回数分学習させる
	while (learning_times > count) {
		sum_error = 0;
		//一括学習の1エポック
		for (int i = 0; i < input_size; i++) {
			forward_caliculation(input_data[i], all_middle[i], all_output[i], weights);
			all_update[i]=backward(input_data[i], all_middle[i], all_output[i], all_update[i],input_label[i]);
			for (int j = 0; j < input_label[i].size(); j++) {
				sum_error += -input_label[i][j] * log(all_output[i][j]);
			}
		}
		update = sum_update(all_update, reset);
		
		double size = input_size;
		//change_weight(weights, update, rate, 1);
		cout << count << ":" << sum_error/size << endl;
		change_weight(weights, update,rate,size);
		count++;
	}
	cout << "試行回数：" << count << "," << "学習率:" << rate <<",コンポネント数:"<<middle_element<< endl;
	cout << "データ数" << input_data.size() << endl;

	string file_d1("patarn1.csv");
	string file_d2("patarn2.csv");
	string file_d3("patarn3.csv");
	string file_d4("patarn4.csv");
	//ifstream file_d("input.txt");
	string file_l1("label1.csv");
	string file_l2("label2.csv");
	string file_l3("label3.csv");
	string file_l4("label4.csv");
	//ifstream file_l("teacher.txt");
	answer_rate(file_d1, file_l1, weights, middle, output);
	answer_rate(file_d2, file_l2, weights, middle, output);
	answer_rate(file_d3, file_l3, weights, middle, output);
	answer_rated4(file_d4, file_l4, weights, middle, output);
}
