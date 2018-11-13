#include <wrapper_diffu_eqn_ser.h> 


void wrapper_set_parameters(double *Diffu, double *dx, double *time_init, double Diffu_val, double dx_val, double time_val)
{
  set_parameters(Diffu, dx, time_init, Diffu_val, dx_val, time_val);
}

double wrapper_calc_dtmax(double Diffu, double dx, double alpha)
{  
  return calc_dtmax(Diffu, dx, alpha);
}

void wrapper_calc_x(double *x, int il, int iu, int ib,  double dx, double xl) 
{
  calc_x(x, il, iu, ib, dx, xl);
}

void wrapper_data_set_value(double *arr, int il, int iu, double val)
{
  data_set_value(arr, il, iu, val);
}

void wrapper_bc_Dirichlet(double *arr, int ilb, int iub, double valib, double valub)
{
  bc_Dirichlet(arr, ilb, iub, valib, valub);
}

void wrapper_data_solve_diff_eqn(double *arrnew, double *arrold, int il, int iu, double Diffu, double dx, double dt)
{
  data_solve_diff_eqn(arrnew, arrold, il, iu, Diffu, dx, dt);
}

void wrapper_output_data_console(double *arr, int il, int iu, double *x, double time)
{
  output_data_console(arr, il, iu, x, time);
}
void wrapper_data_copy(double *arrclone, double *arrref, int il, int iu)
{
  data_copy(arrclone, arrref, il, iu);
}
void wrapper_Diffu_module(double *data, double *xcoords, DiffuParams params)
{
  Diffu_module(data, xcoords, params);
}
