% movie(M, Y), M is a movie released in the year Y
movie(barton_fink, 1991).
movie(the_big_lebowski, 1998).
movie(fargo, 1996).
movie(lick_the_star, 1998).
movie(mission_impossible, 1996).
movie(fall, 1997).
% director(M, D), M is a movie directed by D
director(the_big_lebowski, joel_coen).
director(barton_fink, ethan_coen).
director(fargo, joel_coen).
director(lick_the_star, sofia_coppola).
director(mission_impossible, brian_de_palma).
% actor(M, A, R), the actor A played the role of R in the movie M
actor(mission_impossible, tom_cruise, ethan_hunt).
actor(mission_impossible, jon_voight, jim_phelps).
actor(barton_fink, john_turturro, barton_fink).
actor(barton_fink, john_goodman, charlie_meadows).
actor(the_big_lebowski, jeff_bridges, jeffrey_lebowski__the_dude).
actor(the_big_lebowski, john_goodman, walter_sobchak).
actor(the_big_lebowski, philip_seymour_hoffman, brandt).
actor(the_big_lebowski, john_turturro, jesus_quintana).

moviesTwoActors(M, A1, A2) :- actor(M, A1, _), actor(M, A2, _), A1 \== A2.

moviesYear(M1, M2, Y) :- movie(M1, Y), movie(M2, Y), M1 \== M2.