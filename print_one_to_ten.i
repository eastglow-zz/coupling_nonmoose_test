
[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 1
[]
[Variables]
  [./d]
  [../]
[]
[Executioner]
  type = MyExecutioner
  [./TimeStepper]
    type = MyTimeStepper
    library_path_name ='./my_external_app/static/lib/wrapper.so'
    function_name = 'wrap_print_one_to_ten'
  [../]
  end_time = 2
[]
[Problem]
  kernel_coverage_check = false
  solve = false
[]
