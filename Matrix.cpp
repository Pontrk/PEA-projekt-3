#include "Matrix.h"
#include <iostream>

using namespace std;

Matrix::Matrix(){

}
void Matrix::printMatrix()
{
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++)
            cout << matrix[i][j] << " ";
        cout << endl;
    }
}
void Matrix::loadFromFile(const string& filename){
    matrix.clear();
    string dump,opis="";
    ifstream plik(filename);
    int wagi;
    if (plik.is_open()) {
        getline(plik,dump);
        opis.append(dump+"\n");
        getline(plik,dump);
        opis.append(dump+"\n");
        getline(plik,dump);
        opis.append(dump+"\n");
        getline(plik,dump, ' ');
        opis.append(dump+"\n");
        plik >> liczbaWierzcholkow;
        getline(plik,dump);
        opis.append(dump+"\n");
        getline(plik,dump);
        opis.append(dump+"\n");
        getline(plik,dump);
        opis.append(dump+"\n");
        getline(plik,dump);
        opis.append(dump+"\n");
        // Plik został otwarty pomyślnie, możesz kontynuować operacje na pliku.
        vector<int> wiersz;
        for (int j = 0; j < liczbaWierzcholkow; j++){
            for (int i = 0; i < liczbaWierzcholkow; i++){
                plik >> wagi;
                if(i==j)
                    wagi = INT_MAX;
                wiersz.push_back(wagi);
            }
            matrix.push_back(wiersz);
            wiersz.clear();
        }
    } else {
        cout << "Nie udało się otworzyć pliku.";
    }
}
vector<vector<int>> Matrix::getMatrix(){
    return matrix;
}
int Matrix::getVerteces(){
    return liczbaWierzcholkow;
}