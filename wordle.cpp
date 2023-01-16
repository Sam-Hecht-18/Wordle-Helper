//wordle.cpp
//Author: Sam Hecht
//Date: 2/3/22
//Purpose: This program takes wordle guesses and outputs what possible remaining
//         words may be the answer to the wordle.


#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

void populate(vector<string> &words, string filename);
void determine_guesses(vector <string> &guesses, vector <string> &feedback);
string toLower(string str);
void sort_by_commonality(vector <string> &words);
void calculate_letter_commonalities(map<char, float> &letter_commonalities);
void calculate_word_commonalities(vector<string> &words, 
                                  map<string, float> &word_commonalities,
                                  map<char, float> &letter_commonalities);
void bubble_sort(vector<string> &words, map<string, float> &word_commonalities);
void swap(vector<string> &words, int i, int j);
void factor_in_guesses(vector<string> &words, vector<string> &guesses, 
                       vector<string> &feedback);

void handle_green(vector<string> &words, char green_letter, int index);
void handle_yellow(vector<string> &words, char yellow_letter, int index);
void handle_black(vector<string> &words, char black_letter);




int main() {
    
    
    vector<string> words;
    populate(words, "words.txt");

    // vector<string> words_sorted;
    // populate(words_sorted, "words_sorted.txt");

    //sort_by_commonality(words); - Unnecessary - now stored in a file

    vector<string> guesses;
    vector<string> feedback;

    determine_guesses(guesses, feedback);

    factor_in_guesses(words, guesses, feedback);
    cout << "Remaining words: " << endl;
    for (size_t i = 0; i < words.size(); i++) {
        cout << i+1 << ": " << words[i] << endl;
    }

    // factor_in_guesses(words_sorted, guesses, feedback);
    // cout << "Remaining words sorted: " << endl;
    // for (size_t i = 0; i < words_sorted.size(); i++) {
    //     cout << i+1 << ": " << words_sorted[i] << endl;
    // }

}


void factor_in_guesses(vector<string> &words, vector<string> &guesses, 
                       vector<string> &feedback) {

    for(size_t i = 0; i < guesses.size(); i++) {
        for(size_t j = 0; j < guesses[i].size(); j++) {
            if(feedback[i][j] == 'g') {
                handle_green(words, guesses[i][j], j);
            }
            else if(feedback[i][j] == 'y') {
                handle_yellow(words, guesses[i][j], j);
            }
            else {
                handle_black(words, guesses[i][j]);
            }
        }
    }
}

void handle_green(vector<string> &words, char green_letter, int index) {
    for(int i = words.size() - 1; i >= 0; i--) {
        if(words[i][index] != green_letter) {
            string word_to_erase = words[i];
            words.erase(words.begin() + i);
        }
    }
}

void handle_yellow(vector<string> &words, char yellow_letter, int index) {
    for(int i = words.size() - 1; i >= 0; i--) {
        if(words[i][index] == yellow_letter or 
           words[i].find(yellow_letter) == string::npos) {

            words.erase(words.begin() + i);
        }
    }
}

void handle_black(vector<string> &words, char black_letter) {
    for(int i = words.size() - 1; i >= 0; i--) {
        if(words[i].find(black_letter) != string::npos) {
            words.erase(words.begin() + i);
        }
    }
}

void populate(vector<string> &words, string filename) {

    ifstream input;

    input.open(filename);
    if(input.fail()) {
        cerr << "Failed to open file with name: " << filename << endl;
        exit(1);
    }

    int num_words;
    input >> num_words;
    string word;
    for (int i = 0; i < num_words; i++) {
        input >> word;
        words.push_back(word);
    }
    words.shrink_to_fit();
    input.close();
}

void determine_guesses(vector <string> &guesses, vector <string> &feedback) {
    cout << "Enter number of guesses: ";
    int num_guesses;

    cin >> num_guesses;
    for(int i = 0; i < num_guesses; i++) {
        string guess, feedback_str;
        cout << "Enter guess " << (i+1) << ": " ;
        cin >> guess;
        guesses.push_back(toLower(guess));
        cout << "Enter feedback (g = green, y = yellow, b = black): ";
        cin >> feedback_str;
        feedback.push_back(toLower(feedback_str));

    }
}

string toLower(string str) {
    for (size_t i = 0; i < str.size(); i++) {
        if(str[i] >= 'A' and str[i] <= 'Z') {
            str[i] = ('a' - 'A') + str[i];
        }
    }
    return str;
}


void sort_by_commonality(vector<string> &words) {
    map<char, float> letter_commonalities;
    

    calculate_letter_commonalities(letter_commonalities);

    map<string, float> word_commonalities;
    calculate_word_commonalities(words, word_commonalities, 
                                        letter_commonalities);
                
    
    bubble_sort(words, word_commonalities);
    // for (size_t i = 0; i < words.size(); i++) {
    //     cout << words[i] << endl; //<< ": " << word_commonalities[words[i]] << endl;
    // }
}

void calculate_word_commonalities(vector<string> &words, 
                                  map<string, float> &word_commonalities,
                                  map<char, float> &letter_commonalities) {
    for(size_t i = 0; i < words.size(); i++) {
        float word_commonality = 0;
        for(size_t j = 0; j < words[i].size(); j++) {
            word_commonality += letter_commonalities[words[i][j]];
        }
        word_commonalities[words[i]] = word_commonality;
    }
}


void calculate_letter_commonalities(map<char, float> &letter_commonalities) {
    ifstream input;
    string filename = "letter_commonalities.txt";
    input.open(filename);
    if(input.fail()) {
        cerr << "Failed to open file with name: " << filename << endl;
        exit(1);
    }
    int numItems;
    input >> numItems;
    char letter;
    float frequency;
    for (int i = 0; i < numItems; i++) {
        input >> letter >> frequency;
        letter_commonalities[letter] = frequency;
    }
    
}

void bubble_sort(vector<string> &words, map<string, float> &word_commonalities)
{
    size_t size = words.size();
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size - i - 1; j++) {
            if(word_commonalities[words[j]] < word_commonalities[words[j + 1]]) 
            {
                swap(words, j, j+1);
            }
        }
    }
}

void swap(vector<string> &words, int i, int j) {
    string temp = words[i];
    words[i] = words[j];
    words[j] = temp;
}
