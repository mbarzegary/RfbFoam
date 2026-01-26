
# *************************************************************   ENVIROMENT SETTING   ************************************************************************


import sys
from Functions.main_funct import * # Call the function 

# OpenFOAM case
case_directory = "../examples/verification-validation/FTFF_Model.run_470um/"


# *********************************************************   PARAMETERS SWEEP DEFINITIONS   *********************************************************************


potentials = [-0.77, -0.8, -0.85, -0.9, -0.95, -1, -1.05, -1.1, -1.15, -1.2]
velocity = [(0, 0,-0.005141), (0, 0, -0.0137852)] #, (0, 0, -0.0275705)]

results_folder_name = 2   # Set the component of velocity to use as name of the folder where the results will be saved   ---->  (0 1 2)

# OPTIONS

run_fluid_dynamics = True
run_electrochemistry = True

continuation_sweep = True

plot_residual = False
save_residual_plot = False

#Relative only for electrochemistry simulation

plot_result = True
save_results_plot = True

# To maintain the field results
save_results = True


# **********************************************************   PARAMETRIC SWEEP SIMULATIONS   ******************************************************************

# Convert to absolute path if relative
case_directory = os.path.abspath(case_directory)


# Clean up old results
cleanup_case_directory(case_directory)
# Define the log file path for residual monitoring
log_file_path = os.path.join(case_directory, "log.RfbFoam")

if run_fluid_dynamics and velocity:
    for j, vel in enumerate(velocity):
        fluid_dynamic(j, vel, potentials, case_directory, log_file_path, continuation_sweep, run_fluid_dynamics, run_electrochemistry, plot_residual, save_residual_plot,results_folder_name, save_results)

        if run_electrochemistry:
            for i, potential in enumerate(potentials):
                electrochemistry(i, vel, velocity, potential, case_directory, fluid_dynamic.progress_folder, log_file_path, continuation_sweep, run_electrochemistry, run_fluid_dynamics, plot_residual, plot_result, save_results_plot, save_residual_plot, results_folder_name, save_results)
else:
    vel=None
    progress_folder="self"
    for i, potential in enumerate(potentials):
            electrochemistry(i, vel, velocity, potential, case_directory, progress_folder, log_file_path, continuation_sweep, run_electrochemistry, run_fluid_dynamics, plot_residual, plot_result, save_results_plot, save_residual_plot, results_folder_name, save_results)
