import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import re
import time

def plot_residuals(log_file_path, save_path, plot_index, save_residual_plot, title):
    
    '''Live plotting of the residuals as subprocess while teh simualtion is running. It save also teh final plot if teh flag save_residual is enabled'''

    if not save_residual_plot:
        return
    
     # Compile regex for residual extraction
    residual_pattern = re.compile(
        r"Solving for (\w+), Initial residual = ([\d.e+-]+), Final residual = ([\d.e+-]+)"
    )

    # Data storage
    fields = {}
    iterations = []
    lines = {}

    # Initialize plot
    fig, ax = plt.subplots()
    fig.set_size_inches(9, 6)  # Set figure size (Width, Height in inches)
    ax.set_title(f"Residuals - {title} {plot_index:.3f}")
    ax.set_xlabel("Iteration")
    ax.set_ylabel("Residuals")
    ax.set_yscale("log")  # Use logarithmic scale
    ax.grid(True, which="both", linestyle="--")  # Show grid lines on both major and minor ticks

    # Global variable to track file position
    log_file_position = 0

    def animate(i):
        nonlocal log_file_position  # Track the last position in the log file
        new_data = False

        # Open the log file in read mode
        with open(log_file_path, "r") as log_file:
            log_file.seek(log_file_position)  # Start reading from the last position
            graph_data = log_file.readlines()  # Read new lines
            log_file_position = log_file.tell()  # Update the position marker

        # Parse new data
        for line in graph_data:

            if "End" in line:
                time.sleep(3)
                with open(log_file_path, "r") as log_file: #if appears new lines it avoid to close
                    log_file.seek(log_file_position)  
                    new_lines = log_file.readlines()

                if not new_lines:
                    print("End detected in log file.")
                    ani.event_source.stop()  # Stop the animation    
                    if save_residual_plot:
                        plt.savefig(save_path, format="svg", bbox_inches="tight")  # Save the plot IN THE CASE OF ACTIVATED FLAG 
                        print(f"Residual plot saved to {save_path}")
                    plt.close(fig)  # Close the figure
                    plt.close('all') #Close the also the results plot to avoid blocking process (MAYBE PROBLEM WHEN RESIDUAL PLOT ARE DEACTIVATED)
                    return

            match = residual_pattern.search(line)
            if match:
                field, initial_residual, final_residual = match.groups()
                if field not in fields:
                    # Initialize field data
                    fields[field] = {"initial": [], "final": []}
                    lines[f"{field}_initial"], = ax.plot(
                        [], [], label=f"{field} (Initial)", linestyle="--"
                    )
                    lines[f"{field}_final"], = ax.plot(
                        [], [], label=f"{field} (Final)", linestyle="-"
                    )
                fields[field]["initial"].append(float(initial_residual))
                fields[field]["final"].append(float(final_residual))
                new_data = True

        # Synchronize iterations with residual data
        if any(len(fields[field]["initial"]) > len(iterations) for field in fields):
            iterations.append(len(iterations) + 1)

        # Fill missing data for all fields
        for field in fields:
            while len(fields[field]["initial"]) < len(iterations):
                fields[field]["initial"].append(None)
            while len(fields[field]["final"]) < len(iterations):
                fields[field]["final"].append(None)

        # Update lines with new data
        for field in fields:
            # Filter out None values for plotting
            x, y_initial = zip(*((i, res) for i, res in zip(iterations, fields[field]["initial"]) if res is not None))
            x, y_final = zip(*((i, res) for i, res in zip(iterations, fields[field]["final"]) if res is not None))
            lines[f"{field}_initial"].set_data(x, y_initial)
            lines[f"{field}_final"].set_data(x, y_final)

        # Adjust axis limits dynamically
        ax.relim()
        ax.autoscale_view()

        # Update legend if there are valid lines
        if lines:
            ax.legend(loc="upper left", bbox_to_anchor=(1, 1))

    # Start animation and plot/save
    #manager = plt.get_current_fig_manager()
    #manager.window.wm_geometry("+100+200")  # Set position (X=1000, Y=200)
    plt.ion()  # Turn on interactive mode
    ani = animation.FuncAnimation(fig, animate, interval=10)
    plt.show(block=True)
    plt.close('all')        

def save_residuals_plot(log_file_path, save_path, plot_index, title):

    '''Save the residuals plot after the simulations in the case without live plotting reading the final log file'''

     # Compile regex for residual extraction
    residual_pattern = re.compile(r"Solving for (\w+), Initial residual = ([\d.e+-]+), Final residual = ([\d.e+-]+)")

    # Data storage
    fields = {}
    iterations = []
    lines = {}

    # Initialize plot
    fig, ax = plt.subplots()
    fig.set_size_inches(9, 6)  # Set figure size (Width, Height in inches)
    ax.set_title(f"Residuals - {title} {plot_index:.3f}")
    ax.set_xlabel("Iteration")
    ax.set_ylabel("Residuals")
    ax.set_yscale("log")  # Use logarithmic scale
    ax.grid(True, which="both", linestyle="--")  # Show grid lines on both major and minor ticks

    with open(log_file_path, "r") as log_file:
        for line in log_file:
            match = residual_pattern.search(line)
            if match:
                #print("Matched residual line.")
                field, initial_residual, final_residual = match.groups()
                # Initialize field data if not already present
                if field not in fields:
                    fields[field] = {"initial": [], "final": []}
                    
                # Collect data
                fields[field]["initial"].append(float(initial_residual))
                fields[field]["final"].append(float(final_residual))
                #print(fields)
            
            if any(len(fields[field]["initial"]) > len(iterations) for field in fields):
                iterations.append(len(iterations) + 1)
        #print(fields)

    # Fill missing data for all fields
    for field in fields:
        while len(fields[field]["initial"]) < len(iterations):
            fields[field]["initial"].append(None)
        while len(fields[field]["final"]) < len(iterations):
            fields[field]["final"].append(None)

    #Plotting
    for field, data in fields.items():
        lines[f"{field}_initial"], = ax.plot(
            range(len(data["initial"])), data["initial"], label=f"{field} (Initial)", linestyle="--"
        )
        lines[f"{field}_final"], = ax.plot(
            range(len(data["final"])), data["final"], label=f"{field} (Final)", linestyle="-"
        )

    '''
    #It will be possible to integrate this passage for a more consistent plotting of residuals
    
    for field in fields:
        # Filter out None values for plotting
        x, y_initial = zip(*((i, res) for i, res in zip(iterations, fields[field]["initial"]) if res is not None))
        x, y_final = zip(*((i, res) for i, res in zip(iterations, fields[field]["final"]) if res is not None))
        lines[f"{field}_initial"].set_data(x, y_initial)
        lines[f"{field}_final"].set_data(x, y_final)
    '''

    # Adjust axis limits dynamically
    ax.relim()
    ax.autoscale_view()

    # Update legend if there are valid lines
    if lines:
        ax.legend(loc="upper left", bbox_to_anchor=(1, 1))
        
    plt.savefig(save_path, format="svg", bbox_inches="tight")  # Save the plot
    print(f"Residual plot saved to {save_path}")
    plt.close(fig)  # Close the figure


def plot_electrochemistry_results(all_eta_values, all_i_values, save_path, potential, plot_electrochemistry_result, save_results_plot, results_plot_title):

    '''Plot the overpotential curve results and save the plot if the flags are activated'''

    if plot_electrochemistry_result or save_results_plot:
        # Initialize the plot
        plt.close('all')
        plt.show(block=False)
        plt.ioff() if not plot_electrochemistry_result else plt.ion()
        fig, ax = plt.subplots()
        fig.set_size_inches(9, 6)  # Set figure size (Width, Height in inches)

        #if plot_electrochemistry_result:
            #manager = plt.get_current_fig_manager()
            #manager.window.wm_geometry("+1000+200")  # Set position (X=1000, Y=200)

        # Plot the data
        for eta, i in zip(all_eta_values, all_i_values):
            ax.plot(i, eta, marker='o', color='r')

        ax.set_xlabel(r"$i$ [mA cm$^{-2}$]")
        ax.set_ylabel(r"$\eta$ [V]")
        ax.set_title(f"{results_plot_title} {potential:.3f}")
        ax.grid(True)

        # Save the plot if required
        if save_results_plot:
            plt.savefig(save_path, format="svg", bbox_inches="tight")
            print(f"Plot saved to {save_path}")

        if plot_electrochemistry_result:
            print("Plotting results")

        #plt.close(fig)


def plot_overpotentials(all_eta_values, all_i_values, save_path, potential, plot_electrochemistry_result, save_results_plot, results_plot_title):

    '''Plot the overpotential curve results and save the plot if the flags are activated'''

    if plot_electrochemistry_result or save_results_plot:
        # Initialize the plot
        plt.close('all')
        plt.show(block=False)
        plt.ioff() if not plot_electrochemistry_result else plt.ion()
        fig, ax = plt.subplots()
        fig.set_size_inches(9, 6)  # Set figure size (Width, Height in inches)

        #if plot_electrochemistry_result:
            #manager = plt.get_current_fig_manager()
            #manager.window.wm_geometry("+1000+200")  # Set position (X=1000, Y=200)

        # Plot the data
        for eta, i in zip(all_eta_values, all_i_values):
            ax.plot(i, eta, marker='o', color='r')

        ax.set_xlabel(r"$i$ [mA cm$^{-2}$]")
        ax.set_ylabel(r"$\eta$ [V]")
        ax.set_title(f"{results_plot_title} {potential:.3f}")
        ax.grid(True)

        # Save the plot if required
        if save_results_plot:
            plt.savefig(save_path, format="svg", bbox_inches="tight")
            print(f"Plot saved to {save_path}")

        if plot_electrochemistry_result:
            print("Plotting results")

        #plt.close(fig)