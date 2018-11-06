#include "gamestate.h"

#include <gtest/gtest.h>

TEST(PerftTest, InitialPosition)
{
    GameState gs;

    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(0), 20);
    EXPECT_EQ(gs.Perft(1), 400 );
    EXPECT_EQ(gs.Perft(2), 8902 );
    EXPECT_EQ(gs.Perft(2), 197281 );
}
