//Evan Mason
//Branch Predictors Simulator
//10-15-2020

#ifndef SIM_BP_H
#define SIM_BP_H

#include <cmath>

typedef struct bp_params{
    unsigned long int K;
    unsigned long int M1;
    unsigned long int M2;
    unsigned long int N;
    char*             bp_name;
}bp_params;

//bimodal predictor class
class BimodalPredictor{
protected:
    double numPredictions;
    double numMispredictions;
    int tableSize;
    int m2;
    int * predictionTable;

public:
    BimodalPredictor(unsigned long m){
        numMispredictions = 0;
        numPredictions = 0;
        m2 = m;
        tableSize = pow(2, m);
        predictionTable = new int [tableSize];
        for(int i = 0; i<tableSize; i++){
            //table counters initialized to "weakly taken"
            predictionTable[i] = 2;
        }
    }
    void instructionRequest(unsigned long addr, char outcome);
    double numPred(){return numPredictions;}
    double numMiss(){return numMispredictions;}
    double rate(){return (numMispredictions/numPredictions)*100;}
    void printContents(){
        for(int i = 0; i<tableSize; i++){
            printf(" %i\t%d\n", i, predictionTable[i]);
        }
    }
};

void BimodalPredictor::instructionRequest(unsigned long addr, char outcome) {
    numPredictions++;
    //lowest 2 bits are zero
    unsigned int newAddr = addr>>2;
    unsigned int mask = pow(2, m2) - 1;
    unsigned int tableIndex = newAddr & mask;
    char predicted;
    if(predictionTable[tableIndex] >= 2){
        predicted = 't';
    }
    else{
        predicted = 'n';
    }
    if(outcome != predicted){
        numMispredictions++;
    }
    if(outcome == 't'){
        if(predictionTable[tableIndex] < 3) {
            predictionTable[tableIndex]++;
        }
    }
    else{
        if(predictionTable[tableIndex] > 0) {
            predictionTable[tableIndex]--;
        }
    }
}

//gshare predictor class
class GSharePredictor{
protected:
    double numPredictions;
    double numMispredictions;
    int tableSize;
    int m1;
    int n;
    unsigned int globalBHR;
    int * predictionTable;

public:
    GSharePredictor(unsigned long m, unsigned long n_arg) : m1(1), n(1){
        numMispredictions = 0;
        numPredictions = 0;
        m1 = m;
        n = n_arg;
        globalBHR = 0;
        tableSize = pow(2, m1);
        predictionTable = new int [tableSize];
        for(int i = 0; i<tableSize; i++){
            //table counters initialized to "weakly taken"
            predictionTable[i] = 2;
        }
    }
    void instructionRequest(unsigned long addr, char outcome);
    double numPred(){return numPredictions;}
    double numMiss(){return numMispredictions;}
    double rate(){return (numMispredictions/numPredictions)*100;}
    void printContents(){
        for(int i = 0; i<tableSize; i++){
            printf(" %i\t%d\n", i, predictionTable[i]);
        }
    }
};

void GSharePredictor::instructionRequest(unsigned long addr, char outcome) {
    numPredictions++;
    //lowest 2 bits are zero
    unsigned int newAddr = addr>>2;
    unsigned int mask = pow(2, m1) - 1;
    unsigned int temp = newAddr & mask;
    unsigned int temp2 = temp >> (m1-n);
    unsigned int addr1 = temp2 ^ globalBHR;
    mask = pow(2, (m1-n))-1;
    temp = newAddr & mask;
    unsigned int tableIndex = (addr1<<(m1-n)) | temp;
    char predicted;
    if(predictionTable[tableIndex] >= 2){
        predicted = 't';
    }
    else{
        predicted = 'n';
    }
    if(outcome != predicted){
        numMispredictions++;
    }
    if(outcome == 't'){
        if(predictionTable[tableIndex] < 3) {
            predictionTable[tableIndex]++;
        }
    }
    else{
        if(predictionTable[tableIndex] > 0) {
            predictionTable[tableIndex]--;
        }
    }
    //update BHR
    globalBHR = globalBHR >> 1;
    if(outcome == 't') {
        globalBHR += pow(2, n-1);
    }
}
#endif
