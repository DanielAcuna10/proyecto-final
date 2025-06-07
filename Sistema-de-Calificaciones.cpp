#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include <string>
#include <algorithm>
using namespace std;

const int NUM_NOTAS = 5; // ahora son 5 exámenes

// Aquí hice la estructura para guardar la info de cada alumno
struct Alumno {
    string nombre;
    string apellido1;
    string apellido2;
    int ciclo;              // Agregué el ciclo para saber en qué parte del proceso va
    int cedula;             // Validé que la cédula tenga exactamente 9 dígitos
    vector<float> notas;   // Aquí guardo las notas
    float promedio;        // Y este es el promedio de esas notas
};

// Verifico si la nota está entre 0 y 100
bool esNumeroValido(float n) {
    return n >= 0.0 && n <= 100.0;
}

// Función para leer un entero positivo desde consola
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

// Aquí me aseguro que la cédula tenga exactamente 9 dígitos
int leerCedula(string mensaje) {
    int valor;
    while (true) {
        valor = leerEnteroPositivo(mensaje);
        string cedulaStr = to_string(valor);
        if (cedulaStr.length() == 9) {
            return valor;
        } else {
            cout << "Error: La cédula debe tener exactamente 9 dígitos. Intente nuevamente.\n";
        }
    }
}

// Leer nota individual validando su rango
float leerNota(string mensaje) {
    float nota;
    while (true) {
        cout << mensaje;
        cin >> nota;
        if (cin.fail() || !esNumeroValido(nota)) {
            cout << "Error: La nota debe estar entre 0.0 y 100.0.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return nota;
        }
    }
}

// Esta función evita que dejen campos vacíos como nombre o apellidos
string leerTextoNoVacio(string mensaje) {
    string texto;
    while (true) {
        cout << mensaje;
        getline(cin, texto);
        if (!texto.empty()) {
            return texto;
        } else {
            cout << "Error: Este campo no puede estar vacío.\n";
        }
    }
}

// Función para leer texto no vacío y sin números (nueva función)
string leerTextoSinNumeros(string mensaje) {
    string texto;
    while (true) {
        cout << mensaje;
        getline(cin, texto);

        if (texto.empty()) {
            cout << "Error: Este campo no puede estar vacío.\n";
            continue;
        }

        bool tieneNumero = false;
        for (char c : texto) {
            if (isdigit(c)) {
                tieneNumero = true;
                break;
            }
        }
        if (tieneNumero) {
            cout << "Error: No se permiten números en este campo.\n";
            continue;
        }

        return texto;
    }
}

// Para evitar que se repitan cédulas entre los alumnos
bool cedulaRepetida(int cedula, const vector<Alumno>& aprobados, const vector<Alumno>& reprobados) {
    for (const auto& a : aprobados) {
        if (a.cedula == cedula) return true;
    }
    for (const auto& a : reprobados) {
        if (a.cedula == cedula) return true;
    }
    return false;
}

// Aquí se procesan los alumnos uno por uno con validaciones
void procesarAlumnos(int n, vector<Alumno>& aprobados, vector<Alumno>& reprobados) {
    for (int i = 0; i < n; ++i) {
        cout << "\n==================== Alumno #" << i + 1 << " ====================\n";

        Alumno alumno;

        cout << "--- Datos Personales ---\n";
        // Ahora uso la función que no permite números en el nombre/apellidos
        alumno.nombre = leerTextoSinNumeros("Ingrese primer nombre: ");
        alumno.apellido1 = leerTextoSinNumeros("Ingrese primer apellido: ");
        alumno.apellido2 = leerTextoSinNumeros("Ingrese segundo apellido: ");
        alumno.ciclo = leerEnteroPositivo("Ingrese el ciclo que cursa (ej: 1, 2): ");

        // Me aseguro que no haya cédulas repetidas y que tengan 9 dígitos
        do {
            alumno.cedula = leerCedula("Ingrese número de cédula (9 dígitos): ");
            if (cedulaRepetida(alumno.cedula, aprobados, reprobados)) {
                cout << " Error: Esta cédula ya ha sido registrada. Intente otra.\n";
            }
        } while (cedulaRepetida(alumno.cedula, aprobados, reprobados));

        cout << "--- Notas de Exámenes ---\n";
        alumno.notas.resize(NUM_NOTAS);
        float suma = 0;
        for (int j = 0; j < NUM_NOTAS; ++j) {
            alumno.notas[j] = leerNota("Nota del examen #" + to_string(j + 1) + ": ");
            suma += alumno.notas[j];
        }

        alumno.promedio = suma / NUM_NOTAS;

        // Si el promedio sale de rango por algún error, aviso
        if (alumno.promedio < 0.0 || alumno.promedio > 100.0) {
            cout << "Error: Promedio fuera de rango, verifique las notas.\n";
        }

        // Confirmación antes de guardar el alumno
        char confirmar;
        cout << "¿Desea guardar este alumno? (s/n): ";
        cin >> confirmar;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (tolower(confirmar) != 's') {
            cout << "Alumno descartado. Reiniciando entrada...\n";
            --i;
            continue;
        }

        // Según el promedio, lo meto en aprobados o reprobados
        if (alumno.promedio >= 70.0) {
            aprobados.push_back(alumno);
        } else {
            reprobados.push_back(alumno);
        }
    }
}

void imprimirEncabezadoSeccion(int i) {
    cout << "\n==================== Alumno #" << i + 1 << " ====================\n";
}

// Aquí imprimo los reportes de los que pasaron y no pasaron
void imprimirReporte(const vector<Alumno>& lista, string estado) {
    cout << "\n==================== " << estado << " ====================\n";
    cout << left << setw(25) << "Nombre completo"
         << setw(10) << "Ciclo"
         << setw(15) << "Cédula"
         << setw(10) << "Promedio" << "\n";
    cout << "------------------------------------------------------------------\n";

    for (const auto& alumno : lista) {
        cout << "> " << left << setw(25) << (alumno.nombre + " " + alumno.apellido1 + " " + alumno.apellido2)
             << setw(10) << alumno.ciclo
             << setw(15) << alumno.cedula
             << fixed << setprecision(2) << setw(10) << alumno.promedio << "\n";
    }
}

// Aquí inicia todo el programa principal
int main() {
    vector<Alumno> aprobados, reprobados;
    int n = leerEnteroPositivo("¿Cuántos alumnos desea evaluar? ");

    procesarAlumnos(n, aprobados, reprobados);

    imprimirReporte(aprobados, "APROBADOS");
    imprimirReporte(reprobados, "REPROBADOS");

    return 0;
} 
// Fin del programa

