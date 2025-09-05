
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

%Ejercicio 2 

vecino(X, Y, [X|[Y|Ls]]).
vecino(X, Y, [X|Ls]) :- vecino(X, Y, Ls).
% ?- vecino(5, Y, [5,6,5,3])
%      |_  vecino(5,6,[5|6,|[5,3]]) --------------- S = {X := 5, Y := 6, Ls := [5,3]}
%      |
%      |
%      |_ vecino(5,Y,[6,5,3]) -------------- S = {X := 5, Ls := [6,5,3]}
%      |     |_  vecino(5,6,[]) -------------- Falla
%      |
%      |_ vecino(5,Y,[]) falla 

%Que pasa si invertimos el orden de las reglas, el resultado es el mismo pero corta inmediatamente y no pregunta si hay otros

vecino1(X, Y, [X|Ls]) :- vecino1(X, Y, Ls).
vecino1(X, Y, [X|[Y|Ls]]).

%Ejercicio 3

%Al realizar la consulta menorOIgual(0,X), unifica con la primera regla X' := 0 y X := Suc(Y), por lo tanto la consulta se reduce 
%menorIgual(0,Y), y al volver a intentar unificar pasa de nuevo lo mismo X := 0, Y' := Suc(Y) y vuelve a quedar la consulta menorOIgual(X,Y')
%Por lo tanto el programa se cuelga debido a que no encuentra un a resolucion

natural(0).
natural(suc(X)) :- natural(X).
menorOIgual(X, suc(Y)) :- menorOIgual(X, Y).
menorOIgual(X,X) :- natural(X).

%pero si invertimos las reglas, nos genera los numero del 1 al infinito con la consulta menorOIgual(0,X)
menorOIgual1(X,X) :- natural(X).
menorOIgual1(X, suc(Y)) :- menorOIgual1(X, Y).

%Un Programa en Prolog puede colgarse debido a que prolog utiliza DFS e intenta unificar con la primera regla que encuentra
%si esa regla produce una rama infinita de unificaciones sin exito al usar DFS Prolog, se quedara buscando una refutacion en esa
%rama infinita 

%Ejercicio 4

juntar([],L2,L2).
juntar([X|L1],L2,[X|Resto]) :- juntar(L1,L2,Resto). 

%Ejercicio5 

%Definir last(?L,?U)

last(XS,U) :- append(_,[U],XS).

%Definir reverse(+L,?R)

reversa([],[]).
reversa([X|XS],R) :- reversa(XS,R1), append(R1,[X],R).

%definir prefijo 

prefijo(L,P) :- append(P,_,L).

%Definir sufijo 

sufijo(L,S) :- append(_,S,L).

%sublista

sublista(L,S) :-  prefijo(L,P),sufijo(P,S), S \= [].

%pertenece

pertenece(X,[X]).
pertenece(X,[_|Ys]) :- pertenece(X,Ys).

%Ejercicio 6 

aplanar([],[]).
aplanar([X|XS],[X|YS]) :- not(is_list(X)),aplanar(XS,YS).
aplanar([X|XS],YS) :- is_list(X), aplanar(X,YS1),aplanar(XS,YS2),append(YS1,YS2,YS).

%Ejercicio 7 

%interseccion(L1,L2,L3)

interseccion([],_,[]).
interseccion(_,[],[]).
interseccion([X|XS],YS,[X|R]) :- member(X, YS),interseccion(XS,YS,R).
interseccion([X|XS],YS,R) :- not(member(X,YS)), interseccion(XS,YS,R).

%partir(N,L,L1,L2). 

partir(0,L,[],L).
partir(N,[X|XS],[X|L1],L2) :- N > 0, N1 is N - 1, partir(N1,XS,L1,L2).

%reversibilidad N tiene que estar instanciado si o si, debido a que usa is y esto evalua una expresion aritmetica por eso N debe estar instanciado
%luego para L, L1, L2 no hay restricciones, partron de instanciacion partir(+N,?L,?L1,?L2).

%definir borrar

borrar([],_,[]).
borrar([X|XS],X,YS) :- borrar(XS,X,YS).
borrar([Y|XS],X,[Y|YS]) :-Y \= X,borrar(XS,X,YS).

%sacarDuplicados

sacarDuplicados([],[]).
sacarDuplicados([X|XS],[X|YS]) :-borrar(XS,X,YS1),sacarDuplicados(YS1,YS).

%permutacion 

permutacion([],[]).
permutacion(L1,[E|RP]) :- select(E, L1, Resto), permutacion(Resto,RP).

%reparto 

reparto(L1,1,[L1]).
reparto(L,N,[L1|LL]) :- 
    N > 1, append(L1,L2,L), 
    N1 is N - 1,
    reparto(L2,N1,LL).
 

repartoSinVacias([],[]).
repartoSinVacias(L, [P1|Ps]) :-
    append(P1, Rest, L),
    P1 \= [],
    repartoSinVacias(Rest, Ps).


%Ejercicio 8 
%Esta el predicado sumlist
sumaLista([],0).
sumaLista([X|XS],N) :- sumaLista(XS,N1), N is N1 + X.


subconjuntos([],[]).
subconjuntos([X|XS],[X|YS]) :- subconjuntos(XS,YS).
subconjuntos([_|XS],YS):- subconjuntos(XS,YS).

parteQueSuma(L,S,P) :- subconjuntos(L,P),sumaLista(P,S).

%Ejercicio 9 

%Patron de instanciacion desde(+X,-Y), debido a que X debe estar instanciado porque se hace una operacion aritmetica con su valor 
desde(X,X).
desde(X,Y) :- N is X+1, desde(N,Y).

%ahora desdeReversible(+X,?Y)
desdeReversible(X,X).
desdeReversible(X,Y) :- nonvar(Y), Y > X.
desdeReversible(X,Y) :- var(Y), N is X + 1, desde(N,Y).

%Ejercicio 10 
%intercalar(?L1,?L2,?L3 parton de instanciacion)
intercalar(L1,[],L1).
intercalar([],L2,L2).
intercalar([X|L1],[Y|L2],[X,Y|Resto]) :- intercalar(L1,L2,Resto). 

%ejercicio 11 

vacio(nil).

raiz(bin(_,R,_),R).

max(N1,N2,N1) :- N1 >= N2.
max(N1,N2,N2) :- N2 > N1.

altura(nil,0).
altura(bin(I,_,D),N) :- altura(I,NI),altura(D,ND), max(NI,ND,M) ,N is 1 + M.

cantNodos(nil,0).
cantNodos(bin(I,_,D),N) :- cantNodos(I,NI), cantNodos(D,ND), N is 1 + NI + ND.


%Ejercicio 12 

inorder(nil,[]).
inorder(bin(I,R,D),L) :- inorder(I,LI),inorder(D,LD), append(LI,[R|LD],L).

% Caso base: La lista vacía corresponde a un árbol nil.
arbolConInorder([], nil).

% Caso recursivo: Para construir el árbol
arbolConInorder(L, bin(TI, R, TD)) :-
    L \= [],                     % Asegura que la lista no esté vacía
    length(L, N),                % Obtiene la longitud de la lista
    I is N // 2,                 % Calcula el índice del elemento central (división entera)
    nth0(I, L, R),               % Obtiene la raíz R en el índice I (nth0 usa índices desde 0)
    append(LI, [_|LD], L),       % Divide la lista en la izquierda (LI) y la derecha (LD)
    length(LI, I),               % Asegura que LI tiene la longitud correcta
    arbolConInorder(LI, TI),     % Llamada recursiva para el subárbol izquierdo
    arbolConInorder(LD, TD).     % Llamada recursiva para el subárbol derecho


aBB(nil).
aBB(bin(I,R,D)) :- inorder(bin(I,R,D),L),msort(L, L).

aBBInsertar(X,nil,bin(nil,X,nil)).
aBBInsertar(X,bin(I,R,D),bin(RI,R,D)) :- R > X, aBBInsertar(X,I,RI).
aBBInsertar(X,bin(I,R,D),bin(I,R,RD)) :- R < X, aBBInsertar(X,D,RD).

%Ejercicio 13 

generarPares(X,Y) :- desdeReversible(0,S), between(0, S, X), Y is S -X.


%coprimos(X,Y) :- generarPares(X,Y), gcd(X,Y).

%Ejercicio 14


% listaQueSuma(+S, N, -XS)
listaQueSuma(0, 0, []).
listaQueSuma(S, N, [X | XS]) :- N > 0, between(0, S, X), S2 is S-X, N2 is N-1, listaQueSuma(S2, N2, XS).

% filasQueSuman(+S, +N, +XSS)
filasQueSuman(_, _, []).
filasQueSuman(S, N, [XS| XSS]) :- listaQueSuma(S, N, XS), filasQueSuman(S, N, XSS).

% cuadradoSemiMágico(+N, -XS)
cuadradoSemiMágico(N, L) :- length(L, N), desde(0, S), filasQueSuman(S, N, L).

%Ahora vamos con cuadradoMagico(+N,-XS)
cuadradoMagico(N,XS) :- 
    length(XS,N),desde(0,S), filasQueSuman(S,N,XS),
    trasponer(XS,T), filasQueSuman(S,N,T).


cabezasDeLista([],[],[]).
cabezasDeLista([[X|XS]|XSS],[X|YS],[XS|Resto]) :- 
    cabezasDeLista(XSS,YS,Resto).


trasponer([],[]).
trasponer([[]|_],[]).
trasponer(M,[Y|YS]) :- 
    cabezasDeLista(M,Y,RestoMatriz),
    trasponer(RestoMatriz,YS).


%Ejercicio 15 

esTriangulo(tri(A,B,C)) :- 
        A < B + C, B < A + C, C < A + B,
        A > abs(B-C), B > abs(A -C), C > abs(A-B). 

generarTriplasQueSuman(A,B,C,P) :- between(1,P,A), between(1,P,B), C is P - A - B, C > 0.

perimetro(tri(A,B,C),P) :- ground(tri(A,B,C)),esTriangulo(tri(A,B,C)), P is A + B + C.
perimetro(tri(A,B,C),P) :- var(A), var(B), var(C), desdeReversible(1,P),generarTriplasQueSuman(A,B,C,P),esTriangulo(tri(A,B,C)).


triangulo(T) :- perimetro(T,_).

%Ejercicio 16 negacion por falla y cut

frutal(frutilla).
frutal(banana).
frutal(manzana).
cremoso(banana).
cremoso(americana).
cremoso(frutilla).
cremoso(dulceDeLeche).

leGusta(X) :- frutal(X), cremoso(X).
cucurucho(X,Y) :- leGusta(X), leGusta(Y).
/*
Consulta inicial:
?- cucurucho(X,Y).

Nivel 0:
cucurucho(X,Y)

Nivel 1: (se expande por la regla)
cucurucho(X,Y) :- leGusta(X), leGusta(Y).
  → [leGusta(X), leGusta(Y)]

---------------------------------------------------
Expansión de leGusta(X):

leGusta(X) :- frutal(X), cremoso(X).

  - Caso X = frutilla
    frutal(frutilla) ✓
    cremoso(frutilla) ✓
    => leGusta(frutilla) ✓

  - Caso X = banana
    frutal(banana) ✓
    cremoso(banana) ✓
    => leGusta(banana) ✓

  - Caso X = manzana
    frutal(manzana) ✓
    cremoso(manzana) ✗
    => leGusta(manzana) ✗ (falla)

---------------------------------------------------
Expansión de leGusta(Y) para cada X válido:

1) X = frutilla
   Submetas: [leGusta(Y)]

   - Y = frutilla
       frutal(frutilla) ✓
       cremoso(frutilla) ✓
       => leGusta(frutilla) ✓
       => SOLUCIÓN: (X,Y) = (frutilla, frutilla)

   - Y = banana
       frutal(banana) ✓
       cremoso(banana) ✓
       => leGusta(banana) ✓
       => SOLUCIÓN: (frutilla, banana)

   - Y = manzana
       frutal(manzana) ✓
       cremoso(manzana) ✗
       => leGusta(manzana) ✗ (falla)

2) X = banana
   Submetas: [leGusta(Y)]

   - Y = frutilla
       frutal(frutilla) ✓
       cremoso(frutilla) ✓
       => leGusta(frutilla) ✓
       => SOLUCIÓN: (banana, frutilla)

   - Y = banana
       frutal(banana) ✓
       cremoso(banana) ✓
       => leGusta(banana) ✓
       => SOLUCIÓN: (banana, banana)

   - Y = manzana
       frutal(manzana) ✓
       cremoso(manzana) ✗
       => leGusta(manzana) ✗ (falla)

---------------------------------------------------

Árbol SLD resumido:

cucurucho(X,Y)
 └── leGusta(X), leGusta(Y)
       ├── X = frutilla
       │     ├── Y = frutilla → ✓
       │     ├── Y = banana   → ✓
       │     └── Y = manzana  → ✗
       │
       ├── X = banana
       │     ├── Y = frutilla → ✓
       │     ├── Y = banana   → ✓
       │     └── Y = manzana  → ✗
       │
       └── X = manzana
             └── cremoso(manzana) ✗ → falla total

---------------------------------------------------
Respuestas finales (hojas de éxito):
(X,Y) = (frutilla, frutilla)
(X,Y) = (frutilla, banana)
(X,Y) = (banana, frutilla)
(X,Y) = (banana, banana)
*/

%Ejercicio 17 

%Ejemplo 

p(a).
p(b).
q(b).


/*

i. Sean los predicados P(?X) y Q(?X), ¾qué signica la respuesta a la siguiente consulta?
?- P(Y), not(Q(Y)).

Procurá encontrar alguna instancia de Y tal que P(Y) sea verdadera y Q(Y) no pueda demostrarse verdadera en Prolog

ii. ¾Qué pasaría si se invirtiera el orden de los literales en la consulta anterior?

si la hago al rezve es decir not(Q(Y)), P(Y) esa consutla reduce a Q(Y),!,fail,P(Y), tenemos dos casos 
si si existe un Y que haga que se cumple Q(Y), entonces falla automatimante, porque el predicado fail falla siempre 
si no esta definido Q en la base de conocimiento nos va a lanza un error, por lo tanto estamos perdiendo soluciones en el caso que deberia funcionar

iii. Sea el predicado P(?X), ¾Cómo se puede usar el not para determinar si existe una única Y tal que P(?Y)
es verdadero?

P(Y), not((P(X),X \= Y))

*/

%Ejercicio 18
corte(L,L1,L2) :- append(L1,L2,L).

hayCorteMasParejo(L,L1,L2) :- corte(L3,L4,L), L1 \= L3, L2 \=L4, length(L1,Len1),length(L2,Len2),length(L3,Len3),length(L4,Len4),
                abs(Len4 - Len3) < abs(Len2 - Len1).

corteMasParejo(L,L1,L2) :- corte(L,L1,L2), not(hayCorteMasParejo(L,L1,L2)).

%Ejercicio 19 

/* 
Dado un predicado unario P(?X) sobre números naturales, defnir un predicado que determine el mínimo X que
satisfaga P(X). Se puede suponer que hay una cantidad finita de naturales que lo satisfacen.
desdeReversible(0,X), P(X), not((between(1,X,Y), Y \= X, P(Y))).
P(X) 
*/

%Ejercicio 20 
%ejercicio principal
%proximoNumeroPoderoso(+X,-Y) 
proximoNumeroPoderoso(X,Y) :- X1 is X + 1, desde(X1,Y), esNumeroPoderoso(Y),!.


%esNumeroPoderoso(+X)
esNumeroPoderoso(X) :-
    \+ ( divisoresPrimosDeX(X,P),
         P2 is P*P,
         X mod P2 =\= 0 ).
%esPrimo(+X)
esPrimo(X) :- 
    X > 1,
    \+ (X1 is X-1, between(2,X1,D), X mod D =:= 0).

%divisoresPrimosDeX(+X,?P)
divisoresPrimosDeX(X,P) :- 
    between(2,X,P),
    esPrimo(P),
    X mod P =:= 0.

%Ejercicio 21

perteneceConj(Elemento, [Elemento]).
perteneceConj(Elemento,[_|XS]) :- pertenece(Elemento,XS).

%Definir el predicado conjuntoDeNaturales(+X) debido a que el not no sirve para instancia por lo tanto falla y se cuelga 

conjuntoDeNaturales(X) :- not((perteneceConj(E,X),not(natural(E)))).

/*
Indicar el error en la siguiente denición alternativa, justifcando por qué no funciona correctamente:
conjuntoDeNaturalesMalo(X) :- not( (not(natural(E)), pertenece(E,X)) ).
como not no sirve para instanciar una variable not(natural(E)) siempre falla por lo tanto el predicado siempre falla
*/

%Ejercicio 22 
%Asumimos que la implementacion del grafo es un lista de aristas 

%predicado caminoSimple(+G,+I,+F,-L)
caminoSimple(G,I,F,L) :- 
    caminoAux(G,I,F,[I],LR),
    reverse(LR,L).

%caso base
caminoAux(_,I,I,Visitados,Visitados).
 %caso recursivo
 caminoAux([(I,Y)|GS],I,F,Visitados,L) :- 
    not(member(Y,Visitados)), 
    caminoAux(GS,Y,F,[Y|Visitados],L).

caminoAux([(J,_)|GS],I,F,Visitados,L) :- 
        J \= I, caminoAux(GS,I,F,Visitados,L).

grafo1([(a,b), (b,c), (c,d)]).

%caminoHalmiltoniano(+G,?L)

caminoHamiltoniano(G, L) :-
    listaDeNodos(G,[] ,LS),
    cantNodos(G,[] ,N),
    member(D, LS),          % nodo inicial
    member(H, LS),          % nodo final
    caminoSimple(G, D, H, L),
    length(L, N).           % la longitud debe ser igual al número de nodos

listaDeNodos([],_,[]).
listaDeNodos([(X,Y)|Gs],Visitados,[X|L]) :-
    member(Y,Visitados), not(member(X,Visitados)), listaDeNodos(Gs,[X|Visitados],L).
listaDeNodos([(X,Y)|Gs],Visitados,[Y|L]) :- 
    member(X,Visitados),not(member(Y,Visitados)),listaDeNodos(Gs,[Y|Visitados],L).
listaDeNodos([(X,Y)|Gs],Visitados,[X,Y|L]) :- 
    not(member(X,Visitados)),not(member(Y,Visitados)), listaDeNodos(Gs,[X,Y|Visitados],L).

cantNodos([],_,0).
cantNodos([(X,Y)|Gs],Visitados,N) :- 
    member(X,Visitados), not(member(Y,Visitados)), cantNodos(Gs,[Y|Visitados],N1), N is N1 +1.
cantNodos([(X,Y)|GS],Visitados,N) :-
    member(Y,Visitados),not(member(X,Visitados)), cantNodos(GS,[X|Visitados],N1), N is N1 + 1.
cantNodos([(X,Y)|GS],Visitados,N) :-
    member(X,Visitados),member(Y,Visitados),cantNodos(GS,Visitados,N).
cantNodos([(X,Y)|Gs],Visitados,N) :- 
    not(member(X,Visitados)),not(member(Y,Visitados)),
    cantNodos(Gs,[X,Y|Visitados],N1), N is N1 + 2.

%esConexo()

esConexo(G) :- not((listaDeNodos(G,[],L), member(I,L),member(D,L), I \= D, not(caminoSimple(G,I,D,_)))).

%esEstrella 
esEstrella(G) :- esConexo(G), listaDeNodos(G,[],L), member(N,L), esNodoComun(G,N).

esNodoComun([],_).
esNodoComun([(N,_)|GS],N) :- esNodoComun(GS,N).
esNodoComun([(_,N)|GS],N) :- esNodoComun(GS,N).

%Ejercicio 23 

%predicado auxiliar
generarNnodo(0,nil).
generarNnodo(N,bin(I,_,D)) :- N > 0,
    K is N - 1, between(0,K,NI), ND is K - NI, generarNnodo(NI,I),generarNnodo(ND,D).

%predicado princiapl 
arbol(A) :- desde(0,N),generarNnodo(N,A).

%predicado nodosEn(?A,+?)

nodosEn(nil,_).
nodosEn(bin(I,R,D),L) :- member(R,L), nodosEn(I,L),nodosEn(D,L).

%predicado sinRepEn(-A,+L)

sinRepEn(nil,_).
sinRepEn(bin(I,R,D),L) :-  member(R,L), append(L1,[R|L2],L), sinRepEn(I,L1),sinRepEn(D,L2).
