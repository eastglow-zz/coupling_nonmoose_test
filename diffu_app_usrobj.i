[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 21
  xmax = 10
[]
[Variables]
  [./d]
  [../]
[]
[AuxVariables]
  [./Auxv]
  [../]
  [./var_to_extapp]
  [../]
[]
[ICs]
  [./Init_Aux_const]
    type = ConstantIC
    variable = Auxv
    value = 0
  [../]
  [./IC_Aux_to_ext]
    type = BoundingBoxIC
    variable = var_to_extapp
    inside = 1
    outside = 0
    x1 = 5
    x2 = 6
    y1 = -0.1
    y2 = 0.1
  [../]
[]

[AuxKernels]
  [./Spatial_Auxv]
    type = SpatialUserObjectAux
    variable = Auxv
    user_object = 'Ext_diffu_solver'
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
  [./Ext_diffu_solver]
    type = MyDiffusionUserObject
    dim = 1
    nx = 21
    xmax = 10
    variable = Auxv
    variable_to_extapp = var_to_extapp
    library_path_name ='./my_external_app/diffu_eqn/mylib/wrapper_diffu_eqn_ser.so'
  [../]
[]
[Outputs]
  exodus = true
[]
