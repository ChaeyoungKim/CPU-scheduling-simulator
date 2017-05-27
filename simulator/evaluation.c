#include <stdio.h>
#include "simulator.h"

void evaluation(double *WT1, double *TT1, double *WT2, double* TT2, double *WT3, double *TT3, double *WT4, double* TT4, double *WT5, double *TT5, double *WT6, double* TT6) {
	printf("\n**************************************\n\tEvaluation\n**************************************\n\n");

	printf("                      FCFS     SJF_NP   SJF_P    Pri_NP   Pri_P     RR\n");

	printf("avg_waiting_time :    %.2f    %.2f    %.2f    %.2f    %.2f    %.2f    \n", *WT1, *WT2, *WT3, *WT4, *WT5, *WT6);
	printf("avg_turnaround_time : %.2f    %.2f    %.2f    %.2f    %.2f    %.2f    \n", *TT1, *TT2, *TT3, *TT4, *TT5, *TT6);
	//printf("RR avg_turnaround_time : %g\n", *TT6);
}