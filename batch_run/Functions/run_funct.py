import os
import subprocess
import threading
import time
from plot_funct import plot_residuals

'''Functions which run the OpenFOAM simulations'''

def run_simulation(case_directory, run_fluid_dynamics, run_electrochemistry, log_file_path, plot_filename, plot_index, plot_residual, save_residual_plot, title):
    """
    Run the OpenFOAM simulation (fluid dynamics and electrochemistry) and save the output to a log file.
    Live plotting of residuals is enabled as subprocess.
    """
    def ensure_directory_exists(directory):
        if not os.path.isdir(directory):
            raise FileNotFoundError(f"The specified case directory does not exist: {directory}")

    def run_command(command, log_path):
        print(f"Running command: {command}")
        with open(log_path, "w") as log_file:
            process = subprocess.Popen(
                ["bash", command],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                env=os.environ,
            )
            for line in iter(process.stdout.readline, ""):
                log_file.write(line)
                log_file.flush()
            process.wait()

        if process.returncode != 0:
            raise RuntimeError(f"Command failed: {command}")

    def plot_live(log_path, plot_file, index, plot_residual, title):
        while not os.path.exists(log_path):
            time.sleep(0.1)
        try:
            plot_residuals(log_path, plot_file, index, plot_residual, title)  # Assuming this function is implemented
        except Exception as e:
            print(f"Plotting error: {e}")

    ensure_directory_exists(case_directory)
    initial_dir = os.getcwd()
    os.chdir(case_directory)

    # Run fluid dynamics simulation
    if run_fluid_dynamics:
        print("Starting fluid dynamics simulation.")
        fluid_dynamics_thread = threading.Thread(target=run_command, args=("./Run_U.sh", log_file_path))
        fluid_dynamics_thread.start()
        plot_live(log_file_path, plot_filename, plot_index, plot_residual, title)
        fluid_dynamics_thread.join()

    # Run electrochemistry simulation
    if run_electrochemistry:
        print("Starting electrochemistry simulation.")
        electrochemistry_thread = threading.Thread(target=run_command, args=("./Run_Scalar.sh", log_file_path))
        electrochemistry_thread.start()
        plot_live(log_file_path, plot_filename, plot_index, plot_residual, title)
        electrochemistry_thread.join()

    os.chdir(initial_dir)

def run_simulation_wr(case_directory, run_fluid_dynamics, run_electrochemistry, log_file_path):
    """
    Run the OpenFOAM simulation (fluid dynamics and electrochemistry) and save the output to a log file in teh case without live plot of results.
    """
    def ensure_directory_exists(directory):
        if not os.path.isdir(directory):
            raise FileNotFoundError(f"The specified case directory does not exist: {directory}")

    def run_command(command, log_path):
        """
        Run a shell command and log its output to the specified file.
        """
        print(f"Running command: {command}")
        exit_code = os.system(f"{command} > {log_path} 2>&1")
        if exit_code != 0:
            raise RuntimeError(f"Command failed: {command}")

    ensure_directory_exists(case_directory)
    initial_dir = os.getcwd()
    os.chdir(case_directory)

    # Run fluid dynamics simulation
    if run_fluid_dynamics:
        print("Starting fluid dynamics simulation.")
        fluid_dynamics_thread = threading.Thread(target=run_command, args=("./Run_U.sh", log_file_path))
        fluid_dynamics_thread.start()
        fluid_dynamics_thread.join()

    # Run electrochemistry simulation
    if run_electrochemistry:
        print("Starting electrochemistry simulation.")
        electrochemistry_thread = threading.Thread(target=run_command, args=("./Run_Scalar.sh", log_file_path))
        electrochemistry_thread.start()
        electrochemistry_thread.join()

    os.chdir(initial_dir)