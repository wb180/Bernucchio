#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "board.h"
#include "evaluator.h"
#include "hashlist.h"
#include "moves.h"

#include <atomic>
#include <cstdint>
#include <functional>
#include <vector>

struct Signature
{
    uint8_t buf[64];
    int length_;
};

class GameState
{
public:
    GameState();

    bool SetFen(const std::string &fen_string);
    std::string GetFen() const;

    bool MakeMove(const std::string &move_string);

    uint64_t Perft(std::size_t depth);
    uint64_t SplitPerft(std::size_t depth);
    void SpeedPerft();

    Side GetSide() const;
    uint64_t GetHash() const;

    int NegaMax(std::size_t depth, std::size_t *pv_line);
    int AlphaBeta(std::size_t depth, int alpha, int beta, std::size_t *pv_line);
    int QuiescenceSearch(int alpha, int beta);

    void Search(std::size_t depth, std::atomic<bool> *stop = nullptr);

    void getSignature(Signature *signature);

    bool ByteToMove(uint8_t byte, Move *move);

    Moves *GetMoves(){return &moves_;}

    int GetScore(std::vector<int> &w)
    {
        return evaluator_.Score(w);
    }

    bool IsMateSituation()
    {
        return board_.IsKingSolo(Side::kBlack) || board_.IsKingSolo(Side::kWhite);
    }

    bool useBook() const;
    void setUseBook(bool newUseBook);

private:
    Board board_;
    Moves moves_;
    HashList hashes_;
    Evaluator evaluator_;

    std::array<MoveInfo, 512> move_infos;
    MoveInfo *last_move_;

    Side active_side_;

    std::size_t castlings_;

    uint64_t en_passant_;
    std::size_t fifty_moves_counter_;
    std::size_t full_moves_counter_;

    std::atomic<bool> *stop_;
    bool time_out = false;

    bool found_any_move_;

    uint64_t nodes;
    std::size_t moves_in_root_;
    std::size_t current_depth_;

    bool mUseBook = false;

private:
    bool is_en_passant_possible();
};

#endif // GAMESTATE_H
