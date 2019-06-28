#include<iostream>
#include<fstream>
#include<string.h>
#include<string>
#include<vector>
#include<iomanip>
#define MAX 10000
#define MAX2 100
using namespace std;
//�ı�����%Ϊ->����#Ϊ��
fstream token, production_txt, prase_table, first_follow, print_result;//�ֱ��token�Ͳ���ʽ���ĵ�

char token_char[MAX] = { '0' };
char production[MAX] = { '0' };
char terminal[MAX2][MAX2] = { "#", ";", "[", "int", "void", "]", "(", ")", ",", "{", "}", "if", "else", "while", "return", "=", "<", ">", "!", "+", "-", "*", "/", "ID", "NUM", "==", "!="};
string unterminal[MAX2];//ÿ�����ս��
string tree[MAX];//�洢�Ƶ�ʽǰ���string
bool judge[MAX];
 int treeNUM = 0;//�洢�Ƶ�ʽ����
int unterminal_number = 0;//����ʽ����
int proce_number[MAX2];//�洢ÿ�����ս���м�������ʽ




vector<string>first[MAX2];//���ÿһ�����ս����first
vector<string>follow[MAX2];//���ÿһ�����ս����follow
vector<string>proce[MAX2][MAX2];//ÿ�����ս����ÿ������ʽ
vector<string>first_proc[MAX2][MAX2];//�洢ÿ�����ս��ÿ������ʽ��first
vector<string> TOKENS;//�洢token
vector<string> PraseTable[MAX2][MAX2];//�洢Ԥ���������ά����
vector<string>PraseStack;//�洢����ջ
vector<string>tree_stack[MAX];//�洢�Ƶ�ʽ������ַ���
vector<string>final_stack;//ϴ�빤�����һ��vector����Ҫ�������������


void OpenSourceFile();
void Proc_Readin();
void First(int i);
void Follow(int i);
bool Isterminal(string temp);
int WhichUnterminal(string temp);
bool IsInFirst(string temp, int a);
bool IsInFollow(string temp, int a);
bool Have_��(int i);
void write_first_follow();
void Produce_Parse();
int Have_terminal(int a, string temp);
bool Have_follow(int a, string temp);
void Reading_tokens();
int Whichterminal(string temp);
void transTOKENS();
void Tree();
int WhichFinalPop(string temp);


int main() {
	char choose;
	char print_result_char[MAX] = {'0'};
	OpenSourceFile();
	Proc_Readin();
	for (int i = 0; i < unterminal_number; i++) {
		if(first[i].size() == 0)
			First(i);
	}
	for (int i = 0; i < unterminal_number; i++) {
		if(follow[i].size() == 0)
			Follow(i);
	}
	write_first_follow();
	Produce_Parse();
	Reading_tokens();
	transTOKENS();
	token.close();
	production_txt.close();
 	prase_table.close();
	first_follow.close();
	final_stack.push_back(tree[0]);
	cout << "�Ƿ��ӡ���� Y/N" << endl;
	cin >> choose;
	if(choose == 'Y')
		Tree();

	cout << "�Ƿ��ӡFIRST&FOLLOW���ϣ� Y/N" << endl;
	cin >> choose;
	if (choose == 'Y') {
		print_result.open("FIRST&&FOLLOW.txt", ios::in | ios::binary);
		print_result.read((char *)&print_result_char, sizeof(print_result_char));
		cout << print_result_char << endl;
		print_result.close();
	}
	cout << "�Ƿ��ӡPraseTable? Y/N" << endl;
	cin >> choose;
	if (choose == 'Y') {
		print_result.open("PraseTable.txt", ios::in | ios::binary);
		print_result.read((char *)&print_result_char, sizeof(print_result_char));
		int aa = strlen(print_result_char);
		for (int i = 0; i < aa; i++)
			cout << print_result_char[i];
		cout << endl;
		print_result.close();
	}






	system("pause");
}
void Tree() {
	cout << final_stack[0] << endl;
	final_stack.push_back("eof");
	while (final_stack.size() != 1) {
		string temp = final_stack.front();
		final_stack.erase(final_stack.begin(),  final_stack.begin() + 1);
		if (!Isterminal(temp) && temp != "eof") {
			int a = WhichFinalPop(temp);
			if (a == -1)
				continue;
		for (int i = 0; i < tree_stack[a].size(); i++) {
				cout << tree_stack[a][tree_stack[a].size() - 1 - i] << " ";
				final_stack.push_back(tree_stack[a][tree_stack[a].size() - 1 - i]);
			}
		}
		else if (temp == "eof") {
			cout << endl;
			final_stack.push_back("eof");
		}
	}
}
int WhichFinalPop(string temp) {
	for (int i = 0; i < treeNUM; i++) {
		if (tree[i] == temp && !judge[i]) {
			judge[i] = true;
			return i;
		}
	}
	return -1;
}
void Reading_tokens() {
	token.open("tokens_prase.txt", ios::in | ios::binary);
	if (token.fail()) {
		cout << "tokens_prase.txt��ʧ��" << endl;
	}
	token.read((char *)&token_char, sizeof(token_char));
	string temp = "";
	int token_lenth = strlen(token_char);
	token_char[token_lenth] = '\n';
	int token_a = 0;
	while (token_a < token_lenth) {
		if (token_char[token_a] != '\r' && token_char[token_a] != '\n') {
			temp += token_char[token_a];
			token_a += 1;
		}
		else if (temp != "") {
			TOKENS.push_back(temp);
			temp = "";
			token_a += 1;
		}
		else
			token_a += 1;
	}
}
void transTOKENS() {
	PraseStack.push_back(unterminal[0]);
	for (int i = 0; i < TOKENS.size();) {
		string temp = PraseStack.back();
		if (!Isterminal(temp)) {
			cout << left << setw(6) << i;
			cout << "$ ";
			for (int j = 0; j < PraseStack.size(); j++)
				cout << PraseStack[j] << " ";
			cout << endl;
			tree[treeNUM] = temp;
			treeNUM += 1;
			PraseStack.pop_back();
			int a = WhichUnterminal(temp);
			int b = Whichterminal(TOKENS[i]);
			for (int j = 0; j < PraseTable[a][b].size(); j++) {
				string temp_stack = PraseTable[a][b][PraseTable[a][b].size() - j - 1];
				tree_stack[treeNUM - 1].push_back(temp_stack);
				if(temp_stack != "��" && temp_stack != "error")
					PraseStack.push_back(temp_stack);
				else if (temp_stack == "error") {
					cout << "�����������﷨����" << endl << "����λ���ڣ�" << TOKENS[i] << endl;
					return;
				}
			}
		}
		else {
			//���ս��match��ȥ
			if (temp == TOKENS[i]) {
				PraseStack.pop_back();
				i += 1;
			}
			else {
				cout << "�����������﷨����" << endl << "����λ���ڣ�" << TOKENS[i] << endl;
				return;
			}
		}
	}
	cout << endl << endl;
	cout << "ACCEPT������û���﷨����" << endl << endl;
}
void OpenSourceFile() {
	production_txt.open("Cminus.txt", ios::in | ios::binary);
	if (production_txt.fail()) {
		cout << "Cminus����ʽ�ı�������" << endl;
		return;
	}
	production_txt.read((char *)& production, sizeof(production));
	production[strlen(production)] = '\r';
}
void Proc_Readin() {
	int size = strlen(production);
	for (int i = 0; i < size; i++) {
		string temp;
		while (i < size && production[i] != '%' ) {
			temp += production[i];
			i += 1;
		}
		i += 1;//�ѡ��Ե�
		unterminal[unterminal_number] = temp;
		unterminal_number += 1;
		temp = "";
		int a = 0;//��¼�м�������ʽ
		while (production[i] != '\r') {
			if (production[i] == '|') {
				proce[unterminal_number - 1][a].push_back(temp);
				temp = "";
				a += 1;
			}
			else if (production[i] == ' ') {
				proce[unterminal_number - 1][a].push_back(temp);
				temp = "";
			}
			else {
				temp += production[i];
			}
			i += 1;
		}
		if(temp != "")
			proce[unterminal_number - 1][a].push_back(temp);
		a += 1;
		proce_number[unterminal_number - 1] = a;
		i += 1;//�Թ�\n
	}
}
void First(int i) {
	for (int j = 0; j < proce_number[i]; j++) {
		int k = 0;
		//�������ʽ��һ�����ս���������first
		if (Isterminal(proce[i][j][k])) {
			if (!IsInFirst(proce[i][j][k], i))
				first_proc[i][j].push_back(proce[i][j][k]);
				first[i].push_back(proce[i][j][k]);
		}
		else if (proce[i][j][k] == "#") {
			//��һ���Ǧ�
			first_proc[i][j].push_back("#");
			first[i].push_back("#");
		}
		//�Ƿ��ս��ʱ
		else {
			//ֱ���еķ��ս��û�ЦŻ����������ս��
			while (k < proce[i][j].size() && !Isterminal(proce[i][j][k]) ) {
				int a = WhichUnterminal(proce[i][j][k]);
				if(first[a].size() == 0)
					First(a);
				if (!Have_��(a)) {
					//��һ���ս����first���ަţ�ֱ�Ӷ�������
					for (int b = 0; b < first[a].size(); b++) {
						if (!IsInFirst(first[a][b], i)) {
							first_proc[i][j].push_back(first[a][b]);
							first[i].push_back(first[a][b]);
						}
					}
					break;
				}
				else{
					for (int b = 0; b < first[a].size(); b++) {
						//�����ս���е��ս���ڸñ��ʽ��û��ʱ
						if (!IsInFirst(first[a][b], i)) {
							if (first[a][b] != "#") {
								first_proc[i][j].push_back(first[a][b]);
								first[i].push_back(first[a][b]);
							}
							//�����ս�����Ц�ʱ
							else {
								if (k == proce[i][j].size() - 1) {
									//�������һ�����ս��ʱ�����first��û�Цţ������
									if (!IsInFirst(first[a][b], i)) {
										first_proc[i][j].push_back(first[a][b]);
										first[i].push_back(first[a][b]);
									}
								}
								//�������һ�������Ѿ������˦ţ��������һ��
							}
						}
					}
				}
				k += 1;
			}
			//�����ս��
			if (k < proce[i][j].size()) {
				if (Isterminal(proce[i][j][k])) {
					//����Ϊ���ս������ѭ�����ս������first����
					if (!IsInFirst(proce[i][j][k], i)) {
						first_proc[i][j].push_back(proce[i][j][k]);
						first[i].push_back(proce[i][j][k]);
					}
				}
			}
		}
	}
}
void Follow(int xx) {
	if (xx == 0)
		follow[xx].push_back("$");
	//�������з��ս���Ĳ���ʽ
	for (int i = 0; i < unterminal_number; i++) {
		if (xx == 29 && i == 25) {
			int mm = 0;
		}
		//����ÿ�����ս����ÿһ������ʽ
		for (int j = 0; j < proce_number[i]; j++) {
			for (int k = 0; k < proce[i][j].size(); k++) {
				if (unterminal[xx] == proce[i][j][k]) {
					//���滹�з���
					if (k < proce[i][j].size() - 1) {
						if (Isterminal(proce[i][j][k + 1])) {
							//����ķ������ս��
							if(!IsInFollow(proce[i][j][k + 1], xx))
								follow[xx].push_back(proce[i][j][k + 1]);
						}
						else {
							//����ķ����Ƿ��ս��
							int a = WhichUnterminal(proce[i][j][k + 1]);
							if (follow[a].size() == 0)
								Follow(a);
							if (!Have_��(a)) {
								//����ķ��ս��������
								for (int b = 0; b < first[a].size(); b++) {
									if (!IsInFollow(first[a][b], xx))
										follow[xx].push_back(first[a][b]);
								}
							}
							else {
								//������ս������
								int temp_k = k + 1;
								while (temp_k < proce[i][j].size() && Have_��(a)) {
									for (int b = 0; b < first[a].size(); b++) {
										if (first[a][b] != "#") {
											if (!IsInFollow(first[a][b], xx))
												follow[xx].push_back(first[a][b]);
										}
									}
									temp_k += 1;
									//���ս��
									if (temp_k == proce[i][j].size())//���һλ
										break;
									if (Isterminal(proce[i][j][temp_k])) {
										if (!IsInFollow(proce[i][j][temp_k], xx))
											follow[xx].push_back(proce[i][j][temp_k]);
										break;
									}
									else {
										a = WhichUnterminal(proce[i][j][temp_k]);
										//if (a == -1)
										//	continue;
										if (follow[a].size() == 0)
											Follow(a);
										if (!Have_��(a)) {
											for (int b = 0; b < first[a].size(); b++) {
												if (!IsInFollow(first[a][b], xx))
													follow[xx].push_back(first[a][b]);
											}
										}
									}
								}
								if (temp_k == proce[i][j].size()) {
									if (follow[i].size() == 0)
										Follow(i);
									for (int b = 0; b < follow[i].size(); b++) {
										if (!IsInFollow(follow[i][b], xx))
											follow[xx].push_back(follow[i][b]);
									}
								}
							}
						}
					}
					else {
						//����û�з�����
						if (xx == i)
							continue;
						if(follow[i].size() == 0)
							Follow(i);
						for (int b = 0; b < follow[i].size(); b++) {
							if(!IsInFollow(follow[i][b], xx))
							follow[xx].push_back(follow[i][b]);
						}
					}
				}
			}
		}
	}
}
bool Isterminal(string temp) {
	for (int i = 0; i < 26; i++) {
		string temp_compara = terminal[i + 1];
		if (temp == temp_compara)
			return true;
	}
	return false;
}
int WhichUnterminal(string temp) {
	for (int i = 0; i < unterminal_number; i++) {
		if (temp == unterminal[i])
			return i;
	}
//	return -1;
}
//�жϸ��ս���Ƿ��Ѿ�����first����
bool IsInFirst(string temp, int a) {
	for (int i = 0; i < first[a].size(); i++) {
		if (temp == first[a][i])
			return true;
	}
	return false;
}
bool IsInFollow(string temp, int a) {
	for (int i = 0; i < follow[a].size(); i++) {
		if (temp == follow[a][i])
			return true;
	}
	return false;
}
bool Have_��(int i) {
	//�ж�first�Ƿ��Ц�
	for (int j = 0; j < first[i].size(); j++) {
		if (first[i][j] == "#")
			return true;
	}
	return false;
}
void write_first_follow() {
	first_follow.open("FIRST&&FOLLOW.txt", ios::out | ios::binary);
	if (first_follow.fail()) {
		cout << "FIRST&&FOLLOW�ļ�������" << endl;
	}
	for (int i = 0; i < unterminal_number; i++) {
		first_follow << "Vu:  " + unterminal[i] + "\r\n";
		first_follow << "FIRST: ";
		for (int j = 0; j < first[i].size(); j++) {
			first_follow << first[i][j] << " ";
		}
		first_follow << "\r\n";
		first_follow << "FOLLOW: ";
		for (int j = 0; j < follow[i].size(); j++) {
			first_follow << follow[i][j] << " ";
		}
		first_follow << "\r\n\r\n";
	}
}
void Produce_Parse() {
	prase_table.open("PraseTable.txt", ios::out | ios::binary);
	string ky = " ";
	prase_table << left << setw(40) << ky ; 
	for (int i = 0; i < 26; i++) {
		prase_table << left <<setw(40) << terminal[i + 1] ;
	}
	prase_table<<left  << setw(40) << "$" ;
	prase_table << "\r\n";
	for (int i = 0; i < unterminal_number; i++) {
		prase_table << left << setw(40) << unterminal[i] ;
		if (!Have_��(i)) {
			//first������û�Ц�
			for (int j = 0; j < 26; j++) {
				string temp = terminal[j + 1];
				int a = Have_terminal(i, temp);
				if (a != -1) {
					//���ս����first���и��ս����aΪ���ս������������ʽ
					int temp_strlen = unterminal[i].size() + 4;
					prase_table << unterminal[i] + " -> ";
					for (int b = 0; b < proce[i][a].size(); b++) {
						prase_table << proce[i][a][b] << " ";
						PraseTable[i][j + 1].push_back(proce[i][a][b]);
						temp_strlen = temp_strlen + proce[i][a][b].size() + 1;
					}
					prase_table << setw(40 - temp_strlen) << ky ;
				}
				else {
					//���ս�����޸�first
					prase_table << setw(40) << "error" ;
					PraseTable[i][j + 1] .push_back("error");
				}
			}
		}
		else {
			//�÷��ս�����Ц�
			for (int j = 0; j < 26; j++) {
				string temp = terminal[j + 1];
				//�ж�first���Ƿ���
				int a = Have_terminal(i, temp);
				if (a != -1) {
					//���ս����first���и��ս����aΪ���ս������������ʽ
					int temp_strlen = unterminal[i].size() + 4;
					prase_table << unterminal[i] + " -> ";
					for (int b = 0; b < proce[i][a].size(); b++) {
						PraseTable[i][j + 1].push_back(proce[i][a][b]);
						prase_table << proce[i][a][b] << " ";
						temp_strlen = temp_strlen + proce[i][a][b].size() + 1;
					}
					prase_table << setw(40 - temp_strlen) << ky ;
				}
				else {
					//���ս�����޸�first
					if (Have_follow(i, temp)) {
						//���ս����follow��
						int temp_strlen = unterminal[i].size() + 4;
						prase_table << unterminal[i] + " -> " + "��";
						prase_table << setw(40 - temp_strlen - strlen("��")) ;
						PraseTable[i][j + 1].push_back("��");
					}
					else {
						//�Ȳ���firstҲ����follow��
						prase_table << setw(40) << "error" ;
						PraseTable[i][j + 1] .push_back("error");
					}
				}
			}
		}
		prase_table << "\r\n";
	}
}
int Have_terminal(int a, string temp) {
	//�ж��Ƿ��ս�����ĸ�����ʽ
	for (int i = 0; i < proce_number[a]; i++) {
		for (int j = 0; j < first_proc[a][i].size(); j++) {
			if (first_proc[a][i][j] == temp) {
				return i;
			}
		}
	}
	return -1;
}
bool Have_follow(int a, string temp){
	//�ж�follow�������Ƿ��и��ս��
	for (int i = 0; i < follow[a].size(); i++) {
		if (temp == follow[a][i])
			return true;
	}
	return false;
}
int Whichterminal(string temp) {
	for (int i = 0; i < 26; i++) {
		string temp_terminal = terminal[i + 1];
		if (temp == temp_terminal)
			return i + 1;
	}
}