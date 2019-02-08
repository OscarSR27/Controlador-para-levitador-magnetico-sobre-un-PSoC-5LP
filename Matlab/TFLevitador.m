%Programador: Oscar Soto Rivera y Daniel Tristan
%Curso:Laboratorio de control automatico
%Proyecto:Control de un levitador magnetico

%Este programa se implementa la ecuaciones caracteristicas del levitador
%magnetico y con ayuda de Simulink se disenan los controladores para
%evaluar su comportamiento

%Constantes
g = 9.77;
R = 2.818;
L = 13.847e-3;
m = 5.85e-3;
v = 6;
u = 1.2558;
k = 7.5e-5;
y = 1;
B = 1;
syms s;

%Ecuaciones caracteristicas
G1 = (-g*R/L/v)/[(R/L+s)*(s^2 - 4*g*(g*m*R)^(.25)/(k*v)^(.25))];
G2 = (y*s^(2)/L+[2*B*g*R*(g*m*R)^(.75)/(L*v*(k*v)^(.75))-4*y*g*(g*m*R)^(.25)/(L*(k*v)^(.25))])/[(R/L+s)*(s^2 - 4*g*(g*m*R)^(.25)/(k*v)^.25)];

