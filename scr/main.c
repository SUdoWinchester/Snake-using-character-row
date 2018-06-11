#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define CAMPX 80 //num colunas campo
#define CAMPY 23 //num linhas campo

//velocidade inicial da cobrinha:
int sleepHorizontal = 75;
int sleepVertical = 100;

typedef struct corpo {
	char c;
	int x;
	int y;
	struct corpo *next;
} corpo;

typedef struct {
	corpo *first;
} cabeca;

typedef struct {
	char c;
	int x;
	int y;
} alvo;

void add(cabeca *c, int x, int y, char caracter) {
	corpo *newCorpo = (corpo*) malloc(sizeof(corpo));
	newCorpo->c = caracter;
	newCorpo->x = x;
	newCorpo->y = y;
	newCorpo->next = NULL;

	if(c->first == NULL) {
		c->first = newCorpo;
		return ;
	}

	corpo *aux = c->first;
	while(aux->next != NULL)
		aux = aux->next;

	aux->next = newCorpo;
}

cabeca *sneake() {
	cabeca *c = (cabeca*) malloc(sizeof(cabeca));
	c->first = NULL;

	return c;
}

alvo *criaAlvo() {
	alvo *a = (alvo*) malloc(sizeof(alvo));
	a->c = 'o';
	a->x = -1;
	a->y = -1;

	return a;
}

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = y;
	coord.Y = x;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hidecursor() {
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void criaCampo(int campo[CAMPY][CAMPX]) {
	for(int i = 0; i < CAMPY; i++)
		for(int j = 0; j < CAMPX; j++) {
			if(i == 0)
				campo[i][j] = 220;
			else
				if(i == CAMPY-1)
					campo[i][j] = 223;
				else						
					if(j == 0 || j == CAMPX-1)
						campo[i][j] = 219;
					else
						campo[i][j] = 0;
		}
}

void printCampo(int campo[CAMPY][CAMPX]) {
	for(int i = 0; i < CAMPY; i++) {
		for(int j = 0; j < CAMPX; j++) {
			printf("%c", campo[i][j]);
		}
		printf("\n");
	}
}

void printCobra(cabeca *c) {
	corpo *aux = c->first;

	while(aux != NULL) {
		gotoxy(aux->x, aux->y);
		printf("%c", aux->c);
		aux = aux->next;
	}
}

void geraComida(alvo *comida, int campo[CAMPY][CAMPX]) {
	srand(time(NULL));

	do {
		comida->x = rand()%CAMPY;
		comida->y = rand()%CAMPX;
	} while(campo[comida->x][comida->y] != 0);

	do {
		comida->c = rand()%101 + 48;
	} while(comida->c == 127);

	campo[comida->x][comida->y] = 2;
	gotoxy(comida->x, comida->y);
	printf("%c", comida->c);
}

int verificaImpacto(cabeca *c, alvo *comida, int campo[CAMPY][CAMPX], int x, int y, int *count) {
	if(x == 0 || y == 0 || x == CAMPY-1 || y == CAMPX-1)
		return 1;

	if(campo[x][y] == 1)
		return 1;

	if(campo[x][y] != 0) {
		add(c, comida->x, comida->y, comida->c);
		geraComida(comida, campo);
		*count += 1;
		if(*count%5 == 0) {
			sleepHorizontal -= 5;
			sleepVertical -= 5;
		}

		return 0;
	}

	return 0;
}

void start(int campo[CAMPY][CAMPX]) {
	char tecla = 'd';
	int controle = 1;
	int auxX[2], auxY[2], i, count = 0;

	alvo *comida = criaAlvo();
	geraComida(comida, campo);
	
	cabeca *c = sneake();
	add(c, CAMPY/2, CAMPX/2, 219);
	campo[CAMPY/2][CAMPX/2] = 1;

	corpo *aux;

	do {
		aux = c->first;

		if(kbhit()) {
			tecla = getch();
		}

		switch(tecla) {
			case 'w':
				if(verificaImpacto(c, comida, campo, aux->x-1, aux->y, &count))
					controle = 0;

				auxX[0] = aux->x;
				auxY[0] = aux->y;
				aux->x--;
				campo[aux->x][aux->y] = 1;

				aux = aux->next;
				i = 0;

				while(aux != NULL) {
					if(i%2 == 0) {
						auxX[1] = aux->x;
						auxY[1] = aux->y;
						aux->x = auxX[0];
						aux->y = auxY[0];
					} else {
						auxX[0] = aux->x;
						auxY[0] = aux->y;
						aux->x = auxX[1];
						aux->y = auxY[1];
					}
					aux = aux->next;
					i++;
				}

				if(i%2 == 0) {
					gotoxy(auxX[0], auxY[0]);
					printf(" ");
					if(controle != 0)
					campo[auxX[0]][auxY[0]] = 0;
				} else {
					gotoxy(auxX[1], auxY[1]);
					printf(" ");
					if(controle != 0)
					campo[auxX[1]][auxY[1]] = 0;
				}

				break;
			case 's':
				if(verificaImpacto(c, comida, campo, aux->x+1, aux->y, &count))
					controle = 0;

				auxX[0] = aux->x;
				auxY[0] = aux->y;
				c->first->x++;
				campo[aux->x][aux->y] = 1;

				aux = aux->next;
				i = 0;
				while(aux != NULL) {
					if(i%2 == 0) {
						auxX[1] = aux->x;
						auxY[1] = aux->y;
						aux->x = auxX[0];
						aux->y = auxY[0];
					} else {
						auxX[0] = aux->x;
						auxY[0] = aux->y;
						aux->x = auxX[1];
						aux->y = auxY[1];
					}
					aux = aux->next;
					i++;
				}

				if(i%2 == 0) {
					gotoxy(auxX[0], auxY[0]);
					printf(" ");
					if(controle != 0)
					campo[auxX[0]][auxY[0]] = 0;
				} else {
					gotoxy(auxX[1], auxY[1]);
					printf(" ");
					if(controle != 0)
					campo[auxX[1]][auxY[1]] = 0;
				}

				break;
			case 'd':
				if(verificaImpacto(c, comida, campo, aux->x, aux->y+1, &count))
					controle = 0;

				auxX[0] = aux->x;
				auxY[0] = aux->y;
				c->first->y++;
				campo[aux->x][aux->y] = 1;

				aux = aux->next;
				i = 0;
				while(aux != NULL) {
					if(i%2 == 0) {
						auxX[1] = aux->x;
						auxY[1] = aux->y;
						aux->x = auxX[0];
						aux->y = auxY[0];
					} else {
						auxX[0] = aux->x;
						auxY[0] = aux->y;
						aux->x = auxX[1];
						aux->y = auxY[1];
					}
					aux = aux->next;
					i++;
				}

				if(i%2 == 0) {
					gotoxy(auxX[0], auxY[0]);
					printf(" ");
					if(controle != 0)
					campo[auxX[0]][auxY[0]] = 0;
				} else {
					gotoxy(auxX[1], auxY[1]);
					printf(" ");
					if(controle != 0)
					campo[auxX[1]][auxY[1]] = 0;
				}

				break;
			case 'a':
				if(verificaImpacto(c, comida, campo, aux->x, aux->y-1, &count))
					controle = 0;

				auxX[0] = aux->x;
				auxY[0] = aux->y;
				c->first->y--;
				campo[aux->x][aux->y] = 1;

				aux = aux->next;
				i = 0;
				while(aux != NULL) {
					if(i%2 == 0) {
						auxX[1] = aux->x;
						auxY[1] = aux->y;
						aux->x = auxX[0];
						aux->y = auxY[0];
					} else {
						auxX[0] = aux->x;
						auxY[0] = aux->y;
						aux->x = auxX[1];
						aux->y = auxY[1];
					}
					aux = aux->next;
					i++;
				}

				if(i%2 == 0) {
					gotoxy(auxX[0], auxY[0]);
					printf(" ");
					if(controle != 0)
					campo[auxX[0]][auxY[0]] = 0;
				} else {
					gotoxy(auxX[1], auxY[1]);
					printf(" ");
					if(controle != 0)
					campo[auxX[1]][auxY[1]] = 0;
				}

				break;
			case 'p':
				gotoxy(CAMPY/2, CAMPX/2-6);
				printf("--JOGO PAUSADO--");
				gotoxy(CAMPY/2+1, CAMPX/2-16);
				printf("Precione Qualquer Tecla Para Continuar");
				tecla = getch();
				gotoxy(CAMPY/2, CAMPX/2-6);
				printf("                ");
				gotoxy(CAMPY/2+1, CAMPX/2-16);
				printf("                                      ");
				Sleep(250);
		}

		printCobra(c);
		if(tecla == 'a' || tecla == 'd')
			Sleep(sleepHorizontal);
		else
			Sleep(sleepVertical);

		gotoxy(23, 0);
		printf("Pontos: %d\n", count);
	} while(controle);
}

void main() {
	int campo[CAMPY][CAMPX];

	system("mode con:cols=80 lines=25");

	hidecursor();
	criaCampo(campo);
	printCampo(campo);

	start(campo);
	gotoxy(CAMPY+2, 0);
}