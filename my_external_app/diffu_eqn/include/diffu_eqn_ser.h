#include <stdio.h>

void set_parameters(double *Diffu, double *dx, double *time_init, double Diffu_val, double dx_val, double time_val);
double calc_dtmax(double Diffu, double dx, double alpha);
void calc_x(double *x, int il, int iu, int ib,  double dx, double xl); 
void data_set_value(double *arr, int il, int iu, double val);
void bc_Dirichlet(double *arr, int ilb, int iub, double valib, double valub);
void data_solve_diff_eqn(double *arrnew, double *arrold, int il, int iu, double Diffu, double dx, double dt);
void output_data_console(double *arr, int il, int iu, double *x, double time);
void data_copy(double *arrclone, double *arrref, int il, int iu);
typedef struct DiffuParams {
  double DiffCoef;
  double x0;
  double dx;
  double time_start;
  double time_end;
  int iLower;
  int iUpper;
  int nghost;
  int numdata;
  int iboundaryL;
  int iboundaryU;
  double bc_Lval_Dirichlet;
  double bc_Uval_Dirichlet;
}DiffuParams;
void Diffu_module(double *data, double *xcoords, DiffuParams params);
