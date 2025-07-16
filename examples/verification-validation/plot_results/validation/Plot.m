clear all, close all, clc;

% Define file lists for three groups
file_list_1 = {'1_0_ch_PGCA_4.23.txt', '1_0_ch_PGCA_10.58.txt'};
file_list_2 = {'2_0_ch_PGCA_4.23.txt', '2_0_ch_PGCA_10.58.txt'};
file_list_3 = {'3_0_ch_PGCA_4.23.txt', '3_0_ch_PGCA_10.58.txt'};

all_file_lists = {file_list_1, file_list_2, file_list_3};

R_files = {file_list_1, file_list_2, file_list_3};  % R group

% Configuration parameters
cell_area = 2.55; % cm^2
tolerance = 0.005; % V
voltage_step = 0.025; % V
IR_GPM=(0.019);

IR = [
    0.0822, 0.0824, 0.0820;
    0.087, 0.089, 0.090;
    0.0822, 0.0824, 0.0820;
]; 

% Model base directory and geometry
channel_ids = [1, 2, 3]; % Maximum 3 channels assumed; adjust as needed
base_dir = "../../FTFF_Model.run_470um/";
velocity_strings = {'-0.00514', '-0.01379'}; % Corresponding velocity values
flow_rates = {'4.23', '10.58', '21.15'}; % Flow rates
legend_prefix = {"1 cm s^{-1}","2.5 cm s^{-1}"};

% Define colors and markers
colors = lines(3)';
markers = {'o', 's', '^', 'v', '*', '+', 'd'};

figure;
hold on;

for flow_rate_idx = 1:length(file_list_1)
    velocity = velocity_strings{flow_rate_idx};

    % Storage for interpolation
    current_density_group = {};
    all_corrected_voltages = [];

    % Process individual samples (D and R)
    for group_idx = 1:length(all_file_lists)
        file_list = all_file_lists{group_idx};
        file_name = file_list{flow_rate_idx};

        % Read and process data
        fid = fopen(file_name, 'r');
        if fid == -1
            error('Unable to open the data file "%s".', file_name);
        end
        raw_data = textscan(fid, '%s', 'Delimiter', '\n');
        fclose(fid);
        raw_data = raw_data{1}; % Extract lines
        numeric_lines = raw_data(3:end); % Skip first two lines
        numeric_lines = strrep(numeric_lines, ',', '.'); % Replace commas with dots
        data = cellfun(@(x) sscanf(x, '%f %f')', numeric_lines, 'UniformOutput', false);
        data = vertcat(data{:}); % Concatenate rows
        
        % Extract columns
        current = data(:, 1);
        voltage = data(:, 2);
        valid_rows = current ~= 0 & voltage ~= 0;
        current = current(valid_rows);
        voltage = voltage(valid_rows);
        current_density = current / cell_area;

        % Compute averages in voltage intervals
        intervals = min(voltage):voltage_step:max(voltage);
        avg_voltage = [];
        avg_current_density = [];

        for v = intervals
            indices = voltage >= (v - tolerance) & voltage <= (v + tolerance);
            if any(indices)
                avg_voltage = [avg_voltage; mean(voltage(indices))];
                avg_current_density = [avg_current_density; mean(current_density(indices))];
            end
        end
        
        % Apply IR correction
        corrected_voltage = avg_voltage - avg_current_density*cell_area*IR_GPM/1000;

        % Store corrected voltages for defining interpolation range
        all_corrected_voltages = [all_corrected_voltages; corrected_voltage];

        % Store for group averaging
        current_density_group{end+1} = [corrected_voltage, avg_current_density];
        
    end

    % Define a global voltage range for interpolation (0.025V step)
    voltage_min_global = min(all_corrected_voltages);
    voltage_max_global = max(all_corrected_voltages);
    voltage_range = voltage_min_global:0.025:voltage_max_global;

    % Interpolation
    interp = cellfun(@(x) pchip(x(:,1), x(:,2), voltage_range), current_density_group, 'UniformOutput', false);

    % Convert to matrix
    interp_mat = cell2mat(interp');

    % Compute mean and standard deviation
    mean_exp = mean(interp_mat, 1);
    std_exp = std(interp_mat, 0, 1);

    if flow_rate_idx == 1
        % Escludi gli ultimi 3 valori
        valid_indices= 1:(length(mean_exp)-3);
    elseif flow_rate_idx == 3
        % Escludi gli ultimi 3 valori
        valid_indices = 1:(length(mean_exp)-0);
    else
        % Usa tutti i dati
        valid_indices = 1:length(mean_exp);
    end
    
    % Plot con valori filtrati
    vel={"1 cm s^{-1}","2.5 cm s^{-1}"};
    lgd_name= sprintf("Exp. %s", vel{flow_rate_idx});
    errorbar(mean_exp(valid_indices), voltage_range(valid_indices), std_exp(valid_indices), ...
        'horizontal', 'Color', "k", 'LineStyle', "none", "Marker", "o", "MarkerEdgeColor", colors(:,flow_rate_idx), "MarkerSize", 6, 'DisplayName', lgd_name);

    hold on;

% === Add Model Data ===
    model_file = fullfile(base_dir, sprintf('sim_progress_%s', velocity), 'Results.txt');

    if ~isfile(model_file)
        warning('Model file not found: %s', model_file);
        continue;
    end

    model_data = readmatrix(model_file, 'Delimiter', '\t', 'NumHeaderLines', 1);
    if isempty(model_data), continue; end

    current = model_data(:, 1);
    overpot = model_data(:, 2) * 2; % multiply if needed

    j_fine = linspace(min(current), max(current), 300);
    overpot_fine = pchip(current, overpot, j_fine);
    plot(j_fine, overpot_fine, ...
            'Color', colors( :,flow_rate_idx), ...
            'LineStyle', '-', ...
            'LineWidth', 1.2, ...
            'DisplayName', sprintf('Model %s', legend_prefix{flow_rate_idx}));
    

end

hold on;


    box on;
    grid on;
    xlabel('Average current density [mA cm^{-2}]', 'FontName', 'Times New Roman', 'FontSize', 10);
    ylabel('Overpotential [V]', 'FontName', 'Times New Roman', 'FontSize', 10);
    set(findall(gcf, '-property', 'FontSize'), 'FontSize', 11, 'FontName', 'Times New Roman');
    set(gca, 'LooseInset', [0, 0, 0.01, 0]);
    xlim([0 1200]);
    ylim([0 0.625]);

    l=legend('Location', 'southeast', 'FontSize', 11);
    %l.IconColumnWidth = 20;

    % Save the figure
    set(gcf, 'Units', 'centimeters', 'Position', [10, 7, 12, 9]);
    saveas(gcf, sprintf('validation_IR_compenstated.svg'));
    % saveas(gcf, sprintf('validation_IR_compenstated.png'));
