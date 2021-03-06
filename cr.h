/***************************************************************************
*   Copyright (C) 2006 by martin lukac   				  *
*   lukacm@ece.pdx.edu   						  *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/
//Uncomment to allow multi-threaded computation for smaller circuits 8 < qubits
//#define __SSIZE__
//Uncoment to allow QMDD representation for circuits up to 7 qubits
//#define __QMDD__
//Uncoment to unleash the standard things
//#define __STDR__
//#define __TIMING__

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cmath>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <complex>
#include <sstream>

//#include <cutil.h>
//#include <cutil_inline.h>
//#include "cublas.h"
//#include "cuda.h"

//#include <gsl_cblas.h>
//#include <gsl_complex_math.h>
//#include <gsl_permutation.h>
//#include <gsl_blas.h>
//#include <gsl_linalg.h>

using namespace std;

// Thread block size
#define MAXGATEINPUT 20
#define BLOCK_SIZE 2

// max number of of inputs
const int MAXVARS = 100;
// max number of generated base functions
const int MAXGEN = 1000000;	

//int base_functions;
// input file stream
static ifstream in_stream;	
// output file stream
static ofstream out_stream;	


void read_pla(string);
int read_pla_file(std::ifstream&, int, int*, int**, int**);	
int read_real_file(std::ifstream&, int, int*, int**, int**);
void order_level_tree(int, int, int, int, int, int**, int, int**);
void sift_pla(int, int, int*, int**, int**, int**, int*);
void sift_real(int, int, int*, int**, int**, int**, int*, bool);

int main(int, char**);

