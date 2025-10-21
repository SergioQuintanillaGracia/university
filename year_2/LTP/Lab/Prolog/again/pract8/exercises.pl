% Exercise 4
mymember(E,[E|_]).
mymember(E,[_|L]) :- mymember(E,L).

% Exercise 6
myappend([],L,L).
myappend([E|L1],L2,[X|L3]) :- X = E, myappend(L1,L2,L3)