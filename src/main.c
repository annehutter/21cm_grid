#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_math.h>    //included because M_PI is not defined in <math.h>
#include <assert.h>
#include <complex.h>
#include <time.h>
#include <string.h>

#ifdef __MPI
#include <fftw3-mpi.h>
#include <mpi.h>
#else
#include <fftw3.h>
#endif

#include "xray_heating_lya_coupling/phys_const.h"
#include "xray_heating_lya_coupling/chem_const.h"
#include "xray_heating_lya_coupling/cosmology.h"
#include "xray_heating_lya_coupling/collisional_coupling.h"
#include "xray_heating_lya_coupling/xray_heating.h"
#include "xray_heating_lya_coupling/wouthuysen_effect.h"
#include "xray_heating_lya_coupling/21cm_grid.h"
#include "xray_heating_lya_coupling/spin_temperature.h"
#include "xray_heating_lya_coupling/3cm_grid.h"
#include "xray_heating_lya_coupling/spin_temperature_He.h"
#include "xray_heating_lya_coupling/compute_grid.h"

#include "ion_and_temp_evolution/cell.h"
#include "ion_and_temp_evolution/recomb_rates.h"
#include "ion_and_temp_evolution/evolution_loop.h"
#include "ion_and_temp_evolution/solve_ionfraction.h"
#include "ion_and_temp_evolution/solve_temperature.h"
#include "ion_and_temp_evolution/evolution_loop.h"

#include "inputs.h"
#include "confObj.h"
#include "evolution.h"

int main (int argc, /*const*/ char * argv[]) { 
#ifdef __MPI
    int size = 1;
#endif
    int myRank = 0;
    
    char iniFile[1000];
    confObj_t simParam;
        
    double t1, t2;

#ifdef __MPI
    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank); 
    
    t1 = MPI_Wtime();
    
    fftw_mpi_init();
#else
    t1 = time(NULL);
#endif
    
    //parse command line arguments and be nice to user
    if (argc != 2) {
        printf("cifog: (C)  - Use at own risk...\n");
        printf("USAGE:\n");
        printf("cifog iniFile\n");
        
        exit(EXIT_FAILURE);
    } else {
        strcpy(iniFile, argv[1]);
    }
        
    //read paramter file
    simParam = readConfObj(iniFile);
    
    evolve(simParam, myRank);

    confObj_del(&simParam);
    
    if(myRank==0) printf("Finished\n");
#ifdef __MPI
    fftw_mpi_cleanup();
    
    t2 = MPI_Wtime();
    if(myRank == 0) printf("Execution took %f s\n", t2-t1);
    MPI_Finalize();
#else
    fftw_cleanup();
    
    t2 = time(NULL);
    printf("Execution took %f s\n", t2-t1);
#endif
    
    return 0;
}
