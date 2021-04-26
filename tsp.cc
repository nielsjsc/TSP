#include "cities.hh"
#include <random>
#include <fstream>
#include <iostream>
#include <map>





bool isEmpty(char c) {//checking to see if the char is an empty one, aka space, tab, eol, etc..
	switch (c) {
	case ' ':
	case '\t':
	case '\n':
	case '\b':
		return true;
	}
	return false;
}

bool isNumber(char c) {//if the input is a number returns true, just another way to make sure the file has correct inputs
	return  (c >= '0' && c <= '9');
}

int getNextNum(std::istream& is, bool &success) {
	// eat spaces
	char curr_val;

    while (!isNumber(curr_val = is.get()) && curr_val != -1 && curr_val != '-');
	if (curr_val == -1) {
		success = false;//checks to see if the eof char was reached for any reason
		return 0;
	}

	// read int
	bool isNeg = curr_val == '-';
	int val = 0;
	do {
		val *= 10;
		val += curr_val - '0';
	} while (isNumber(curr_val = is.get()) && curr_val != -1);

	if (curr_val < 0) {
		success = false;
		return -1;
	}

	success = true;
	return val;//returns the next integer it finds
}


std::istream& operator >> (std::istream& is, Cities& city) {

	do {
		bool success;
		int val0 = getNextNum(is, success);//get the next two numbers, if eof char, break loop
		if (!success) break;

		int val1 = getNextNum(is, success);
		if (!success) break;

		city.coord_vect.push_back(Cities::coord_t(val0, val1));//add the new point to the city's vector of coordinates
	} while (true);

	return is;
}



std::ostream& operator<< (std::ostream& os, Cities& city) {
	long unsigned int count = 0;
	while (count < city.coord_vect.size()) {
		Cities::coord_t curr_pair = city.coord_vect[count];//takes the current coordinate pair from the object's vector for outputting
		char curr_int[20] = {};//20 char buffer for any number
		_itoa_s(curr_pair.first, curr_int, 10);//convert int to string
		int char_count=0;
		while (curr_int[char_count]!=0)
		{
			os.put(curr_int[char_count]);//iterates through string and outputs chars
			char_count++;
		}
		
		os.put(' ');
		_itoa_s(curr_pair.second, curr_int, 10);
		char_count = 0;
		while (curr_int[char_count] != 0)//same for second number
		{
			os.put(curr_int[char_count]);
			char_count++;
		}
		os.put('\n');//line break
		count += 1;

	}


	return os;
}






double Cities::total_path_distance(const Cities::permutation_t& ordering) const {
	int i = 0;
	Cities::coord_t curr_coord;
	Cities::coord_t prev_coord;
	Cities::coord_t first_coord;//initializing vars
	double tot_dist = 0;
	double curr_dist = 0;
	while (i < ordering.size()) {
		curr_coord = coord_vect[ordering[i]];//takes the current coord pair
		if (i == 0) {
			prev_coord = coord_vect[ordering[0]];
			first_coord = coord_vect[ordering[0]];
			i++;
			continue;
		}
		curr_dist = sqrt(pow(prev_coord.first - curr_coord.first, 2) + pow(prev_coord.second - curr_coord.second, 2));//compares the dist from the prev pair to the curr
		tot_dist += curr_dist;
		prev_coord = curr_coord;//adds the distance to total, sets prev pair to curr
		i++;
	}
	curr_dist = sqrt(pow(first_coord.first - curr_coord.first, 2) + pow(first_coord.second - curr_coord.second, 2));//adds the distance from first to last
	return tot_dist+curr_dist;
}



Cities Cities::reorder(const permutation_t& ordering) const {//simply takes a permutation and makes a new vector of cities with that order, then returns the object
	int i = 0;
	Cities new_city;
	while (i<ordering.size())
	{
		new_city.coord_vect.push_back(coord_vect[ordering[i]]);
		i++;
	}
	return new_city;
}


Cities::permutation_t Cities::random_permutation(unsigned len) {//creates a random vector of permutations from 0-len-1
	static bool isSRandInit = false;

	if (isSRandInit == false) {
		std::srand(std::time(0));//only sets the random time once
		isSRandInit = true;
	}

	permutation_t out_vect;
	for (int i=0; i < len; i++)out_vect.push_back(i);
	std::random_shuffle(out_vect.begin(), out_vect.end());//randomly shuffles the full vector.
	return out_vect;
}

std::vector<std::pair<double, unsigned int>> rand_perm(unsigned int len, Cities city) {//takes a city and the number of iterations and randomly searches for the lowest distance
	unsigned int size = city.coord_vect.size();
	double low_dist=0;
	std::vector<std::pair<double, unsigned int>> out_vec;
	for (int i = 0; i < len; i++) {
		Cities::permutation_t curr_perm =city.random_permutation(size);
		double curr_dist= city.total_path_distance(curr_perm);
		if (curr_dist < low_dist || low_dist == 0) {//if the current distance is lower than the low, set the lowest to curr.
			low_dist = curr_dist;
			out_vec.push_back(std::pair<double, unsigned int>(low_dist,i));
		}
	}
	return out_vec;//returns the iterations of lowest distances
}
void print_map(std::vector<std::pair<double, unsigned int>> vec, std::string filename) {//outputs the lowest distances with the iteration count to a new file
	std::ofstream outfile(filename + ".fast");
	int i = 0;
	Cities city;
	
	while (i < vec.size()) {
		std::pair<double, unsigned int> curr_pair = vec[i];
		city.coord_vect.push_back(curr_pair) ;
		i++;
	}
	outfile << city;
}
int main(int argc, char* argv[]) {
	if (argc < 2)return 0;
	std::string filename = argv[1];
	Cities city;
	std::ifstream f;
	f.open(filename);
	f >> city;
	std::vector<std::pair<double, unsigned int>> rand_vec = rand_perm(100000, city);//currently set to run 100000 times, can be changed
	print_map(rand_vec,  filename);
	return 0;

}