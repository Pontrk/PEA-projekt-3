#include "GeneticAlgorithm.h"
using namespace std;

GeneticAlgorithm::GeneticAlgorithm(Matrix graph, int time){
    matrix = graph.getMatrix();
    size = graph.getVerteces();
    searchTime = time;
}

void GeneticAlgorithm::startAlgorithmTime(int populationSize, int generationNumber, double mutationProbability, double crossProbability) {
    int tempPopulationSize = populationSize;
    int generations=0;
    random_device rg; // Inicjalizacja generatora liczb losowych.
    mt19937 randomGenerator(rg());  // Generator liczb losowych.
    generateParents(randomGenerator, populationSize);
    auto firstIterator = population.begin();
    auto secondIterator = population.begin();
    vector<unsigned int> firstChild(size-1, 0);
    vector<unsigned int> secondChild(size-1, 0);
    auto startTime = std::chrono::steady_clock::now();
    bool condition = true;
    while (condition) {
        //cout<<"generacja numer: "<<generations<<endl;
        for (int numberOfPopulation = 0; numberOfPopulation < tempPopulationSize; numberOfPopulation++) {
            //cout<<"iteracja w populacji nr: "<<numberOfPopulation<<endl;
            uniform_real_distribution<double> generateProbabilityOfCrossing(0.0, 1.0);
            double probabilityOfCrossing = generateProbabilityOfCrossing(randomGenerator);
            //cout<<"szansa: "<<probabilityOfCrossing;
            if(probabilityOfCrossing <= 1) { //crossProbability) {
                pair<int,int> parents = tournamentSelection(randomGenerator);
                firstIterator = population.begin() + parents.first;
                secondIterator = population.begin() + parents.second;
                firstChild.assign(size - 1, 0);
                secondChild.assign(size - 1, 0);
                orderCrossoverOperator(randomGenerator, firstIterator->second, secondIterator->second, firstChild, secondChild);
                mutation(randomGenerator, firstChild, mutationProbability, generations * tempPopulationSize + numberOfPopulation);
                mutation(randomGenerator, secondChild, mutationProbability, generations * tempPopulationSize + numberOfPopulation);
            } else tempPopulationSize--;
        }
        generations++;
        sort(population.begin(), population.end(), comparison);
        copyPopulation(tempPopulationSize);
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedTime = currentTime - startTime;
        double elapsedSeconds = elapsedTime.count();
        if (elapsedSeconds >= searchTime) {
            condition = false;
        }
    }
    showPath(globalPath);
    showNewBestPath(generations * tempPopulationSize);
}

void GeneticAlgorithm::generateParents(mt19937 &randomGenerator, int populationSize) {
    vector<unsigned int> parentPath;
    int costOfParent;
    for (int i = 0; i < populationSize; i++)
    {
        parentPath = shufflePath(randomGenerator);
        costOfParent = calculatePathCost(parentPath);
        pair <int, vector<unsigned int>> parentPair = {costOfParent, parentPath};
        cout<<"Koszt rodzica: " << costOfParent << " sciezka: ";
        for(int j = 0; j < parentPath.size(); j++)
            cout << parentPath[j] << " ";
        cout << endl;
        if(parentPair.first < finalCost){
            finalCost = parentPair.first;
            globalPath = parentPair.second;
            showNewBestPath(i);
        }
        population.push_back(parentPair);
    }
}

vector<unsigned int> GeneticAlgorithm::shufflePath(mt19937 &randomGenerator) {
    vector<unsigned int> temp;
    temp.reserve(size);
    for (int i = 0; i < size; i++) {
        temp.push_back(i); // Dodanie każdego wierzchołka do permutacji.
    }
    shuffle(temp.begin(), temp.end(), randomGenerator); // Mieszanie permutacji.
    return temp;
}

int GeneticAlgorithm::calculatePathCost(vector<unsigned int> path) {
    int cost = 0;
    auto iteration = path.begin();
    while(iteration != (path.end() - 1)){
        cost += matrix[iteration.operator*()][(iteration + 1).operator*()];
        iteration.operator++();
    }
    cost += matrix[path.back()][path.front()]; // Dodanie kosztu powrotu do punktu startowego.
    return cost;
}

void GeneticAlgorithm::showNewBestPath(int iteration) {
    cout<<iteration<<"  koszt:  "<<finalCost<<"  "<<endl;
}

void GeneticAlgorithm::printPopulation(){
    cout<<"sciezki: "<<endl;
    for(int x = 0; x < population.size(); x++)
    {
        cout<<"koszt: "<<population[x].first<<" ";
        for (int z = 0; z < population[x].second.size(); z++)
            cout<<population[x].second[z]<<" ";
        cout<<endl;
    }
}

pair<int, int> GeneticAlgorithm::tournamentSelection(mt19937 &randomGenerator) {
    uniform_int_distribution<> randomParents(0, static_cast<int>(population.size())-1);
    int firstParent, secondParent;
    firstParent = randomParents(randomGenerator);
    secondParent = randomParents(randomGenerator);
    pair<int,int> parents;
    if (population.at(firstParent).first < population.at(secondParent).first)
        parents.first = firstParent;
    else
        parents.first = secondParent;
    firstParent = randomParents(randomGenerator);
    secondParent = randomParents(randomGenerator);
    if (population.at(firstParent).first < population.at(secondParent).first)
        parents.second = firstParent;
    else
        parents.second = secondParent;
    return parents;
}

void GeneticAlgorithm::orderCrossoverOperator(std::mt19937 randomGenerator, const vector<unsigned int> &firstParent,
                                              const vector<unsigned int> &secondParent, vector<unsigned int> &firstChild,
                                              vector<unsigned int> &secondChild) {

    uniform_int_distribution<> cuts(1, static_cast<int>(firstParent.size()) - 3);
    int firstCut = 0, secondCut = 0;
    while(firstCut == secondCut) {
        firstCut = cuts(randomGenerator);
        secondCut = cuts(randomGenerator);
    }
    if(firstCut > secondCut)
        swap(firstCut, secondCut);
    for(int i = firstCut; i <= secondCut; i++) {
        firstChild.at(i) = firstParent.at(i);
        secondChild.at(i) = secondParent.at(i);
    }
    orderCrossoverOperatorCopy(secondParent, firstChild, firstCut, secondCut);
    orderCrossoverOperatorCopy(firstParent, secondChild, firstCut, secondCut);
}

void GeneticAlgorithm::orderCrossoverOperatorCopy(const vector<unsigned int> &parent, vector<unsigned int> &child, const int &firstCut,
                                                  const int &secondCut) {
    int start = secondCut + 1;
    auto iterator = parent.begin() + start;
    do {
        if(find(child.begin() + firstCut, child.begin() + secondCut + 1, *iterator) == child.begin() + secondCut + 1) {
            if(start<=child.size()-1){
                child.at(start) = *iterator;
                if(iterator != parent.end() - 1)
                    iterator++;
                 else iterator = parent.begin();
            }
        } else {
            if (iterator != parent.end() - 1)
                iterator++;
            else iterator = parent.begin();
            continue;
        }
        if (start < child.size() - 1)
            start++;
        else start = 0;
    } while (start != firstCut);
}
void GeneticAlgorithm::mutation(mt19937 &randomGenerator, vector<unsigned int> &child, double mutationProbability,
                                int iterator) {
    uniform_real_distribution<double> generateMutationProbability(0.0, 1.0);
    population.emplace_back(calculatePathCost(child), child);

    pair<int, vector<unsigned int>> pointerLast((population.end() - 1)->first, (population.end() - 1)->second);
    if(finalCost > pointerLast.first) {
        finalCost = pointerLast.first;
        globalPath = pointerLast.second;
        showNewBestPath(iterator);
    }
    double generateMutationCheck = generateMutationProbability(randomGenerator);
    if(generateMutationCheck <= mutationProbability)
        makeMutation(&pointerLast, randomGenerator);
    else return;
    population.push_back(pointerLast);
    if (finalCost > pointerLast.first) {
        finalCost = pointerLast.first;
        globalPath = pointerLast.second;
        showNewBestPath(iterator);
    }

}

void GeneticAlgorithm::makeMutation(pair<int, vector<unsigned int>> *path, std::mt19937 &randomGenerator) {
    int firstToSwap = 0, secondToSwap = 0, swapValue, bestSwap = INT_MAX;
    for(int i = 1; i < path->second.size() - 3; i++) {
        for(int j = 1; j < path->second.size() - 2; j++) {
            swapValue = costOfSwappedNeighbours(&path->second, i, j);
            if (swapValue < bestSwap) {
                firstToSwap = i;
                secondToSwap = j;
                bestSwap = swapValue;
            }
        }
    }
    swap(path->second.at(firstToSwap), path->second.at(secondToSwap));
    path->first += bestSwap;
}
int GeneticAlgorithm::costOfSwappedNeighbours(vector<unsigned int> *path, int firstToSwap, int secondToSwap) {
    int oldEdges = 0, newEdges = 0;
    if (secondToSwap - firstToSwap == 1) {
        oldEdges += matrix[path->at(firstToSwap - 1)][path->at(firstToSwap)];
        oldEdges += matrix[path->at(firstToSwap)][path->at(secondToSwap)];
        oldEdges += matrix[path->at(secondToSwap)][path->at(secondToSwap + 1)];

        newEdges += matrix[path->at(firstToSwap - 1)][path->at(secondToSwap)];
        newEdges += matrix[path->at(secondToSwap)][path->at(firstToSwap)];
        newEdges += matrix[path->at(firstToSwap)][path->at(secondToSwap + 1)];

    } else {
        oldEdges += matrix[path->at(firstToSwap - 1)][path->at(firstToSwap)];
        oldEdges += matrix[path->at(firstToSwap)][path->at(firstToSwap + 1)];
        oldEdges += matrix[path->at(secondToSwap - 1)][path->at(secondToSwap)];
        oldEdges += matrix[path->at(secondToSwap)][path->at(secondToSwap + 1)];

        newEdges += matrix[path->at(firstToSwap - 1)][path->at(secondToSwap)];
        newEdges += matrix[path->at(secondToSwap)][path->at(firstToSwap + 1)];
        newEdges += matrix[path->at(secondToSwap - 1)][path->at(firstToSwap)];
        newEdges += matrix[path->at(firstToSwap)][path->at(secondToSwap + 1)];
    }

    return newEdges - oldEdges;
}

void GeneticAlgorithm::copyPopulation(int number) {
    vector<pair<int, vector<unsigned int>>> tempPopulation;
    for (int i = 0; i < number; i++)
        tempPopulation.push_back(population.at(i));
    population = tempPopulation;
}
void GeneticAlgorithm::showPath(vector<unsigned int> path) {
    for (int i: path)
        cout<< i << "->";
    cout << path.front() << endl;
}