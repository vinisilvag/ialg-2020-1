#include <iostream>
#include <string>
#include <fstream>
#include <ctype.h>

/*
	Graduando: Vin�cius Gomes;
	Tema do trabalho: Rob�s;
	M�todo de busca: Bin�ria;
	M�todo de ordena��o: Shell sort;
	Modo de grava��o: Texto.
*/

using namespace std;

// Declara��o do struct
struct robot {
	int id;
	string type;
	int creation_year;
	string description;
	string main_use;
};

// Declara��o das fun��es
int menu();

void createRobot(robot robots[], int* used_space);

void selectRobots(robot robots[], int tam);

void selectUniqueRobot(robot robot);

void updateRobot(robot robots[], int index, int changed_attribute);

void removeRobot(robot robots[], int* used_space, int index);

char continueFunctionTest();

void writeInFile(robot robots[], int used_space);

int binarySearch(robot robots[], int start_pos, int end_pos, int value);

void shellSort(robot robots[], int size);

int main() {
	
	const int TAM = 100;
	int option, used_space = 0;
	
	// Aloca dinamicamente o vetor de registros
	robot* robots = new robot[TAM];
	
	do {
		
		// Vari�vel que recebe a op��o escolhida pelo usu�rio que � retornada na fun��o menu()
		option = menu();
		
		switch(option) {
			case 1: {
				// Verifica se mais um cadastro excederia o limite estipulado 
				if(used_space + 1 > TAM) {
					cout << endl << "Nao eh poss�vel inserir um novo registro! Capacidade maxima atingida." << endl << endl;
				} else {
					cout << endl << ">> NOVO REGISTRO <<" << endl;
					
					// Cadastra um novo registro no vetor de registros
					createRobot(robots, &used_space);
						
					// Ordena esse novo vetor pelo ID
					shellSort(robots, used_space);
						
					cout << endl << "Registro incluido com sucesso!" << endl << endl;
				}
				
				break;
			}
			
			case 2: {
				int robotID;
			
				cout << endl << "Informe o ID do registro que voce deseja deletar: ";
				cin >> robotID;
				
				// Procura o ID a ser deletado
				int index = binarySearch(robots, 0, used_space, robotID);
				
				if(index != -1) {
					cout << endl << "> Voce tem certeza que deseja deletar esse robo?" << endl << endl;
					
					// Mostra na tela o registro que est� pendente para ser deletado
					selectUniqueRobot(robots[index]);
					
					// Verifica se o usu�rio quer mesmo deletar aquele registro
					char deleteRobot = continueFunctionTest();
					
					if(deleteRobot == 'S') {
						// Deleta o registro caso a resposta seja sim
						removeRobot(robots, &used_space, index);
						
						cout << endl << "Registro excluido com sucesso!" << endl << endl;
					} else {
						cout << endl << "Operacao cancelada!" << endl << endl;
					}
				} else {
					cout << endl << "Registro nao encontrado!" << endl << endl;
				}
				
				break;
			}
			
			case 3: {
				cout << endl << ">> REGISTROS CADASTRADOS <<" << endl << endl;
				
				// Exibe todos os registros no vetor
				selectRobots(robots, used_space);
				
				break;
			}
			
			case 4: {
				int alteredID, changed_attribute;
			
				cout << endl << "Informe o ID do robo que voce deseja alterar: ";
				cin >> alteredID;
				
				// Busca a ID informada pelo usu�rio
				int index = binarySearch(robots, 0, used_space, alteredID);
				
				/* Caso ela exista -> index != -1 e a condi��o cai no bloco if
				Caso n�o -> index == -1 e a condi��o cai no bloco else */
				if(index != -1) {
					int changed_attribute;
					
					// � verificado qual atributo daquela ID o usu�rio deseja alterar
					cout << endl << "> Qual informacao voce deseja alterar?" << endl << endl;
					cout << "1) Tipo" << endl;
					cout << "2) Ano de criacao" << endl;
					cout << "3) Descricao" << endl;
					cout << "4) Uso principal" << endl;
					
					// Verifica se o usu�rio informou um campo v�lido
					do {
						cout << endl << "R: ";
						cin >> changed_attribute;
						
						if(changed_attribute < 1 or changed_attribute > 4) {
							cout << endl << "Informe um n�mero valido!" << endl;
						}
					} while(changed_attribute < 1 or changed_attribute > 4);
					
					// A propriedade � devidamente alterada pela fun��o
					updateRobot(robots, index, changed_attribute);
				} else {
					cout << endl << "Registro nao encontrado!" << endl << endl;
				}
				
				break;
			}
			
			case 5: {
				// Salva as informa��es atuais do vetor no arquivo
				writeInFile(robots, used_space);
				cout << endl << "Gravacao realizada com sucesso!" << endl << endl;
				
				break;
			}
			
			case 6: {
				cout << endl << "Deseja salvar as modificacoes realizadas?" << endl;
			
				// Verifica se o usu�rio deseja salvar as altera��es antes de sair
				char save = continueFunctionTest();
				
				if(save == 'S') {
					// Caso sim, o programa salva antes de finalizar sua execu��o
					writeInFile(robots, used_space);
					cout << endl << "Gravacao realizada com sucesso!" << endl;
				}
				
				cout << endl << "Saindo do programa...";
				
				break;
			}
		}
		
		/* C�digo que pausa o console ao final da execu��o de cada op��o, desde que n�o seja requisitada a sa�da do programa
		Serve apenas para melhorar a experi�ncia do usu�rio com o programa */
		if(option != 6) {
			if(option != 1 && option != 4) {
				cin.ignore();
			}
			cout << "Aperte qualquer tecla para continuar... ";
			cin.get();
		}
		
		// Limpa a tela com alguns endlines
		for(int i=0; i<8; i++) {
			cout << endl;
		}
		
	} while(option != 6);
	
	// Desaloca a mem�ria do vetor de registros
	delete[] robots;
	
	return 0;
	
}

// Menu para intera��o com o usu�rio
int menu() {
	int answer;
	
	cout << ">> MENU <<" << endl << endl;
	cout << "1) Incluir" << endl;
	cout << "2) Excluir" << endl;
	cout << "3) Listar" << endl;
	cout << "4) Alterar" << endl;
	cout << "5) Gravar dados" << endl;
	cout << "6) Sair" << endl << endl;
	
	// Verifica se o usu�rio inseriu uma op��o v�lida
	do {
		
		cout << "R: ";
		cin >> answer;
		
		if(answer < 1 or answer > 6) {
			cout << endl << "Opcao nao existente na lista!" << endl << endl;
		}
		
	} while(answer < 1 or answer > 6);
	
	// Retorna o valor escolhido pelo usu�rio para ser usado na main
	return answer;
}

// Cria o novo registro dentro do vetor de registros
void createRobot(robot robots[], int* used_space) {
	robot newRobot;
	int index;
	
	// Testa se o ID inserindo j� existe no vetor ou n�o
	do {
		cout << "ID: ";
		cin >> newRobot.id;
		
		index = binarySearch(robots, 0, *used_space, newRobot.id);
		
		if(index != -1) {
			cout << endl << "ID ja cadastrado! Digite outro." << endl << endl;
		}
	} while(index != -1);
	
	// Termina de ler os valores do novo registro
	
	cout << "Tipo: ";
	cin.ignore();
	getline(cin, newRobot.type);
			
	cout << "Ano de criacao: ";
	cin >> newRobot.creation_year;
			
	cout << "Descricao: ";
	cin.ignore();
	getline(cin, newRobot.description);
			
	cout << "Uso principal: ";
	getline(cin, newRobot.main_use);
	
	robots[*used_space] = newRobot;
	*used_space += 1;
}

// Mostra na tela todos os registros no vetor
void selectRobots(robot robots[], int used_space) {
	cout << "Numero de robos cadastrados: " << used_space << endl << endl;
	
	for(int i=0; i<used_space; i++) {
		cout << "> ROBO " << i+1 << endl;
		cout << "ID: " << robots[i].id << endl;
		cout << "Tipo: " << robots[i].type << endl;
		cout << "Ano de criacao: " << robots[i].creation_year << endl;
		cout << "Descricao: " << robots[i].description << endl;
		cout << "Uso principal: " << robots[i].main_use << endl << endl;
	}
}

// Mostra na tela um registro espec�fico passado por par�metro
void selectUniqueRobot(robot robot) {
	cout << "ID: " << robot.id << endl;
	cout << "Tipo: " << robot.type << endl;
	cout << "Ano de criacao: " << robot.creation_year << endl;
	cout << "Descricao: " << robot.description << endl;
	cout << "Uso principal: " << robot.main_use << endl << endl;
}

// Fun��o que deleta o registro com a ID escolhida
void removeRobot(robot robots[], int* used_space, int index) {
	// Passa os registros da direita para a esquerda para remover o registro
	for(int i=index; i<*used_space; i++) {
		robots[i] = robots[i+1];
	}
	
	// Subtrai 1 da vari�vel por causa do registro exclu�do
	*used_space -= 1;
}

/* Fun��o que atualiza a informa��o com o novo valor. A variavel changed_attribute 
informa qual a op��o escolhida pelo usu�rio e dever� ser atualizada */
void updateRobot(robot robots[], int index, int changed_attribute) {
	switch(changed_attribute) {
		case 1: {
			cout << endl << "Tipo (atual: " << robots[index].type << "): ";
			cin.ignore();
			getline(cin, robots[index].type);
			
			break;
		}
		
		case 2: {
			cout << endl << "Ano de Criacao (atual: " << robots[index].creation_year << "): ";
			cin.ignore();
			cin >> robots[index].creation_year;
			
			break;
		}
		
		case 3: {
			cout << endl << "Descricao (atual: " << robots[index].description << "): ";
			cin.ignore();
			getline(cin, robots[index].description);
		
			break;
		}
		
		case 4: {
			cout << endl << "Uso principal (atual: " << robots[index].main_use << "): ";
			cin.ignore();
			getline(cin, robots[index].main_use);
			
			break;
		}
	}
	
	cout << endl << "Valor alterado com sucesso!" << endl << endl;
}

/* Fun��o que testa se o usu�rio digitou sim ou n�o na sa�da
Usada para a verifica��o na sa�da do programa e ao deletar algum usu�rio */
char continueFunctionTest() {
	char input;
	
	do {
		cout << "R (S/N): ";
		cin >> input;
		
		input = toupper(input);
					
		if(input != 'S' and input != 'N') {
			cout << endl << "Informe o caractere correto!" << endl << endl;
		}
	} while(input != 'S' and input != 'N');
	
	return input;
}

// Escreve as informa��es do vetor de registros no arquivo
void writeInFile(robot robots[], int used_space) {
	string fileName;
	
	cout << endl << "Informe o nome do arquivo para salvar os dados (nome + extensao): ";
	cin >> fileName;
	
	ofstream write_file(fileName.c_str());
			
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

// M�todo de ordena��o do vetor: Shell sort
void shellSort(robot robots[], int size) {
	int gaps[9] = {1, 4, 10, 23, 57, 132, 301, 701, 1750}; // Sequ�ncia de Ciura
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
