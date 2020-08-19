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

void removeRobot(robot robots[], int* used_space, int index);

void updateRobot(robot* robot);

void readFromFile(robot robots[], int* used_space);

void writeInFile(robot robots[], int used_space);

int binarySearch(robot robots[], int start_pos, int end_pos, int value);

void shellSort(robot robots[], int size);

int main() {
	
	setlocale(LC_ALL,"");
	
	const int tam = 4;
	int option, used_space = 0;
	
	robot* robots = new robot[tam];
	
	readFromFile(robots, &used_space);
	
	do {
		
		system("cls");
		
		option = menu();
		
		if(option == 1) {
			if(used_space + 1 > tam) {
				cout << endl << "N�o � poss�vel inserir um novo registro! Capacidade m�xima de aloca��o atingida." << endl << endl;
			} else {
				cout << endl << ">> NOVO ROB� <<" << endl;
				
				robot newRobot;
				createRobot(&newRobot);
				
				if(binarySearch(robots, 0, used_space, newRobot.id) == -1) {
					robots[used_space] = newRobot;
					used_space++;
					
					shellSort(robots, used_space);
					
					cout << endl << "Registro inclu�do com sucesso!" << endl;
				} else {
					cout << endl << "J� existe um registro com esse ID. Tente cadastrar com um n�mero diferente!" << endl;
				}
				
				cout << endl;
			}
			
		} else if(option == 2) {
			int robotID;
			
			cout << endl << "Informe o ID do rob� que voc� deseja deletar: ";
			cin >> robotID;
			
			int index = binarySearch(robots, 0, used_space, robotID);
			
			if(index != -1) {
				cout << endl << "> Voc� tem certeza que deseja deletar esse rob�?" << endl << endl;
				
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
					removeRobot(robots, &used_space, index);
					
					cout << endl << "Registro exclu�do com sucesso!" << endl << endl;
				} else {
					cout << endl << "Opera��o de remo��o de um registro cancelada!" << endl << endl;
				}
				
			} else {
				cout << endl << "N�o foi poss�vel encontrar um registro com esse ID!" << endl << endl;
			}
		} else if(option == 3) {
			
			cout << endl << ">> ROB�S CADASTRADOS <<" << endl << endl;
			selectRobots(robots, used_space);
			
		} else if(option == 4) {
			int alteredID;
			
			cout << endl << "Informe o ID do rob� que voc� deseja alterar: ";
			cin >> alteredID;
			
			int index = binarySearch(robots, 0, used_space, alteredID);
			
			if(index != -1) {
				cout << endl << "> Informe os novos dados do rob�" << endl << endl;
				
				updateRobot(&robots[index]);
				
				cout << endl << "Registro alterado com sucesso!" << endl << endl;
			} else {
				cout << endl << "O ID inserido n�o est� associado a nenhum rob�!" << endl << endl;
			}
		} else if(option == 5) {
			writeInFile(robots, used_space);
			
			cout << endl << "Grava��o realizada com sucesso!" << endl << endl;
		} else if(option == 6) {
			cout << endl << "Deseja sair sem salvar as altera��ess realizadas?" << endl;
			
			char save;		
			
			do {
				cout << "R (S/N): ";
				cin >> save;
				
				save = toupper(save);
				
				if(save != 'S' and save != 'N') {
					cout << endl << "Informe o caractere correto!" << endl;
				}
			} while(save != 'S' and save != 'N');
			
			if(save == 'S') {
				writeInFile(robots, used_space);
				cout << endl << "Grava��o realizada com sucesso!" << endl;
			}
			
			cout << endl << "Saindo do programa...";
		}
		
		if(option != 6) {
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
			cout << "Op��o n�o existente na lista!" << endl << endl;
		}
		
	} while(answer < 1 or answer > 6);
	
	return answer;
}

void createRobot(robot* robot) {
	cout << "ID: ";
	cin >> robot->id;
			
	cout << "Tipo: ";
	cin >> robot->type;
			
	cout << "Ano de cria��o: ";
	cin >> robot->creation_year;
			
	cout << "Descri��o: ";
	cin >> robot->description;
			
	cout << "Uso principal: ";
	cin >> robot->main_use;
}

void selectRobots(robot robots[], int used_space) {
	cout << "N�mero de rob�s cadastrados: " << used_space << endl << endl;
	
	for(int i=0; i<used_space; i++) {
		cout << "> ROB� " << i+1 << endl;
		cout << "ID: " << robots[i].id << endl;
		cout << "Tipo: " << robots[i].type << endl;
		cout << "Ano de cria��o: " << robots[i].creation_year << endl;
		cout << "Descri��o: " << robots[i].description << endl;
		cout << "Uso principal: " << robots[i].main_use << endl << endl;
	}
}

void selectUniqueRobot(robot robot) {
	cout << "ID: " << robot.id << endl;
	cout << "Tipo: " << robot.type << endl;
	cout << "Ano de cria��o: " << robot.creation_year << endl;
	cout << "Descri��o: " << robot.description << endl;
	cout << "Uso principal: " << robot.main_use << endl << endl;
}

void removeRobot(robot robots[], int* used_space, int index) {
	for(int i=index; i<*used_space; i++) {
		robots[i] = robots[i+1];
	}
					
	*used_space -= 1;
}

void updateRobot(robot* robot) {
	cout << "Tipo (" << robot->type << "): ";
	cin >> robot->type;
	
	cout << "Ano de cria��o (" << robot->creation_year << "): ";
	cin >> robot->creation_year;
	
	cout << "Descri��o (" << robot->description << "): ";
	cin >> robot->description;
	
	cout << "Uso principal (" << robot->main_use << "): ";
	cin >> robot->main_use;
}

void readFromFile(robot robots[], int* used_space) {
	ifstream read_file("data.txt");
	
	read_file >> *used_space;
	
	if(used_space != 0) {
		for(int i=0; i<*used_space; i++) {
			read_file >> robots[i].id;
			read_file >> robots[i].type;
			read_file >> robots[i].creation_year;
			read_file >> robots[i].description;
			read_file >> robots[i].main_use;
		}
	}
	
	read_file.close();
}

void writeInFile(robot robots[], int used_space) {
	ofstream write_file("data.txt");
			
	write_file << used_space << endl;
			
	for(int i=0; i<used_space; i++) {
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
