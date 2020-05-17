#ifndef CTG_H
#define CTG_H

#include "board.h"
#include "constants.h"
#include "gamestate.h"

#include <memory>

struct  PageBounds
{
    int pad;
    int low;
    int high;
};

struct Entry
{
    int num_moves;
    uint8_t moves[100];
    int total;
    int wins;
    int losses;
    int draws;
    int unknown1;
    int avg_rating_games;
    int avg_rating_score;
    int perf_rating_games;
    int perf_rating_score;
    int recommendation;
    int unknown2;
    int comment;
};

class CTGReader
{
public:    
    ~CTGReader();

    static CTGReader& GetInstance(std::string name)
    {
        static CTGReader reader{ name };
        return reader;
    }

    bool GetMove( GameState *state, Signature *signature, Move *move );
    bool PickMove(GameState *state, Entry *entry, Move *move);
    int64_t MoveWeight(GameState *state, Move move, uint8_t annotation, bool* recommended);

private:

    explicit CTGReader(std::string name);

    int32_t ConvertSignatureToHash(Signature *signature);
    bool GetEntry( int page_index, Signature *signature, Entry *entry);


    int GetPageIndex(int32_t hash);

    std::string name_;    
    PageBounds bounds_;

    std::ifstream *ctg_input_, *cto_input_, *ctb_input_;
    std::unique_ptr<std::ifstream> ctg_file_stream_, cto_file_stream_, ctb_file_stream_;
};

#endif // CTG_H
