#include <iostream>
#include <fstream>
#include <random>

// simple binary search returning letter based on the array of cumulative totals
// and random number gotten by uniform distribution
// std::discrete_distribution would do the job here as well, but the best
// solution (most appropriate and fastest) would be Huffman coding
char get_letter(const int range[], int distr_val) {
    int low = 0, mid = 0, high = 26;
    while (high != low) {
        mid = (low + high) / 2;
        if (distr_val < range[mid])
            high = mid;
        else // distr_val >= range[mid]
            low = mid + 1;
    }

    // return (char)(97 + low - 1);
    return 'a' + low - 1;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Incorrect input! Usage: " << argv[0] << " <length> <filename>" << std::endl;
        return -1;
    }

    // array of frequencies (a = 0, b = 1, c = 2, ..., z = 25) expressed in % of all
    // letters used based on the sample of 40000 words
    double freq[26] = {
        8.12, 1.49, 2.71, 4.32, 12.02, 2.30, 2.03, 5.92, 7.31, 0.10, 0.69, 3.98, 2.61, 
        6.95, 7.68, 1.82, 0.11,  6.02, 6.28, 9.10, 2.88, 1.11, 2.09, 0.17, 2.11, 0.07
    };
    
    // convert the frequency table to an array of cumulative totals, so:
    // from 0 to val1 = A, from val1 + 1 to val2 = B, ..., over val25 = Z
    int range[26] = {0};
    for (int i = 1; i < 26; i++)
        range[i] = range[i - 1] + 100 * freq[i - 1];

    // for (int i = 0; i < 26; i++)
        // std::cout << char(97 + i) << " " << range[i] << std::endl;
    
    // finally create uniform distribution to generate letters and words
    std::mt19937 generator {std::random_device{}()};
    std::uniform_int_distribution uniform_distro(0, 10000);
    std::binomial_distribution    binomial_distro(11, 0.5);

    // prepare for generating text: open file and generate first word length
    std::ofstream output(argv[2]);
    int text_length = std::atoi(argv[1]);
    int word_length = binomial_distro(generator) + 1;

    for (int i = 0; i < text_length; i++) {
        if (word_length == 0) {
            output << " ";
            word_length = binomial_distro(generator) + 1;
            std::cout << word_length << " ";
        } else {
            word_length--;
        }

        output << get_letter(range, uniform_distro(generator));
    }
}

// How does the range table work?
//    idx   0       1       2       3       4 ...     24     25 
// letter   a       b       c       d       e ...      y      z
//  range   0     811     960    1231    1663 ...   9780   9991
// output   [   a   )[   b  )[   c  )[   d  ) ...   x  )[  y  )[  z  ] (10000)

// Output of exercise 4 from previous list (number 6) after generating text
// with 1000000 letters shows that it works pretty close to frequency table:
// Number of occurences of letters and their frequency in text
// a: 81227, 7.10688%
// b: 14886, 1.30244%
// c: 27350, 2.39296%
// d: 43299, 3.7884%
// e: 119775, 10.4796%
// f: 22817, 1.99635%
// g: 20227, 1.76974%
// h: 59708, 5.22409%
// i: 72831, 6.37228%
// j: 983, 0.0860066%
// k: 6809, 0.595747%
// l: 39573, 3.4624%
// m: 26326, 2.30337%
// n: 69563, 6.08635%
// o: 76721, 6.71263%
// p: 18044, 1.57874%
// q: 1088, 0.0951935%
// r: 60239, 5.27055%
// s: 62817, 5.49611%
// t: 91108, 7.97141%
// u: 28865, 2.52552%
// v: 11011, 0.963397%
// w: 20759, 1.81629%
// x: 1723, 0.150752%
// y: 21233, 1.85776%
// z: 1018, 0.0890689%