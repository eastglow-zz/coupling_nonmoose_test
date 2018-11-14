
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
  [./T]
  [../]
[]
[Executioner]
  type = MyDiffuExecutioner
  [./TimeStepper]
    type = MyDiffuTimeStepper
    dt = 0.5
    library_path_name ='./my_external_app/diffu_eqn/mylib/wrapper_diffu_eqn_ser.so'
  [../]
  end_time = 2
[]
[Problem]
  kernel_coverage_check = false
  solve = false
[]
