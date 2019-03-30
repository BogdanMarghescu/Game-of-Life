#define _CRT_SECURE_NO_WARNINGS
#define BUFFSIZE 1024 //dimensiunea maxima de octeti de pe o linie dintr-un fisier

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

char **matrix_alloc(int n, int m) //functie ce aloca dinamic o matrice de dimensiuni n * m
{
	char **p, *q;
	p = (char**)malloc(sizeof(char*)*n);
	if (p == NULL)
		return NULL;
	q = (char*)malloc(n * m * sizeof(char));
	if (q == NULL)
	{
		free(p);
		return NULL;
	}
	for (int i = 0; i < n; ++i, q += m)
	{
		p[i] = q;
	}
	return p;
}

void matrix_delete(char **matrix) //functie ce elibereaza spatiul alocat dinamic de o matrice din memoria heap
{
	free(matrix[0]);
	free(matrix);
}

int nr_linii(FILE *pf) //Numărul de linii din fișier;
{
	int nr_linii = 0;
	char *buff = (char*)malloc(BUFFSIZE * sizeof(char));
	if (!pf)
	{
		puts("Eroare!");
		nr_linii = -1;
	}
	else
	{
		while (fgets(buff, BUFFSIZE, pf) != NULL)
		{
			nr_linii++;
		}
	}
	free(buff);
	rewind(pf);
	return nr_linii;
}

int nr_coloane(FILE *pf) //functie ce determina numărul de coloane dintr-un fișier(care contine o structura de tip matrice)
{
	int nr_coloane = -1, c;
	if (!pf)
	{
		puts("Eroare!");
		exit(1);
	}
	else
	{
		do
		{
			c = fgetc(pf);
			nr_coloane++;
		} while (c != '\n');
	}
	rewind(pf);
	return nr_coloane;
}

void evolution(char **table, int height, int width) //functia prin care evolueaza tabla de celule
{
	char **board = table, **new_table = matrix_alloc(height, width);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int neighbours_ij = 0; ////calculeaza numarul de vecini al celulei (i,j)
			for (int a = i - 1; a <= i + 1; a++)
			{
				for (int b = j - 1; b <= j + 1; b++)
				{
					if (board[(a + height) % height][(b + width) % width])
						neighbours_ij++;
				}
			}
			if (board[i][j])
				neighbours_ij--;
			//initializam o tabla noua in functie de vecinii fiecarei celule din vechea tabla
			if (neighbours_ij == 3 || (neighbours_ij == 2 && board[i][j] != 0))
				new_table[i][j] = 1;
			else
				new_table[i][j] = 0;
		}
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			board[i][j] = new_table[i][j]; //actualizam tabla de celule
		}
	}
	matrix_delete(new_table);
}

/*Acum urmeaza functiile pentru cele trei modalitati de citire si interpretare a fisierelor de input:
modul normal, unde este specificat nr de linii si coloane pe prima linie, modul super_dinamic, unde nu 
este specificat numarul de linii si coloane pe prima linie si modul comprimat, unde este specificat pe
prima linie cate linii si coloane sunt in matrice si, in continuare, perechile de coordonate ale celulelor vii*/

char **read_file_in_normal(FILE *pfile, int *height, int *width)
{
	/*functie care citeste un fisier de input in mod normal(cu numarul de linii si coloane
	specificat pe prima linie a fisierului) si returneaza configuratia initiala*/
	fscanf(pfile, "%d", height);
	fscanf(pfile, "%d", width);
	char **board = matrix_alloc(*height, *width);
	int i, j; //i si j indecsii cu care parcurgem tabla
	char *line = (char*)malloc(BUFFSIZE * sizeof(char)); //un buffer care va fi folosit cu scopul retinerii fiecarei linii din tabla care va fi citita si parsata
	for (i = 0; i < *height; i++)
	{
		fscanf(pfile, "%s", line);
		for (j = 0; j < *width; j++)
		{
			board[i][j] = line[j] - '0';
		}
	}
	rewind(pfile);
	free(line);
	return board;
}

char **read_file_in_super_dinamic(FILE *pfile, int *height, int *width)
{
	/*functie care citeste un fisier de input in mod super_dinamic(fara numarul de linii si coloane
	specificat pe prima linie a fisierului, ci doar tabla cu celule) si returneaza configuratia initiala*/
	*height = nr_linii(pfile);
	*width = nr_coloane(pfile);
	char **board = matrix_alloc(*height, *width);
	int i, j; //i si j indecsii cu care parcurgem tabla
	char *line = (char*)malloc(BUFFSIZE * sizeof(char)); //un buffer care va fi folosit cu scopul retinerii  care va fi citita si parsata
	for (i = 0; i < *height; i++)
	{
		fgets(line, BUFFSIZE, pfile);
		for (j = 0; j < *width; j++)
		{
			board[i][j] = line[j] - '0';
		}
	}
	rewind(pfile);
	free(line);
	return board;
}

char **read_file_in_comprimat(FILE *pfile, int *height, int *width)
{
	/*functie care citeste un fisier de input in mod comprimat(cu numarul de linii si coloane specificat pe prima linie a
	fisierului, aparand pe restul liniilor, pe fiecare in parte, cate o pereche de coordonate (x,y) ale celulelor vii)*/
	fscanf(pfile, "%d", height);
	fscanf(pfile, "%d", width);
	char **board = matrix_alloc(*height, *width);
	int i, j, x, y; //x si y sunt coordonatele celulei vii, i si j indecsii cu care parcurgem tabla
	for (i = 0; i < *height; i++)
	{
		for (j = 0; j < *width; j++)
		{
			board[i][j] = 0; //zero-izam tabla
		}
	}
	while (!feof(pfile))
	{
		fscanf(pfile, "%d", &x);
		fscanf(pfile, "%d", &y);
		board[x][y] = 1;
	}
	rewind(pfile);
	return board;
}

char **matrix_cpy(char **src, char **dest, int height, int width) //functie care copiaza continutul unei matrici de tipul height * width in alta de aceleasi dimensiuni
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			src[i][j] = dest[i][j];
		}
	}
	return src;
}

int matrix_cmp(char **m1, char **m2, int height, int width) //verifica daca 2 matrici de aceleasi dimensiuni sunt egale
{
	int cmp = 1; //variabila care determina daca cele 2 matrici sunt egale sau nu
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (m1[i][j] != m2[i][j])
			{
				cmp = 0;
				break;
			}
		}
	}
	return cmp;
}

void board_file_write(char **board, int height, int width, FILE *file_out) 
{
	//functie care scrie o matrice board de dimensiuni height * width intr-un fisier file_out
	if (!file_out)
	{
		puts("Eroare!");
		exit(1);
	}
	else
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				fprintf(file_out, "%c", board[i][j] + '0');
			}
			if (i < height - 1) //scriem cate un caracter newline dupa fiecare linie, in afara de ultima
			{
				fprintf(file_out, "\n");
			}
		}
		rewind(file_out);
	}
}

void game_of_life_normal(int nr_pasi, FILE *file_in, FILE *file_out, char **(*read_function)(FILE*, int*, int*))
{
	/*functia care deruleaza jocul pana la un anume pas (nr_pasi este numarul de pasi) care foloseste o anumita 
	functie de interpretare a fisierului de citire, fara optiunea de verificare a configuratiei stabile*/
	int N, M, step = 0; //N linii si M coloane, step este pasul la care se afla configuratia curenta;
	char **board = read_function(file_in, &N, &M);
	if (N < 3 || M < 3)
	{
		fputs("Configuratie invalida", file_out);
		exit(1);
	}
	while (step <= nr_pasi)
	{
		if (step == nr_pasi)
		{
			board_file_write(board, N, M, file_out); //afisarea configuratei finale in fisierul de output cand am ajuns la ultimul pas
		}
		evolution(board, N, M); //evolueaza tabla
		step++;
	}
	matrix_delete(board);
	fclose(file_in);
	fclose(file_out);
}

void game_of_life_stabil(int nr_pasi, FILE *file_in, FILE *file_out, char **(*read_function)(FILE*, int*, int*))
{
	/*functia care deruleaza jocul pana la un anume pas(nr_pasi este numarul de pasi) care foloseste o anumita 
	functie de interpretare a fisierului de citire, cu optiunea de verificare a configuratiei stabile*/
	int N, M, step = 0, nr_config = 0, index_config; //N linii si M coloane, nr_config-folosit pt a indica cu cate table anterioare va fi comparata configuratia de la pasul "steps", cu ajutorul indexului index_config fiind parcurs vectorul de table 
	char **board = read_function(file_in, &N, &M);
	if (N < 3 || M < 3)
	{
		fputs("Configuratie invalida", file_out);
		exit(1);
	}
	char ***array_of_boards = NULL, pozitie_configuratie_stabila = -1; //am creat un vector de matrici care va fi populat la fiecare pas cu configuratia de la pasul curent si am declarat o variabila cu care vom identifica pozitia configuratia ciclica
	while (step <= nr_pasi)
	{
		array_of_boards = (char***)realloc(array_of_boards, (step + 1) * sizeof(char**)); //populam vectorul de matrici cu noua tabla
		array_of_boards[nr_config] = matrix_alloc(N, M);
		array_of_boards[nr_config] = matrix_cpy(array_of_boards[nr_config], board, N, M); 
		for (index_config = 0; index_config < nr_config; index_config++) //comparam tabla actuala cu cele precedente si verificam daca sunt 2 identice
		{
			if (matrix_cmp(array_of_boards[index_config], board, N, M))
			{
				pozitie_configuratie_stabila = index_config; //identificam pozitia configuratiei stabile
				break;
			}
		}
		if ((step == nr_pasi) && (pozitie_configuratie_stabila == -1)) //daca nu avem configuratie stabila pana la numarul "nr_pasi" de pasi
		{
			fprintf(file_out, "Nu a fost gasita o configuratie stabila.\n");
			board_file_write(board, N, M, file_out);
		}
		else if(pozitie_configuratie_stabila >= 0)
		{
			fprintf(file_out, "Configuratie stabila la pasul:%d\n", pozitie_configuratie_stabila);
			board_file_write(array_of_boards[pozitie_configuratie_stabila], N, M, file_out);
			break;
		}
		evolution(board, N, M);
		step++; 
		nr_config++;
	}
	for (nr_config = 0; nr_config < step; nr_config++) //stergem fiecare element al vectorului de matrici
	{
		matrix_delete(array_of_boards[nr_config]);
	}
	free(array_of_boards); //stergem vectorul de matrici
	matrix_delete(board);
	fclose(file_in);
	fclose(file_out);
}

void main(int nr_parameters, char **parameter)  //functia principala a progrmaului
{
	FILE *file_in, *file_out;
	char filename_in[200], filename_out[200];
	int nr_pasi, index;
	char **(*read_function)(FILE*, int*, int*) = read_file_in_normal; //declaram un pointer la functia normala de citit fisiere input, dar pe parcursul programului acest pointer la functie poate va pointa catre alta functie de citire
	int read_option = 0, stable_configuration_option = 0; //optiunile de interpretare a fisierului de input sunt cele implicite, adica modul normal de citire si detectorul de configuratii stabile este dezactivat, ambele reprezentate de valoarea 0
	for (index = 1; index < nr_parameters; index++) //interpretam fiecare parametru din lista de parametri
	{
		if ((!strcmp(parameter[index], "-in"))) //cautam parametrul cu numele fisierului de input
			strcpy(filename_in, parameter[index + 1]);
		
		if ((!strcmp(parameter[index], "-out"))) //cautam parametrul cu numele fisierului de output
			strcpy(filename_out, parameter[index + 1]);
		
		if (!strcmp(parameter[index], "-steps")) //cautam parametrul care reprezinta numarul de pasi care se vor efectua
			nr_pasi = atoi(parameter[index + 1]);

		if (!strcmp(parameter[index], "-super_dinamic")) //verificam daca este activat modul super_dinamic
			read_option = 1;
		else if (!strcmp(parameter[index], "-comprimat")) //verificam daca este activat modul comprimat
			read_option = 2;

		if (!strcmp(parameter[index], "-stabil")) //verificam daca este activat detectorul de configuratii stabile
			stable_configuration_option = 1;
	}
	if (read_option == 0)  /* 0 este modul normal de citire a fisierului de intrare (cu numarul de linii si coloane pe prima linie a fisierului) */
		read_function = read_file_in_normal;
	else if (read_option == 1)  /* 1 este modul super_dinamic de citire a fisierului de intrare */
		read_function = read_file_in_super_dinamic;
	else if (read_option == 2)  /* 2 este modul comprimat de citire a fisierului de intrare */
		read_function = read_file_in_comprimat;
	file_in = fopen(filename_in, "r");
	file_out = fopen(filename_out, "w");
	if (!file_in) //verificam daca exista fisier de intrare
	{
		fputs("Fisierul nu exista!", file_out);
		exit(1);
	}
	else
	{
		if (stable_configuration_option == 0)
			game_of_life_normal(nr_pasi, file_in, file_out, read_function); //se executa functia jocului, cu ajutorul functiei de citire determinate mai sus si fara determinarea configuratiei stabile
		else
			game_of_life_stabil(nr_pasi, file_in, file_out, read_function); //se executa functia jocului, cu ajutorul functiei de citire determinate mai sus si cu determinarea configuratiei stabile
	}
}