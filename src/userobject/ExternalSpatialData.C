//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ExternalSpatialData.h"
//MOOSE includes

registerMooseObject("MooseApp", ExternalSpatialData);

template <>
InputParameters
validParams<ExternalSpatialData>()
{
  InputParameters params = validParams<NodalUserObject>();
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

  return params;
}

ExternalSpatialData::ExternalSpatialData(const InputParameters & parameters)
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
  _u(_var.dofValues())
{
  std::cout<<"Initialization of ExternalSpatialData"<<std::endl;

  // Initializing the external mesh parameters
  nNode_x = _nx + 1;
  nNode_y = _ny + 1;
  nNode_z = _nz + 1;
  xmax = getMaxInDimension(0);
  ymax = getMaxInDimension(1);
  zmax = getMaxInDimension(2);
  xmin = getMinInDimension(0);
  ymin = getMinInDimension(1);
  zmin = getMinInDimension(2);
  dx = (xmax - xmin)/_nx;
  dy = (ymax - ymin)/_ny;
  dz = (zmax - zmin)/_nz;

  // Initializing the external mesh coordinate array
  _ext_coord = initExtCoords();

  // Printing the external mesh coordinates just for check
  print_ext_coord(_ext_coord);
}

void
ExternalSpatialData::initialize()
{
  //This member function is called once at a time step.
  // The parameters of the external application will be initialized here

  std::cout << "This is ExternalSpatialData::initialize!!" << std::endl;
  std::cout << "time_now and time_prev = " << _t << "," << _t - _dt << std::endl;
  std::cout << "_nx,_ny,_nz = " << _nx << "," << _ny << "," <<_nz << std::endl;
  std::cout << "_ext_coord.size() = " << _ext_coord.size() << std::endl;
  std::cout << "_ext_data.size() = " << _ext_data.size() << std::endl;
}

void
ExternalSpatialData::execute()
{
  //This member function is called at every node points.
  // ext_coord and ext_data will be initialized here
  // ext_data is initialized by bringing the values from an assigned AuxVariables
  //  --> MOOSE variable interface is required (See how NodalNormalEvaluator access the AuxVariables)

  Real x = (*_current_node)(0);
  Real y = (*_current_node)(1);
  Real z = (*_current_node)(2);
  unsigned int nodeID = _current_node->id();
  unsigned int ii = map_MOOSE2Ext(*_current_node);
  Point xyz_ex = _ext_coord[ii];

  // Initializing _ext_data with the AuxVariable value; _u[0]
  _ext_data[ii] = _u[0];

  std::cout << "This is ExternalSpatialData::execute!!, x,y,z = " << x <<","<< y <<","<< z << std::endl;
  std::cout<<"ii = "<<ii<<std::endl;
  std::cout << "Nearest Ext. Coordnate = "<< xyz_ex(0) <<","<< xyz_ex(1) <<","<< xyz_ex(2) << std::endl;
  std::cout << "time_now and time_prev = " << _t << "," << _t - _dt << std::endl;
  std::cout << "_nx,_ny,_nz = " << _nx << "," << _ny << "," <<_nz << std::endl;
  std::cout << "_u value here = " << _u[0] << std::endl;
  std::cout << "nodeID = " << nodeID << std::endl;
}

void
ExternalSpatialData::finalize()
{
  //This member function is called once at a time step.
  // The external app will be solved and update the ext_data here

  std::cout << "This is ExternalSpatialData::finalize!!" << std::endl;
  std::cout << "time_now and time_prev = " << _t << "," << _t - _dt << std::endl;
  std::cout << "_nx,_ny,_nz = " << _nx << "," << _ny << "," <<_nz << std::endl;
}

void
ExternalSpatialData::threadJoin(const UserObject & /*y*/)
{
  // I don't know what to do with this yet.
}

Real
ExternalSpatialData::calc_spatial_value(const Point & p) const // p is the position vector of the current node
{
  //This member function is called as per spatialValue() called, which is called at every node points.
  // The corresponding ext_data to the current node will be retunred by this function

  //Getting the Node coordinate
  Real x = p(0);
  Real y = p(1);
  Real z = p(2);
  unsigned int ii = map_MOOSE2Ext(*_current_node);

  //Does _current_node work here? YES!!
  std::cout << "This is ExternalSpatialData::calc_spatial_value()!!, x,y,z = " << x <<","<< y <<","<< z << std::endl;
  std::cout << "time_now and time_prev = " << _t << "," << _t - _dt << std::endl;
  std::cout << "_ext_data value at this node :  = " << _ext_data[ii] << std::endl;
  return _ext_data[ii];
}

Real
ExternalSpatialData::getMinInDimension(unsigned int component) const
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
ExternalSpatialData::getMaxInDimension(unsigned int component) const
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
ExternalSpatialData::initExtCoords() const
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
ExternalSpatialData::map_MOOSE2Ext(const Node & MOOSEnode) const
{
  unsigned int i, j, k;
  // Looking for the nearest i-j-k index of the mesh point of Ext. App.
  i = (MOOSEnode(0)/dx - (Real)std::floor(MOOSEnode(0)/dx)) > 0.5 ? std::floor(MOOSEnode(0)/dx)+1 : std::floor(MOOSEnode(0)/dx);
  j = (MOOSEnode(1)/dy - (Real)std::floor(MOOSEnode(1)/dy)) > 0.5 ? std::floor(MOOSEnode(1)/dy)+1 : std::floor(MOOSEnode(1)/dy);
  k = (MOOSEnode(2)/dz - (Real)std::floor(MOOSEnode(2)/dz)) > 0.5 ? std::floor(MOOSEnode(2)/dz)+1 : std::floor(MOOSEnode(2)/dz);

  return nNode_y * nNode_z * i + nNode_z * j + k;
}

void
ExternalSpatialData::print_mesh_params() const
{
  std::cout<<"xmin, xmax = "<<xmin<<","<<xmax<<std::endl;
  std::cout<<"ymin, ymax = "<<ymin<<","<<ymax<<std::endl;
  std::cout<<"zmin, zmax = "<<zmin<<","<<zmax<<std::endl;
  std::cout<<"nNode_x, nNode_y, nNode_z = "<<nNode_x<<","<<nNode_y<<","<<nNode_z<<std::endl;
  std::cout<<"dx, dy, dz = "<<dx<<","<<dy<<","<<dz<<std::endl;
}

void
ExternalSpatialData::print_ext_coord(std::vector<Point> a) const
{
  for(unsigned int i = 0; i < a.size() ; i++)
  {
    std::cout<<"x,y,z = "<<a[i](0)<<","<<a[i](1)<<","<<a[i](2)<<std::endl;
  }
}

/*
// Generalized node mapping routine - not ready yet
// As possible, This method should be avoided because this routine is VERY inefficient
void
ExternalSpatialData::getNearestNodeIndexMapAll(std::vector<unsigned int> map_MOOSE2Ext, std::vector<unsigned int> map_Ext2MOOSE, std::vector<Point> xyz_ext) const
{
  std::vector<dof_id_type> nodelist;
  nodelist = _mesh.getNodeList();
  for (auto i = 0; i < nodelist.size(); i++)
  {
    Node * nd = _mesh.queryNodePtr(i); //This call retrives a Node pointer corresponding to the Node ID provided.
    unsigned int nodeID = nd->id();
    std::cout<<"getNearestNodeIndexMap; node loop; id: "<<nodeID<<std::endl;
  }
}
*/
