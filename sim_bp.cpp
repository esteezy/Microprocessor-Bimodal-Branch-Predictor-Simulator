//Evan Mason
//Branch Predictors Simulator
//10-15-2020


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim_bp.h"

/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim bimodal 6 gcc_trace.txt
    argc = 4
    argv[0] = "sim"
    argv[1] = "bimodal"
    argv[2] = "6"
    ... and so on
*/
int main (int argc, char* argv[])
{
    FILE *FP;               // File handler
    char *trace_file;       // Variable that holds trace file name;
    bp_params params;       // look at sim_bp.h header file for the the definition of struct bp_params
    char outcome;           // Variable holds branch outcome
    unsigned long int addr; // Variable holds the address read from input file
    bool isBimodal = false;         // Variable tells which instantiation to use
    
    if (!(argc == 4 || argc == 5 || argc == 7))
    {
        printf("Error: Wrong number of inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }
    
    params.bp_name  = argv[1];
    
    // strtoul() converts char* to unsigned long. It is included in <stdlib.h>
    if(strcmp(params.bp_name, "bimodal") == 0)              // Bimodal
    {
        if(argc != 4)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.M2       = strtoul(argv[2], NULL, 10);
        params.M1       = 1;
        params.N        = 1;
        trace_file      = argv[3];
        printf("COMMAND\n%s %s %lu %s\n", argv[0], params.bp_name, params.M2, trace_file);

        isBimodal = true;

    }
    else if(strcmp(params.bp_name, "gshare") == 0)          // Gshare
    {
        if(argc != 5)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.M1       = strtoul(argv[2], NULL, 10);
        params.N        = strtoul(argv[3], NULL, 10);
        params.M2       = 1;
        trace_file      = argv[4];
        printf("COMMAND\n%s %s %lu %lu %s\n", argv[0], params.bp_name, params.M1, params.N, trace_file);

    }
    else if(strcmp(params.bp_name, "hybrid") == 0)          // Hybrid
    {
        if(argc != 7)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.K        = strtoul(argv[2], NULL, 10);
        params.M1       = strtoul(argv[3], NULL, 10);
        params.N        = strtoul(argv[4], NULL, 10);
        params.M2       = strtoul(argv[5], NULL, 10);
        trace_file      = argv[6];
        printf("COMMAND\n%s %s %lu %lu %lu %lu %s\n", argv[0], params.bp_name, params.K, params.M1, params.N, params.M2, trace_file);

    }
    else
    {
        printf("Error: Wrong branch predictor name:%s\n", params.bp_name);
        exit(EXIT_FAILURE);
    }
    
    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }

    unsigned int m2 = params.M2;
    unsigned int m1 = params.M1;
    unsigned int n = params.N;
    //instantiate bimodal predictor object
    BimodalPredictor bimodal(m2);
    //instantiate gshare predictor object
    GSharePredictor gshare(m1, n);
    char str[2];
    while(fscanf(FP, "%lx %s", &addr, str) != EOF)
    {
        
        outcome = str[0];
        if(isBimodal){
            bimodal.instructionRequest(addr, outcome);
        }
        else{
            gshare.instructionRequest(addr, outcome);
        }

    }

    //Contents and Measurements
    printf("OUTPUT\n");
    if(strcmp(params.bp_name, "bimodal") == 0){
        printf(" number of predictions:     %.0f\n", bimodal.numPred());
        printf(" number of mispredictions:  %.0f\n", bimodal.numMiss());
        printf(" misprediction rate:        %2.2f%c\n", bimodal.rate(), '%');
        printf("FINAL BIMODAL CONTENTS\n");
        bimodal.printContents();
    }
    else if(strcmp(params.bp_name, "gshare") == 0){
        printf(" number of predictions:     %.0f\n", gshare.numPred());
        printf(" number of mispredictions:  %.0f\n", gshare.numMiss());
        printf(" misprediction rate:        %2.2f%c\n", gshare.rate(), '%');
        printf("FINAL GSHARE CONTENTS\n");
        gshare.printContents();
    }



    return 0;
}
