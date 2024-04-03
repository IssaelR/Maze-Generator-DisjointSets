//
// Created by RYZEN on 19/11/2023.
//

#ifndef MAZE_1_0_COMPROBADORES_H
#define MAZE_1_0_COMPROBADORES_H

#endif //MAZE_1_0_COMPROBADORES_H

#include <iostream>
#include <regex>

using namespace std;

template <typename T>
T validarNumeroFlotante(string str);
bool esNumero(string str); // valida que sea digito
bool esPrimo(int numero);  // pide se ingrese el numero en un string temporal
bool validadorEntero(const string &str);
bool validadorFloat(const string &str);
int comprobarNumeroEntero(string str); // valida que sea entero mediante un regex y pide el reingreso en caso de no serlo, devuelvo el numero en un int
bool validadorFloat(const string &str);

bool esNumero(string str)
{
    for (char c : str)
    {
        if (!isdigit(c))
        {
            return false; // Retorna false si encuentra un carácter que no es un dígito
        }
    }
    return true; // Retorna true si todos los caracteres son dígitos
}

bool esPrimo(int numero)
{
    int cont = 0;

    for (int i = 1; i <= numero; i++)
    { // en un for recorre todos los numeros desde el 1 al ingresado
        if (numero % i == 0)
        {           // si el numero ingresado y el numero recorrido en el for son divisibles entonces sera igual a 0 y por lo tanto el contado aumentara
            cont++; // contador aumenta y luego vuelve a verificar el siguiente numero
        }
    }

    return cont == 2; // Si solo tiene 2 divisores (1 y sí mismo), es primo  y retorna true de lo contrario false
}

bool validadorEntero(const string &str)
{
    // Aqui utilizo una expresión regular que se descompone en lo siguiente
    // ^: Representa el inicio de la línea o cadena.
    // -? : Indica que el signo "-" (menos)es opcional.Esto permite que el número sea negativo, pero no es necesario.
    // [0 - 9] + : Representa uno o más dígitos numéricos del 0 al 9.
    // $ : Indica el final de la línea o cadena.
    regex integerRegex("^-?[0-9]+$");
    if (regex_match(str, integerRegex))
    {
        // en caso el parametro coincida con el regex la función devuelve verdadero
        return true;
    }
    else
    {
        // en caso contrario devuelve falso
        return false;
    }
}

bool validadorFloat(const string &str)
{
    // Aqui utilizo una expresión regular que se descompone en lo siguiente
    // ^: Representa el inicio de la línea o cadena.
    // -? : Indica que el signo "-" (menos)es opcional.Esto permite que el número sea negativo, pero no es necesario.
    // [0 - 9] + : Representa uno o más dígitos numéricos del 0 al 9.
    // $ : Indica el final de la línea o cadena.
    regex integerRegex("^-?[0-9]+(\.[0-9]+)?$");
    if (regex_match(str, integerRegex))
    {
        // en caso el parametro coincida con el regex la función devuelve verdadero
        return true;
    }
    else
    {
        // en caso contrario devuelve falso
        return false;
    }
}

int comprobarNumeroEntero(string str)
{

    while (!validadorEntero(str))
    {
        cout << "\033[1;91mDato incorrecto, debe ingresar un entero: \033[0m";
        cin >> str;
    }
    return stoi(str);
}

template <typename T>
T validarNumeroFlotante(string str)
{
    while (!validadorFloat(str))
    {
        cout << "\033[1;91mDato incorrecto, debe ingresar un flotante: \033[0m";
        cin >> str;
    }

    T result;
    istringstream iss(str);
    iss >> result;

    return result;
}
