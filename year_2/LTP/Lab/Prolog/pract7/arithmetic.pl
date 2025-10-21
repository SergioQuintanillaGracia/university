factorial(1,1).
factorial(X,Y) :- X1 is X - 1, factorial(X1, Y1), Y is X * Y1.

tab(0).
tab(N) :- put_char('.'), N1 is N - 1, tab(N1).