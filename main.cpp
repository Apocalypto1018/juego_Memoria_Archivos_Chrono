#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <sstream>
#include <fstream>
#include <string.h>
#include <cstring>
#include <iomanip>
#include <Windows.h>
#include <chrono>
#include <conio.h>
#define STRING(num) #num

#define MI_SLEEP 1000

using namespace std;

// prototipos

void menu(bool *);
void agregarNuevoJuego(string, int, int, float);
const string Fecha();

void interrumpir();
void elegirCategoria(int *);
void imprimir(string **matriz, int, int);
void puntaje(int, int, string);
void mostrarTablero(int, int, string **matriz, int[16]);
int generarNumero();
void juego(int, int, int);
void juegoMaquina(int, int, int);
void intercambioAleatorio(string **matriz, int, int);
void menuJugar();
void clean_stdin(void);
void continuar();
void generarPalabras(string **matriz, int, int, int);
int validarMostrarPlabrasSeleccionadas(int, int, string **matriz, int, int);
void coordenada(int *, int *, int);
int posValidas(int, int, int[16], int);
int ocupado(int, int, int, int, int[16]);
void vaciar(int[16]);
void gotoxy(int, int);
void setCColor(int);

int color[7] = {
	0x009,
	0x00E,
	0x00C,
	0x002,
	0x00B,
	0x005,
	0x00F

};

int main()
{
	setCColor(color[5]);
	bool salir = false;

	while (!salir)
	{
		system("cls");
		menu(&salir);
	}
	return 0;
}
void menu(bool *salir)
{
	srand(time(0)); // reteando la semilla

	int opc = 0;

	gotoxy(20, 0);
	cout << "Menu de Opciones";
	gotoxy(5, 2);
	cout << "1. Jugar";
	gotoxy(5, 4);
	cout << "2. Simulacion";
	gotoxy(5, 6);
	cout << "3. SALIR\n->";
	cin >> opc;

	system("cls");

	switch (opc)
	{

	case 1:
	{

		menuJugar();
		break;
	}
	case 2:
	{
		juegoMaquina(4, 3, (1 + rand() % 3));

		break;
	}
	case 3:
	{
		*salir = true;
		break;
	}
	default:
		break;
	}
}

void menuJugar()
{
	bool volver = false;
	int filas = 4, columnas = 3;

	int categoria = 1;
	int opcion;
	while (!volver)
	{
		system("cls");
		gotoxy(20, 0);
		cout << "Menu Jugar";
		gotoxy(2, 2);
		cout << "1. Elegir Tema para la memoria";
		gotoxy(2, 4);
		cout << "2. Iniciar Juego";
		gotoxy(2, 6);
		cout << "3. Volver al menu principal\n->";

		cin >> opcion;

		switch (opcion)
		{
		case 1:
			system("cls");
			elegirCategoria(&categoria);
			break;

		case 2:
			juego(filas, columnas, categoria);
			cin.ignore();
			continuar();
			break;
		case 3:
			volver = true;
			break;
		default:
			cout << "Ingrese una opcion valida.";
			break;
		}
	}
}

void elegirCategoria(int *categoria)
{

	int opc = 0;

	do
	{
		gotoxy(20, 0);
		cout << "SubMenu de Eleccion de Categoria de Palabras del juego" << endl
			 << endl;
		;
		gotoxy(1, 1);
		cout << "*Ingrese 1 para elegir Categoria 'Aves'" << endl
			 << endl;
		gotoxy(1, 2);
		cout << "*Ingrese 2 para elegir Categoria 'Deportes'" << endl
			 << endl;
		gotoxy(1, 3);
		cout << "*Ingrese 3 para elegir Categoria 'Autos'\n\n->";
		cin >> opc;

		system("cls");
	} while (opc < 1 || opc > 3);

	*categoria = opc;
}

void juego(int filas, int columnas, int categoria)
{
	srand(time(0)); // seteando la semilla
	unsigned t0, t1;

	t0 = clock(); int jugar = 1;

	int generar = 1;
	int pos1 = 0;
	int pos2 = 0;

	int posAcertadas[16]; // vector que almacena las posiciones acertadas
	int pos = 0;

	int fin = 0; // var condicional
	int intentos = 0;
	int aciertos = 0;
	int correcto = 0;

	string alias;

	bool iniLetra = false;

	bool impTableroSolucion = false;

	string **matriz = new string *[columnas];
	for (int i = 0; i < filas; i++)
		matriz[i] = new string[filas];

	// rutina de registrar alias
	do
	{
		system("cls");
		cout << endl
			 << "*Ingrese el alias\n->";
		cin >> alias;

		if (isalpha(alias.at(0)))
		{
			iniLetra = true;
		}
		else
		{

			cout << endl
				 << "*Error!. El alias debe iniciar en una letra del alfabeto" << endl;
			continuar();
		}

		if (alias.length() < 4)
		{

			cout << endl
				 << "*Error!. El alias ingresado tiene menos de 4 caracteres" << endl;
			continuar();
		}
		else if (alias.length() > 12)
		{

			cout << endl
				 << "*Error!. El alias ingresado tiene mas de 12 caracteres" << endl;
			continuar();
		}

		cin.ignore();

	} while (iniLetra != true || alias.length() < 4 || alias.length() > 12);

	system("cls");

	while (jugar == 1 || jugar == 2)
	{

		if (generar == 1)
		{

			generarPalabras(matriz, filas, columnas, categoria);

			imprimir(matriz, filas, columnas);
			cout << "\nTiempo de previsualizacion: 3s\n";
			Sleep(MI_SLEEP); // pausa por 1 segundos
			system("cls");

			imprimir(matriz, filas, columnas);
			cout << "\nTiempo de previsualizacion: 2s\n";
			Sleep(MI_SLEEP); // pausa por 1 segundos
			system("cls");

			imprimir(matriz, filas, columnas);
			cout << "\nTiempo de previsualizacion: 1s\n";
			Sleep(MI_SLEEP); // pausa por 1 segundos
			system("cls");

			generar++;
			vaciar(posAcertadas);
		}

		do
		{
			printf("Cargando posiciones...\n");
			Sleep(MI_SLEEP);
			system("cls");
			puntaje(aciertos, intentos, alias);
			if (impTableroSolucion)
			{
				cout << "         Tablero de Solucion\n";
				imprimir(matriz, filas, columnas);
				cout << "\n          Tablero Normal\n";
			}
			mostrarTablero(filas, columnas, matriz, posAcertadas); // se invoca la funcion que mostrara las posiciones
			// solicitud de las posiciones
			Sleep(MI_SLEEP);

			cout << endl
				 << "*Ingrese la primera posicion\n->";
			cin >> pos1;

			cout << endl
				 << "*Ingrese la segunda posicion\n->";
			cin >> pos2;

			if (pos1 <= 0 || pos2 <= 0 || pos1 > filas * columnas || pos2 > filas * columnas)
			{

				cout << endl
					 << "Error!. Posicion invalida detectada!" << endl;
				cin.ignore();
				continuar();
			}

			cout << endl
				 << endl;

		} while (posValidas(pos1, pos2, posAcertadas, pos) != 1 || pos1 <= 0 || pos2 <= 0 || pos1 > filas * columnas || pos2 > filas * columnas); // ciclo que asegura que las opsiciones ingresadas no sean iguales a las ya acertadas

		cin.ignore();

		correcto = validarMostrarPlabrasSeleccionadas(filas, columnas, matriz, pos1, pos2);
		intentos++;
		cout << "/---------------------------------------/\n";

		if (correcto == 1)
		{

			cout << endl
				 << "*Posiciones ingresadas: " << pos1 << " y " << pos2 << endl;
			cout << "\n*Has acertado!!\n\n";
			continuar();
			aciertos++;

			posAcertadas[pos] = pos1;
			pos++;
			posAcertadas[pos] = pos2;
			pos++;

			// Sistema de aciertos por nivel para saber cuando finaliza el Juego

			if (aciertos == 6)
			{
				fin = 1;
			}
		}
		else
		{

			cout << endl
				 << "*Posiciones ingresadas: " << pos1 << " y " << pos2 << endl;
			cout << "\n*Lo siento. No has acertado!!\n\n";
			continuar();
		}

		do
		{
			if (fin != 1)
			{

				system("cls");

				gotoxy(2, 1);
				cout << "*Ingrese 1 para CONTINUAR jugando";
				gotoxy(2, 3);
				cout << "*Ingrese 2 para Seguir jugando Viendo el Tablero de Solucion";
				gotoxy(2, 5);
				cout << "*Ingrese 0 para SALIR al menu principal\n\n->";
				cin >> jugar;

				if (jugar == 2)
				{
					impTableroSolucion = true;
				}

				system("cls");
			}

		} while (jugar < 0 || jugar > 2);

		if (fin == 1)
		{ // si terminar el usuario de descubrir los pares...

			t1 = clock();

			double tiempo = (double(t1 - t0) / CLOCKS_PER_SEC);
			do
			{
				system("cls");
				gotoxy(1, 0);
				cout << "*" << alias << " Has terminado el Juego!!" << endl
					 << endl;
				gotoxy(1, 3);
				cout << "*Duracion: " << intentos << " intentos totales." << endl
					 << endl;
				gotoxy(1, 5);
				cout << "*Tiempo total de Juego: " << tiempo << " Segundos." << endl;

				agregarNuevoJuego(alias, intentos, categoria, tiempo);
				gotoxy(1, 7);
				cout << "*Datos del juego almacenados en el archivo correctamente!!" << endl;

				gotoxy(0, 10);
				cout << "*Ingrese 1 para Volver a Jugar\n\n";
				cout << "*Ingrese 0 para SALIR al menu principal\n\n->";
				cin >> jugar;

				system("cls");

				if (jugar == 1 && jugar == 2)
				{
					generar = 1;
					pos1 = 0;
					pos2 = 0;

					vaciar(posAcertadas);
					pos = 0;

					fin = 0;
					intentos = 0;
					aciertos = 0;
					correcto = 0;
				}

			} while (jugar < 0 || jugar > 2);
		}
	}
}
void juegoMaquina(int filas, int columnas, int categoria)
{
	srand(time(0)); // seteando la semilla
	unsigned t0, t1;
	t0 = clock();
	int jugar = 1;

	int generar = 1;
	int pos1 = 0;
	int pos2 = 0;

	int posAcertadas[12]; // vector que almacena las posiciones acertadas
	int pos = 0;

	int fin = 0; // var condicional
	int intentos = 0;
	int aciertos = 0;
	int correcto = 0;
	bool maquinaTonta = false;

	string alias = "PC";

	string **matriz = new string *[columnas];

	for (int i = 0; i < filas; i++)
		matriz[i] = new string[filas];

	while (jugar == 1)
	{

		if (generar == 1)
		{

			generarPalabras(matriz, filas, columnas, categoria);

			imprimir(matriz, filas, columnas);
			cout << "\nTiempo de previsualizacion: 4s\n";
			Sleep(MI_SLEEP); // pausa por 1 segundos
			system("cls");

			imprimir(matriz, filas, columnas);
			cout << "\nTiempo de previsualizacion: 3s\n";
			Sleep(MI_SLEEP); // pausa por 1 segundos
			system("cls");

			imprimir(matriz, filas, columnas);
			cout << "\nTiempo de previsualizacion: 2s\n";
			Sleep(MI_SLEEP); // pausa por 1 segundos
			system("cls");

			imprimir(matriz, filas, columnas);
			cout << "\nTiempo de previsualizacion: 1s\n";
			Sleep(MI_SLEEP); // pausa por 1 segundos
			system("cls");

			generar++;
			vaciar(posAcertadas);
		}

		do
		{

			cout << "Cargando posiciones...\n";
			Sleep(MI_SLEEP);
			system("cls");
			puntaje(aciertos, intentos, alias);
			mostrarTablero(filas, columnas, matriz, posAcertadas); // se invoca la funcion que mostrara las posiciones
			// solicitud de las posiciones
			Sleep(MI_SLEEP);

			cout << endl
				 << "*Ingrese la primera posicion\n->";

			do
			{
				pos1 = 1 + rand() % 12;

				maquinaTonta = false;
				for (int i = 0; i < 12; i++)
				{

					if (pos1 == posAcertadas[i])
						maquinaTonta = true;
				}
			} while (maquinaTonta);

			cout << pos1;
			Sleep(MI_SLEEP);
			cout << endl
				 << "*Ingrese la segunda posicion\n->";
			do
			{
				pos2 = 1 + rand() % 12;

				maquinaTonta = false;
				for (int i = 0; i < 12; i++)
				{
					if (pos1 == pos2 || pos1 == posAcertadas[i] || pos2 == posAcertadas[i])
						maquinaTonta = true;
				}
			} while (maquinaTonta);

			cout << pos2;
			Sleep(MI_SLEEP);
			if (pos1 <= 0 || pos2 <= 0 || pos1 > filas * columnas || pos2 > filas * columnas)
			{

				cout << endl
					 << "Error!. Posicion invalida detectada!" << endl;
				Sleep(MI_SLEEP);
				// continuar();
			}

			cout << "\n\n";

		} while (posValidas(pos1, pos2, posAcertadas, pos) != 1 || pos1 <= 0 || pos2 <= 0 || pos1 > filas * columnas || pos2 > filas * columnas); // ciclo que asegura que las opsiciones ingresadas no sean iguales a las ya acertadas

		correcto = validarMostrarPlabrasSeleccionadas(filas, columnas, matriz, pos1, pos2);
		intentos++;
		cout << "/---------------------------------------/\n";

		if (correcto == 1)
		{

			cout << endl
				 << "*Posiciones ingresadas: " << pos1 << " y " << pos2 << endl;
			cout << "\n*Has acertado!!\n\n";
			Sleep(MI_SLEEP);
			aciertos++;

			posAcertadas[pos] = pos1;
			pos++;
			posAcertadas[pos] = pos2;
			pos++;

			// Sistema de aciertos por nivel para saber cuando finaliza el uego

			if (aciertos == 6)
			{
				fin = 1;
			}
		}
		else
		{

			cout << endl
				 << "*Posiciones ingresadas: " << pos1 << " y " << pos2 << endl;
			cout << "\n*Lo siento. No has acertado!!\n\n";
			Sleep(MI_SLEEP);
		}

		if (fin == 1)
		{ // si terminar el usuario de descubrir los pares...
			do
			{
				t1 = clock();

				float tiempo = (float(t1 - t0) / CLOCKS_PER_SEC);
				system("cls");
				gotoxy(5, 0);
				cout<< "*Simulacion Terminada!!" << endl
					<< endl;
				gotoxy(1, 3);
				cout << "*Duracion: " << intentos << " intentos totales." << endl
					<< endl;
				gotoxy(1, 5);
				cout << "*Tiempo total de Juego: " << tiempo << " Segundos." << endl;

				agregarNuevoJuego(alias, intentos, categoria, tiempo);
				gotoxy(1, 7);
				cout << "*Datos del juego almacenados en el archivo correctamente!!" << endl
					<< endl;
				
				jugar = 0;
				
				Sleep(MI_SLEEP * 5);
				
				system("cls");

				if (jugar == 1)
				{

					generar = 1;
					pos1 = 0;
					pos2 = 0;

					vaciar(posAcertadas);
					pos = 0;

					fin = 0;
					intentos = 0;
					aciertos = 0;
					correcto = 0;
				}

			} while (jugar < 0 || jugar > 1);
		}
	}
}

// funcion que cambiara la posicion ingresada por el usuario a coordenada
void coordenada(int *f, int *c, int pos)
{

	switch (pos)
	{
	case 1:
		*f = 0;
		*c = 0;
		break;
	case 2:
		*f = 0;
		*c = 1;
		break;
	case 3:
		*f = 0;
		*c = 2;
		break;
	case 4:
		*f = 1;
		*c = 0;
		break;
	case 5:
		*f = 1;
		*c = 1;
		break;
	case 6:
		*f = 1;
		*c = 2;
		break;
	case 7:
		*f = 2;
		*c = 0;
		break;
	case 8:
		*f = 2;
		*c = 1;
		break;
	case 9:
		*f = 2;
		*c = 2;
		break;
	case 10:
		*f = 3;
		*c = 0;
		break;
	case 11:
		*f = 3;
		*c = 1;
		break;
	case 12:
		*f = 3;
		*c = 2;
		break;
	default:
		break;
	}
}

// funcion que muestra el puntaje
void puntaje(int aciertos, int intentos, string nombre)
{
	cout << "                                 MEMORIA      " << endl
		 << endl;
	cout << "*Jugador: " << nombre;
	cout << "   ||    *Numero de Intentos: " << intentos << "    ||    *Pares encontrados: " << aciertos << endl
		 << endl;
}

void generarPalabras(string **matriz, int filas, int columnas, int categoria)
{

	string aves[5] = {"Halcon", "Lechuza", "Pato", "Picargo", "Gallo"};
	string deporte[5] = {"Balon", "Bate", "Guantes", "Nadar", "Correr"};
	string autos[5] = {"Capo", "Calle", "Chasis", "Diesel", "Faros"};

	int nroElemento = (filas * columnas) / 2; // esto seria n^2/2

	int n = 0;
	int k = 0, cont = 0;

	string numAleatorio[4 * 3];

	// llenamos el vector con pares de letras aleatorias
	while (nroElemento)
	{
		n = generarNumero();
		if (categoria == 1)
		{
			numAleatorio[nroElemento - 1] = aves[n];
		}
		else if (categoria == 2)
		{
			numAleatorio[nroElemento - 1] = deporte[n];
		}
		else
		{
			numAleatorio[nroElemento - 1] = autos[n];
		}
		nroElemento--;
	}

	for (int i = 0; i < filas; i++)
	{
		for (int j = 0; j < columnas; j++)
		{

			matriz[i][j] = numAleatorio[k];
			cont++;

			if (cont % 2 == 0)
			{
				k++;
			}
		}
	}

	intercambioAleatorio(matriz, filas, columnas); // invocamos la funcion que intercambiara aleatoriamentente las pareas generadas
}

int generarNumero()
{

	int n = 0;
	n = rand() % 5;

	return n;
}

// intercambio de forma aleatoria los elementos
void intercambioAleatorio(string **matriz, int filas, int columnas)
{
	int tempI, tempJ;
	string temp;
	// recorremos la matriz
	for (int i = 0; i < filas; i++)
	{
		for (int j = 0; j < columnas; j++)
		{
			// asigna aleatoriamente valores de la matriz durante el recorrido
			tempI = rand() % filas;
			tempJ = rand() % columnas;
			temp = matriz[i][j];
			matriz[i][j] = matriz[tempI][tempJ];
			matriz[tempI][tempJ] = temp;
		}
	}
}

void imprimir(string **matriz, int filas, int columnas)
{

	for (int i = 0; i < filas; i++)
	{
		for (int j = 0; j < columnas; j++)
		{

			cout << matriz[i][j] << "\t\t";
		}
		cout << endl
			 << endl;
	}
}

// funcion que determina si las posiciones ingresadas son validas
int posValidas(int pos1, int pos2, int posAcertadas[36], int pos)
{

	for (int i = 0; i < pos; i++)
	{

		if (pos1 == posAcertadas[i])
		{
			system("cls");
			cout << "*La posicion " << pos1 << " ya ha sido descubierta" << endl;
			continuar();
			return 0;
		}

		if (pos2 == posAcertadas[i])
		{
			system("cls");
			cout << "*La posicion " << pos2 << " ya ha sido descubierta" << endl;
			continuar();
			return 0;
		}
	}

	return 1;
}

// funcion que imprime las posiciones de la matriz
void mostrarTablero(int filas, int columnas, string **matriz, int posAcertadas[16])
{
	// variables locales
	int posicion = 1;
	int control = 0;

	for (int i = 0; i < filas; i++)
	{

		for (int j = 0; j < columnas; j++)
		{
			// recorre la matriz y muestra la variable posicion que se aumenta por cada iteracion, mostrando asi cada posicion
			control = ocupado(i, j, filas, columnas, posAcertadas); // invocacion de la funcion que valida si hubo coincidencia de una posicion acertada

			// de existir una posicion acertada, se muestra la palabra
			if (control == 1)
			{
				cout << matriz[i][j] << "\t\t";
			}
			else
			{ // de no existir coincidencia, solo se muestra la ####

				cout << posicion << "\t\t";
			}

			posicion++;
		}

		cout << endl
			 << endl;
	}
}

// funcion que valida si la posicion ya esta ocupada
int ocupado(int i, int j, int filas, int columnas, int posAcertadas[16])
{
	int f1, c1; // almacenan las filas y las columnas de las posiciones en la matriz
	int m = 0;
	// se recorre la matriz
	for (int k = 0; k < filas; k++)
	{

		for (int l = 0; l < columnas; l++)
		{
			// se llama a la funcion que retornara las coordenadas de las posiciones acertadas almacenadas en el vector de posiciones acertadas
			coordenada(&f1, &c1, posAcertadas[m]);

			// se valida si existe coincidencia entre las coordenadas de la matriz y las coordenadas de la posicion del vector durante la iteracion
			if (f1 == i && c1 == j && posAcertadas[m] != 0)
			{
				return 1;
			}

			m++;
		}
	}

	return 0;
}

// funcion que muestra las posiciones elegidas
int validarMostrarPlabrasSeleccionadas(int filas, int columnas, string **matriz, int pos1, int pos2)
{
	string aux, aux2;
	int f1 = 0, c1 = 0, f2 = 0, c2 = 0; // almacenan las filas y las columnas de las posiciones en la matriz

	int posicion = 1;

	coordenada(&f1, &c1, pos1); // invocacion de la funcion que convierte la posicion a coordenada de la primera posicion ingresada
	coordenada(&f2, &c2, pos2); // invocacion de la funcion que convierte la posicion a coordenada de la segunda posicion ingresada

	system("cls");

	for (int i = 0; i < filas; i++)
	{

		for (int j = 0; j < columnas; j++)
		{

			if (i == f1 && j == c1)
			{
				aux = matriz[i][j];
				cout << matriz[i][j] << "\t\t";
				posicion++;
			}
			else if (i == f2 && j == c2)
			{
				aux2 = matriz[i][j];
				cout << matriz[i][j] << "\t\t";
				posicion++;
			}
			else
			{
				cout << posicion << "\t\t";
				posicion++;
			}
		}

		cout << endl
			 << endl;
	}

	cout << endl
		 << aux << "=" << aux2 << endl;

	cout << endl
		 << "Coordenada Pos1: " << f1 << " " << c1 << endl
		 << "Coordenada Pos2: " << f2 << " " << c2 << endl;

	if (aux == aux2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// vaciar array
void vaciar(int posAcertadas[16])
{
	for (int i = 0; i < 16; i++)
	{
		posAcertadas[i] = 0;
	}
}

void agregarNuevoJuego(string alias, int intentos, int categoria, float tiempo)
{
	fstream archivo("registros.txt", ios::app); // se abre el archivo en modo modificacion para evitar sobreescritura

	if (!archivo.is_open())
	{ // si ocurre un error al abrir el archivo...
		cout << "Fichero no encontrado\n";
		system("pause");
	}
	else
	{ // si todo sale bien...

		// alias
		archivo << "Alias: ";
		archivo << alias;
		// intentosTotales
		archivo << " | intentos: ";
		string aux(to_string(intentos));
		archivo << aux;

		// categoria
		archivo << " | Categoria: ";
		if (categoria == 1)
		{
			archivo << "Aves";
		}
		if (categoria == 2)
		{
			archivo << "Deportes";
		}
		if (categoria == 3)
		{
			archivo << "Autos";
		}
		// tiempo
		archivo << " | Tiempo Total: ";
		string num_str1(to_string(tiempo));
		archivo << num_str1;
		// fecha
		archivo << " | Fecha: ";
		archivo << Fecha();
		archivo << endl;
		archivo.close(); // se cierra el fichero
	}
	cin.ignore();
}

const string Fecha()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}

void continuar()
{
	cout << "\n\nPresione enter para continuar...";
	cin.get();
}

void clean_stdin(void)
{
	int c;
	do
	{
		c = getchar();
	} while (c != '\n' && c != EOF);
}

void gotoxy(int x, int y)
{
	COORD a;
	a.X = x;
	a.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), a);
}
void setCColor(int color)
{
	static HANDLE hConsole;
	int backcolor = 0;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, color | (backcolor * 0x10 + 0x100));
}
