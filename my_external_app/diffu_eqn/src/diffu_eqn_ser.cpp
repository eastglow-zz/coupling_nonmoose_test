#include <diffu_eqn_ser.h>
#include <stdio.h>

void set_parameters(double *Diffu, double *dx, double *time_init, double Diffu_val, double dx_val, double time_val)
{
  *Diffu = Diffu_val;
  *dx = dx_val;
  *time_init = time_val;
}

double calc_dtmax(double Diffu, double dx, double alpha)
{
  return 0.5 * alpha * dx * dx / Diffu;
}

void calc_x(double *x, int il, int iu, int ib,  double dx, double xl)
{
  for (int i=il; i<=iu; i++){
    x[i] = xl + (i-il)*dx;
  }
}

void data_set_value(double *arr, int il, int iu, double val)
{
  for (int i=il; i<=iu; i++){
    arr[i] = val;
  }
}

void bc_Dirichlet(double *arr, int ilb, int iub, double valib, double valub)
{
  arr[ilb] = valib;
  arr[iub] = valub;
}

void data_solve_diff_eqn(double *arrnew, double *arrold, int il, int iu, double Diffu, double dx, double dt)
{
  for (int i=il; i<=iu; i++){
    arrnew[i] = arrold[i] + dt * Diffu * (arrold[i-1] - 2.0 * arrold[i] + arrold[i+1])/dx/dx;
  }
}

void output_data_console(double *arr, int il, int iu, double *x, double time)
{
  printf("\nTime = %16.5lf\n", time);
  printf("x, data\n");
  for (int i=il; i<=iu; i++){
    printf("%15.6lf\t%15.6lf\n", x[i], arr[i]);
  }
}

void data_copy(double *arrclone, double *arrref, int il, int iu)
{
  for (int i=il; i<=iu; i++) {
    arrclone[i] = arrref[i];
  }
}

void Diffu_module(double *data, double *xcoords, DiffuParams params)
{
  /*
  int ib = 1;
  int nd = 21;
  int nd_alloc = 2 * ib + nd;
  int il = 1;
  double time_start = 0.0;
  double time_end = 1.0;
  double xl = 0.0;
  */

  int ib = params.nghost;
  int nd_alloc = params.numdata;
  int il = params.iLower;
  int iu = params.iUpper;
  double time_start = params.time_start;
  double time_end = params.time_end;
  double xl = params.x0;

  int ilb = params.iboundaryL;
  int iub = params.iboundaryU;
  double *T_old;
  T_old = new double[nd_alloc];
  double *T_new;
  T_new = new double[nd_alloc];
  double *x;
  x = new double[nd_alloc];

  double Diffu;
  double dx;
  double time;
  double dt, dtmax; 

  set_parameters(&Diffu, &dx, &time, params.DiffCoef, params.dx, time_start);
  dtmax = calc_dtmax(Diffu, dx, 1.0);
  data_copy(x, xcoords, il, iu);
  data_copy(T_old, data, il, iu);
  data_copy(T_new, data, il, iu);

  dt = dtmax;
  double time_check;
  for (int i=0; time <= time_end; i++, (time_check=time+dt > time_end) && (time_end-time) > 1e-15 ? dt=time_end-time : dt = dtmax, time+=dt) {
    bc_Dirichlet(T_old, ilb, iub, params.bc_Lval_Dirichlet, params.bc_Uval_Dirichlet);
    data_solve_diff_eqn(T_new, T_old, il, iu, Diffu, dx, dt);
    data_copy(T_old,T_new,il,iu);
    //output_data_console(T_new, il, iu, x, time);
  }
  data_copy(data, T_new, il, iu);

}
