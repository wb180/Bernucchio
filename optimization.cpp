#include "openga.hpp"

#include "gamestate.h"
#include "evaluator.h"

#include <random>
#include <vector>
#include <fstream>
#include <sstream>

#include "optimization.h"

const int SizeOpt = 388;

class Weights
{
public:
    Weights()
    {
        std::mt19937_64 rng;
        rng.seed(std::random_device{}());
        std::uniform_int_distribution dist(-50, 50);
        std::uniform_int_distribution dist2(-10, 10);

        for(std::size_t i = 0; i < kPieceValue.size(); ++i)
            weights.push_back(kPieceValue[i] + dist(rng));

        for(std::size_t i = 0; i < kPstWhitePawns.size(); ++i)
            weights.push_back(kPstWhitePawns[i] + dist2(rng));

        for(std::size_t i = 0; i < kPstWhiteKnights.size(); ++i)
            weights.push_back(kPstWhiteKnights[i] + dist2(rng));

        for(std::size_t i = 0; i < kPstWhiteBishops.size(); ++i)
            weights.push_back(kPstWhiteBishops[i] + dist2(rng));

        for(std::size_t i = 0; i < kPstWhiteRooks.size(); ++i)
            weights.push_back(kPstWhiteRooks[i] + dist2(rng));

        for(std::size_t i = 0; i < kPstWhiteQueens.size(); ++i)
            weights.push_back(kPstWhiteQueens[i] + dist2(rng));

        for(std::size_t i = 0; i < kPstWhiteKing.size(); ++i)
            weights.push_back(kPstWhiteKing[i] + dist2(rng));
    }

    std::vector<int> &GetWeights()
    {
        return weights;
    }

    std::vector<int> GetWeights() const
    {
        return weights;
    }

    std::string ToString() const
    {
        std::stringstream ss;
        ss << std::endl <<
              "Pawn weight: " << weights[0] << std::endl <<
              "Knight weight: " << weights[1] << std::endl <<
              "Bishop weight: " << weights[2] << std::endl <<
              "Rook weight: " << weights[3] << std::endl <<
              "Queen weight: " << weights[4] << std::endl << std::endl <<
              "White pawn pst: " << std::endl;

        for(int i = 0; i < 8; ++i)
        {
            for(int j = 0; j < 8; ++j)
            {
                int weight = weights[5 + 8 * i + j];

                if(weight >= 0 && weight < 10)
                    ss << "  ";

                if(weight >= 10)
                    ss << " ";

                if(weight < 0 && weight > -10)
                    ss << " ";

                ss << weight << ", ";
            }
            ss << std::endl;
        }

        ss << "White knight pst: " << std::endl;

        for(int i = 0; i < 8; ++i)
        {
            for(int j = 0; j < 8; ++j)
            {
                int weight = weights[69 + 8 * i + j];

                if(weight >= 0 && weight < 10)
                    ss << "  ";

                if(weight >= 10)
                    ss << " ";

                if(weight < 0 && weight > -10)
                    ss << " ";

                ss << weight << ", ";
            }
            ss << std::endl;
        }

        ss << "White bishop pst: " << std::endl;

        for(int i = 0; i < 8; ++i)
        {
            for(int j = 0; j < 8; ++j)
            {
                int weight = weights[133 + 8 * i + j];

                if(weight >= 0 && weight < 10)
                    ss << "  ";

                if(weight >= 10)
                    ss << " ";

                if(weight < 0 && weight > -10)
                    ss << " ";

                ss << weight << ", ";
            }
            ss << std::endl;
        }

        ss << "White rook pst: " << std::endl;

        for(int i = 0; i < 8; ++i)
        {
            for(int j = 0; j < 8; ++j)
            {
                int weight = weights[197 + 8 * i + j];

                if(weight >= 0 && weight < 10)
                    ss << "  ";

                if(weight >= 10)
                    ss << " ";

                if(weight < 0 && weight > -10)
                    ss << " ";

                ss << weight << ", ";
            }
            ss << std::endl;
        }

        ss << "White queen pst: " << std::endl;

        for(int i = 0; i < 8; ++i)
        {
            for(int j = 0; j < 8; ++j)
            {
                int weight = weights[261 + 8 * i + j];

                if(weight >= 0 && weight < 10)
                    ss << "  ";

                if(weight >= 10)
                    ss << " ";

                if(weight < 0 && weight > -10)
                    ss << " ";

                ss << weight << ", ";
            }
            ss << std::endl;
        }

        ss << "White king pst: " << std::endl;

        for(int i = 0; i < 8; ++i)
        {
            for(int j = 0; j < 8; ++j)
            {
                int weight = weights[325 + 8 * i + j];

                if(weight >= 0 && weight < 10)
                    ss << "  ";

                if(weight >= 10)
                    ss << " ";

                if(weight < 0 && weight > -10)
                    ss << " ";

                ss << weight << ", ";
            }
            ss << std::endl;
        }

        ss << std::endl;

        return ss.str();
    }
private:
    std::vector<int> weights;
};

struct MiddleCost
{
    double cost;
};

typedef EA::Genetic<Weights, MiddleCost> GA_Type;
typedef EA::GenerationType<Weights, MiddleCost> Generation_Type;

void init_genes( Weights&, const std::function<int(void)>&)
{
}

bool eval_solution( const Weights& w, MiddleCost &c)
{
    std::ifstream infile("out.epd");
    std::string line;
    c.cost = 0;

    Weights r = w;

    GameState state;

    while (std::getline(infile, line))
    {
        std::string fen = line.substr(0, line.find("score: "));
        std::string score = line.substr(line.find("score: ") + 7);

        std::istringstream ss(score);
        double sc;
        ss >> sc;

        state.SetFen(fen);
        c.cost += std::pow(sc - (state.GetScore(r.GetWeights())/100.0), 2);

//        int result = 0;
//        std::for_each(r.GetWeights().begin() + 5, r.GetWeights().end(), [&result](const int &s){result += std::pow(s,2);});

//        c.cost += result;

//        ++i;
    }


    return true;
}

Weights mutate( const Weights& X_base, const std::function<int(void)> &, int)
{
    Weights X_new = X_base;

    std::random_device dev;
    std::mt19937 rng(dev());

    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 4);
    int idx = dist(rng);

    int low, high;

    low = X_new.GetWeights()[idx] - 0.1*X_new.GetWeights()[idx];
    high = X_new.GetWeights()[idx] + 0.1*X_new.GetWeights()[idx];

    low = low < 100 ? 100 : low;

    std::uniform_int_distribution<std::mt19937::result_type> dist2(low, high);
    int new_weight = dist2(rng);

    X_new.GetWeights()[idx] = new_weight;

    std::uniform_int_distribution<std::mt19937::result_type> dist1(5, 68);
    idx = dist1(rng);

    low = X_new.GetWeights()[idx] - 5;
    high = X_new.GetWeights()[idx] + 5;

    std::uniform_int_distribution<std::mt19937::result_type> dist3(low, high);
    new_weight = dist3(rng);

    X_new.GetWeights()[idx] = new_weight;

    std::uniform_int_distribution<std::mt19937::result_type> dist4(69, 132);
    idx = dist4(rng);

    low = X_new.GetWeights()[idx] - 5;
    high = X_new.GetWeights()[idx] + 5;

    std::uniform_int_distribution<std::mt19937::result_type> dist5(low, high);
    new_weight = dist5(rng);

    X_new.GetWeights()[idx] = new_weight;

    std::uniform_int_distribution<std::mt19937::result_type> dist6(133, 196);
    idx = dist6(rng);

    low = X_new.GetWeights()[idx] - 5;
    high = X_new.GetWeights()[idx] + 5;

    std::uniform_int_distribution<std::mt19937::result_type> dist7(low, high);
    new_weight = dist7(rng);

    X_new.GetWeights()[idx] = new_weight;

    std::uniform_int_distribution<std::mt19937::result_type> dist8(197, 260);
    idx = dist8(rng);

    low = X_new.GetWeights()[idx] - 5;
    high = X_new.GetWeights()[idx] + 5;

    std::uniform_int_distribution<std::mt19937::result_type> dist9(low, high);
    new_weight = dist9(rng);

    X_new.GetWeights()[idx] = new_weight;

    std::uniform_int_distribution<std::mt19937::result_type> dist10(261, 324);
    idx = dist10(rng);

    low = X_new.GetWeights()[idx] - 5;
    high = X_new.GetWeights()[idx] + 5;

    std::uniform_int_distribution<std::mt19937::result_type> dist11(low, high);
    new_weight = dist11(rng);

    X_new.GetWeights()[idx] = new_weight;

    std::uniform_int_distribution<std::mt19937::result_type> dist12(325, SizeOpt);
    idx = dist12(rng);

    low = X_new.GetWeights()[idx] - 5;
    high = X_new.GetWeights()[idx] + 5;

    std::uniform_int_distribution<std::mt19937::result_type> dist13(low, high);
    new_weight = dist13(rng);

    X_new.GetWeights()[idx] = new_weight;

    return X_new;
}

Weights crossover( const Weights& X1, const Weights& X2, const std::function<int(void)> &)
{
    Weights X_new;

    std::random_device dev;
    std::mt19937 rng(dev());

    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 4);
    int result = dist(rng);

    int i;

    for( i = 0; i <= result; ++i )
        X_new.GetWeights()[i] = X1.GetWeights()[i];

    for( ; i <= 4; ++i )
        X_new.GetWeights()[i] = X2.GetWeights()[i];

    std::uniform_int_distribution<std::mt19937::result_type> dist1(5, 68);
    result = dist1(rng);

    for( i = 5; i <= result; ++i )
        X_new.GetWeights()[i] = X1.GetWeights()[i];

    for( ; i <= 68; ++i )
        X_new.GetWeights()[i] = X2.GetWeights()[i];

    std::uniform_int_distribution<std::mt19937::result_type> dist2(69, 132);
    result = dist2(rng);

    for( i = 69; i <= result; ++i )
        X_new.GetWeights()[i] = X1.GetWeights()[i];

    for( ; i <= 132; ++i )
        X_new.GetWeights()[i] = X2.GetWeights()[i];

    std::uniform_int_distribution<std::mt19937::result_type> dist3(133, 196);
    result = dist3(rng);

    for( i = 133; i <= result; ++i )
        X_new.GetWeights()[i] = X1.GetWeights()[i];

    for( ; i <= 196; ++i )
        X_new.GetWeights()[i] = X2.GetWeights()[i];

    std::uniform_int_distribution<std::mt19937::result_type> dist4(197, 260);
    result = dist4(rng);

    for( i = 197; i <= result; ++i )
        X_new.GetWeights()[i] = X1.GetWeights()[i];

    for( ; i <= 260; ++i )
        X_new.GetWeights()[i] = X2.GetWeights()[i];

    std::uniform_int_distribution<std::mt19937::result_type> dist5(261, 324);
    result = dist5(rng);

    for( i = 261; i <= result; ++i )
        X_new.GetWeights()[i] = X1.GetWeights()[i];

    for( ; i <= 324; ++i )
        X_new.GetWeights()[i] = X2.GetWeights()[i];

    std::uniform_int_distribution<std::mt19937::result_type> dist6(325, SizeOpt);
    result = dist6(rng);

    for( i = 325; i <= result; ++i )
        X_new.GetWeights()[i] = X1.GetWeights()[i];

    for( ; i <= SizeOpt; ++i )
        X_new.GetWeights()[i] = X2.GetWeights()[i];

    if(std::equal(std::cbegin(X1.GetWeights()), std::cend(X1.GetWeights()),
                  std::cbegin(X2.GetWeights()), std::cend(X2.GetWeights()),
                  [](int a, int b)->bool {return a == b;}))
    {
        X_new = mutate(X_new, nullptr, 0);
    }

    return X_new;
}

double calculate_SO_total_fitness(const GA_Type::thisChromosomeType &X)
{
    return X.middle_costs.cost;
}

std::ofstream output_file;

void SO_report_generation(
    int generation_number,
    const EA::GenerationType<Weights,MiddleCost> &last_generation,
    const Weights& best_genes)
{
    std::cout
        <<"Generation ["<<generation_number<<"], "
        <<"Best="<<last_generation.best_total_cost<<", "
        <<"Average="<<last_generation.average_cost<<", "
        <<"Best genes=("<< best_genes.ToString()
        <<"Exe_time="<<last_generation.exe_time
        <<std::endl;

    output_file
        <<generation_number<<"\t"
        <<last_generation.average_cost<<"\t"
        <<last_generation.best_total_cost<<"\t"
       << best_genes.ToString() << std::endl;
}

int optimize()
{
    output_file.open("result.txt");

    EA::Chronometer timer;
    timer.tic();

    GA_Type ga_obj;
    ga_obj.problem_mode= EA::GA_MODE::SOGA;
    ga_obj.multi_threading=true;
    ga_obj.idle_delay_us=1; // switch between threads quickly
    ga_obj.verbose=false;
    ga_obj.population=1000;
    ga_obj.generation_max=1000;
    ga_obj.calculate_SO_total_fitness=calculate_SO_total_fitness;
    ga_obj.init_genes= init_genes;
    ga_obj.eval_solution= eval_solution;
    ga_obj.mutate= mutate;
    ga_obj.crossover= crossover;
    ga_obj.SO_report_generation= SO_report_generation;
    ga_obj.best_stall_max=1000;
    ga_obj.elite_count=125;
    ga_obj.crossover_fraction=0.66;
    ga_obj.mutation_rate=0.25;
    ga_obj.solve();

    std::cout<<"The problem is optimized in "<<timer.toc()<<" seconds."<<std::endl;

    output_file.close();
    return 0;
    return 0;
}
