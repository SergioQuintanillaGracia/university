% aplanar lista
flatten([],[]).
flatten([X|L],[X|P]) :- atomic(X), flatten(L,P).
flatten([X|L],P) :- not(atomic(X)), flatten(X,P_X), flatten(L,P_L), append(P_X,P_L,P).        

% prefijo, sufijo, sublista
prefix(P,L) :- append(P,_,L).
suffix(P,L) :- append(_,P,L).
sublist(S,L) :- suffix(L1,L),prefix(S,L1).

% Another version of sublist:
sublist2(S,L) :- append(_,L1,L),append(S,_,L1).

% invertir lista
inverse([],[]).
inverse([H|T],L) :- inverse(T,Z), append(Z,[H],L).

% Another version of inverse:
inverse2(L,I) :- inv(L,[],I).
inv([],I,I).
inv([X|L],A,I) :- inv(L,[X|A],I).
