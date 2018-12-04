[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 2
  ny = 2
  nz = 2
  xmax = 2
  ymax = 2
  zmax = 2
[]
[Variables]
  [./d]
  [../]
[]
[AuxVariables]
  [./Auxv]
  [../]
[]
[ICs]
  [./Init_Aux_const]
    type = ConstantIC
    variable = Auxv
    value = 1
  [../]
[]

[AuxKernels]
  [./Spatial_Auxv]
    type = SpatialUserObjectAux
    variable = Auxv
    user_object = 'Auxv_modi'
  [../]
[]
[Executioner]
  type = Transient
  [./TimeStepper]
    type = ConstantDT
    dt = 1
  [../]
  start_time = 0
  end_time = 2
[]
[Problem]
  kernel_coverage_check = false
  solve = false
[]
[UserObjects]
  [./Auxv_modi]
    type = ExternalSpatialData
    dim = 3
    nx = 2
    ny = 2
    nz = 2
    xmax = 2
    ymax = 2
    zmax = 2
    variable = Auxv
  [../]
[]
[Outputs]
  exodus = true
[]
