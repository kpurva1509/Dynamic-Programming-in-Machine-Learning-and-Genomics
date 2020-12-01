#include "headers.hpp"

#define MAX_VALUE 32767

/* Function returns a pair of int(# of coins), and a vector of int
 * which is the set of coins required for the given amount 
 * 
 */
int change_coins(int money, vector<int> coins) {
    
    /* We maintain a vector for the minimum # of coins
     * and initialize all the values(except 0) to MAX_VALUE
     * The value of each entry in this vector is the least
     * # of coins required to represent the amount
     */
    std::vector<int> minimum_coins(money+1);
    minimum_coins[0] = 0;
    std::fill(minimum_coins.begin()+1, minimum_coins.end(), MAX_VALUE);

    /* Begin the iterator from the 2nd index, i.e 
     * 1 instead of 0, and iterate for every value
     * of money until max money is reached
     */
    for(auto amount = 1; amount < minimum_coins.size(); amount++) {

        /* For every amount of money, iterate for each
         * denomination available
         */
        for(auto coin : coins){ /* for(std::vector<int>::iterator it = coins.begin(); it!=coins.end(); it++) */
            if(amount > coin) {
                if((minimum_coins[amount-coin]+1) < minimum_coins[amount]){
                    minimum_coins[amount] = minimum_coins[amount-coin]+1;
                }
            }
        }
    }

    return minimum_coins[money];
}

int main() {
    int money;
    cout << "Enter the amount to be changed : ";
    cin >> money;

    vector<int> coins;
    int coin;

    cout << "Enter the denominations available : ";
    while (cin >> coin) {
        coins.push_back(coin);

        cin.ignore(4,',');
    }

    cout << change_coins(money, coins) << endl;

    return 0;
}
