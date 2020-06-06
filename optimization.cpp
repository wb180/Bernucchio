#include "openga.hpp"

#include "gamestate.h"
#include "evaluator.h"

#include <random>
#include <vector>
#include <fstream>
#include <sstream>

#include "optimization.h"

const int SizeOpt = 774;

class Weights
{
public:
    Weights()
    {
        std::mt19937_64 rng;
        rng.seed(std::random_device{}());
        std::uniform_int_distribution dist(0, 0);
        std::uniform_int_distribution dist2(0, 0);

        for(std::size_t i = 0; i < kPieceValue.size(); ++i)
            weights.push_back(kPieceValue[i]);

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

        weights.push_back( kEndGame );

        for(std::size_t i = 0; i < kPstWhitePawnsEnd.size(); ++i)
            weights.push_back(kPstWhitePawnsEnd[i] + dist2(rng));

        for(std::size_t i = 0; i < kPstWhiteKnightsEnd.size(); ++i)
            weights.push_back(kPstWhiteKnightsEnd[i] + dist2(rng));

        for(std::size_t i = 0; i < kPstWhiteBishopsEnd.size(); ++i)
            weights.push_back(kPstWhiteBishopsEnd[i] + dist2(rng));

        for(std::size_t i = 0; i < kPstWhiteRooksEnd.size(); ++i)
            weights.push_back(kPstWhiteRooksEnd[i] + dist2(rng));

        for(std::size_t i = 0; i < kPstWhiteQueensEnd.size(); ++i)
            weights.push_back(kPstWhiteQueensEnd[i] + dist2(rng));

        for(std::size_t i = 0; i < kPstWhiteKingEnd.size(); ++i)
            weights.push_back(kPstWhiteKingEnd[i] + dist2(rng));
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
              "Endgame threshold value: " << weights[389] << std::endl << std::endl <<
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

        ss << "White pawn pst end: " << std::endl;

  for(int i = 0; i < 8; ++i)
  {
      for(int j = 0; j < 8; ++j)
      {
          int weight = weights[390 + 8 * i + j];

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

  ss << "White knight pst end: " << std::endl;

  for(int i = 0; i < 8; ++i)
  {
      for(int j = 0; j < 8; ++j)
      {
          int weight = weights[454 + 8 * i + j];

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

  ss << "White bishop pst end: " << std::endl;

  for(int i = 0; i < 8; ++i)
  {
      for(int j = 0; j < 8; ++j)
      {
          int weight = weights[518 + 8 * i + j];

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

  ss << "White rook pst end: " << std::endl;

  for(int i = 0; i < 8; ++i)
  {
      for(int j = 0; j < 8; ++j)
      {
          int weight = weights[582 + 8 * i + j];

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

  ss << "White queen pst end: " << std::endl;

  for(int i = 0; i < 8; ++i)
  {
      for(int j = 0; j < 8; ++j)
      {
          int weight = weights[646 + 8 * i + j];

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

  ss << "White king pst end: " << std::endl;

  for(int i = 0; i < 8; ++i)
  {
      for(int j = 0; j < 8; ++j)
      {
          int weight = weights[710 + 8 * i + j];

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

        if(!state.IsMateSituation())
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

    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 5);
    int idx = dist(rng);

    if(idx == 5)
        idx = 389;

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

    std::uniform_int_distribution<std::mt19937::result_type> dist12(325, 388);
    idx = dist12(rng);

    low = X_new.GetWeights()[idx] - 5;
    high = X_new.GetWeights()[idx] + 5;

    std::uniform_int_distribution<std::mt19937::result_type> dist13(low, high);
    new_weight = dist13(rng);

    X_new.GetWeights()[idx] = new_weight;

    std::uniform_int_distribution<std::mt19937::result_type> dist14(390, 453);
    idx = dist14(rng);

    low = X_new.GetWeights()[idx] - 5;
    high = X_new.GetWeights()[idx] + 5;

    std::uniform_int_distribution<std::mt19937::result_type> dist15(low, high);
    new_weight = dist15(rng);

    X_new.GetWeights()[idx] = new_weight;

    std::uniform_int_distribution<std::mt19937::result_type> dist16(454, 517);
    idx = dist16(rng);

    low = X_new.GetWeights()[idx] - 5;
    high = X_new.GetWeights()[idx] + 5;

    std::uniform_int_distribution<std::mt19937::result_type> dist17(low, high);
    new_weight = dist17(rng);

    X_new.GetWeights()[idx] = new_weight;

    std::uniform_int_distribution<std::mt19937::result_type> dist18(518, 581);
    idx = dist18(rng);

    low = X_new.GetWeights()[idx] - 5;
    high = X_new.GetWeights()[idx] + 5;

    std::uniform_int_distribution<std::mt19937::result_type> dist19(low, high);
    new_weight = dist19(rng);

    X_new.GetWeights()[idx] = new_weight;

    std::uniform_int_distribution<std::mt19937::result_type> dist20(582, 645);
    idx = dist20(rng);

    low = X_new.GetWeights()[idx] - 5;
    high = X_new.GetWeights()[idx] + 5;

    std::uniform_int_distribution<std::mt19937::result_type> dist21(low, high);
    new_weight = dist21(rng);

    X_new.GetWeights()[idx] = new_weight;

    std::uniform_int_distribution<std::mt19937::result_type> dist22(646, 709);
    idx = dist22(rng);

    low = X_new.GetWeights()[idx] - 5;
    high = X_new.GetWeights()[idx] + 5;

    std::uniform_int_distribution<std::mt19937::result_type> dist23(low, high);
    new_weight = dist23(rng);

    X_new.GetWeights()[idx] = new_weight;

    std::uniform_int_distribution<std::mt19937::result_type> dist24(710, SizeOpt - 1);
    idx = dist24(rng);

    low = X_new.GetWeights()[idx] - 5;
    high = X_new.GetWeights()[idx] + 5;

    std::uniform_int_distribution<std::mt19937::result_type> dist25(low, high);
    new_weight = dist25(rng);

    X_new.GetWeights()[idx] = new_weight;

    return X_new;
}

Weights crossover( const Weights& X1, const Weights& X2, const std::function<int(void)> &)
{
    Weights X_new;

    std::random_device dev;
    std::mt19937 rng(dev());

    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 5);
    int result = dist(rng);

    int i;

    if( result < 5)
    {
        for( i = 0; i <= result; ++i )
            X_new.GetWeights()[i] = X1.GetWeights()[i];

        for( ; i <= 4; ++i )
            X_new.GetWeights()[i] = X2.GetWeights()[i];

        X_new.GetWeights()[389] = X2.GetWeights()[389];
    }
    else
    {
        for( i = 0; i <= 4; ++i )
            X_new.GetWeights()[i] = X1.GetWeights()[i];

        X_new.GetWeights()[389] = X1.GetWeights()[389];
    }

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

    for( ; i <= 388; ++i )
        X_new.GetWeights()[i] = X2.GetWeights()[i];

    std::uniform_int_distribution<std::mt19937::result_type> dist7(390, 453);
    result = dist7(rng);

    for( i = 390; i <= result; ++i )
        X_new.GetWeights()[i] = X1.GetWeights()[i];

    for( ; i <= 453; ++i )
        X_new.GetWeights()[i] = X2.GetWeights()[i];

    std::uniform_int_distribution<std::mt19937::result_type> dist8(454, 517);
    result = dist8(rng);

    for( i = 454; i <= result; ++i )
        X_new.GetWeights()[i] = X1.GetWeights()[i];

    for( ; i <= 517; ++i )
        X_new.GetWeights()[i] = X2.GetWeights()[i];

    std::uniform_int_distribution<std::mt19937::result_type> dist9(518, 581);
    result = dist9(rng);

    for( i = 518; i <= result; ++i )
        X_new.GetWeights()[i] = X1.GetWeights()[i];

    for( ; i <= 581; ++i )
        X_new.GetWeights()[i] = X2.GetWeights()[i];

    std::uniform_int_distribution<std::mt19937::result_type> dist10(582, 645);
    result = dist10(rng);

    for( i = 582; i <= result; ++i )
        X_new.GetWeights()[i] = X1.GetWeights()[i];

    for( ; i <= 645; ++i )
        X_new.GetWeights()[i] = X2.GetWeights()[i];

    std::uniform_int_distribution<std::mt19937::result_type> dist11(646, 709);
    result = dist11(rng);

    for( i = 646; i <= result; ++i )
        X_new.GetWeights()[i] = X1.GetWeights()[i];

    for( ; i <= 709; ++i )
        X_new.GetWeights()[i] = X2.GetWeights()[i];

    std::uniform_int_distribution<std::mt19937::result_type> dist12(710, SizeOpt);
    result = dist12(rng);

    for( i = 710; i <= result; ++i )
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
    ga_obj.population=500;
    ga_obj.generation_max=2000;
    ga_obj.calculate_SO_total_fitness=calculate_SO_total_fitness;
    ga_obj.init_genes= init_genes;
    ga_obj.eval_solution= eval_solution;
    ga_obj.mutate= mutate;
    ga_obj.crossover= crossover;
    ga_obj.SO_report_generation= SO_report_generation;
    ga_obj.best_stall_max=1000;
    ga_obj.elite_count=100;
    ga_obj.crossover_fraction=0.75;
    ga_obj.mutation_rate=0.33;
    ga_obj.solve();

    std::cout<<"The problem is optimized in "<<timer.toc()<<" seconds."<<std::endl;

    output_file.close();
    return 0;
    return 0;
}
