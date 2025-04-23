import os
import shutil
from .run_funct import*
from .plot_funct import*
from .control_funct import*

'''Main functions for the fluid dynamics and electrochemestry simulation process management.'''


def fluid_dynamic(j, vel, potentials, case_directory, log_file_path, continuation_sweep, run_fluid_dynamics, run_electrochemistry, plot_residual, save_residual_plot, res_f_n, save_results):
    
    folders = [f for f in os.listdir(case_directory) if os.path.isdir(os.path.join(case_directory, f)) and f.isdigit()] #Get folder with integer number (time steps)
    highest_folder = max(folders, key=int) #Select the folder with higher number (last time step)
    
    if j==0 or not continuation_sweep:
        U_file_path = os.path.join(case_directory, "0", "U")
        sim_prog_folder="1" # from where to copy the results in the progress folder to save them
    else:      
        #To inizialize the results independently form previous electrochemistry simulations if presents
        source_folder = os.path.join(case_directory, "0")
        destination_folder = os.path.join(case_directory, "1")
        numeric_values = list(map(int, folders))
        highest_value = int(highest_folder)
        last_fluid_dynamic_results_folder = os.path.join(case_directory, str(highest_value))
        shutil.copytree(source_folder, destination_folder, dirs_exist_ok=True)
        file_names = ["U", "p"]
        for file_name in file_names:
            source_file = os.path.join(last_fluid_dynamic_results_folder, file_name)
            destination_file = os.path.join(destination_folder, file_name)
            if source_file != destination_file:
                shutil.copy2(source_file, destination_file)        

        #Remove folders with previous results (maybe can be possible also with the controlDict)
        for num in numeric_values:
            if 0 < num <= highest_value and num != 1: #Remove all the results folders exept the new initial folder "1"
                folder_to_remove = os.path.join(case_directory, str(num))
                shutil.rmtree(folder_to_remove)

        U_file_path = os.path.join(case_directory, "1", "U")
        sim_prog_folder="2"
        #U_file_path = os.path.join(case_directory, highest_folder, "U") # To reuse previous results (parametric sweep)

    if not run_electrochemistry or not potentials:
        Phi_file_path = os.path.join(case_directory, "0", "Phi2")
        with open(Phi_file_path, 'r') as file:
            lines = file.readlines()
            
        for i, line in enumerate(lines):
                # Locate the "mem" block
                if line.strip() == "mem":
                    # Search for the "value" line within the block
                    for j in range(i + 1, len(lines)):
                        if "value" in lines[j]:
                            line_content = lines[j].strip()  # Remove leading/trailing whitespace
                            if "uniform" in line_content:
                                # Extract the part after "uniform"
                                start_index = line_content.index("uniform") + len("uniform")
                                value_str = line_content[start_index:].strip()  # Remove leading/trailing whitespace
                                value_str = value_str.split("//")[0].strip()  # Remove comments starting with `//`
                                value_str = value_str.strip(";")  # Remove the trailing semicolon
                                potential_0 = float(value_str)  # Convert to float
                                print(f"Extracted potential: {potential_0} as reference as the electrochemistry simulation will not run")
                            break
                    break

        if potential_0 is None:
            print("Potential value not found.")        

    # Format the velocity tuple as a string without commas
    vel_string = f"({vel[0]} {vel[1]} {vel[2]})"
    modify_velocity(U_file_path, vel_string)

    progress_folder_name = f"sim_progress_{vel[res_f_n]:.5f}"  # name of the folder where the results will be saved

    # Define the folder to save progress
    fluid_dynamic.progress_folder = os.path.join(case_directory, progress_folder_name)
    os.makedirs(fluid_dynamic.progress_folder, exist_ok=True)

    print(f"Step {j + 1}: Running simulation for velocity {vel[res_f_n]:.6f}")
    plot_filename = os.path.join(fluid_dynamic.progress_folder, f"plot_residuals_{vel[res_f_n]:.5f}.svg")
    residuals_plot_title = 'Velocity [m/s]:'

    plot_index = vel[res_f_n]

    save_results=False
    if not run_electrochemistry or not potentials: #save the current results only if the electrochemistry simulation is not performed
        save_results=True

    if plot_residual:
        run_electrochemistry=False
        run_simulation(case_directory, run_fluid_dynamics, run_electrochemistry, log_file_path, plot_filename, plot_index, plot_residual, save_residual_plot, residuals_plot_title)
    else:
        run_electrochemistry=False
        run_simulation_wr(case_directory, run_fluid_dynamics, run_electrochemistry, log_file_path)
        if save_residual_plot:
            save_residuals_plot(log_file_path, plot_filename, plot_index, residuals_plot_title)
    
    if save_results:
        save_fluid_dynamics_results(case_directory, fluid_dynamic.progress_folder, sim_prog_folder, potential_0, save_results) #results of the fluid dynamic simulation saved in a folder which title of the boundary potential in the 0 folder
        





def electrochemistry(i, vel, velocity, potential, case_directory, progress_folder, log_file_path, continuation_sweep, run_electrochemistry, run_fluid_dynamics, plot_residual, plot_result, save_results_plot, save_residual_plot, res_f_n, save_results):
    # Initialize data storage for cumulative plotting

    folders = [f for f in os.listdir(case_directory) if os.path.isdir(os.path.join(case_directory, f)) and f.isdigit()] #Get folder with integer number (time steps)
    highest_folder = max(folders, key=int) #Select the folder with higher number (last time step)

    if i==0 or not continuation_sweep:
            phi2_file_path = os.path.join(case_directory, "0", "Phi2")  # Path to the file with potential value (Potentiostatic conditions)
 
    else:
        phi2_file_path = os.path.join(case_directory, highest_folder, "Phi2") # To reuse previous results (parametric sweep)
        
        #Remove folders with previous results (maybe can be possible also with the controlDict)
        numeric_values = list(map(int, folders))
        highest_value = int(highest_folder) 
        for num in numeric_values:
            if 0 < num < highest_value:
                folder_to_remove = os.path.join(case_directory, str(num))
                shutil.rmtree(folder_to_remove)

    print(f"Step {i + 1}: Running simulation for potential {potential:.3f}")
 
    # Modify the potential value in the input file
    modify_potential(phi2_file_path, potential)


#Assign name of the progress folder with the boundary velocity in the 0 folder if velocity is not specified in teh Main Control 

    if not run_fluid_dynamics or not velocity:
        U_file_path = os.path.join(case_directory, "0", "U")
        vel = ()  # Initialize the variable to store the velocity

        with open(U_file_path, 'r') as file:
            lines = file.readlines()
        # Iterate through lines to find the `inlet` and extract `inlet_velocity` from teh 0 folder
        for i, line in enumerate(lines):
            if line.strip().startswith("inlet"):
                for j in range(i, len(lines)):
                    if "value" in lines[j]:
                        # Extract the velocity vector from the line
                        line_content = lines[j].strip()  # Remove leading/trailing whitespace
                        if "uniform" in line_content:
                            # Extract the part after "uniform"
                            start_index = line_content.index("uniform") + len("uniform")
                            vector_str = line_content[start_index:].strip()  # Remove leading/trailing whitespace
                            vector_str = vector_str.split("//")[0].strip()  # Remove comments starting with `//`
                            vector_str = vector_str.strip("();")  # Remove parentheses and semicolon
                            vector_parts = vector_str.split()  # Split the vector into components
                            if len(vector_parts) == 3:
                                vel = (float(vector_parts[0]), float(vector_parts[1]), float(vector_parts[2]))
                                #print(f"Extracted velocity vector: {vel}")
                        break
                break

        progress_folder_name = f"sim_progress_{vel[res_f_n]:.5f}"  # name of the folder where the results will be saved
        progress_folder = os.path.join(case_directory, progress_folder_name)
        os.makedirs(progress_folder, exist_ok=True)

    plot_filename = os.path.join(progress_folder, f"plot_residuals_{potential:.3f}.svg")
    plot_index = potential
    residuals_plot_title = f'Velocity [m/s]: {vel[res_f_n]}; Potential [V]:'

#******************************************************************************** start simulation process *****************************************************************************************************

    if plot_residual:
        run_fluid_dynamics=False
        run_simulation(case_directory, run_fluid_dynamics, run_electrochemistry, log_file_path, plot_filename, plot_index, plot_residual, save_residual_plot, residuals_plot_title)
    else:
        run_fluid_dynamics=False
        run_simulation_wr(case_directory, run_fluid_dynamics, run_electrochemistry, log_file_path)
        if save_residual_plot:
            save_residuals_plot(log_file_path, plot_filename, plot_index, residuals_plot_title)

    # Extract results from the log file and store them
    plot_filename = os.path.join(progress_folder, f"Results.txt")
    extract_and_write_results(log_file_path, plot_filename)

    # Save the results to a folder named after the current potential
    if save_results:
       save_electrochemistry_results(case_directory, progress_folder, highest_folder, potential, save_results)

    save_log(case_directory, progress_folder, highest_folder, potential)
    