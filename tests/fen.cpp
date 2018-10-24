#include "gamestate.h"

#include <gtest/gtest.h>

bool IsFensEqual(std::string fen_1, std::string fen_2)
{
    if(fen_1.back() == '-')
    {
        return fen_1 == fen_2.substr(0, fen_1.size());
    }
    else
        return fen_1 == fen_2;
}

TEST(FenTest, Correct)
{
    GameState gs;

    std::string fen_1 = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                fen_2 = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1",
                fen_3 = "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2",
                fen_4 = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2",
                fen_5 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -",
                fen_6 = "5k2/3K2p1/6Pp/7P/2B5/8/8/8 w - - 0 1";

    EXPECT_TRUE(gs.SetFen(fen_1));
    EXPECT_TRUE(IsFensEqual(fen_1, gs.GetFen()));
    EXPECT_TRUE(gs.SetFen(fen_2));
    EXPECT_TRUE(IsFensEqual(fen_2, gs.GetFen()));
    EXPECT_TRUE(gs.SetFen(fen_3));
    EXPECT_TRUE(IsFensEqual(fen_3, gs.GetFen()));
    EXPECT_TRUE(gs.SetFen(fen_4));
    EXPECT_TRUE(IsFensEqual(fen_4, gs.GetFen()));
    EXPECT_TRUE(gs.SetFen(fen_5));
    EXPECT_TRUE(IsFensEqual(fen_5, gs.GetFen()));
    EXPECT_TRUE(gs.SetFen(fen_6));
    EXPECT_TRUE(IsFensEqual(fen_6, gs.GetFen()));
};

TEST(FenTest, NotCorrect)
{
    GameState gs;

    std::string fen_1 = "rnbqkbnr/ppppppLp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                fen_2 = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR/ b KQkq e3 0 1",
                fen_3 = "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR e KQkq c6 0 2",
                fen_4 = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b - 1 2",
                fen_5 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq m4",
                fen_6 = "5k2/3K2p1/6Pp/7P/2B5/8/8/8 w - - 0";

    EXPECT_FALSE(gs.SetFen(fen_1));
    EXPECT_FALSE(gs.SetFen(fen_2));
    EXPECT_FALSE(gs.SetFen(fen_3));
    EXPECT_FALSE(gs.SetFen(fen_4));
    EXPECT_FALSE(gs.SetFen(fen_5));
    EXPECT_FALSE(gs.SetFen(fen_6));
    //EXPECT_FALSE(gs.SetFen(fen_7));
    //EXPECT_FALSE(gs.SetFen(fen_8));
};
