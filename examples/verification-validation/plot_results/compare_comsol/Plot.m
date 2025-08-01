clc;
close all;
clear;

fs = 11;

% Define all file names
fileNames = ["COMSOL.txt", "OF.txt"];
legendLabels = {"COMSOL Model", "RfbFoam Model"};%, "No mass transfer", "Mass transfer implementation", "Exchange current implementation", "COMSOL model (TCD)"}; % Descriptive labels for the legend

data = cell(length(fileNames), 1);

% Load data from each file
for i = 1:length(fileNames)
    data{i} = importdata(fileNames{i});
end

% Define colors for different plots
colors = lines(length(fileNames));

% Create a figure and set its size
figHandle = figure;
set(figHandle, 'Units', 'centimeters', 'Position', [10, 10, 16, 10]);

% Main plot
hold on;

% Plot the data for the plot (current-overpotential)
for i = 1:length(fileNames)
    PolCurve = data{i};
    x = PolCurve(:, 1); % x-axis data
    y = PolCurve(:, 2); % y-axis data

    plot(x, y, 'Color', colors(i, :), 'LineStyle', '-', 'LineWidth', 1.2, ...
        'Marker', '+', 'MarkerSize', 5);
end

% Configure the main plot
xlabel('Average current density [mA cm$^{-2}$]', 'Interpreter', 'latex');
ylabel('Overpotential [V]', 'Interpreter', 'latex');
ylim([0, 0.45]);
xlim([0, 1200]);

% Set style, font size, and other properties in one line
set(gca, 'TickLabelInterpreter', 'latex', 'FontSize', fs, ...
    'XColor', 'k', 'YColor', 'k', 'Box', 'on');

grid on;

% Add the legend
legend(legendLabels, 'Interpreter', 'latex', 'FontSize', fs, 'Location', 'northwest' );

% % Add zoomed inset
% axesInset = axes('Position', [0.60, 0.68, 0.13, 0.21]); % Adjust [x, y, width, height]
% hold on;
% 
% % Plot zoomed-in data
% for i = 1:length(fileNames)
%     PolCurve = data{i};
%     x = PolCurve(:, 1); % x-axis data
%     y = PolCurve(:, 2); % y-axis data
% 
%     plot(x, y, 'Color', colors(i, :), 'LineStyle', '-', 'LineWidth', 1.2, ...
%         'Marker', '+', 'MarkerSize', 5);
% end
% 
% % Set the limits for the zoomed region
% xlim(axesInset, [1050,1150]); % Zoomed x-axis range
% ylim(axesInset, [0.35, 0.45]); % Zoomed y-axis range
% 
% % Add a minor grid to the inset
% grid on;
% grid minor;
% 
% % Style for the inset
% set(axesInset, 'TickLabelInterpreter', 'latex', 'FontSize', fs-2, ...
%     'XColor', 'k', 'YColor', 'k', 'Box', 'on');
% 
% % Add a red box in the main plot to highlight the zoomed area
% mainBoxX = [330, 345, 345, 330, 330]; % Define x-coordinates for the rectangle
% mainBoxY = [0.19, 0.19, 0.21, 0.21, 0.19]; % Define y-coordinates for the rectangle
% plot(mainBoxX, mainBoxY, 'r-', 'LineWidth', 1.2, 'LineStyle','--');
% 
% % Add a red box around the zoomed inset using annotation
% annotation('rectangle', [0.81, 0.81, 0.06, 0.1], 'Color', 'r', 'LineWidth', 1.2, "LineStyle","--");

% Export the plot as SVG and figure
print(figHandle, 'OverpotentialComparisons.svg', '-dsvg');
savefig(figHandle, 'OverpotentialComparisons.fig');
