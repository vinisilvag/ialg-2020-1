#include <iostream>
#include <string>
#include <fstream>
#include <ctype.h>

/*
	Graduando: Vinícius Gomes;
	Tema do trabalho: Robôs;
	Método de busca: Binária;
	Método de ordenação: Shell sort;
	Modo de gravação: Texto.
*/

using namespace std;

// Declaração do struct
struct robot {
	int id;
	string type;
	int creation_year;
	string description;
	string main_use;
};

// Declaração das funções
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
		
		// Variável que recebe a opção escolhida pelo usuário que é retornada na função menu()
		option = menu();
		
		switch(option) {
			case 1: {
				// Verifica se mais um cadastro excederia o limite estipulado 
				if(used_space + 1 > TAM) {
					cout << endl << "Nao eh possível inserir um novo registro! Capacidade maxima atingida." << endl << endl;
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
					
					// Mostra na tela o registro que está pendente para ser deletado
					selectUniqueRobot(robots[index]);
					
					// Verifica se o usuário quer mesmo deletar aquele registro
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
				
				// Busca a ID informada pelo usuário
				int index = binarySearch(robots, 0, used_space, alteredID);
				
				/* Caso ela exista -> index != -1 e a condição cai no bloco if
				Caso não -> index == -1 e a condição cai no bloco else */
				if(index != -1) {
					int changed_attribute;
					
					// É verificado qual atributo daquela ID o usuário deseja alterar
					cout << endl << "> Qual informacao voce deseja alterar?" << endl << endl;
					cout << "1) Tipo" << endl;
					cout << "2) Ano de criacao" << endl;
					cout << "3) Descricao" << endl;
					cout << "4) Uso principal" << endl;
					
					// Verifica se o usuário informou um campo válido
					do {
						cout << endl << "R: ";
						cin >> changed_attribute;
						
						if(changed_attribute < 1 or changed_attribute > 4) {
							cout << endl << "Informe um número valido!" << endl;
						}
					} while(changed_attribute < 1 or changed_attribute > 4);
					
					// A propriedade é devidamente alterada pela função
					updateRobot(robots, index, changed_attribute);
				} else {
					cout << endl << "Registro nao encontrado!" << endl << endl;
				}
				
				break;
			}
			
			case 5: {
				// Salva as informações atuais do vetor no arquivo
				writeInFile(robots, used_space);
				cout << endl << "Gravacao realizada com sucesso!" << endl << endl;
				
				break;
			}
			
			case 6: {
				cout << endl << "Deseja salvar as modificacoes realizadas?" << endl;
			
				// Verifica se o usuário deseja salvar as alterações antes de sair
				char save = continueFunctionTest();
				
				if(save == 'S') {
					// Caso sim, o programa salva antes de finalizar sua execução
					writeInFile(robots, used_space);
					cout << endl << "Gravacao realizada com sucesso!" << endl;
				}
				
				cout << endl << "Saindo do programa...";
				
				break;
			}
		}
		
		/* Código que pausa o console ao final da execução de cada opção, desde que não seja requisitada a saída do programa
		Serve apenas para melhorar a experiência do usuário com o programa */
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
	
	// Desaloca a memória do vetor de registros
	delete[] robots;
	
	return 0;
	
}

// Menu para interação com o usuário
int menu() {
	int answer;
	
	cout << ">> MENU <<" << endl << endl;
	cout << "1) Incluir" << endl;
	cout << "2) Excluir" << endl;
	cout << "3) Listar" << endl;
	cout << "4) Alterar" << endl;
	cout << "5) Gravar dados" << endl;
	cout << "6) Sair" << endl << endl;
	
	// Verifica se o usuário inseriu uma opção válida
	do {
		
		cout << "R: ";
		cin >> answer;
		
		if(answer < 1 or answer > 6) {
			cout << endl << "Opcao nao existente na lista!" << endl << endl;
		}
		
	} while(answer < 1 or answer > 6);
	
	// Retorna o valor escolhido pelo usuário para ser usado na main
	return answer;
}

// Cria o novo registro dentro do vetor de registros
void createRobot(robot robots[], int* used_space) {
	robot newRobot;
	int index;
	
	// Testa se o ID inserindo já existe no vetor ou não
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

// Mostra na tela um registro específico passado por parâmetro
void selectUniqueRobot(robot robot) {
	cout << "ID: " << robot.id << endl;
	cout << "Tipo: " << robot.type << endl;
	cout << "Ano de criacao: " << robot.creation_year << endl;
	cout << "Descricao: " << robot.description << endl;
	cout << "Uso principal: " << robot.main_use << endl << endl;
}

// Função que deleta o registro com a ID escolhida
void removeRobot(robot robots[], int* used_space, int index) {
	// Passa os registros da direita para a esquerda para remover o registro
	for(int i=index; i<*used_space; i++) {
		robots[i] = robots[i+1];
	}
	
	// Subtrai 1 da variável por causa do registro excluído
	*used_space -= 1;
}

/* Função que atualiza a informação com o novo valor. A variavel changed_attribute 
informa qual a opção escolhida pelo usuário e deverá ser atualizada */
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

/* Função que testa se o usuário digitou sim ou não na saída
Usada para a verificação na saída do programa e ao deletar algum usuário */
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

// Escreve as informações do vetor de registros no arquivo
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

// Método de ordenação do vetor: Shell sort
void shellSort(robot robots[], int size) {
	int gaps[9] = {1, 4, 10, 23, 57, 132, 301, 701, 1750}; // Sequência de Ciura
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
