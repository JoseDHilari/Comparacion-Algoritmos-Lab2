#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <time.h>
#include "resource.h"

HINSTANCE hInst;
using namespace std;
int N;
int M[16][1000];
int Tiempo;
long long iteracionesBurbuja = 0;
long long iteracionesSeleccion = 0;
long long iteracionesInsercion = 0;
double tiempoBurbuja = 0;
double tiempoSeleccion = 0;
double tiempoInsercion = 0;

void Inicia(int *A, int NN, int T) {
    N = NN;
    Tiempo = T;
    for (int i = 0; i < N; i++)
        M[0][i] = M[1][i] = M[2][i] = M[3][i] = M[4][i] = M[5][i] = M[6][i] = M[7][i] = M[8][i] = A[i];
}

void Muestra(HDC hdc, int *A, float xi, float yi, float xf, float yf) {
    Rectangle(hdc, xi, yi, xf, yf);
    TextOut(hdc, 130, 8, "Burbuja", 7);
        TextOut(hdc, 430, 8, "Seleccion", 9);
    TextOut(hdc, 730, 8, "Insercion", 9);
    int i;
    int Mayor = A[0];
    for (i = 1; i < N; i++)
        if (A[i] > Mayor)
            Mayor = A[i];
    float ancho = (xf - xi) / N;
    float alto = (yf - yi) / Mayor;
    for (i = 0; i < N; i++)
        Rectangle(hdc, xi + i * ancho, yf - A[i] * alto, xi + (i + 1) * ancho - 1, yf);
}

class OrdenaBurbuja {
public:
    void operator()(HWND hwndDlg) {
        LARGE_INTEGER frequency, start, end;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&start);

        iteracionesBurbuja = 0;
        bool cambiado;
        for (int i = 0; i < N; i++) {
            cambiado = false;
            for (int j = 0; j < N - i - 1; j++) {
                iteracionesBurbuja++;
                if (M[0][j] > M[0][j + 1]) {
                    int tmp = M[0][j];
                    M[0][j] = M[0][j + 1];
                    M[0][j + 1] = tmp;
                    cambiado = true;
                    RedrawWindow(hwndDlg, NULL, NULL, RDW_INVALIDATE);
                    Sleep(Tiempo);
                }
            }
            if (!cambiado) break;
        }

        QueryPerformanceCounter(&end);
        tiempoBurbuja = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
        cout << "Burbuja \t Tiempo: " << tiempoBurbuja << " s,\t Iteraciones: " << iteracionesBurbuja << endl;
    }
};

class OrdenaSeleccion {
public:
    void operator()(HWND hwndDlg) {
        LARGE_INTEGER frequency, start, end;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&start);

        iteracionesSeleccion = 0;
        bool cambiado = false;
        for (int i = 0; i < N - 1; i++) {
            int Menor = i;
            for (int j = i + 1; j < N; j++) {
                iteracionesSeleccion++;
                if (M[2][j] < M[2][Menor]) {
                    Menor = j;
                    cambiado = true;
                    Sleep(Tiempo);
                }
            }
            if (Menor != i) {
                int tmp = M[2][i];
                M[2][i] = M[2][Menor];
                M[2][Menor] = tmp;
                RedrawWindow(hwndDlg, NULL, NULL, RDW_INVALIDATE);
                Sleep(Tiempo);
            }
            if (!cambiado) break;
        }

        QueryPerformanceCounter(&end);
        tiempoSeleccion = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
        cout << "Seleccion \t Tiempo: " << tiempoSeleccion << " s,\t Iteraciones: " << iteracionesSeleccion << endl;
    }
};

class OrdenaInsercion {
public:
    void operator()(HWND hwndDlg) {
        LARGE_INTEGER frequency, start, end;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&start);

        iteracionesInsercion = 0;

        for (int i = 1; i < N; i++) {
            int key = M[7][i];
            int j = i - 1;
            while (j >= 0 && M[7][j] > key) {
                iteracionesInsercion++;
                M[7][j + 1] = M[7][j];
                j--;
            }
            M[7][j + 1] = key;
            RedrawWindow(hwndDlg, NULL, NULL, RDW_INVALIDATE);
            Sleep(Tiempo);
        }

        QueryPerformanceCounter(&end);
        tiempoInsercion = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
        cout << "Insercion \t Tiempo: " << tiempoInsercion << " s, \t Iteraciones: " << iteracionesInsercion << endl;
    }
};



BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_INITDIALOG: {
        int N = 50, i, j, T, Salir;
        int AA[200];
        srand(time(NULL));

        for(i=0;i<N;i++)
            {   do
                {   Salir=1;
                    T=rand()% N+1;
                    for(j=0;j<i;j++)
                    {   if(AA[j]==T)
                        {   Salir=0;
                            break;
                        }
                    }
                    AA[i]=T;
                }  while(Salir==0);

            }
        Inicia(AA, N, 200);
    }
        return TRUE;
    case WM_CLOSE: {
        EndDialog(hwndDlg, 0);
    }
        return TRUE;
    case WM_PAINT: {
        HDC hdc;
        PAINTSTRUCT ps;
        hdc = BeginPaint(hwndDlg, &ps);
        Muestra(hdc, M[0], 10, 30, 300, 220);
        Muestra(hdc, M[2], 310, 30, 600, 220);
        Muestra(hdc, M[7], 610, 30, 900, 220);
        EndPaint(hwndDlg, &ps);
    }
        return TRUE;
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case B_PROBAR:
            thread((OrdenaBurbuja()), hwndDlg).detach();
            thread((OrdenaSeleccion()), hwndDlg).detach();
            thread((OrdenaInsercion()), hwndDlg).detach();
            return TRUE;
        }
    }
        return TRUE;
    }
    return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    hInst = hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
