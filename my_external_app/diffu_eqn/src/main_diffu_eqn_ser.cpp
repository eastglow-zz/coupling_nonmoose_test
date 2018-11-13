#include <diffu_eqn_ser.h>

int main()
{
  DiffuParams params;
  params.time_start = 0.0;
  params.time_end = 1.0;
  params.x0 = 0.0;
  params.dx = 0.5;
  params.nghost = 1;
  params.iLower = 1;
  params.iUpper = 21;
  params.numdata = 2 * params.nghost + params.iUpper - params.iLower + 1;
  params.iboundaryL = params.iLower - params.nghost;
  params.iboundaryU = params.iUpper + params.nghost;
  params.DiffCoef = 1.0;
  params.bc_Lval_Dirichlet = 1.0;
  params.bc_Uval_Dirichlet = -1.0;

  double *data;
  data = new double[params.numdata];
  double *x;
  x = new double[params.numdata];

  calc_x(x, params.iLower, params.iUpper, params.nghost, params.dx, params.x0);
  data_set_value(data, params.iLower, params.iUpper, 0.0);

  output_data_console(data, params.iLower, params.iUpper, x, params.time_start);

  Diffu_module(data, x, params);

  output_data_console(data, params.iLower, params.iUpper, x, params.time_end);

  return 0;
}
