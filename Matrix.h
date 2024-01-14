#ifndef PEA3NOWE_MATRIX_H
#define PEA3NOWE_MATRIX_H
#include <vector>
#include <string>
#include <fstream>

using namespace std;
class Matrix {
public:
    Matrix();
    void loadFromFile(const string& filename);
    void printMatrix();
    int getVerteces();
    vector<vector<int>> getMatrix();
private:
    vector<vector<int>> matrix;
    int liczbaWierzcholkow;
};


#endif //PEA3NOWE_MATRIX_H
