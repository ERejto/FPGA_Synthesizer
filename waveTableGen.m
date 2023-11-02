%{ %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Kevin Box
% kbox@hmc.edu
% Oct 31 2023
% Wavetable Generation
%} %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear
sysClock = 10000;
sampFreq = sysClock;

dictNotes4 = dictionary('c4', 261.63, 'cS4', 277.18, 'd4', 293.66, ...
                    'dS4', 311.13, 'e4', 329.63, 'f4', 349.23, 'fS4', 369.99, ...
                    'g4', 392, 'gS4', 415.30, 'a4', 440, 'aS4', 466.16, 'b4', 493.88);

notes4 = [261.63 277.18 293.66 311.13 293.63 349.23 369.99 392 415.30 440 466.16 493.88];

t = 0:1/sampFreq:0.2; % times to sample at more than one full period for any note in 4th? key

for x = 1:length(notes4) 
    table(x, :) = uint8(127*sin(2*pi*notes4(x)*t) + 127);
end 

loopLocsc4 = find(table(1, :)==127);
loopLocscS4 = find(table(2, :)==127);
loopLocsd4 = find(table(3, :)==127);
loopLocsdS4 = find(table(4, :)==127);
loopLocse4 = find(table(5, :)==127);
loopLocsf4 = find(table(6, :)==127);
loopLocsfS4 = find(table(7, :)==127);
loopLocsg4 = find(table(8, :)==127);
loopLocsgS4 = find(table(9, :)==127);
loopLocsa4 = find(table(10, :)==127);
loopLocsaS4 = find(table(11, :)==127);
loopLocsb4 = find(table(12, :)==127);

finalList = cat(2, table(1, 1:344), table(2, 1:469), table(3, 1:647), ...
             table(4, 1:225), ...
             table(5, 1:613), ...
             table(6, 1:1174), ...
             table(7, 1:500), ...
             table(8, 1:1250), ...
             table(9, 1:915), ...
             table(10, 1:125), ...
             table(11, 1:665), ...
             table(12, 1:81) ...
             );

%%
clear
t = 0:0.00125:1;
table = uint8(127*sin(2*pi*t) + 127);
plot(table)