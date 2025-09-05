parPositivo(X,Y) :- mayor(X, 0), mayor(Y, 0).
natural(0).
natural(succ(N)) :- natural(N).
mayor(succ(X),0) :- natural(X).
mayor(succ(X),succ(Y)) :- mayor(X,Y).