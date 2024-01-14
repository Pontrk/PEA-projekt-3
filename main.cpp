#include <iostream>
#include "Matrix.h"
#include "GeneticAlgorithm.h"
using namespace std;

int main(){
    Matrix graf;
    int  populationSize=100, generationNumber=3000;
    int menu = 0, time = 60;
    string fileName;
    double mutationProbability=0.01, crossProbability=0.8;
    do {
        cout << "Wybierz opcje:" << endl;
        cout << "1. Wczytaj dane z pliku" << endl;
        cout << "2. Ustawienie kryterium stopu" << endl;
        cout << "3. Ustawienie wielkosci populacji poczatkowej (domyslnie 100)" << endl;
        cout << "4. Ustawienie wspolczynnika mutacji (domyslnie 0.01)" << endl;
        cout << "5. Ustawienie wspolczynnika krzyzowania (domyslnie 0.8)" << endl;
        cout << "6. Uruchom algorytm genetyczny" << endl;
        cout << "0. Zakoncz program"<<endl;
        cin >> menu;
        switch (menu) {
            case 1:
                cout << "Podaj nazwe pliku: ";
                cin >> fileName;
                cout << endl;
                graf.loadFromFile(fileName);
                break;
            case 2:
                cout << "podaj kryterium stopu (w sekundach): ";
                cin >> time;
                cout << endl;
                break;
            case 3:
                cout << "Podaj wielkosc populacji poczatkowej: ";
                cin >> populationSize;
                cout << endl;
                break;
            case 4:
                cout << "Podaj wspolczynnik mutacji (Domyslnie 0.01): ";
                cin >> mutationProbability;
                cout << endl;
                break;
            case 5:
                cout << "Podaj wspolczynnik krzyzowania (Domyslnie 0.9): ";
                cin >> crossProbability;
                cout << endl;
                break;
            case 6:
                GeneticAlgorithm geneticAlgorithm(graf, time);
                geneticAlgorithm.startAlgorithmTime(populationSize, generationNumber, mutationProbability, crossProbability);
                break;
        }
    } while (menu != 0);
    return 0;
}