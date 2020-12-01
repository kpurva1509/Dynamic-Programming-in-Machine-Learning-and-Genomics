#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <sstream>

using std::string;
using std::vector;
using std::map;
using std::cin;
using std::cout;
using std::endl;

/*
 * Computes the optimal path through the HMM given an emitted string
 *
 * \param x             a string emitted string
 * \param sigma         a vector of chars containing the HMM alphabet
 * \param states        a vector of chars containing all possible states
 * \param transition    a map s.t. transition[x][y] is the probability
 *                      of transitioning from state x to state y. 
 *                      x and y are characters in states.
 * \param emission      a map s.t. emission[x][y] is the probability
 *                      of emitting y from state x. x and y are characters.
 *                      x is in states, y is in sigma.
 *
 * \return a string representing the optimal decoded path
 */

string optimal_path(const string& x, const vector<char>& sigma, vector<char>& states, 
      map<char,map<char,double> >& transition, map<char,map<char,double> >& emission) {

  std::map<char, std::map<int, double>> forward_matrix;
  std::map<char, std::map<int, char>> backpointer;
  std::string backtrack;

  /* Initial step for the firsrt column */  
  // char initial_max;
  // double initial_prob = 0.0;
  for(char state : states) {
    forward_matrix[state][0] = 0.5*emission[state][x[0]];
    backpointer[state][0] = '0';
    /*if(forward_matrix[state][0] > initial_prob) {
      initial_prob = forward_matrix[state][0];
      initial_max = state;
    }*/
    // backtrack += initial_max;
  }
  // backtrack += initial_max;

  /* Recurring steps for the next (n-1) columns */
  for(int i=1; i< x.length(); i++) {

    char current_max;
    double current_prob = 0.0;

    /* SETTING THE FORWARD MATRIX OF EACH STATE */
    for(char each_state : states) {
      
      /* set the forward matrix of each state */ 
      /* set the backpointer of each state */
      forward_matrix[each_state][i] = 0.0;
      for(char tracking_state : states) {
        auto temp = forward_matrix[tracking_state][i-1]*transition[tracking_state][each_state]*emission[each_state][x[i]];
        if(temp > forward_matrix[each_state][i]) {
          forward_matrix[each_state][i] = temp;
          backpointer[each_state][i] = tracking_state;
        }
      }
    }

    /* iterate through the ith column of the matrix to find max */
    /*for(char each_state : states) {
      if(forward_matrix[each_state][i] > current_prob) {
        current_prob = forward_matrix[each_state][i];
        current_max = each_state;
      }
    }*/
      // backtrack += current_max; NO WAY!!!
  // }
  //backtrack += current_max;
  }

  double best_path_prob = 0.0;
  char first_to_backtrack;
  for(char state : states) {
    if(forward_matrix[state][x.length()-1] > best_path_prob){
      best_path_prob = forward_matrix[state][x.length()-1];
      first_to_backtrack = state;
    }
  }

  backtrack += first_to_backtrack;
  for(int i=x.length()-1; i>0; i--) {
    char book_keeping = backpointer[first_to_backtrack][i];
    backtrack += book_keeping;
    first_to_backtrack = book_keeping;
  }

  std::reverse(backtrack.begin(), backtrack.end());

  return backtrack;
}

double outcome_likelihood( string& x, vector<char>& sigma, vector<char>& states, 
          map<char,map<char, double>>& transition, map<char,map<char,double>>& emission) {

  std::map<char, std::map<int, double>> forward_matrix;
  double maximum_likelihood_path = 0.0;

  /* Initial step for the firsrt column */  
  for(char state : states) {
    forward_matrix[state][0] = (double)emission[state][x[0]]/states.size();
  }
  // backtrack += initial_max;

  /* Recurring steps for the next (n-1) columns */
  for(int i=1; i< x.length(); i++) {
    for(char each_state : states) {
      forward_matrix[each_state][i] = 0.0;
      for(char tracking_state : states) {
        forward_matrix[each_state][i] += forward_matrix[tracking_state][i-1]*transition[tracking_state][each_state]*emission[each_state][x[i]];
      }
    }
  }

  for(char state : states){
    maximum_likelihood_path += forward_matrix[state][x.length()-1];
  }

  return maximum_likelihood_path;
}

int main(int argc, char* argv[]) {
    string x;
    vector<char> sigma, states;
    map<char,map<char,double> > emission, transition;

    // read in x
    cin >> x;
    cin.ignore(1, '\n'); // last newline
    cin.ignore(100, '\n'); // delimiter

    // read in sigma
    char letter;
    cin >> letter;
    while (letter != '-') {
        sigma.push_back(letter);
        cin >> letter;
    }
    cin.ignore(100, '\n'); // delimiter

    // read in states
    char state;
    cin >> state;
    while (state != '-') {
        states.push_back(state);
        cin >> state;
    }

    cin.ignore(100, '\n'); // delimiter
    cin.ignore(100, '\n'); // first line of transition matrix
    char curr_state;
    double curr_prob;

    // read in transition matrix
    for (char state : states) {
        cin >> curr_state;
        for (char other_state : states) {
            cin >> curr_prob;
            transition[curr_state][other_state] = curr_prob;
        }
    }
    
    cin.ignore(1, '\n'); // transition last newline
    cin.ignore(100, '\n'); // delimiter
    cin.ignore(100, '\n'); // first line of emission matrix

    // read in emission matrix
    for (char state : states) {
        cin >> curr_state;
        for (char letter : sigma) {
            cin >> curr_prob;
            emission[curr_state][letter] = curr_prob;
        }
    }

    cout << "\n--------------------------\n";

    cout << "Viterbi Decoded Path:\n";
    cout << optimal_path(x, sigma, states, transition, emission) << endl;
    cout << "Maximum-Likelihood Estimate: \n";
    cout << outcome_likelihood(x, sigma, states, transition, emission) << endl;

    return 0;
}
