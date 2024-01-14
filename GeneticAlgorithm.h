#ifndef PEA3NOWE_GENETICALGORITHM_H
#define PEA3NOWE_GENETICALGORITHM_H
#include "Matrix.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <ctime>

using namespace std;
class GeneticAlgorithm {
public:
    GeneticAlgorithm(Matrix graph, int time);
    void startAlgorithmTime(int populationSize, int generationNumber, double mutationProbability, double crossProbability);
    void startAlgorithm(int populationSize, int generationNumber, double mutationProbability, double crossProbability);
private:
    vector<vector<int>> matrix;
    vector<unsigned int> globalPath;
    vector <pair<int, vector<unsigned int>>> population;
    int size;
    int finalCost = INT_MAX;
    int searchTime;
    static bool comparison(pair<int, vector<unsigned int>> &first, pair<int, vector<unsigned int>> &second) {
        return first.first < second.first;
    }
    vector<unsigned int> shufflePath(mt19937 &randomGenerator);
    void generateParents(mt19937 &randomGenerator, int populationSize);
    void showNewBestPath(int iteration);
    pair<int,int> tournamentSelection(mt19937 &randomGenerator);
    void orderCrossoverOperator(mt19937 randomGenerator, const vector<unsigned int> &firstParent, const vector<unsigned int> &secondParent, vector<unsigned int> &firstChild, vector<unsigned int> &secondChild);
    void orderCrossoverOperatorCopy(const vector<unsigned int> &parent, vector<unsigned int> &child, const int &firstCut, const int &secondCut);
    void mutation(mt19937 &randomGenerator, vector<unsigned int> &child, double mutationProbability, int iterator);
    int calculatePathCost(vector<unsigned int> path);
    void makeMutation(pair<int, vector<unsigned int>> *path, mt19937 &randomGenerator);
    int costOfSwappedNeighbours(vector<unsigned int> *path, int firstToSwap, int secondToSwap);
    void copyPopulation(int number);
    void showPath(vector<unsigned int> path);
    void printPopulation();
};


#endif //PEA3NOWE_GENETICALGORITHM_H
