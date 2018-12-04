//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "MyDiffusionUserObject.h"
#include <dlfcn.h>
//MOOSE includes

registerMooseObject("MooseApp", MyDiffusionUserObject);

template <>
InputParameters
validParams<MyDiffusionUserObject>()
{
  InputParameters params = validParams<NodalUserObject>();
  params.addClassDescription("Executes the external application solving for a simple diffusion equation using Finite Difference method. The mesh parameters should be copied from GeneratedMesh and pasted here.");
  MooseEnum dims("1=1 2 3");
  params.addRequiredParam<MooseEnum>(
      "dim", dims, "The dimension of the mesh to be generated"); // Make this parameter required

  params.addParam<unsigned int>("nx", 1, "Number of elements in the X direction");
  params.addParam<unsigned int>("ny", 1, "Number of elements in the Y direction");
  params.addParam<unsigned int>("nz", 1, "Number of elements in the Z direction");
  params.addParam<Real>("xmin", 0.0, "Lower X Coordinate of the generated mesh");
  params.addParam<Real>("ymin", 0.0, "Lower Y Coordinate of the generated mesh");
  params.addParam<Real>("zmin", 0.0, "Lower Z Coordinate of the generated mesh");
  params.addParam<Real>("xmax", 1.0, "Upper X Coordinate of the generated mesh");
  params.addParam<Real>("ymax", 1.0, "Upper Y Coordinate of the generated mesh");
  params.addParam<Real>("zmax", 1.0, "Upper Z Coordinate of the generated mesh");
  params.addRequiredCoupledVar("variable", "The name of the variable that this object operates on");
  params.addParam<std::string>("library_path_name",
                               "default",
                               "Name with the path for the dynamic library to load");

  return params;
}

MyDiffusionUserObject::MyDiffusionUserObject(const InputParameters & parameters)
  : NodalUserObject(parameters),
  MooseVariableInterface<Real>(this,
                               false,
                               "variable",
                               Moose::VarKindType::VAR_ANY,
                               Moose::VarFieldType::VAR_FIELD_STANDARD),
  _ext_coord(_mesh.nNodes()),
  _ext_data(_mesh.nNodes()),
  _dim(getParam<MooseEnum>("dim")),
  _nx(getParam<unsigned int>("nx")),
  _ny(getParam<unsigned int>("ny")),
  _nz(getParam<unsigned int>("nz")),
  _xmin(getParam<Real>("xmin")),
  _xmax(getParam<Real>("xmax")),
  _ymin(getParam<Real>("ymin")),
  _ymax(getParam<Real>("ymax")),
  _zmin(getParam<Real>("zmin")),
  _zmax(getParam<Real>("zmax")),
  _var(*mooseVariable()),
  _u(_var.dofValues()),
  _ext_lib_path_name(getParam<std::string>("library_path_name"))
{
  std::cout<<"Initialization of MyDiffusionUserObject"<<std::endl;

  /* Finite Difference mesh parameters for the External App.
     using MOOSE mesh parameters assigned in GeneratedMesh.
     Only works for uniform QUAD4 mesh.
     Assuming that MOOSE mesh and the mesh of the External App. has the same geometry.
     initExtCoords() generates a finite difference mesh that contains an ordered set of contiguous memory address
       --> Depends on how the External App. solves the finite difference problem.
       --> In this example, the External App. calculates the spatial second derivative as (f[i-1] + 2.*f[i] + f[i+1])/dx/dx;
       --> Thus the contiguous memory is required.
  */
  nNode_x = _nx + 1;
  nNode_y = _dim >= 2 ? _ny + 1 : _ny;
  nNode_z = _dim >= 3 ? _nz + 1 : _nz;
  xmax = getMaxInDimension(0);
  ymax = getMaxInDimension(1);
  zmax = getMaxInDimension(2);
  xmin = getMinInDimension(0);
  ymin = getMinInDimension(1);
  zmin = getMinInDimension(2);
  dx = (xmax - xmin)/_nx;
  dy = (ymax - ymin)/_ny;
  dz = (zmax - zmin)/_nz;

  _ext_coord = initExtCoords();
  print_ext_coord(_ext_coord);
}

void
MyDiffusionUserObject::initialize()
{
  // The parameters of the External Application will be initialized here
  // This member function is called once at a MOOSE time step (maybe able to be specified by using execute_on parameter in the input file)

  // Preparing for the external library handle
  _ext_lib_handle = dlopen(_ext_lib_path_name.c_str(), RTLD_LAZY);
  if (!_ext_lib_handle) {
    std::cerr << "Cannot open library: " << dlerror() << '\n';
  }
  dlerror();

  // Parameter initialization for the external diffusion Solver
  mdp.time_start = _t - _dt;
  mdp.time_end = _t;
  mdp.x0 = xmin;
  mdp.dx = dx;
  mdp.nghost = 1;
  mdp.iLower = 0 + mdp.nghost;
  mdp.iUpper = nNode_x-1 + mdp.nghost;
  mdp.numdata = 2 * mdp.nghost + mdp.iUpper - mdp.iLower + 1;
  mdp.iboundaryL = mdp.iLower - mdp.nghost;
  mdp.iboundaryU = mdp.iUpper + mdp.nghost;
  mdp.DiffCoef = 1.0;
  mdp.bc_Lval_Dirichlet = 1.0;
  mdp.bc_Uval_Dirichlet = -1.0;
}

void
MyDiffusionUserObject::execute()
{
  // Data transfer from MOOSE to External App.
  // This member function is called at every MOOSE node points.
  // _ext_data is initialized by copying the values from the assigned AuxVariable

  unsigned int ii = map_MOOSE2Ext(*_current_node);

  // Initializing _ext_data with the AuxVariable value; _u[0]
  _ext_data[ii] = _u[0];

}

void
MyDiffusionUserObject::finalize()
{
  // Updating _ext_data by executing the External App.
  // This member function is called once at a time step.
  // The external app will be instanciated and update the _ext_data here

  // Instanciating the external functions
  void (*mdp_output_data_console)(double*, int, int, double*, double) = (void (*)(double*, int, int, double*, double)) dlsym(_ext_lib_handle, "wrapper_output_data_console");
  const char *dlsym_error1 = dlerror();

  void (*mdp_Diffu_module)(double*, double*, DiffuParams) = (void (*)(double*, double*, DiffuParams)) dlsym(_ext_lib_handle, "wrapper_Diffu_module");
  const char *dlsym_error2 = dlerror();
  if ( (dlsym_error1 != NULL) || (dlsym_error2 != NULL) )
  {
    // Error messages
    if (dlsym_error1 != NULL) std::cerr << "Cannot load symbol " << "wrapper_output_data_console" << ": " << dlsym_error1 << '\n';
    if (dlsym_error2 != NULL) std::cerr << "Cannot load symbol " << "wrapper_Diffu_module" << ": " << dlsym_error2 << '\n';
    dlclose(_ext_lib_handle);
  }else{
    // Normal run
    // Temporary buffers for the External App.
    double *mdp_data;
    mdp_data = new double[mdp.numdata];
    double *mdp_x;
    mdp_x = new double[mdp.numdata];

    //Copying the coordinates and data to the temporary buffers for the External App.
    for (unsigned int i = mdp.iLower; i <= mdp.iUpper; i++) {
      mdp_x[i] = _ext_coord[i-mdp.nghost](0); // Copying x-coordinates; beware that the index of mdp_x has been shifted as mdp.nghost
      mdp_data[i] = _ext_data[i-mdp.nghost]; // Copying the data; beware that the index of mdp_data has also been shifted as mdp.nghost
    }

    //Printing the data just for checking; before the update
    mdp_output_data_console(mdp_data, mdp.iLower, mdp.iUpper, mdp_x, mdp.time_start);

    // Updating mdp_data
    mdp_Diffu_module(mdp_data, mdp_x, mdp);

    //Printing the data just for checking; after the update
    mdp_output_data_console(mdp_data, mdp.iLower, mdp.iUpper, mdp_x, mdp.time_end);

    //Updating _ext_data with mdp_data updated
    for (unsigned int i = mdp.iLower; i <= mdp.iUpper; i++) {
      _ext_data[i-mdp.nghost] = mdp_data[i]; // Copying the data; beware that the index of mdp_data has also been shifted as mdp.nghost
    }

    // Deallocating the temporary buffers
    delete[] mdp_data;
    delete[] mdp_x;

    // Deallocating the external library handle
    dlclose(_ext_lib_handle);
  }
}

void
MyDiffusionUserObject::threadJoin(const UserObject & /*y*/)
{
  //I don't know what to do with this yet.
}

Real
MyDiffusionUserObject::calc_spatial_value() const
{
  // Data transfer from External App. to MOOSE
  // This member function is called as per spatialValue() called, which is called at every node points.
  // The corresponding ext_data to the current node will be retunred by this function

  unsigned int ii = map_MOOSE2Ext(*_current_node);

  // Returning _ext_data values to pass to the AuxKernel that will modify the AuxVariable values
  return _ext_data[ii];
}

Real
MyDiffusionUserObject::getMinInDimension(unsigned int component) const
{
  switch (component)
  {
    case 0:
      return _xmin;
    case 1:
      return _dim > 1 ? _ymin : 0;
    case 2:
      return _dim > 2 ? _zmin : 0;
    default:
      mooseError("Invalid component");
  }
}

Real
MyDiffusionUserObject::getMaxInDimension(unsigned int component) const
{
  switch (component)
  {
    case 0:
      return _xmax;
    case 1:
      return _dim > 1 ? _ymax : 0;
    case 2:
      return _dim > 2 ? _zmax : 0;
    default:
      mooseError("Invalid component");
  }
}

std::vector<Point>
MyDiffusionUserObject::initExtCoords() const
{
  std::vector<Point> xyz(_mesh.nNodes());
  print_mesh_params();
  // Contiguous memory chunk for the Ext. app. (i-outermost order)
  for (unsigned int i = 0; i < nNode_x; i++) {
    for (unsigned int j = 0; j < nNode_y; j++) {
      for (unsigned int k = 0; k < nNode_z; k++) {
        unsigned int ii = nNode_y * nNode_z * i + nNode_z * j + k;
        Point xyz_build={0.0};
        xyz_build(0) = xmin + i * dx;
        xyz_build(1) = ymin + j * dy;
        xyz_build(2) = zmin + k * dz;
        xyz[ii] = xyz_build;
        std::cout<<"x,y,z = "<<xyz[ii](0)<<","<<xyz[ii](1)<<","<<xyz[ii](2)<<","<<std::endl;
      }
    }
  }
  return xyz;
}

unsigned int
MyDiffusionUserObject::map_MOOSE2Ext(const Node & MOOSEnode) const
{
  unsigned int i, j, k;
  // Looking for the nearest i-j-k index of the mesh point of Ext. App.
  i = (MOOSEnode(0)/dx - (Real)std::floor(MOOSEnode(0)/dx)) > 0.5 ? std::floor(MOOSEnode(0)/dx)+1 : std::floor(MOOSEnode(0)/dx);
  j = (MOOSEnode(1)/dy - (Real)std::floor(MOOSEnode(1)/dy)) > 0.5 ? std::floor(MOOSEnode(1)/dy)+1 : std::floor(MOOSEnode(1)/dy);
  k = (MOOSEnode(2)/dz - (Real)std::floor(MOOSEnode(2)/dz)) > 0.5 ? std::floor(MOOSEnode(2)/dz)+1 : std::floor(MOOSEnode(2)/dz);

  return nNode_y * nNode_z * i + nNode_z * j + k;
}

void
MyDiffusionUserObject::print_mesh_params() const
{
  std::cout<<"xmin, xmax = "<<xmin<<","<<xmax<<std::endl;
  std::cout<<"ymin, ymax = "<<ymin<<","<<ymax<<std::endl;
  std::cout<<"zmin, zmax = "<<zmin<<","<<zmax<<std::endl;
  std::cout<<"nNode_x, nNode_y, nNode_z = "<<nNode_x<<","<<nNode_y<<","<<nNode_z<<std::endl;
  std::cout<<"dx, dy, dz = "<<dx<<","<<dy<<","<<dz<<std::endl;
}

void
MyDiffusionUserObject::print_ext_coord(std::vector<Point> a) const
{
  for(unsigned int i = 0; i < a.size() ; i++)
  {
    std::cout<<"x,y,z = "<<a[i](0)<<","<<a[i](1)<<","<<a[i](2)<<std::endl;
  }
}
