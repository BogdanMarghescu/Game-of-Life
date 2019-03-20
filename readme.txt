MARGHESCU BOGDAN
C-111C
Tema 3: GAME OF LIFE

La inceputul main-ului, am parcurs lista de argumente si am identificat denumirea fisierelor de input si output, numarul de pasi, felul in care vom interpreta fisierul de input si daca vom verifica configuratia stabila sau nu.
Variabila read_option(initializata pt inceput cu 0) ne indica modul in care vor fi citite fisierele de input. Acestea sunt:
	- 0(optiunea implicita): fisierul va fi citit normal(cu numarul de linii si coloane specificat pe prima linie a fisierului, apoi urmand tabla de celule vii si moarte) cu ajutorul functiei read_file_in_normal
	- 1: fisierul va fi citit in super_dinamic(in fisier se afla doar tabla de celule, nu si numarul de linii si coloane pe prima linie) cu ajutorul functiei read_file_in_super_dinamic
	- 2: fisierul va fi citit in format comprimat(cu numarul de linii si coloane specificat pe prima linie a fisierului, aparand pe restul liniilor, pe fiecare in parte, cate o pereche de coordonate (x,y) ale 
	     celulelor vii), cu ajutorul functiei read_file_in_comprimat

In functie de valoarea lui read_option(0, 1 sau 2), determinata dupa interpretarea parametrilor, alegem pe care din cele 3 functii de citire a fisierului de input o vom folosi in una din cele 2 functii care deruleaza jocul
(game_of_life_normal sau game_of_life_stabil). Aceasta functie de citire va fi retinuta cu ajutorul pointer-ului la functie **(*read_function)(FILE*, int*, int*), care point-eaza catre functia implicita de citire read_file_in_normal.
Acum voi prezenta pe scurt functiile de citire ale fisierelor de input:
 - read_file_in_normal: in aceasta functie se citesc numarul de linii si coloane de pe prima linie a fisierului. Apoi citim fiecare linie din fisier ca sir de caractere intr-un buffer alocat dinamic pe 1024 octeti.
   Cu ajutorul lui cream tabla de celule vii si moarte, in functie de nr linii si coloane determinat prin citire.
 - read_file_in_super_dinamic: in aceasta functie nu se mai citesc numarul de linii si coloane de pe prima linie a fisierului, deoarece nu mai sunt specificate, ci le vom determina noi cu ajutorul functiilor auxiliare 
   nr_linii(care determina numarul de linii dintr-un fisier) si nr_coloane(care determina numãrul de coloane dintr-un fi?ier care contine o structura de tip matrice). Apoi citim fiecare linie din fisier ca sir de caractere 
   intr-un buffer alocat dinamic pe 1024 octeti. Cu ajutorul lui cream tabla de celule vii si moarte, in functie de nr linii si coloane determinat cu ajutorul functiilor.
 - read_file_in_comprimat: in aceasta functie se citesc numarul de linii si coloane de pe prima linie a fisierului. Apoi alocam dinamic si initializam fiecare element al tablei de joc cu 0. Apoi vom citi toate perechile de numere
   de pe fiecare linie pana ajungem la finalul fisierului si punem 1 pe tabla la pozitia indicata de perechile de coordonate citite din fisier.
Toate 3 functiile returneaza tabla de joc in stare initiala.

Alta variabila din main, stable_configuration_option(initializata pt inceput cu 0), ne indica daca trebuie sau nu sa stabilim existenta unei configuratii stabile.
Problema am gandit-o sa functioneze in doua moduri in functie de existenta parametrului "-stabil" in lista de argumente si, respectiv, de valoarea lui stable_configuration_option: 
	
	- modul normal(in care nu detecteaza configuratia stabila si o afiseaza pe cea finala, obtinuta la pasul "steps"). Acesta e declansat daca stable_configuration_option este 0.
	In aceasta situatie, vom folosi functia jocului game_of_life_normal care va obtine tabla initiala cu ajutorul functiei read_function(care este un parametru al functiei game_of_life_normal) si va verifica initial daca avem
	o configuratie valida, in functie de numarul de linii si coloane. Apoi functia va evolua tabla pentru un numar "nr_pasi" de pasi cu ajutorul functiei "evolution", verificand initial daca s-a ajuns la ultimul pas, pentru a 
	scrie in fisierul file_out ultima configuratie cu ajutorul functiei "board_file_write"(care scrie o matrice board de dimensiuni height * width intr-un fisier file_out). La final dezalocam matricea in care am stocat tabla de
	joc si inchidem fisierele de input si output.
	
	- modul stabil(in care se verifica existenta unei configuratii stabile si o afiseaza pe prima configuratie care devine ciclica si a cata configuratie este, sau, in cazul negasirii unei configuratii ciclice, se afiseaza pe 
	prima linie faptul ca nu s-a gasit o configuratie ciclica si apoi ultima configuratie, de la pasul "nr_pasi"). Acesta e declansat daca stable_configuration_option este 1, valoare determinata in urma parcurgerii parametrilor 
	lui main. In aceasta situatie, vom folosi functia jocului game_of_life_stabil care va obtine tabla initiala cu ajutorul functiei read_function(care este un parametru al functiei game_of_life_stabil) si va verifica initial 
	daca avem o configuratie valida, in functie de numarul de linii si coloane. Apoi vom crea, pas cu pas, un vector care contine toate configuratiile pana la pasul la care am ajuns. Acest vector este initial egal cu NULL, dar
	la fiecare pas este realocat dinamic cu inca un spatiu pentru o noua tabla, spatiu in care se pune tabla de la pasul "step", adica pasul la care a ajuns la un moment dat configuratia. Noua matrice din vectorul de configuratii
	este alocata dinamic cu ajutorul functiei "matrix_alloc" si este copiata noua configuratie in ea cu ajutorul functiei "matrix_cpy"(functie care copiaza continutul unei matrici de tipul height * width in alta de aceleasi 
	dimensiuni). Vom folosi variabila pozitie_configuratie_stabila initializata initial cu -1(ceea ce indica ca nu am gasit inca pozitia de inceput a configuratiei stabile). Apoi, dupa ce am reactualizat vectorul de matrici, vom 
	compara actuala tabla cu cele precedente cu ajutorul functiei "matrix_cmp"(care verifica daca 2 matrici de aceleasi dimensiuni sunt egale) si daca dam de 2 matrici egale, atunci pozitia celei din vectorul de matrici care este 
	egala cu configuratia de la pasul la care am ajuns va fi pozitia de la care incepe ciclicitatea configuratiei, deci vom initializa variabila pozitie_configuratie_stabila cu aceasta valoare indentificata de noi a pozitiei. In 
	acest caz, afisam pe prima linie faptul ca avem configuratie stabila la pozitia pozitie_configuratie_stabila, apoi urmand sa afisam in fisierul de output configuratia de unde incepe ciclicitatea. In caz contrar, daca nu gasim 
	2 matrici identice pana la numarul de pasi primit ca parametru, vom afisa ultima configuratie, cea de la pozitia "nr_pasi". Rezultatul acestui proces este scris in fisierul file_out cu ajutorul functiei "board_file_write".
	La final dezalocam matricea in care am stocat tabla de joc, dezalocam pe rand fiecare element al vectorului de matrici, apoi dezalocam vectorul de matrici si, intr-un final, inchidem fisierele de input si output.	
	
	Dupa executarea uneia dintre aceste functii, se incheie programul.

Acum voi prezenta functia "evolution",  cea mai importanta functie din tema, cu care am produs evolutia tablei de organisme:
Am creat un pointer dublu la tabla veche si am alocat dinamic o noua tabla de aceleasi dimensiuni ca tabla veche. Parcurgem vechea tabla element cu element si calculam pt fiecare cati vecini are. Parcurgem matricea de 3*3 din jurul 
fiecarui element si vedem care din elemente sunt egale cu 1. La fiecare element "viu", incrementam nr de vecini cu 1. Matricea din jurul unui element de pe margini este diferita fata de elementele din interior, de aceea a trebuit 
sa gasim o solutie optima pt toate cazurile posibile de elemente din matrice. De aceea, am luat coordonatele vecinilor lui board[i][j] de forma board[(a + height) % height][(b + width) % width], cu a intre i-1 si i+1, iar b intre
j-1 si j+1, deoarece ele nu aduc nicio schimbare coordonatelor vecinilor elementelor din interior, iar elementelor de pe margini le transforma coordonatele de -1 in cele de pe partea opusa a marginii tablei. Daca elementul analizat este 
egal cu 1, il vom scadea din nr de vecini, deoarece cand am analizat matricea de vecini, am inclus si elementul nostru. Acum, daca avem 3 vecini sau 2 vecini si elementul analizat este 1, atunci pe noua tabla celula de la pozitia (i,j) 
va fi vie. In caz contrar va fi moarta. Apoi elementele din noua tabla vor fi puse in vechea tabla si noua tabla va fi stearsa.

OBSERVATII:
- pentru alocarea si dezalocarea matricilor am folosit functiile "matrix_alloc" si respectiv "matrix_delete"
- matricile din fisiere de input/output nu au spatii intre linii si coloane, toate elementele sunt lipite intre ele, de aceea am citit liniile ca string-uri si nu am folosit fscanf cu format string-ul de forma "%d".
  Am ales aceasta forma deoarece arata mai bine din punct de vedere grafic si este mai usor sa observati organismele care evolueaza, la asta adaugandu-se faptul ca este mai usor de citit/scris din/in fisier
- cea mai interesanta parte a proiectului si cea mai buna idee din el este, din punctul meu de vedere, faptul ca am folosit polimorfism la functiile jocului, pentru a usura procesul de citire a fisierului de input si de a face programul
  mai versatil.

Printre probleme intampinate s-au numarat:
* gasirea unui procedeu cat mai bun pentru evolutia tablei de joc, in special in cazul celulelor de pe margini
* gasirea unei metode de a usura procesul de interpretare a fisierelor de input si de utilizare a rezultatelor obtinute, ca solutie fiind folosit polimorfismul
* intelegerea si crearea procedeului de stabilire a configuratiei stabilire, precum si alegerea unei metode mai bune. A trebuit sa fac un compromis legat de memorie in favoarea vitezei de executare a programului si sa folosesc metoda 
  vectorului de matrici. Acest compromis a fost intr-o oarecare masura atenuat de folosirea matricilor cu elemente de tipul char in loc de int, ceea ce a redus memoria consumata de 4 ori, precum si a realocarii dinamice.
* Incadrarea in limita de 50 de linii de cod, in special la functia main si functiile care deruleaza jocul, in special "game_of_life_stabil". Initial am vrut cele doua functii ale jocului sa fie combinate si sa se comporte diferit in functie
  de un parametru care determina daca trebuia sa verificam configuratia stabila sau nu, dar acea functie depasea 50 de linii de cod, asa ca am fost nevoit sa o despart in doua.
* cateva erori minore legate de memorie si scriere a fisierelor, remediate destul de rapid