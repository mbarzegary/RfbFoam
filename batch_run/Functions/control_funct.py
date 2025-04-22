import os
import shutil
import re
import time

def modify_potential(file_path, boundary_potential): #internal_potential
    """Modify internalField and boundary conditions in the OpenFOAM input file."""
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Flags to track if modifications were made
    #internal_modified = False
    boundary_modified = False

    # Iterate through lines to find and modify the fields
    for i, line in enumerate(lines):
        
        # Modify the boundary condition for `mem` field
        if line.strip().startswith("mem"):
            for j in range(i, len(lines)):
                if "value" in lines[j]:
                    lines[j] = f"        value           uniform {boundary_potential}; //set the desired overpotential\n"
                    boundary_modified = True
                    break

    if not boundary_modified:
        raise ValueError("Could not find the boundaryField 'mem' value to modify.")

    # Write the modified lines back to the file
    with open(file_path, 'w') as file:
        file.writelines(lines)

    print(f"Successfully updated  'mem' boundary to {boundary_potential}.") #internalField to {internal_potential}

def modify_velocity(file_path, inlet_velocity): #internal_potential
    """Modify internalField and boundary conditions in the OpenFOAM input file."""
    with open(file_path, 'r') as file:
        lines = file.readlines()

    boundary_modified = False

    # Iterate through lines to find and modify the fields
    for i, line in enumerate(lines):
        
        # Modify the boundary condition for `inlet` field
        if line.strip().startswith("inlet"):
            for j in range(i, len(lines)):
                if "value" in lines[j]:
                    lines[j] = f"        value           uniform {inlet_velocity}; //set the desired velocity\n"
                    boundary_modified = True
                    break

    if not boundary_modified:
        raise ValueError("Could not find the boundaryField 'inlet' value to modify.")

    # Write the modified lines back to the file
    with open(file_path, 'w') as file:
        file.writelines(lines)

    print(f"Successfully updated  'inlet' boundary to {inlet_velocity}.") #internalField to {internal_potential} and

def save_electrochemistry_results(case_dir, progress_folder, original_results_folder, potential, save_results):
    """Save the electrochemistry results to a new folder named with the current potential in the 0 folder."""

    results_folder = os.path.join(progress_folder, f"{potential:.3f}")
    if not os.path.exists(results_folder):
        os.makedirs(results_folder)
    if save_results:
        time_folder = os.path.join(case_dir, original_results_folder)
        shutil.copytree(time_folder, results_folder, dirs_exist_ok=True)
        print(f"Results saved to: {results_folder}")

def save_log(case_dir, progress_folder, original_results_folder, potential):
    """Move the RfbFoam.log file into the newly created results folder."""
    results_folder = os.path.join(progress_folder, f"{potential:.3f}")
    if not os.path.exists(results_folder):
        os.makedirs(results_folder)
    
    log_file_path = os.path.join(case_dir, "log.RfbFoam")
    if os.path.exists(log_file_path):
        shutil.move(log_file_path, os.path.join(results_folder, "log.RfbFoam"))
        print(f"log.RfbFoam moved to: {results_folder}")
    else:
        print("log.RfbFoam file not found.")
    
    print(f"Results saved to: {results_folder}")

def save_fluid_dynamics_results(case_dir, progress_folder, original_results_folder, potential, save_results):
    """Save the fluid dynamic results to a new folder named with the current potential in the 0 folder."""

    results_folder = os.path.join(progress_folder, f"{potential:.3f}")
    if not os.path.exists(results_folder):
        os.makedirs(results_folder)
    if save_results:
        time_folder = os.path.join(case_dir, original_results_folder)
        shutil.copytree(time_folder, results_folder, dirs_exist_ok=True)
        print(f"Results saved to: {results_folder}")


def cleanup_case_directory(case_directory):
    """Remove only result folders in the case directory (e.g., numerical folders), excluding '0', 'constant', 'system', etc."""
    protected_folders = {"0", "constant", "system", "postprocessing"}  # Folders to preserve
    for item in os.listdir(case_directory):
        item_path = os.path.join(case_directory, item)
        # Remove only directories with numerical names, excluding protected folders
        if os.path.isdir(item_path) and item not in protected_folders:
            try:
                # Check if the folder name is numeric
                float(item)  # Convertable to a number 
                shutil.rmtree(item_path)
                print(f"Deleted folder: {item_path}")
            except ValueError:
                pass  # Skip non-numerical folders

def extract_and_write_results(log_file_path, output_file):
    time.sleep(1)  # Ensure the log file is fully written

    # Define regex patterns for all required values
    result_pattern = re.compile(r"i \(mA/cm\^2\) ,  eta \(V\) : ([\d.e+-]+) , ([\d.e+-]+)")
    result_pattern_F = re.compile(r"Flow Power \(J/s\): ([\d.e+-]+)")
    result_pattern_E = re.compile(r"Electric Power \(J/s\): ([\d.e+-]+)")
    result_pattern_electrode = re.compile(r"Electrode ohmic losses \(V\): ([\d.e+-]+)")
    result_pattern_electrolyte = re.compile(r"Electrolyte ohmic losses \(V\): ([\d.e+-]+)")
    result_pattern_kinetic = re.compile(r"Kinetic/Transport losses \(V\): ([\d.e+-]+)")

    # Data storage
    results = []

    with open(log_file_path, 'r') as file:
        for line in file:
            match_electrode = result_pattern_electrode.search(line)
            match_electrolyte = result_pattern_electrolyte.search(line)
            match_kinetic = result_pattern_kinetic.search(line)
            match_F = result_pattern_F.search(line)
            match_E = result_pattern_E.search(line)
            match = result_pattern.search(line)

            if match:
                try:
                    i, eta = map(float, match.groups())
                except ValueError as e:
                    print(f"Skipping invalid entry (Error: {e})")
            if match_F:
                try:
                    P_f = float(match_F.group(1))
                except ValueError as e:
                    print(f"Skipping invalid entry (Error: {e})")
            if match_E:
                try:
                    P_e = float(match_E.group(1)) 
                except ValueError as e:
                    print(f"Skipping invalid entry (Error: {e})")
            if match_electrode:
                try:
                    loss_e = float(match_electrode.group(1))
                except ValueError as e:
                    print(f"Skipping invalid entry (Error: {e})")
            if match_electrolyte:
                try: 
                    loss_el = float(match_electrolyte.group(1))
                except ValueError as e:
                    print(f"Skipping invalid entry (Error: {e})")               
            if match_kinetic:
                try:
                    loss_k = float(match_kinetic.group(1))
                except ValueError as e:
                    print(f"Skipping invalid entry (Error: {e})")

        total_power = P_f + P_e
        results.append((i, eta, P_f, P_e, total_power, loss_e, loss_el, loss_k))
        print(results)
               
    # Check if file exists 
    file_exists = os.path.exists(output_file)

    # Write results to file in append mode
    with open(output_file, "a") as file:
        # Write header only if the file is new
        if not file_exists:
            file.write("Current (mA/cm^2)\tOverpotential (V)\tFlow Power (J/s)\tElectric Power (J/s)\tTotal Power (J/s)\tElectrode Ohmic Losses (V)\tElectrolyte Ohmic Losses (V)\tKinetic/Transport Losses (V)\n")
        
        for row in results:
            file.write("\t".join(f"{val:.6f}" for val in row) + "\n")

    print(f"Merged results appended to {output_file}.")

