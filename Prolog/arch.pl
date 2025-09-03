
padre(juan, carlos).
padre(juan, luis).
padre(carlos, daniel).
padre(carlos, diego).
padre(luis, pablo).
padre(luis, manuel).
padre(luis, ramiro).
abuelo(X,Y) :- padre(X,Z), padre(Z,Y).

%Ejercicio 1 

ancestroMal(X, X).
ancestroMal(X, Y) :- ancestroMal(Z, Y), padre(X, Z).

ancestro(X,Y) :- padre(X,Y).
ancestro(X,Y) :- padre(Z,Y), ancestro(X,Z).