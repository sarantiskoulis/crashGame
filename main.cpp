#include <iostream>
#include <random>
#include<vector>
#include<chrono>
#include <algorithm>
#include <numeric>
#include <future>
#include <thread>
#include <set>
#include <fstream>
#include <cmath>
#include <utility>

using std::accumulate;
using std::cout;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;

using namespace std;

std::pair<double, double> GetStats(const std::vector<double>& wins, double sum_wins, long long n, double default_bet) {


    double sum_x_sq = 0;
    for (const auto& win : wins) {
        sum_x_sq += pow(static_cast<double>(win) / default_bet, 2) ;
    }

    // Ensure the multiplication does not overflow
    long double sum_wins_sq = pow( sum_wins / default_bet, 2) ;


    double var = (1.0 / (n - 1)) * (sum_x_sq - (sum_wins_sq / static_cast<double>(n)));
    double std_dev = std::sqrt(var);

    return {var, std_dev};
}

int main() {

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0, 1);
    bool searching = true;
    double b = 1;

    // -------- SETTINGS -------- ///
    for (int target = 0; target < 8; ++target) {
        searching = true;
        while (searching) {
            long long int game_rounds = 1'000'000;
            double game_points = 0;
            double crash_chance_setting = 0.04;

            int auto_crash_count = 0;
            vector<double> wins;
            vector<double> points;
            double winSum = 0;

            for (int i = 0; i < game_rounds; ++i) {


                double x = dist(e2);
                double y = 1.0 / (1.0 - x);
                double crash_chance = dist(e2);


                if (crash_chance <= crash_chance_setting) {
                    ++auto_crash_count;
                } else {

                    if (b <= y) {
                        game_points += b;
                        wins.push_back(b);
                        winSum += b;
                    }

                }


                points.push_back(game_points);

            }
            std::pair<double, double> Stats = GetStats(wins, winSum, game_rounds, 1.0);
            double variance = Stats.first;
            double std = Stats.second;


            if (std <= (target- 0.005)) {
                b += 0.05;
            }
            else if (std >= (target +0.005)) {
                b -= 0.05;
            }
            else {
                searching = false;
                cout << "RTP: " << game_points / static_cast<double>(game_rounds) << endl;
                cout << "Variance " << variance << endl;
                cout << "Standard Dev. " << std << endl;
                cout << b << endl;

                double check = crash_chance_setting  + (1-crash_chance_setting) * (((b -1) + pow((b-1),2) )/ b);
                cout << "Check " << check << endl;


                cout << endl;


    //            std::ofstream file("C:\\Users\\sarantis\\CLionProjects\\crashGame\\distribution.csv");
    //            file << "Points\n";
    //            for (const auto& point : points) {
    //                file << point<< "\n";
    //            }
    //            file.close();
            }

            wins.clear();
            points.clear();

        }


}







    return 0;
}
