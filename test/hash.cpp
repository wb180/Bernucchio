#include "gtest/gtest.h"

#include "../gamestate.h"

TEST(HashTest, EnPassant)
{
    GameState gs;
    EXPECT_TRUE(gs.SetFen("8/3p4/8/4P3/5p2/8/2k1P3/K7 w - - 0 1"));

    EXPECT_TRUE(gs.MakeMove("e2e4"));
    uint64_t hash_e2e4 = gs.GetHash();

    EXPECT_TRUE(gs.MakeMove("f4e3"));
    uint64_t hash_f4e3 = gs.GetHash();

    EXPECT_TRUE(gs.MakeMove("a1a2"));
    uint64_t hash_a1a2 = gs.GetHash();

    EXPECT_TRUE(gs.MakeMove("d7d5"));
    uint64_t hash_d7d5 = gs.GetHash();

    EXPECT_TRUE(gs.MakeMove("e5d6"));
    uint64_t hash_e5d6 = gs.GetHash();

    EXPECT_TRUE(gs.SetFen("8/3p4/8/4P3/4Pp2/8/2k5/K7 b - e3 0 1"));
    uint64_t hash_set_e2e4 = gs.GetHash();

    EXPECT_TRUE(gs.SetFen("8/3p4/8/4P3/8/4p3/2k5/K7 w - - 0 2"));
    uint64_t hash_set_f4e3 = gs.GetHash();

    EXPECT_TRUE(gs.SetFen("8/3p4/8/4P3/8/4p3/K1k5/8 b - - 0 2"));
    uint64_t hash_set_a1a2 = gs.GetHash();

    EXPECT_TRUE(gs.SetFen("8/8/8/3pP3/8/4p3/K1k5/8 w - d6 0 3"));
    uint64_t hash_set_d7d5 = gs.GetHash();

    EXPECT_TRUE(gs.SetFen("8/8/3P4/8/8/4p3/K1k5/8 b - - 0 3"));
    uint64_t hash_set_e5d6 = gs.GetHash();

    EXPECT_EQ(hash_e2e4, hash_set_e2e4);
    EXPECT_EQ(hash_f4e3, hash_set_f4e3);
    EXPECT_EQ(hash_a1a2, hash_set_a1a2);
    EXPECT_EQ(hash_d7d5, hash_set_d7d5);
    EXPECT_EQ(hash_e5d6, hash_set_e5d6);
};

TEST(HashTest, Castlings)
{
    GameState gs;
    EXPECT_TRUE(gs.SetFen("4k2r/8/8/8/8/8/8/R3K3 w Qk - 0 1"));

    EXPECT_TRUE(gs.MakeMove("e1c1"));
    uint64_t hash_e1c1 = gs.GetHash();

    EXPECT_TRUE(gs.MakeMove("e8g8"));
    uint64_t hash_e8g8 = gs.GetHash();

    EXPECT_TRUE(gs.SetFen("4k2r/8/8/8/8/8/8/2KR4 b k - 0 1"));
    uint64_t hash_set_e1c1 = gs.GetHash();

    EXPECT_TRUE(gs.SetFen("5rk1/8/8/8/8/8/8/2KR4 w - - 0 2"));
    uint64_t hash_set_e8g8 = gs.GetHash();

    EXPECT_EQ(hash_e1c1, hash_set_e1c1);
    EXPECT_EQ(hash_e8g8, hash_set_e8g8);

    EXPECT_TRUE(gs.SetFen("r3k3/8/8/8/8/8/8/4K2R b Kq - 0 1"));

    EXPECT_TRUE(gs.MakeMove("e8c8"));
    uint64_t hash_e8c8 = gs.GetHash();

    EXPECT_TRUE(gs.MakeMove("e1g1"));
    uint64_t hash_e1g1 = gs.GetHash();

    EXPECT_TRUE(gs.SetFen("2kr4/8/8/8/8/8/8/4K2R w K - 0 1"));
    uint64_t hash_set_e8c8 = gs.GetHash();

    EXPECT_TRUE(gs.SetFen("2kr4/8/8/8/8/8/8/5RK1 b - - 0 2"));
    uint64_t hash_set_e1g1 = gs.GetHash();

    EXPECT_EQ(hash_e1g1, hash_set_e1g1);
    EXPECT_EQ(hash_e8c8, hash_set_e8c8);
};

TEST(HashTest, PromotionsAndCaptures)
{
    GameState gs;
    EXPECT_TRUE(gs.SetFen("1r1q4/PPPP4/8/4K3/6k1/8/4pppp/4Q1N1 w - - 0 1"));

    EXPECT_TRUE(gs.MakeMove("a7a8q"));
    uint64_t hash_a7a8q = gs.GetHash();

    EXPECT_TRUE(gs.MakeMove("h2h1n"));
    uint64_t hash_h2h1n = gs.GetHash();

    EXPECT_TRUE(gs.MakeMove("c7b8q"));
    uint64_t hash_c7b8q = gs.GetHash();

    EXPECT_TRUE(gs.MakeMove("f2g1q"));
    uint64_t hash_f2g1q = gs.GetHash();

    EXPECT_TRUE(gs.MakeMove("b8d8"));
    uint64_t hash_b8d8 = gs.GetHash();

    EXPECT_TRUE(gs.MakeMove("g1e1"));
    uint64_t hash_g1e1 = gs.GetHash();

    EXPECT_TRUE(gs.MakeMove("b7b8n"));
    uint64_t hash_b7b8n = gs.GetHash();

    EXPECT_TRUE(gs.MakeMove("g2g1b"));
    uint64_t hash_g2g1b = gs.GetHash();

    EXPECT_TRUE(gs.SetFen("Qr1q4/1PPP4/8/4K3/6k1/8/4pppp/4Q1N1 b - - 0 1"));
    uint64_t hash_set_a7a8q = gs.GetHash();

    EXPECT_TRUE(gs.SetFen("Qr1q4/1PPP4/8/4K3/6k1/8/4ppp1/4Q1Nn w - - 0 2"));
    uint64_t hash_set_h2h1n = gs.GetHash();

    EXPECT_TRUE(gs.SetFen("QQ1q4/1P1P4/8/4K3/6k1/8/4ppp1/4Q1Nn b - - 0 2"));
    uint64_t hash_set_c7b8q = gs.GetHash();

    EXPECT_TRUE(gs.SetFen("QQ1q4/1P1P4/8/4K3/6k1/8/4p1p1/4Q1qn w - - 0 3"));
    uint64_t hash_set_f2g1q = gs.GetHash();

    EXPECT_TRUE(gs.SetFen("Q2Q4/1P1P4/8/4K3/6k1/8/4p1p1/4Q1qn b - - 0 3"));
    uint64_t hash_set_b8d8 = gs.GetHash();

    EXPECT_TRUE(gs.SetFen("Q2Q4/1P1P4/8/4K3/6k1/8/4p1p1/4q2n w - - 0 4"));
    uint64_t hash_set_g1e1 = gs.GetHash();

    EXPECT_TRUE(gs.SetFen("QN1Q4/3P4/8/4K3/6k1/8/4p1p1/4q2n b - - 0 4"));
    uint64_t hash_set_b7b8n = gs.GetHash();

    EXPECT_TRUE(gs.SetFen("QN1Q4/3P4/8/4K3/6k1/8/4p3/4q1bn w - - 0 5"));
    uint64_t hash_set_g2g1b = gs.GetHash();

    EXPECT_EQ(hash_a7a8q, hash_set_a7a8q);
    EXPECT_EQ(hash_h2h1n, hash_set_h2h1n);
    EXPECT_EQ(hash_c7b8q, hash_set_c7b8q);
    EXPECT_EQ(hash_f2g1q, hash_set_f2g1q);
    EXPECT_EQ(hash_b8d8, hash_set_b8d8);
    EXPECT_EQ(hash_g1e1, hash_set_g1e1);
    EXPECT_EQ(hash_b7b8n, hash_set_b7b8n);
    EXPECT_EQ(hash_g2g1b, hash_set_g2g1b);
};
