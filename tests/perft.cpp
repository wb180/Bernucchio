#include "gamestate.h"

#include <gtest/gtest.h>

TEST(PerftTest, IllegalEnPassantCapture)
{
    GameState gs;

    std::string fen = "8/5bk1/8/2Pp4/8/1K6/8/8 w - d6";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 824064);

    fen = "8/8/1k6/8/2pP4/8/5BK1/8 b - d3";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 824064);

    fen = "3k4/3p4/8/K1P4r/8/8/8/8 b - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 1134888);

    fen = "8/8/8/8/k1p4R/8/3P4/3K4 w - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 1134888);
}

TEST(PerftTest, EnPassantCaptureChecksOpponent)
{
    GameState gs;

    std::string fen = "8/8/1k6/2b5/2pP4/8/5K2/8 b - d3";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 1440467);

    fen = "8/5k2/8/2Pp4/2B5/1K6/8/8 w - d6";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 1440467);
}

TEST(PerftTest, ShortCastlingGivesCheck)
{
    GameState gs;

    std::string fen = "5k2/8/8/8/8/8/8/4K2R w K - 0 1";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 661072);

    fen = "4k2r/8/8/8/8/8/8/5K2 b k - 0 1";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 661072);
}

TEST(PerftTest, LongCastlingGivesCheck)
{
    GameState gs;

    std::string fen = "3k4/8/8/8/8/8/8/R3K3 w Q -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 803711);

    fen = "r3k3/8/8/8/8/8/8/3K4 b q -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 803711);
}

TEST(PerftTest, CastlingIncludingLosingCRDueToRookCapture)
{
    GameState gs;

    std::string fen = "r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(3), 1274206);

    fen = "r3k2r/7b/8/8/8/8/1B4BQ/R3K2R b KQkq -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(3), 1274206);

    fen = "1k6/1b6/8/8/7R/8/8/4K2R b K -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(4), 1063513);

    fen = "4k2r/8/8/7r/8/8/1B6/1K6 w k -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(4), 1063513);

    fen = "1k6/8/8/8/R7/1n6/8/R3K3 b Q -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(4), 346695);

    fen = "r3k3/8/1N6/r7/8/8/8/1K6 w q -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(4), 346695);
}

TEST(PerftTest, CastlingPrevented)
{
    GameState gs;

    std::string fen = "r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(3), 1720476);

    fen = "r3k2r/8/5Q2/8/8/3q4/8/R3K2R w KQkq -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(3), 1720476);
}

TEST(PerftTest, PromoteOutOfCheck)
{
    GameState gs;

    std::string fen = "2K2r2/4P3/8/8/8/8/8/3k4 w - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 3821001);

    fen = "3K4/8/8/8/8/8/4p3/2k2R2 b - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 3821001);
}

TEST(PerftTest, DiscoveredCheck)
{
    GameState gs;

    std::string fen = "8/8/1P2K3/8/2n5/1q6/8/5k2 b - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(4), 1004658);

    fen = "5K2/8/1Q6/2N5/8/1p2k3/8/8 w - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(4), 1004658);
}

TEST(PerftTest, PromoteToGiveCheck)
{
    GameState gs;

    std::string fen = "4k3/1P6/8/8/8/8/K7/8 w - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 217342);

    fen = "8/k7/8/8/8/8/1p6/4K3 b - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 217342);
}

TEST(PerftTest, UnderpromoteToCheck)
{
    GameState gs;

    std::string fen = "8/P1k5/K7/8/8/8/8/8 w - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 92683);

    fen = "8/8/8/8/8/k7/p1K5/8 b - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 92683);
}

TEST(PerftTest, SelfStalemate)
{
    GameState gs;

    std::string fen = "K1k5/8/P7/8/8/8/8/8 w - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 2217);

    fen = "8/8/8/8/8/p7/8/k1K5 b - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 2217);
}

TEST(PerftTest, StalemateCheckmate)
{
    GameState gs;

    std::string fen = "8/k1P5/8/1K6/8/8/8/8 w - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(6), 567584);

    fen = "8/8/8/8/1k6/8/K1p5/8 b - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(6), 567584);
}

TEST(PerftTest, DoubleCheck)
{
    GameState gs;

    std::string fen = "8/8/2k5/5q2/5n2/8/5K2/8 b - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(3), 23527);

    fen = "8/5k2/8/5N2/5Q2/2K5/8/8 w - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(3), 23527);
}


TEST(PerftTest, InitialPosition)
{
    GameState gs;

    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(0), 20);
    EXPECT_EQ(gs.Perft(1), 400);
    EXPECT_EQ(gs.Perft(2), 8902);
    EXPECT_EQ(gs.Perft(3), 197281);
    EXPECT_EQ(gs.Perft(4), 4865609);
    EXPECT_EQ(gs.Perft(5), 119060324);
}

TEST(PerftTest, Kiwipete)
{
    GameState gs;

    std::string fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(0), 48);
    EXPECT_EQ(gs.Perft(1), 2039);
    EXPECT_EQ(gs.Perft(2), 97862);
    EXPECT_EQ(gs.Perft(3), 4085603);
    EXPECT_EQ(gs.Perft(4), 193690690);
}

TEST(PerftTest, Position3)
{
    GameState gs;

    std::string fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(0), 14);
    EXPECT_EQ(gs.Perft(1), 191);
    EXPECT_EQ(gs.Perft(2), 2812);
    EXPECT_EQ(gs.Perft(3), 43238);
    EXPECT_EQ(gs.Perft(4), 674624);
    EXPECT_EQ(gs.Perft(5), 11030083);
    EXPECT_EQ(gs.Perft(6), 178633661);
}

TEST(PerftTest, Position4)
{
    GameState gs;

    std::string fen = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(0), 6);
    EXPECT_EQ(gs.Perft(1), 264);
    EXPECT_EQ(gs.Perft(2), 9467);
    EXPECT_EQ(gs.Perft(3), 422333);
    EXPECT_EQ(gs.Perft(4), 15833292);
}

TEST(PerftTest, Position4_mirrored)
{
    GameState gs;

    std::string fen = "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(0), 6);
    EXPECT_EQ(gs.Perft(1), 264);
    EXPECT_EQ(gs.Perft(2), 9467);
    EXPECT_EQ(gs.Perft(3), 422333);
    EXPECT_EQ(gs.Perft(4), 15833292);
}

TEST(PerftTest, Position5)
{
    GameState gs;

    std::string fen = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(0), 44);
    EXPECT_EQ(gs.Perft(1), 1486);
    EXPECT_EQ(gs.Perft(2), 62379);
    EXPECT_EQ(gs.Perft(3), 2103487);
    EXPECT_EQ(gs.Perft(4), 89941194);
}

TEST(PerftTest, Position6)
{
    GameState gs;

    std::string fen = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(0), 46);
    EXPECT_EQ(gs.Perft(1), 2079);
    EXPECT_EQ(gs.Perft(2), 89890);
    EXPECT_EQ(gs.Perft(3), 3894594);
    EXPECT_EQ(gs.Perft(4), 164075551);
}

TEST(PerftTest, JVMerlino)
{
    GameState gs;

    std::string fen = "8/7p/p5pb/4k3/P1pPn3/8/P5PP/1rB2RK1 b - d3";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 38633283);

    fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(4), 193690690);

    fen = "8/p7/8/1P6/K1k3p1/6P1/7P/8 w - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(7), 8103790);

    fen = "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 71179139);

    fen = "r3k2r/p6p/8/B7/1pp1p3/3b4/P6P/R3K2R w KQkq -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(5), 77054993);

    fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(6), 178633661);

    fen = "8/5p2/8/2k3P1/p3K3/8/1P6/8 b - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(7), 64451405);

    fen = "r3k2r/pb3p2/5npp/n2p4/1p1PPB2/6P1/P2N1PBP/R3K2R w KQkq -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(4), 29179893);
}

TEST(PerftTest, SimplePositions)
{
    GameState gs;

    std::string fen = "r3k2r/8/8/8/8/8/8/R3K2R w KQkq -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(0), 26);
    EXPECT_EQ(gs.Perft(1), 568);
    EXPECT_EQ(gs.Perft(2), 13744);
    EXPECT_EQ(gs.Perft(3), 314346);
    EXPECT_EQ(gs.Perft(4), 7594526);
    EXPECT_EQ(gs.Perft(5), 179862938);

    fen = "k7/pppppppp/8/8/PPPPPPPP/8/8/K7 w - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(0), 11);
    EXPECT_EQ(gs.Perft(1), 179);
    EXPECT_EQ(gs.Perft(2), 2188);
    EXPECT_EQ(gs.Perft(3), 33747);
    EXPECT_EQ(gs.Perft(4), 436769);
    EXPECT_EQ(gs.Perft(5), 6431450);
    EXPECT_EQ(gs.Perft(6), 85916196);

    fen = "k7/8/8/pppppppp/8/8/PPPPPPPP/K7 w - -";

    EXPECT_TRUE(gs.SetFen(fen));
    EXPECT_EQ(gs.Perft(0), 17);
    EXPECT_EQ(gs.Perft(1), 193);
    EXPECT_EQ(gs.Perft(2), 3073);
    EXPECT_EQ(gs.Perft(3), 37870);
    EXPECT_EQ(gs.Perft(4), 573638);
    EXPECT_EQ(gs.Perft(5), 7421393);
    EXPECT_EQ(gs.Perft(6), 107728058);
}
