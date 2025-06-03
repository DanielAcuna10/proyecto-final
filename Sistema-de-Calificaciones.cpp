#include <iostream>       
#include <vector>        
#include <iomanip>        
#include <limits>         
#include <string>         
using namespace std;     
//añadi las librerias que necesitamos


// Estructura Alumno
struct Alumno {
    string nombre;
    string apellido1;
    string apellido2;
    int ciclo;
    int cedula;
    vector<float> notas;
    float promedio;
};

// Validación de nota entre 0 y 100
bool esNumeroValido(float n) {
    return n >= 0.0 && n <= 100.0;
}

// Leer número entero positivo
int leerEnteroPositivo(string mensaje) {
    string linea;
    int valor;
    while (true) {
        cout << mensaje;
        getline(cin, linea);
        try {
            valor = stoi(linea);
            if (valor > 0) {
                return valor;
            } else {
                cout << "  Error: Ingrese un número entero positivo.\n";
            }
        } catch (...) {
            cout << "  Error: Ingrese un número entero positivo.\n";
        }
    }
}

// Leer una nota entre 0 y 100
float leerNota(string mensaje) {
    float nota;
    while (true) {
        cout << mensaje;
        cin >> nota;
        if (cin.fail() || !esNumeroValido(nota)) {
            cout << "⚠️  Error: La nota debe estar entre 0.0 y 100.0.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // <-- Agrega esta línea
            return nota;
        }
    }
}

// Leer texto con espacios
string leerTexto(string mensaje) {
    string texto;
    cout << mensaje;
    getline(cin, texto);
    return texto;
}
void procesarAlumnos(int n, vector<Alumno>& aprobados, vector<Alumno>& reprobados) {
    for (int i = 0; i < n; ++i) {
        cout << "\n==================== Alumno #" << i + 1 << " ====================\n";

        Alumno alumno;

        cout << "--- Datos Personales ---\n";
        alumno.nombre = leerTexto("Ingrese primer nombre: ");
        alumno.apellido1 = leerTexto("Ingrese primer apellido: ");
        alumno.apellido2 = leerTexto("Ingrese segundo apellido: ");
        alumno.cedula = leerEnteroPositivo("Ingrese número de cédula: ");

        cout << "--- Notas de Exámenes ---\n";
        alumno.notas.resize(3);
        float suma = 0;
        for (int j = 0; j < 3; ++j) {
            alumno.notas[j] = leerNota("Nota del examen #" + to_string(j + 1) + ": ");
            suma += alumno.notas[j];
        }

        alumno.promedio = suma / 3.0;

        if (alumno.promedio >= 70.0) {
            aprobados.push_back(alumno);
        } else {
            reprobados.push_back(alumno);
        }
    }
}
//hice procesamiento de lógica y cálculo de promedios