%Script to fit data to a plot; weighted option commented out
T = [];	%column vector of data points
S = []; %column vector of values
%dS = []; %column vector of uncertainties
pE = ; %polynomial exponent (e.g. parabola = 2)

%w = 1 ./ dS; %weights
%W = diag(w); 
%weighted = W * S;

nPts = length(T);
V = ones(nPts, pE);
V(:, 2) = T;
T2 = T * T;
V(:, 3) = T2; %continue as necessary until V(:, pE)
%wV = W * V; %weighted V
A = V' * V; 
b = V' * S;
x = A \ b;
fit = V * x;
r = fit - S;
chiSq = r * r;

plot(T, S, '*'); 
%errorbar(T, S, dS, '*');
hold on;
plot(T, fit); 
hold off; 
