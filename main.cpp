#include <iostream>
#include <string>
#include <fstream>
#include <ctype.h>
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

void selectUniqueRobot(robot robot);

void writeInFile(robot robots[], int usedSpace);

int binarySearch(robot robots[], int start_pos, int end_pos, int value);

void shellSort(robot robots[], int size);

int main() {
	
	setlocale(LC_ALL,"");
	
	const int tam = 4;
	int option, usedSpace = 0;
	
	robot* robots = new robot[tam];
	
	ifstream read_file("data.txt");
	
	if(!read_file) {
		return 0;
	}
	
	read_file >> usedSpace;
	
	if(usedSpace != 0) {
		for(int i=0; i<usedSpace; i++) {
			read_file >> robots[i].id;
			read_file >> robots[i].type;
			read_file >> robots[i].creation_year;
			read_file >> robots[i].description;
			read_file >> robots[i].main_use;
		}
	}
	
	read_file.close();
	
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
				
				if(binarySearch(robots, 0, usedSpace, newRobot.id) == -1) {
					robots[usedSpace] = newRobot;
					usedSpace++;
					
					shellSort(robots, usedSpace);
					
					cout << endl << "Robô cadastrado com sucesso!" << endl;
				} else {
					cout << endl << "Já existe um robô com esse ID. Tente cadastrar com um número diferente!" << endl;
				}
				
				cout << endl;
			}
			
		} else if(option == 2) {
			
			int robotID;
			
			cout << endl << "Informe o ID do robô que você deseja deletar: ";
			cin >> robotID;
			
			int index = binarySearch(robots, 0, usedSpace, robotID);
			
			if(index != -1) {
				cout << endl << "> Você tem certeza que deseja deletar esse robô?" << endl << endl;
				
				selectUniqueRobot(robots[index]);
				
				char deleteRobot;
				
				do {
					cout << "R (S/N): ";
					cin >> deleteRobot;
					
					deleteRobot = toupper(deleteRobot);
					
					if(deleteRobot != 'S' and deleteRobot != 'N') {
						cout << endl << "Informe o caractere correto!" << endl;
					}
				} while(deleteRobot != 'S' and deleteRobot != 'N');
				
				if(deleteRobot == 'S') {
					for(int i=index; i<usedSpace; i++) {
						robots[i] = robots[i+1];
					}
					
					usedSpace--;
					
					cout << endl << "Robô deletado com sucesso!" << endl << endl;
				} else {
					cout << endl << "Operação de remoção de um robô cancelada!" << endl << endl;
				}
				
			} else {
				cout << endl << "Não foi possível encontrar um robô com esse ID!" << endl << endl;
			}
			
		} else if(option == 3) {
			
			cout << endl << ">> ROBÔS CADASTRADOS <<" << endl << endl;
			
			selectRobots(robots, usedSpace);
			
		} else if(option == 4) {
			
			cout << "alterar";
			
		} else if(option == 5) {
			
			writeInFile(robots, usedSpace);
			
			cout << endl << "Gravação realizada com sucesso!" << endl << endl;
			
		} else if(option == 6) {
			
			char save;
			
			cout << endl << "Deseja sair sem salvar as alteraçõess realizadas?" << endl;
			
			do {
				cout << "R (S/N): ";
				cin >> save;
				
				save = toupper(save);
				
				if(save != 'S' and save != 'N') {
					cout << endl << "Informe o caractere correto!" << endl;
				}
			} while(save != 'S' and save != 'N');
			
			if(save == 'S') {
				writeInFile(robots, usedSpace);
			
				cout << endl << "Gravação realizada com sucesso!" << endl << endl;
			}
			
			cout << "Saindo do aplicativo..." << endl << endl;
			
		}
		
		system("pause");
			
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
	
	return answer;
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

void selectUniqueRobot(robot robot) {
	cout << "ID: " << robot.id << endl;
	cout << "Tipo: " << robot.type << endl;
	cout << "Ano de criação: " << robot.creation_year << endl;
	cout << "Descrição: " << robot.description << endl;
	cout << "Uso principal: " << robot.main_use << endl << endl;
}

void writeInFile(robot robots[], int usedSpace) {
	ofstream write_file("data.txt");
			
	write_file << usedSpace << endl;
			
	for(int i=0; i<usedSpace; i++) {
		write_file << robots[i].id << " " << robots[i].type << " " << robots[i].creation_year << " " << robots[i].description << " " << robots[i].main_use << endl;
	}
	
	write_file.close();
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
