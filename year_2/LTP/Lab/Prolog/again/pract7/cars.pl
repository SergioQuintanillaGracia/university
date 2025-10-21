model('ibiza', 'seat'). % ibiza es un modelo de la marca seat
model('cordoba', 'seat'). 
model('altea', 'seat').
model('golf', 'volkswagen').
model('touran', 'volkswagen').
model('clio', 'renault').
model('twingo', 'renault').
model('megane', 'renault').
model('scenic', 'renault').
model('2008', 'peugeot').
model('3008', 'peugeot').
model('corsa', 'opel').

country('seat', 'espaÃ±a'). % seat es una marca fabricada en espaÃ±a
country('renault', 'francia').
country('peugeot', 'francia').
country('volkswagen', 'alemania').
country('opel', 'alemania').

since('ibiza', 1984). % ibiza es un modelo fabricado desde 1984
since('cordoba', 1993).
since('altea', 2004).
since('golf', 1974).
since('touran', 2003).
since('clio', 1990).
since('twingo', 1993).
since('megane', 1995).
since('scenic', 1995).
since('2008', 2013).
since('3008', 2008).
since('corsa', 1982).

segment('ibiza', 'b'). % ibiza es un modelo del Segmento B
segment('cordoba', 'b').
segment('altea', 'c'). 
segment('golf', 'c').
segment('touran', 'c').
segment('clio', 'b').
segment('twingo', 'a').
segment('megane', 'c').
segment('scenic', 'c').
segment('2008', 'b').
segment('3008', 'c').
segment('corsa', 'b').

% A es marca de B si B es modelo de A
brand(A,B) :- model(B,A).

% A es modelo del pais B si (A es modelo de C) && (C es del pais B)
isModelFrom(A,B) :- model(A,C), country(C,B).

% A, B son modelos de la misma marca C si ...
isSameBrand(A,B) :- model(A,C), model(B,C), A \== B.        

% A, B son modelos del mismo aÃ±o si ...
isSameYear(A,B) :- since(A,C), since(B,C), A \== B.

% A, B estan relacionados si ...
isRelated(A,B) :- isSameBrand(A,B).
isRelated(A,B) :- isSameYear(A,B).
isRelated(A,B) :- segment(A,C), segment(B,C), A \== B.

% Exercise 8
isRelated(A,B) :- isClassic(A), isClassic(B).

% Exercise 7
isCountryOf(A,B) :- isModelFrom(B,A).
isClassic(A) :- since(A,Y), Y < 1995.