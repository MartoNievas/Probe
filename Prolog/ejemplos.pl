parPositivo(X,Y) :- mayor(X, 0), mayor(Y, 0).
natural(0).
natural(succ(N)) :- natural(N).
mayor(succ(X),0) :- natural(X).
mayor(succ(X),succ(Y)) :- mayor(X,Y).


desde(X,X).
desde(X,Y) :- N is X + 1, desde(N,Y).


arbolDeTamanoN(0,x).
arbolDeTamanoN(0,[]).
arbolDeTamanoN(N,A) :- N > 0,N1 is N-1 ,between(0,N1,MitadI), MitadD is N1 - MitadI, arbolDeTamanoN(MitadI,AI),arbolDeTamanoN(MitadD,AD),
        append(AI,[AD],A).

arbolGeneral(A) :- desde(0,N),arbolDeTamanoN(N,A).

%%
variablesLibres(M,L) :- variablesLibresAux(M,[],L).


variablesLibresAux(var(X),Ligadas,[X]) :- not(member(X,Ligadas)).
variablesLibresAux(var(X),Ligadas,[]) :- member(X,Ligadas).
variablesLibresAux(lam(X,M),Ligadas,L) :- variablesLibresAux(M,[X|Ligadas],L).
variablesLibresAux(app(M1,M2),Ligadas,L) :- variablesLibresAux(M1,Ligadas,L1),variablesLibresAux(M2,Ligadas,L2), append(L1,L2,L).

tamano(var(_),1).
tamano(lam(_,M),N) :- tamano(M,TamM), N is 1 + TamM.
tamano(app(M1,M2),N) :- tamano(M1,TamM1), tamano(M2,TamM2), N is 1 + TamM1 + TamM2.



generarLambdaTerminosDeTamanoN(var(X),1,L) :- member(X,L).
generarLambdaTerminosDeTamanoN(lam(X,M),N,L) :- N > 1, N1 is N -1, member(X,L),generarLambdaTerminosDeTamanoN(M,N1,L).
generarLambdaTerminosDeTamanoN(app(M1,M2),N,L) :- N > 1, N1 is N - 1, between(1,N1,NM1), NM2 is N1 - NM1, 
            generarLambdaTerminosDeTamanoN(M1,NM1,L),generarLambdaTerminosDeTamanoN(M2,NM2,L).

generarLambdaTerminos(XS,M) :- desde(1,N),generarLambdaTerminosDeTamanoN(M,N,XS).


unico(L,U) :- select(U,L,Resto), not(member(U,Resto)).

sinRepetidos([]).
sinRepetidos([X|XS]) :- unico([X|XS],X), sinRepetidos(XS).


generarFormulasDeTamanoN(A,1,L) :- member(A,L).
generarFormulasDeTamanoN(not(A),1,L) :- member(A,L).
generarFormulasDeTamanoN(imp(A,B),N,L) :- N > 1, N1 is N - 1, between(1,N1,NA), NB is N1 - NA, 
            generarFormulasDeTamanoN(A,NA,L),generarFormulasDeTamanoN(B,NB,L).


formulas(A,L) :- desde(1,N),generarFormulasDeTamanoN(A,N,L).

