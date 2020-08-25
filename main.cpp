#include <iostream>
#include <cstring>
#include <fstream>
#include <locale.h>
#include <ctype.h>
#include <windows.h>

/*
	Tema do trabalho: Rob�s;
	M�todo de busca: Bin�ria;
	M�todo de ordena��o: Shell sort;
	Modo de grava��o: Texto.
*/

using namespace std;

struct robot {
	int id;
	string type;
	int creation_year;
	string description;
	string main_use;
};

int menu();

void createRobot(robot robots[], int* used_space);

void selectRobots(robot robots[], int tam);

void selectUniqueRobot(robot robot);

void removeRobot(robot robots[], int* used_space, int index);

void updateRobot(robot robots[], int index, int changed_attribute);

void readFromFile(robot robots[], int* used_space);

void writeInFile(robot robots[], int used_space);

int binarySearch(robot robots[], int start_pos, int end_pos, int value);

void shellSort(robot robots[], int size);

int main() {
	
	setlocale(LC_ALL,"");
	
	const int TAM = 100;
	int option, used_space = 0;
	
	robot* robots = new robot[TAM];
	
	readFromFile(robots, &used_space);
	
	do {
		
		system("cls");
		
		option = menu();
		
		switch(option) {
			case 1: {
				if(used_space + 1 > TAM) {
					cout << endl << "N�o � poss�vel inserir um novo registro! Capacidade m�xima de aloca��o atingida." << endl << endl;
				} else {
					cout << endl << ">> NOVO REGISTRO <<" << endl;
					
					createRobot(robots, &used_space);
						
					shellSort(robots, used_space);
						
					cout << endl << "Registro inclu�do com sucesso!" << endl << endl;
				}
				break;
			}
			
			case 2: {
				int robotID;
			
				cout << endl << "Informe o ID do registro que voc� deseja deletar: ";
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
						cout << endl << "Opera��o cancelada!" << endl << endl;
					}
					
				} else {
					cout << endl << "N�o foi poss�vel encontrar um registro com esse ID!" << endl << endl;
				}
				
				break;
			}
			
			case 3: {
				cout << endl << ">> REGISTROS CADASTRADOS <<" << endl << endl;
				selectRobots(robots, used_space);
				
				break;
			}
			
			case 4: {
				int alteredID, changed_attribute;
			
				cout << endl << "Informe o ID do rob� que voc� deseja alterar: ";
				cin >> alteredID;
				
				int index = binarySearch(robots, 0, used_space, alteredID);
				
				if(index != -1) {
					int changed_attribute;
					
					cout << endl << "> Qual informa��o voc� deseja alterar?" << endl << endl;
					cout << "1) Tipo" << endl;
					cout << "2) Ano de cria��o" << endl;
					cout << "3) Descri��o" << endl;
					cout << "4) Uso principal" << endl;
					
					do{
						cout << endl << "R: ";
						cin >> changed_attribute;
						
						if(changed_attribute < 1 or changed_attribute > 4) {
							cout << endl << "Informe um n�mero v�lido!" << endl;
						}
					} while(changed_attribute < 1 or changed_attribute > 4);
					
					updateRobot(robots, index, changed_attribute);
				} else {
					cout << endl << "Registro n�o encontrado!" << endl << endl;
				}
				
				break;
			}
			
			case 5: {
				writeInFile(robots, used_space);
				cout << endl << "Grava��o realizada com sucesso!" << endl << endl;
				
				break;
			}
			
			case 6: {
				cout << endl << "Deseja salvar as modifica��es realizadas?" << endl;
			
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
				
				break;
			}
		}
		
		// C�digo que pausa o programa ao final da execu��o de cada op��o, desde que n�o seja requisitada a sa�da do programa
		// Serve apenas para melhorar a experi�ncia do usu�rio com o programa
		if(option != 6) {
			system("pause");
		}
		
	} while(option != 6);
	
	// Desaloca a mem�ria do vetor de registros
	delete[] robots;
	
	return 0;
	
}

//Menu para intera��o com o usu�rio
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

// Cria o novo registro dentro do vetor de registros, testando se a ID inserinda j� existe no vetor ou n�o
void createRobot(robot robots[], int* used_space) {
	robot newRobot;
	int index;
	
	do{
		cout << "ID: ";
		cin >> newRobot.id;
		
		index = binarySearch(robots, 0, *used_space, newRobot.id);
		
		if(index != -1) {
			cout << endl << "ID j� cadastrado! Digite outro." << endl << endl;
		}
	} while(index != -1);
			
	cout << "Tipo: ";
	cin.ignore();
	getline(cin, newRobot.type);
			
	cout << "Ano de cria��o: ";
	cin >> newRobot.creation_year;
			
	cout << "Descri��o: ";
	cin.ignore();
	getline(cin, newRobot.description);
			
	cout << "Uso principal: ";
	getline(cin, newRobot.main_use);
	
	robots[*used_space] = newRobot;
	*used_space += 1;
}

// Mostra na tela todos os registros no vetor
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

// Mostra na tela um registro espec�fico pedido pela main
void selectUniqueRobot(robot robot) {
	cout << "ID: " << robot.id << endl;
	cout << "Tipo: " << robot.type << endl;
	cout << "Ano de cria��o: " << robot.creation_year << endl;
	cout << "Descri��o: " << robot.description << endl;
	cout << "Uso principal: " << robot.main_use << endl << endl;
}

// Fun��o que deleta o registro com a ID escolhida
void removeRobot(robot robots[], int* used_space, int index) {
	for(int i=index; i<*used_space - 1; i++) {
		robots[i] = robots[i+1];
	}
					
	*used_space -= 1;
}

/* Fun��o que atualiza a informa��o com o novo valor. A variavel changed_attribute 
informa qual a op��o escolhida pelo usu�rio e dever� ser atualizada */
void updateRobot(robot robots[], int index, int changed_attribute) {
	if(changed_attribute == 1) {
		
		cout << endl << "Tipo (atual: " << robots[index].type << "): ";
		cin.ignore();
		getline(cin, robots[index].type);
		
	} else if(changed_attribute == 2) {
		
		cout << endl << "Ano de Cria��o (atual: " << robots[index].creation_year << "): ";
		cin.ignore();
		cin >> robots[index].creation_year;
		
	} else if(changed_attribute == 3) {
		
		cout << endl << "Descri��o (atual: " << robots[index].description << "): ";
		cin.ignore();
		getline(cin, robots[index].description);
		
	} else {
		
		cout << endl << "Uso principal (atual: " << robots[index].main_use << "): ";
		cin.ignore();
		getline(cin, robots[index].main_use);
		
	}
	
	cout << endl << "Valor alterado com sucesso!" << endl << endl;
}

// L� as informa��es do arquivo para o vetor de registros
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

// Escreve as informa��es do vetor no arquivo
void writeInFile(robot robots[], int used_space) {
	ofstream write_file("data.txt");
			
	write_file << used_space << endl;
			
	for(int i=0; i<used_space; i++) {
		write_file << robots[i].id << endl;
		write_file << robots[i].type << endl;
		write_file << robots[i].creation_year << endl;
		write_file << robots[i].description << endl;
		write_file << robots[i].main_use << endl << endl;
	}
	
	write_file.close();
}

// Busca bin�ria para encontrar um ID dentro do vetor de registros
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

// M�todo de ordena��o Shell sort
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
