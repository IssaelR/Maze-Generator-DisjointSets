#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <unistd.h>
#include <fstream>
#include <iomanip>
#include "comprobadores.h"

// Definir la dimension de la matriz Laberinto(Maze)
int lenY = 10;
int lenX = 10;

// Definir el caracter que sera usado para mostrar la posicion actual del jugador
#define leftIndicator '0'

// Colores y fuentes
#define RED     "\x1b[31m"
#define YELLOW  "\x1b[33m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
#define BOLD    "\e[1;37m"
#define UNDER   "\e[4;37m"

int level = 1;
// Estructura de datos Union-Find
class UnionFind {
private:
    // Arreglo para representar la estructura de conjuntos disjuntos
    std::vector<int> parent;

public:
    // Constructor inicializa la estructura de Union-Find con un tamaño específico.
    // parent(size) crea un vector parent con el tamaño especificado
    UnionFind(int size) : parent(size) {
        for (int i = 0; i < size; ++i) {
            parent[i] = i;
        }
    }

    // Se busca el representante (el padre) del conjunto al que pertenece el elemento i.
    int find(int i) {
        // Se busca recursivamente, si el elemento i no es su propio padre
        if (parent[i] != i) {
            parent[i] = find(parent[i]);
        }
        return parent[i];
    }

    // Se une los conjuntos a los que pertenecen a y b mediante la modificación de sus representantes.
    void unite(int a, int b) {
        parent[find(a)] = find(b);
    }
};

// Struct que representa los muros
struct Edge {
    int y1, x1, y2, x2;
};

class Maze {
protected:
    // Puntero doble para almacenar la matriz bidimensional del Laberinto(Maze)
    char** maze;
    // Instancia de UnionFind para realizar operaciones de unión y búsqueda eficientes
    UnionFind uf;
    // Representamos los paredes(bordes) entre las celdas del laberinto.
    std::vector<Edge> edges;
    // Almacenamos el tamaño del laberinto en términos de filas (lenY) y columnas (lenX).
    int lenY, lenX;

public:
    // Inicializa un laberinto con un tamaño específico donde se inicializa lenY, lenX y
    // la instancia de UnionFind, ademas se llama a la funcion generateMaze()
    Maze(int y, int x) : lenY(y), lenX(x), uf(y * x) {
        generateMaze();
    }

    void generateMaze() {
        maze = new char*[lenY];
        // Reservar memoria para las filas
        for (int i = 0; i < lenY; ++i) {
            // Reservar memoria para las columnas
            maze[i] = new char[lenX];
        }
        // Inicializar el laberinto con paredes(de manera visual digamoslo)
        for (int i = 0; i < lenY; i++) {
            for (int j = 0; j < lenX; j++) {
                // Se alterna entre filas y columnas para crear una estructura de laberinto inicial
                // Si i o j es un número par, se coloca un carácter de pared (219)
                maze[i][j] = (i % 2 == 0 || j % 2 == 0) ? 219 : ' ';
            }
        }
        // Inicializar las paredes(bordes) para un cuadradicula y colocar los bordes en vector<Edge> edge
        // Se itera sobre las filas del laberinto, saltando de dos en dos (ya que las paredes se colocan en posiciones pares).
        for (int y = 1; y < lenY - 1; y += 2) {
            // Se itera sobre las columnas del laberinto, de manera similar, saltando de dos en dos.
            for (int x = 1; x < lenX - 1; x += 2) {
                // Agrega un borde entre la celda actual (y, x) y la celda a la derecha (y, x + 2) a la lista de bordes (edges)
                if (x < lenX - 2) edges.push_back({ y, x, y, x + 2 }); // celda derecha
                // Agrega un borde entre la celda actual (y, x) y la celda debajo (y + 2, x) a la lista de bordes (edges)
                if (y < lenY - 2) edges.push_back({ y, x, y + 2, x }); // celda debajo
            }
        }

        // Mezclar los paredes(bordes), std::shuffle mezcla aleatoriamente los elementos dentro del vector
        // edges.begin(), edges.end() son el rango
        // std::default_random_engine(static_cast<unsigned int>(time(nullptr))) crea un generador de números
        // aleatorios utilizando el reloj del sistema como semilla. unsigned int, se usa como semilla para
        // generar una secuencia de números aleatorios única cada vez que se ejecute el programa
        std::shuffle(edges.begin(), edges.end(), std::default_random_engine(static_cast<unsigned int>(time(nullptr))));

        // Algoritmo de kruskal para derrumbar las paredes(bordes)
        // Se itera a través de todos los bordes almacenados en el vector edges
        for (const auto& edge : edges) {
            // Se utiliza la instancia uf para llevar a cabo las operaciones de unión y búsqueda
            // uf.find() para encontrar el conjunto al que pertenecen las celdas divididas por el borde (edge)
            int set1 = uf.find(edge.y1 * lenX + edge.x1);
            int set2 = uf.find(edge.y2 * lenX + edge.x2);

            // Si las 2 celdas divididas por un muro no estan conectadas, entonces "derrumba" la pared(borde)
            if (set1 != set2) {
                uf.unite(set1, set2);
                // Se calculan las coordenadas del punto medio entre las dos celdas
                // las cuales representan la posición en la que se debe "romper" la pared
                int wallY = (edge.y1 + edge.y2) / 2;
                int wallX = (edge.x1 + edge.x2) / 2;
                maze[wallY][wallX] = ' '; // "Romper" el muro
            }
        }

        // Abrir la entrada
        maze[1][0] = ' '; // Entrada

        // Asegurar que las paredes externas esten intactas(excepto la salida)
        for (int y = 0; y < lenY; y++) {
            maze[y][0] = 219; // Paredes al extremo izquierdo
            maze[y][lenX - 1] = 219; // Paredes al extremo derecho
        }
        for (int x = 0; x < lenX; x++) {
            maze[0][x] = 219; // Paredes superiores
            maze[lenY - 1][x] = 219; // Paredes inferiores
        }

        // Terminar el camino a la salida
        maze[lenY - 2][lenX - 1] = ' '; // Salida al fondo a la derecha
        maze[lenY - 2][lenX - 2] = ' '; // Asegurarse que la celda a la izquierda de la salida esta vacia

        // Asegurar que la pared derecha esta intacta con excepcion a la salida
        for (int y = 0; y < lenY - 1; y++) {
            maze[y][lenX - 1] = 219;
        }
        maze[lenY - 2][lenX - 1] = ' ';
    }

    void printMaze() {
        std::cout <<std::setw(60)<< BOLD << "Level: " << level << RESET << "\n\n";
        std::cout<<std::setw(60);
        for (int i = 0; i < lenY; i++) {
            std::cout<<std::setw(60);
            for (int j = 0; j < lenX; j++) {
                if (maze[i][j] == leftIndicator) {
                    std::cout <<YELLOW << maze[i][j] << RESET;
                }
                else {
                    std::cout <<CYAN << maze[i][j] << RESET;
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    int getTotalSize() const {
        return lenX * lenY;
    }

    // Permite que la clase Movement acceda a los miembros y funciones privadas de la clase Maze
    // Es útil porque Movement necesita acceder a la matriz del laberinto y modificarla según el movimiento del jugador.
    friend class Movement;
};

class Movement : public Maze {
private:
    // currentX y currentY almacenan la posición actual del jugador en el laberinto.
    int currentX, currentY, countMoves;
    // Posición predeterminada para iniciar el jugador en el laberinto.
    const int defaultY = 1, defaultX = 1;
    // Posición que define la salida del laberinto.
    const int gameOverY, gameOverX;
    // Puntaje inicial del jugador en función del tamaño del laberinto.
    int score = getTotalSize();

public:
    // Este constructor llama al constructor de Maze con las dimensiones proporcionadas y
    // configura las posiciones iniciales del jugador.
    Movement(int y, int x) : Maze(y, x), gameOverY(y - 2), gameOverX(x - 2) {
        countMoves = 0;
        currentX = defaultX;
        currentY = defaultY;
        // Se sincroniza la posición del jugador en el laberinto con estas coordenadas iniciales.
        updatePlayerLocation();
    }


    bool isGameOver() {
        return currentY == gameOverY && currentX == gameOverX; // Revisar si se ha llegado al punto de salida
    }

    int nextLevel() {
        return level++;
    }

    void resetLastLocation() {
        maze[currentY][currentX] = ' '; // Limpiar la ultima posicion del jugador
    }

    void updatePlayerLocation() {
        maze[currentY][currentX] = leftIndicator; // Actualiza la posicion actual del jugador
    }

    int getMoves() {
        return countMoves;
    }

    int getScore() {
        return score;
    }

    void userInput() {
        char key = 0;

        while (key != 'w' && key != 's' && key != 'a' && key != 'd') {
            key = _getch();
            resetLastLocation();
            switch (key) {
                case 'w':
                    if (maze[currentY - 1][currentX] == ' ') {
                        currentY--;
                        score--;
                    }
                    break;
                case 's':
                    if (maze[currentY + 1][currentX] == ' ') {
                        currentY++;
                        score--;
                    }
                    break;
                case 'a':
                    if (maze[currentY][currentX - 1] == ' ') {
                        currentX--;
                        score--;
                    }
                    break;
                case 'd':
                    if (maze[currentY][currentX + 1] == ' ') {
                        currentX++;
                        score--;
                    }
                    break;
            }
            updatePlayerLocation();
        }
    }

    void doMove() {
        fflush(stdin);
        printMaze(); // Print the current state of the maze

        countMoves++;

        userInput(); // Get user input and move the player

        // Solo limpiar la pantalla si el usuario realiza un movimiento
        if (maze[currentY][currentX] == leftIndicator) {
            system("cls"); // Clear the console
        }
        std::cout << std::setw(68) <<"Movimientos: " << countMoves << std::endl;
        std::cout << std::setw(65) <<"Puntaje: " << score << std::endl;
        std::cout<<"\n";

        if(maze[lenY - 2][lenX - 2] == leftIndicator) {
            system("cls");
            std::cout << std::setw(60)<<"SIGUIENTE NIVEL" << std::endl;
            system("pause");
            nextLevel();
        }
    }
};

void safeFile(int countMoves, int score, int maxScore){
    std::ofstream file;
    file.open("score.txt", std::ios::app);
    file << std::setw(50) <<"Nivel: " << level-1 << " Movimientos: " << countMoves << " Puntaje: " << score << "/" << maxScore << std::endl;
    file.close();
}

void readFile(){
    std::ifstream file;
    std::string line;
    file.open("score.txt");
    while (getline(file, line)) {
        std::cout << line << std::endl;
    }
    file.close();
    system("pause");
}


void welMsg(){
    std::cout << "██╗      █████╗ ██████╗ ███████╗██████╗ ██╗███╗   ██╗████████╗ ██████╗ " <<  std::endl;
    std::cout << "██║     ██╔══██╗██╔══██╗██╔════╝██╔══██╗██║████╗  ██║╚══██╔══╝██╔═══██╗" <<  std::endl;
    std::cout << "██║     ███████║██████╔╝█████╗  ██████╔╝██║██╔██╗ ██║   ██║   ██║   ██║" <<  std::endl;
    std::cout << "██║     ██╔══██║██╔══██╗██╔══╝  ██╔══██╗██║██║╚██╗██║   ██║   ██║   ██║" <<  std::endl;
    std::cout << "███████╗██║  ██║██████╔╝███████╗██║  ██║██║██║ ╚████║   ██║   ╚██████╔╝" <<  std::endl;
    std::cout << "╚══════╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝   ╚═╝    ╚═════╝ " <<  std::endl;
    std::cout << "                                                                       " <<  std::endl;

    std::cout << "\n\n"<<std::setw(60)<<UNDER<<"Juego con WASD"<<RESET<<"\n\n";
}

void game(int lvl) {
    welMsg();

    while (level <= lvl) {
        Movement mv(lenY, lenX);  // Inicializar con el tamaño inicial del laberinto
        while (!mv.isGameOver()) {
            mv.doMove();
        }
        safeFile(mv.getMoves(), mv.getScore(), mv.getTotalSize());
        lenY += 5;
        lenX += 5;
    }
}

void menu(){
    char choice;
    bool exitMenu = false;
    std::string tmp;
    int lvl;

    while (!exitMenu) {
        system("cls");
        welMsg();

        std::cout << std::setw(70) <<"======================" << std::endl;
        std::cout << std::setw(70) <<"   MENU DE OPCIONES   " << std::endl;
        std::cout << std::setw(70) <<"======================" << std::endl;
        std::cout << std::setw(70) <<"1. Empezar Juego" << std::endl;
        std::cout << std::setw(70) <<"2. Record" << std::endl;
        std::cout << std::setw(70) <<"3. Creditos" << std::endl;
        std::cout << std::setw(70) <<"4. Salir" << std::endl;
        std::cout << std::setw(70) <<"======================" << std::endl;
        std::cout << std::setw(68) <<"Elija una opcion: ";
        std::cin >> choice;

        switch (choice) {
            case '1':
                std::cout <<"\n\n"<<std::setw(90)<<"Hasta que nivel quieres llegar? (que soporte tu computadora): ";
                std::cin >> tmp;
                lvl = comprobarNumeroEntero(tmp);
                system("cls");
                game(lvl);
                break;
            case '2':
                system("cls");
                readFile();
                std::cout << "\n\n"<<std::setw(90)<<"Configuración y opciones" << std::endl;
                break;
            case '3':
                system("cls");
                std::cout << "\n\n"<<std::setw(70)<< "Desarrollado por: ????" << std::endl;
                std::cout << "\n\n"<<std::setw(70)<< "Version 1.0" << std::endl;
                system("pause");
                break;
            case '4':
                exitMenu = true;
                system("cls");
                std::cout << "\n\n"<<std::setw(70)<< "Saliendo del juego..." << std::endl;
                system("pause");
                break;
            default:
                system("cls");
                std::cout << "Opcion no valida" << std::endl;
                break;
        }
    }
}

int main() {
    menu();
    return 0;
}
