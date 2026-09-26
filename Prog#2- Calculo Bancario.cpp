#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;

// ===================== UTILIDADES =====================
void color(int c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void limpiar() { system("cls"); }
void centrar(string texto) {
    int ancho = 80;
    int espacios = (ancho - (int)texto.length()) / 2;
    if (espacios < 0) espacios = 0;
    for (int i = 0; i < espacios; i++) cout << " ";
    cout << texto << endl;
}
void linea() {
    color(8);
    centrar("================================================================");
    color(7);
}
void pausa() {
    color(8);
    centrar("Presione una tecla para continuar...");
    color(7);
    system("pause > nul");
}
string obtenerFechaHora() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", ltm);
    return string(buffer);
}

// ===================== ESTRUCTURA DEL PRESTAMO =====================
struct Prestamo {
    string cliente;
    string cedula;
    double monto;
    double tasaAnual;
    int meses;
    double seguroMensual;
    double comisionApertura;
    double itbisPorc;
    double cuotaMensual;
    double totalIntereses;
    double totalSeguro;
    double totalPagar;
    string fecha;
};

// ===================== CALCULO PRINCIPAL =====================
double calcularCuota(double P, double tasaAnual, int n) {
    if (tasaAnual == 0) return P / n;
    double r = (tasaAnual / 100.0) / 12.0;          // tasa mensual
    double factor = pow(1.0 + r, n);
    return P * (r * factor) / (factor - 1.0);
}

void calcularTotales(Prestamo &p) {
    p.cuotaMensual = calcularCuota(p.monto, p.tasaAnual, p.meses);

    // Intereses totales
    p.totalIntereses = (p.cuotaMensual * p.meses) - p.monto;

    // Seguro total
    p.totalSeguro = p.seguroMensual * p.meses;

    // ITBIS (en RD los servicios financieros están EXENTOS)
    double itbis = p.totalIntereses * (p.itbisPorc / 100.0);

    p.totalPagar = (p.cuotaMensual * p.meses) + p.totalSeguro + itbis;
}

// ===================== MOSTRAR RESULTADO =====================
void mostrarResultado(const Prestamo &p) {
    limpiar();
    color(14);
    centrar("==============================================");
    centrar("     RESULTADO DE LA SIMULACION");
    centrar("==============================================");
    color(7);
    cout << endl;

    cout << "  Cliente          : " << p.cliente << endl;
    cout << "  Cedula           : " << p.cedula << endl;
    cout << "  Fecha            : " << p.fecha << endl;
    cout << endl;
    linea();
    cout << fixed << setprecision(2);
    cout << "  Monto del prestamo     : RD$ " << p.monto << endl;
    cout << "  Tasa de interes anual  : " << p.tasaAnual << " %" << endl;
    cout << "  Plazo                  : " << p.meses << " meses" << endl;
    cout << "  Comision de apertura   : " << p.comisionApertura << " %" << endl;
    cout << "  Seguro mensual         : RD$ " << p.seguroMensual << endl;
    cout << "  ITBIS sobre intereses  : " << p.itbisPorc << " %  (Exento en servicios financieros)" << endl;
    linea();
    color(11);
    cout << "  CUOTA MENSUAL (Capital + Interes): RD$ " << p.cuotaMensual << endl;
    color(7);
    cout << "  + Seguro mensual                 : RD$ " << p.seguroMensual << endl;
    color(14);
    cout << "  CUOTA TOTAL A PAGAR            : RD$ " << (p.cuotaMensual + p.seguroMensual) << endl;
    color(7);
    linea();
    cout << "  Total de intereses               : RD$ " << p.totalIntereses << endl;
    cout << "  Total de seguro                  : RD$ " << p.totalSeguro << endl;
    cout << "  Total a pagar al final           : RD$ " << p.totalPagar << endl;
    linea();
    color(10);
    centrar("Simulacion calculada correctamente");
    color(7);
}

// ===================== FACTURA =====================
void imprimirFactura(const Prestamo &p) {
    limpiar();
    color(14);
    centrar("======================================================");
    centrar("           FACTURA / SIMULACION DE PRESTAMO");
    centrar("           BANCO SIMULADOR RD - EDUCATIVO");
    centrar("======================================================");
    color(7);
    cout << endl;
    cout << "  Fecha de emision : " << p.fecha << endl;
    cout << "  Cliente          : " << p.cliente << endl;
    cout << "  Cedula / RNC     : " << p.cedula << endl;
    cout << endl;
    linea();
    cout << fixed << setprecision(2);
    cout << "  DETALLE DEL PRESTAMO" << endl;
    cout << "  ----------------------------------------------------" << endl;
    cout << "  Monto solicitado           : RD$ " << setw(12) << p.monto << endl;
    cout << "  Tasa de interes anual      : " << p.tasaAnual << " %" << endl;
    cout << "  Plazo                      : " << p.meses << " meses" << endl;
    cout << "  Comision de apertura       : " << p.comisionApertura << " %" << endl;
    cout << "  Seguro de vida mensual     : RD$ " << p.seguroMensual << endl;
    cout << endl;
    cout << "  RESUMEN DE PAGOS" << endl;
    cout << "  ----------------------------------------------------" << endl;
    color(11);
    cout << "  Cuota mensual (Cap+Int)    : RD$ " << setw(12) << p.cuotaMensual << endl;
    color(7);
    cout << "  + Seguro mensual           : RD$ " << setw(12) << p.seguroMensual << endl;
    color(14);
    cout << "  CUOTA TOTAL MENSUAL      : RD$ " << setw(12) << (p.cuotaMensual + p.seguroMensual) << endl;
    color(7);
    cout << endl;
    cout << "  Total intereses            : RD$ " << setw(12) << p.totalIntereses << endl;
    cout << "  Total seguro               : RD$ " << setw(12) << p.totalSeguro << endl;
    cout << "  TOTAL A PAGAR AL FINAL    : RD$ " << setw(12) << p.totalPagar << endl;
    linea();
    color(8);
    cout << "  NOTA: Los servicios financieros estan EXENTOS de ITBIS" << endl;
    cout << "  segun la DGII (Ley 11-92). Esta es una simulacion educativa." << endl;
    color(7);
    linea();
    centrar("Gracias por usar el Simulador Bancario RD");
    cout << endl;
}

void exportarFactura(const Prestamo &p) {
    string nombre = "Factura_" + p.cliente + ".txt";
    for (char &c : nombre) if (c == ' ') c = '_';

    ofstream f(nombre);
    if (!f.is_open()) {
        color(12); centrar("Error al crear el archivo de factura."); color(7);
        pausa(); return;
    }

    f << "======================================================\n";
    f << "           FACTURA / SIMULACION DE PRESTAMO\n";
    f << "           BANCO SIMULADOR RD - EDUCATIVO\n";
    f << "======================================================\n\n";
    f << "Fecha de emision : " << p.fecha << "\n";
    f << "Cliente          : " << p.cliente << "\n";
    f << "Cedula / RNC     : " << p.cedula << "\n\n";
    f << fixed << setprecision(2);
    f << "DETALLE DEL PRESTAMO\n";
    f << "----------------------------------------------------\n";
    f << "Monto solicitado           : RD$ " << p.monto << "\n";
    f << "Tasa de interes anual      : " << p.tasaAnual << " %\n";
    f << "Plazo                      : " << p.meses << " meses\n";
    f << "Comision de apertura       : " << p.comisionApertura << " %\n";
    f << "Seguro de vida mensual     : RD$ " << p.seguroMensual << "\n\n";
    f << "RESUMEN DE PAGOS\n";
    f << "----------------------------------------------------\n";
    f << "Cuota mensual (Cap+Int)    : RD$ " << p.cuotaMensual << "\n";
    f << "+ Seguro mensual           : RD$ " << p.seguroMensual << "\n";
    f << "CUOTA TOTAL MENSUAL      : RD$ " << (p.cuotaMensual + p.seguroMensual) << "\n\n";
    f << "Total intereses            : RD$ " << p.totalIntereses << "\n";
    f << "Total seguro               : RD$ " << p.totalSeguro << "\n";
    f << "TOTAL A PAGAR AL FINAL    : RD$ " << p.totalPagar << "\n\n";
    f << "NOTA: Los servicios financieros estan EXENTOS de ITBIS\n";
    f << "segun la DGII (Ley 11-92). Esta es una simulacion educativa.\n";
    f << "======================================================\n";
    f.close();

    color(10);
    centrar("Factura exportada como: " + nombre);
    color(7);
    pausa();
}

// ===================== NUEVA SIMULACION =====================
void nuevaSimulacion() {
    Prestamo p;
    p.fecha = obtenerFechaHora();

    limpiar();
    color(14);
    centrar("==============================================");
    centrar("     NUEVA SIMULACION DE PRESTAMO");
    centrar("==============================================");
    color(7);
    cout << endl;

    cin.ignore();
    cout << "  Nombre del cliente     : ";
    getline(cin, p.cliente);
    cout << "  Cedula / RNC           : ";
    getline(cin, p.cedula);

    cout << "  Monto del prestamo (RD$): ";
    cin >> p.monto;
    while (cin.fail() || p.monto <= 0) {
        cin.clear(); cin.ignore(1000, '\n');
        color(12); cout << "  Monto invalido. Intente de nuevo: "; color(7);
        cin >> p.monto;
    }

    cout << "  Tasa de interes anual (%): ";
    cin >> p.tasaAnual;
    while (cin.fail() || p.tasaAnual < 0 || p.tasaAnual > 50) {
        cin.clear(); cin.ignore(1000, '\n');
        color(12); cout << "  Tasa invalida (0-50). Intente: "; color(7);
        cin >> p.tasaAnual;
    }

    cout << "  Plazo en meses (6-84)  : ";
    cin >> p.meses;
    while (cin.fail() || p.meses < 6 || p.meses > 84) {
        cin.clear(); cin.ignore(1000, '\n');
        color(12); cout << "  Plazo invalido. Intente: "; color(7);
        cin >> p.meses;
    }

    cout << "  Seguro mensual (RD$)   : ";
    cin >> p.seguroMensual;
    while (cin.fail() || p.seguroMensual < 0) {
        cin.clear(); cin.ignore(1000, '\n');
        color(12); cout << "  Valor invalido. Intente: "; color(7);
        cin >> p.seguroMensual;
    }

    cout << "  Comision de apertura (%): ";
    cin >> p.comisionApertura;
    while (cin.fail() || p.comisionApertura < 0) {
        cin.clear(); cin.ignore(1000, '\n');
        color(12); cout << "  Valor invalido. Intente: "; color(7);
        cin >> p.comisionApertura;
    }

    cout << "  ITBIS sobre intereses (%): ";
    cin >> p.itbisPorc;
    while (cin.fail() || p.itbisPorc < 0) {
        cin.clear(); cin.ignore(1000, '\n');
        color(12); cout << "  Valor invalido. Intente: "; color(7);
        cin >> p.itbisPorc;
    }

    // Calcular
    calcularTotales(p);

    // Mostrar resultado
    mostrarResultado(p);
    pausa();

    // Opciones después del cálculo
    int op;
    do {
        limpiar();
        color(14);
        centrar("¿Que desea hacer con esta simulacion?");
        color(7);
        cout << endl;
        centrar("1. Ver factura en pantalla");
        centrar("2. Exportar factura a archivo");
        centrar("0. Volver al menu principal");
        cout << endl;
        cout << "  Opcion: ";
        cin >> op;

        switch (op) {
            case 1: imprimirFactura(p); pausa(); break;
            case 2: exportarFactura(p); break;
            case 0: break;
            default:
                color(12); centrar("Opcion invalida"); color(7);
                pausa();
        }
    } while (op != 0);
}

// ===================== MANUAL / 7 PASOS =====================
void mostrarManual() {
    limpiar();
    color(14);
    centrar("==============================================");
    centrar("     MANUAL Y 7 PASOS DEL PROYECTO");
    centrar("==============================================");
    color(7);
    cout << endl;

    color(11); cout << "  LOS 7 PASOS DEL DESARROLLO DE SOFTWARE\n"; color(7);
    cout << "  1. Planificacion y analisis de requisitos\n";
    cout << "  2. Definicion de requisitos\n";
    cout << "  3. Diseno del sistema\n";
    cout << "  4. Desarrollo (codificacion en C++)\n";
    cout << "  5. Pruebas\n";
    cout << "  6. Implementacion / Despliegue\n";
    cout << "  7. Mantenimiento\n\n";

    color(11); cout << "  FORMULA DE LA CUOTA FIJA (Sistema Frances)\n"; color(7);
    cout << "  Cuota = P * (r * (1+r)^n) / ((1+r)^n - 1)\n";
    cout << "  donde:\n";
    cout << "    P = Monto del prestamo\n";
    cout << "    r = Tasa de interes mensual (anual/12)\n";
    cout << "    n = Numero de meses\n\n";

    color(11); cout << "  REALIDAD BANCARIA RD\n"; color(7);
    cout << "  - Servicios financieros EXENTOS de ITBIS (DGII)\n";
    cout << "  - Seguro de vida/desgravamen casi siempre obligatorio\n";
    cout << "  - Tasas personales actuales: aprox. 14% - 22% anual\n";
    cout << "  - Plazos comunes: 12 a 72 meses\n\n";

    color(11); cout << "  SOBRE LA VERSION ONLINE\n"; color(7);
    cout << "  Este programa esta preparado para ser subido a un servidor.\n";
    cout << "  La version de consola es la base. Luego se puede adaptar.\n";
    cout << endl;
    pausa();
}

// ===================== MENU PRINCIPAL =====================
int main() {
    // Barra de carga
    centrar("Cargando Simulador Bancario RD...");
    for (int i = 0; i <= 25; i++) {
        cout << "\r[";
        for (int j = 0; j < i; j++) cout << char(219);
        for (int j = i; j < 25; j++) cout << char(176);
        cout << "] " << (i * 4) << "%";
        cout.flush();
        Sleep(40);
    }
    cout << endl;
    Sleep(600);

    int opcion;
    do {
        limpiar();
        color(14);
        centrar("======================================================");
        centrar("       SIMULADOR BANCARIO - REPUBLICA DOMINICANA");
        centrar("       Cuota Fija + Interes + Seguro + Factura");
        centrar("======================================================");
        color(7);
        cout << endl;
        centrar("1. Nueva simulacion de prestamo");
        centrar("2. Manual y 7 pasos del proyecto");
        centrar("0. Salir");
        cout << endl;
        linea();
        cout << "  Elija una opcion: ";
        cin >> opcion;

        if (cin.fail()) {
            cin.clear(); cin.ignore(1000, '\n');
            opcion = -1;
        }

        switch (opcion) {
            case 1: nuevaSimulacion(); break;
            case 2: mostrarManual(); break;
            case 0:
                color(14);
                centrar("Gracias por usar el Simulador Bancario RD");
                color(7);
                Sleep(1000);
                break;
            default:
                color(12); centrar("Opcion invalida"); color(7);
                pausa();
        }
    } while (opcion != 0);

    return 0;
}
