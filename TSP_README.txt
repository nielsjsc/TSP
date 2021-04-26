Traveling salesperson problem

Niels Christoffersen



code design:

the challenging part about this homework was having to read in 
integers one character at a time, and dealing with all of the 
other ascii characters that got in the way. I tried to use as 
many helper functions as I could to make it easier


functions:

operator>>:
probably the most difficult function to write,takes an istream
as input and reads the coordinate pairs into the cities object's 
vector of coordinates. 

operator<<:

outputs the vector of coordinate pairs from an object to a stream
It puts one space after the first number of a pair, then a line 
break after the second.


total_path_distance:

takes a permutation of the cities and computes the distance
between the cities. It uses the vector of points from the class
and simply calls the index from the permutation's vector on the 
vector of coordinate points. It returns a double for the distance

reorder:

This was the most confusing function, as it reorders a vector of
coordinate points and returns a new city object. I never used this
function in my code, likely because i never returned the shortest path
for the shortest distance.


random_permutation:

I used the srand(time) and std::random_shuffle to randomize the given 
permutation, I had some trouble getting it working but it worked fine.




Building:

there is nothing to link for this file as the cities.hh file is included in 
tsp.cc.

tsp.cc takes a command line argument for a file with the .tsp formatting, 
and will create a new file with the shortest lengths for the given points.