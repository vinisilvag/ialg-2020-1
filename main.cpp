#include <iostream>
#include <cstring>
#include <fstream>
#include <locale.h>
#include <ctype.h>
#include <windows.h>

/*
	Tema do trabalho: Robôs;
	Método de busca: Binária;
	Método de ordenação: Shell sort;
	Modo de gravação: Texto.
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
					cout << endl << "Não é possível inserir um novo registro! Capacidade máxima de alocação atingida." << endl << endl;
				} else {
					cout << endl << ">> NOVO REGISTRO <<" << endl;
					
					createRobot(robots, &used_space);
						
					shellSort(robots, used_space);
						
					cout << endl << "Registro incluído com sucesso!" << endl << endl;
				}
				break;
			}
			
			case 2: {
				int robotID;
			
				cout << endl << "Informe o ID do registro que você deseja deletar: ";
				cin >> robotID;
				
				int index = binarySearch(robots, 0, used_space, robotID);
				
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
						removeRobot(robots, &used_space, index);
						
						cout << endl << "Registro excluído com sucesso!" << endl << endl;
					} else {
						cout << endl << "Operação cancelada!" << endl << endl;
					}
					
				} else {
					cout << endl << "Não foi possível encontrar um registro com esse ID!" << endl << endl;
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
			
				cout << endl << "Informe o ID do robô que você deseja alterar: ";
				cin >> alteredID;
				
				int index = binarySearch(robots, 0, used_space, alteredID);
				
				if(index != -1) {
					int changed_attribute;
					
					cout << endl << "> Qual informação você deseja alterar?" << endl << endl;
					cout << "1) Tipo" << endl;
					cout << "2) Ano de criação" << endl;
					cout << "3) Descrição" << endl;
					cout << "4) Uso principal" << endl;
					
					do{
						cout << endl << "R: ";
						cin >> changed_attribute;
						
						if(changed_attribute < 1 or changed_attribute > 4) {
							cout << endl << "Informe um número válido!" << endl;
						}
					} while(changed_attribute < 1 or changed_attribute > 4);
					
					updateRobot(robots, index, changed_attribute);
				} else {
					cout << endl << "Registro não encontrado!" << endl << endl;
				}
				
				break;
			}
			
			case 5: {
				writeInFile(robots, used_space);
				cout << endl << "Gravação realizada com sucesso!" << endl << endl;
				
				break;
			}
			
			case 6: {
				cout << endl << "Deseja salvar as modificações realizadas?" << endl;
			
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
					cout << endl << "Gravação realizada com sucesso!" << endl;
				}
				
				cout << endl << "Saindo do programa...";
				
				break;
			}
		}
		
		// Código que pausa o programa ao final da execução de cada opção, desde que não seja requisitada a saída do programa
		// Serve apenas para melhorar a experiência do usuário com o programa
		if(option != 6) {
			system("pause");
		}
		
	} while(option != 6);
	
	// Desaloca a memória do vetor de registros
	delete[] robots;
	
	return 0;
	
}

//Menu para interação com o usuário
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

// Cria o novo registro dentro do vetor de registros, testando se a ID inserinda já existe no vetor ou não
void createRobot(robot robots[], int* used_space) {
	robot newRobot;
	int index;
	
	do{
		cout << "ID: ";
		cin >> newRobot.id;
		
		index = binarySearch(robots, 0, *used_space, newRobot.id);
		
		if(index != -1) {
			cout << endl << "ID já cadastrado! Digite outro." << endl << endl;
		}
	} while(index != -1);
			
	cout << "Tipo: ";
	cin.ignore();
	getline(cin, newRobot.type);
			
	cout << "Ano de criação: ";
	cin >> newRobot.creation_year;
			
	cout << "Descrição: ";
	cin.ignore();
	getline(cin, newRobot.description);
			
	cout << "Uso principal: ";
	getline(cin, newRobot.main_use);
	
	robots[*used_space] = newRobot;
	*used_space += 1;
}

// Mostra na tela todos os registros no vetor
void selectRobots(robot robots[], int used_space) {
	cout << "Número de robôs cadastrados: " << used_space << endl << endl;
	
	for(int i=0; i<used_space; i++) {
		cout << "> ROBÔ " << i+1 << endl;
		cout << "ID: " << robots[i].id << endl;
		cout << "Tipo: " << robots[i].type << endl;
		cout << "Ano de criação: " << robots[i].creation_year << endl;
		cout << "Descrição: " << robots[i].description << endl;
		cout << "Uso principal: " << robots[i].main_use << endl << endl;
	}
}

// Mostra na tela um registro específico pedido pela main
void selectUniqueRobot(robot robot) {
	cout << "ID: " << robot.id << endl;
	cout << "Tipo: " << robot.type << endl;
	cout << "Ano de criação: " << robot.creation_year << endl;
	cout << "Descrição: " << robot.description << endl;
	cout << "Uso principal: " << robot.main_use << endl << endl;
}

// Função que deleta o registro com a ID escolhida
void removeRobot(robot robots[], int* used_space, int index) {
	for(int i=index; i<*used_space - 1; i++) {
		robots[i] = robots[i+1];
	}
					
	*used_space -= 1;
}

/* Função que atualiza a informação com o novo valor. A variavel changed_attribute 
informa qual a opção escolhida pelo usuário e deverá ser atualizada */
void updateRobot(robot robots[], int index, int changed_attribute) {
	if(changed_attribute == 1) {
		
		cout << endl << "Tipo (atual: " << robots[index].type << "): ";
		cin.ignore();
		getline(cin, robots[index].type);
		
	} else if(changed_attribute == 2) {
		
		cout << endl << "Ano de Criação (atual: " << robots[index].creation_year << "): ";
		cin.ignore();
		cin >> robots[index].creation_year;
		
	} else if(changed_attribute == 3) {
		
		cout << endl << "Descrição (atual: " << robots[index].description << "): ";
		cin.ignore();
		getline(cin, robots[index].description);
		
	} else {
		
		cout << endl << "Uso principal (atual: " << robots[index].main_use << "): ";
		cin.ignore();
		getline(cin, robots[index].main_use);
		
	}
	
	cout << endl << "Valor alterado com sucesso!" << endl << endl;
}

// Lê as informações do arquivo para o vetor de registros
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

// Escreve as informações do vetor no arquivo
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

// Busca binária para encontrar um ID dentro do vetor de registros
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

// Método de ordenação Shell sort
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
