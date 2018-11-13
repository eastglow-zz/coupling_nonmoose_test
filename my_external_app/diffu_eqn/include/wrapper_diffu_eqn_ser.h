#include <diffu_eqn_ser.h>
extern "C" {
  void wrapper_set_parameters(double *Diffu, double *dx, double *time_init, double Diffu_val, double dx_val, double time_val);
  double wrapper_calc_dtmax(double Diffu, double dx, double alpha);
  void wrapper_calc_x(double *x, int il, int iu, int ib,  double dx, double xl);
  void wrapper_data_set_value(double *arr, int il, int iu, double val);
  void wrapper_bc_Dirichlet(double *arr, int ilb, int iub, double valib, double valub);
  void wrapper_data_solve_diff_eqn(double *arrnew, double *arrold, int il, int iu, double Diffu, double dx, double dt);
  void wrapper_output_data_console(double *arr, int il, int iu, double *x, double time);
  void wrapper_data_copy(double *arrclone, double *arrref, int il, int iu);
  void wrapper_Diffu_module(double *data, double *xcoords, DiffuParams params);
}
