#include <iostream>
#include <string>
#include <fstream>
#include <locale.h>
#include <windows.h>

using namespace std;

struct robot {
	int id;
	string type;
	int creation_year;
	string description;
	string main_use;
};

int menu();

void createRobot(robot* robot);

void selectRobots(robot robots[], int tam);

int binarySearch(robot robots[], int start_pos, int end_pos, int value);

void shellSort(robot robots[], int size);

int main() {
	
	setlocale(LC_ALL,"");
	
	const int tam = 3;
	int option, usedSpace;
	
	fstream file("data.txt", ios::app | ios::in | ios::out | fstream::out);
		
	if(!file) {
		return 0;
	}
	
	robot* robots = new robot[tam];
	
	file >> usedSpace;
	
	if(usedSpace != 0) {
		for(int i=0; i<usedSpace; i++) {
			file >> robots[i].id;
			file >> robots[i].type;
			file >> robots[i].creation_year;
			file >> robots[i].description;
			file >> robots[i].main_use;
		}
	}
	
	do {
		
		system("cls");
		
		option = menu();
		
		if(option == 1) {
			
			if(usedSpace + 1 > tam) {
				cout << endl << "Não é possível inserir novo registro! Capacidade máxima de alocação atingida." << endl << endl;
			} else {
				cout << endl << ">> NOVO ROBÔ <<" << endl;
				
				robot newRobot;
			
				createRobot(&newRobot);
				
				robots[usedSpace] = newRobot;
				usedSpace++;
			}
			
			system("pause");
			
		} else if(option == 2) {
			
			cout << "excluir";
			
			system("pause");
			
		} else if(option == 3) {
			
			cout << endl << ">> ROBÔS CADASTRADOS <<" << endl << endl;
			
			selectRobots(robots, usedSpace);
			
			system("pause");
			
		} else if(option == 4) {
			
			cout << "alterar";
			
			system("pause");
			
		} else if(option == 5) {
			
			cout << "gravar";
			
			system("pause");
			
		}
			
	} while(option != 6);
	
	delete[] robots;
	
	return 0;
	
}

int menu() {
	int answer;
	
	cout << ">> MENU <<" << endl << endl;
	cout << "1) Incluir" << endl;
	cout << "2) Excluir" << endl;
	cout << "3) Listar" << endl;
	cout << "4) Alterar" << endl;
	cout << "5) Gravar dados" << endl;
	cout << "6) Sair" << endl << endl;
	
	do {
		
		cout << "R: ";
		cin >> answer;
		
		if(answer < 1 or answer > 6) {
			cout << "Opção não existente na lista!" << endl << endl;
		}
		
	} while(answer < 1 or answer > 6);
}

void createRobot(robot* robot) {
	cout << "ID: ";
	cin >> robot->id;
			
	cout << "Tipo: ";
	cin >> robot->type;
			
	cout << "Ano de criação: ";
	cin >> robot->creation_year;
			
	cout << "Descrição: ";
	cin >> robot->description;
			
	cout << "Uso principal: ";
	cin >> robot->main_use;
}

void selectRobots(robot robots[], int tam) {
	for(int i=0; i<tam; i++) {
		cout << "> ROBÔ " << i+1 << endl;
		cout << "ID: " << robots[i].id << endl;
		cout << "Tipo: " << robots[i].type << endl;
		cout << "Ano de criação: " << robots[i].creation_year << endl;
		cout << "Descrição: " << robots[i].description << endl;
		cout << "Uso principal: " << robots[i].main_use << endl << endl;
	}
}

int binarySearch(robot robots[], int start_pos, int end_pos, int value) {
	int half = (start_pos + end_pos) / 2;
	
	if(value == robots[half].id) {
		return half;
	} else if(start_pos < end_pos) {
		if(robots[half].id < value) {
			return binarySearch(robots, half+1, end_pos, value);
		} else {
			return binarySearch(robots, start_pos, half-1, value);
		}
	} else {
		return -1;
	}
}

void shellSort(robot robots[], int size) {
	int gaps[9] = {1, 4, 10, 23, 57, 132, 301, 701, 1750};
	int gap_pos = 8;
	
	while(gaps[gap_pos] > size) {
		gap_pos--;
	}
	
	robot value;
	int j;
	
	while(gap_pos >= 0) {
		int gap = gaps[gap_pos];
		
		for(int i=gap; i<size; i++) {
			value = robots[i];
			j = i;
			
			while((j >= gap) and (value.id < robots[j - gap].id)) {
				robots[j] = robots[j - gap];
				j -= gap;
			}
			
			robots[j] = value;
		}
		
		gap_pos--;
	}
}
