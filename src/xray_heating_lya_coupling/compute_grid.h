#ifndef COMPUTE_GRID_H
#define COMPUTE_GRID_H
#endif

void compute_Tb_grid(grid_21cm_t *this21cmGrid, cosmology_t *thisCosmology);
void do_step_21cm_emission(cosmology_t *thisCosmology, xray_grid_t *thisXray_grid, xray_spectrum_t *thisXray_spectrum, lya_grid_t * thisLya_grid, lya_spectrum_t *thisLya_spectrum, grid_21cm_t *this21cmGrid, k10_t *k10_table, double Xe);


