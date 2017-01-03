/*
	Author: Kai Li
	Date: Jan 3, 2017
	Description: This simple C++ program is used to 
	read words (Delimit by white space) from user 
	input and output to console the count of words 
	that start with an English vowel.
*/
#include <iostream>
#include <string>

int main() {
    const std::string Vowels = "AEIOUaeiou";    
    int count = 0;
    std::string word;
    while (std::cin >> word) {
        if (Vowels.find(word.at(0)) != std::string::npos) {
            count++;
        }
    }
    std::cout << count << std::endl;
    return 0;
}

// End of source code
