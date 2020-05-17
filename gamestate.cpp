#include "bits_functions.h"
#include "constants.h"
#include "ctg.h"
#include "gamestate.h"
#include "logger.h"
#include "timemanager.h"

#include <iostream>
#include <chrono>
#include <cstring>
#include <sstream>

GameState::GameState() : moves_(&board_, &en_passant_, &castlings_, &active_side_, &fifty_moves_counter_), hashes_(&board_), evaluator_(&board_)
{

}

bool GameState::SetFen(const std::string &fen_string)
{
    castlings_ = en_passant_ = 0;

    fifty_moves_counter_ = 0;
    full_moves_counter_ = 0;

    bool result = board_.SetFen(fen_string);

    moves_.Reset();

    if(result)
    {
        std::size_t position = fen_string.find(' ');
        if(position != std::string::npos && ++position != fen_string.size())
        {
            if(fen_string.at(position) == 'w')
                active_side_ = Side::kWhite;
            else if(fen_string.at(position) == 'b')
                active_side_ = Side::kBlack;
            else
                result = false;

             if(++position == fen_string.size() || fen_string.at(position) != ' ')
                 result = false;

            while(result && ++position != fen_string.size() && fen_string.at(position) != ' ')
            {
                switch(fen_string.at(position))
                {
                    case 'K': castlings_ |= kWhiteCastling_0_0; break;
                    case 'Q': castlings_ |= kWhiteCastling_0_0_0; break;
                    case 'k': castlings_ |= kBlackCastling_0_0; break;
                    case 'q': castlings_ |= kBlackCastling_0_0_0; break;

                    case '-': break;
                    default: result = false; break;
                }
            }

            if(result && fen_string.at(position) == ' ' && ++position != fen_string.size())
            {
                if(fen_string.at(position) >= 'a' && fen_string.at(position) <= 'h')
                {
                    std::size_t row = static_cast<std::size_t>(fen_string.at(position)) - 'a';

                    if( ++position != fen_string.size() && (fen_string.at(position) == '3' || fen_string.at(position) == '6'))
                    {
                        std::size_t column = static_cast<std::size_t>(fen_string.at(position)) - '1';

                        en_passant_ = GetBitSet(kBoardSize * column + row);
                    }
                    else
                        result = false;
                }
                else if(position == fen_string.size() || fen_string.at(position) != '-')
                    result = false;
            }

            if(result && ++position != fen_string.size() && fen_string.at(position) != ' ')
                result = false;
            else if(result && position >= fen_string.size())
            {
                hashes_.ComputeInitialHash(en_passant_, castlings_, active_side_);
                return true;
            }

            if(result)
            {
                while(result && ++position != fen_string.size() && fen_string.at(position) != ' ')
                {
                    if(fen_string.at(position) >= '0' && fen_string.at(position) <= '9')
                    {
                        fifty_moves_counter_ *= 10;
                        fifty_moves_counter_ += static_cast<std::size_t>(fen_string.at(position)) - '0';
                    }
                    else
                        result = false;
                }
            }

            if(result && position != fen_string.size())
            {
                while(result && ++position != fen_string.size())
                {
                    if(fen_string.at(position) >= '0' && fen_string.at(position) <= '9')
                    {
                        full_moves_counter_ *= 10;
                        full_moves_counter_ += static_cast<std::size_t>(fen_string.at(position)) - '0';
                    }
                    else
                        result = false;
                }
            }
            else
                result = false;
        }
        else
            result = false;
    }

    hashes_.ComputeInitialHash(en_passant_, castlings_, active_side_);

    return result;
}

std::string GameState::GetFen() const
{
    std::string fen_string = board_.GetFen();
    fen_string.append(1, ' ').append(1, active_side_ ? 'w' : 'b').append(1, ' ');

    std::string castling;
    if(castlings_ & kWhiteCastling_0_0)
        castling.append(1, 'K');
    if(castlings_ & kWhiteCastling_0_0_0)
        castling.append(1, 'Q');
    if(castlings_ & kBlackCastling_0_0)
        castling.append(1, 'k');
    if(castlings_ & kBlackCastling_0_0_0)
        castling.append(1, 'q');

    if(castling.empty())
        fen_string.append("- ");
    else
        fen_string.append(castling).append(1, ' ');

    if(en_passant_)
        fen_string.append(board_.GetPositionFromBitBoard(en_passant_)).append(1, ' ');
    else
        fen_string.append("- ");

    fen_string.append(std::to_string(fifty_moves_counter_)).append(1, ' ');
    fen_string.append(std::to_string(full_moves_counter_));

    return fen_string;
}

bool GameState::MakeMove(const std::string &move_string)
{
    std::size_t move = moves_.GetMove(move_string);    

    if(move)
    {
        if(moves_.MakeMove(move))
        {
            hashes_.UpdateHash(move, moves_.GetLastMoveInfo(), en_passant_, castlings_);
            return true;
        }
    }

    return false;
}

uint64_t GameState::Perft(std::size_t depth)
{
    Move *move = nullptr;
    uint64_t moves_count = 0;

    MoveList move_list;

    if(active_side_)
    {
        moves_.GetWhiteAttacksAndPromotions(&move_list);
        moves_.GetWhiteMoves(&move_list);
    }
    else
    {
        moves_.GetBlackAttacksAndPromotions(&move_list);
        moves_.GetBlackMoves(&move_list);
    }

    while((move = move_list.GetNextMove()))
    {
        if(moves_.MakeMove(move->move_))
        {
            if(depth > 0)
                moves_count += Perft(depth - 1);
            else
                ++moves_count;

            moves_.UnmakeMove(move->move_);
        }
    }

    return moves_count;
}

uint64_t GameState::SplitPerft(std::size_t depth)
{
    Move *move = nullptr;
    uint64_t moves_count = 0;

    MoveList move_list;

    if(active_side_)
    {
        moves_.GetWhiteAttacksAndPromotions(&move_list);
        moves_.GetWhiteMoves(&move_list);
    }
    else
    {
        moves_.GetBlackAttacksAndPromotions(&move_list);
        moves_.GetBlackMoves(&move_list);
    }

    int idx = 1;

    while((move = move_list.GetNextMove()))
    {
        std::string fen_before = GetFen();

        if(moves_.MakeMove(move->move_))
        {
            if(depth > 0)
            {
                std::size_t perft_result = Perft(depth - 1);
                std::cout << ": " << perft_result << std::endl;
                moves_count += perft_result;
            }
            else
            {
                ++moves_count;
                std::cout << std::endl << idx << ": ";
            }

            moves_.UnmakeMove(move->move_);
        }

    }

    return moves_count;
}

void GameState::SpeedPerft()
{
    std::array<std::string, 5> positions = {"r3k2r/8/8/8/3pPp2/8/8/R3K1RR b KQkq e3",
                                             "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq -", "8/3K4/2p5/p2b2r1/5k2/8/8/1q6 b - -",
                                             "rnbqkb1r/ppppp1pp/7n/4Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6", "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq -"
                                            };

    std::array<std::size_t, 5> depths = {5,
                                             5, 6, 5, 5
                                            };

    std::array<uint64_t, 5> results = { 485647607, 706045033, 493407574, 244063299, 706045033};

    uint64_t nodes = 0, duration = 0, sum_nodes = 0, sum_duration = 0;

    auto depthIter = std::begin(depths);
    auto shouldIter = std::begin(results);

    for( auto posIter = std::begin(positions); posIter != std::end(positions); ++posIter )
    {
        SetFen(*posIter);

        auto t1 = std::chrono::high_resolution_clock::now();

        nodes = Perft(*depthIter);

        auto t2 = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1 ).count();

        std::cout << "Depth: " << *depthIter << " " << nodes  << " nodes in " << duration << " microseconds "
                  << "Speed: " <<  nodes / double(duration) << " millions of nodes in second" << std::endl;

        if( nodes != (*shouldIter) )
        {
            std::exit(1);
        }

        sum_nodes += nodes;
        sum_duration += duration;

        ++ depthIter;
        ++ shouldIter;
    }

    std::cout << "Average speed: " << sum_nodes / double(sum_duration) << std::endl << std::endl;
}

Side GameState::GetSide() const
{
    return active_side_;
}

uint64_t GameState::GetHash() const
{
    return hashes_.GetHash();
}

std::string PrintMove(std::size_t &move)
{
    uint64_t from, to;
    std::string move_string;

    move_string.clear();

    from = move & MoveMasks::kFrom;
    to = (move & MoveMasks::kTo) >> 6;

    move_string.append(1, char('a' + from % kBoardSize));
    move_string.append(1, char('1' + from / kBoardSize));

    move_string.append(1, char('a' + to % kBoardSize));
    move_string.append(1, char('1' + to / kBoardSize));

    if((move & MoveMasks::kFlag) == MoveFlags::kPromotion)
    {
        switch((move & MoveMasks::kPromote) >> 14)
        {
        case PromotionType::kQueen:
            move_string.append(1, char('q'));
            break;
        case PromotionType::kRook:
            move_string.append(1, char('r'));
            break;
        case PromotionType::kBishop:
            move_string.append(1, char('b'));
            break;
        case PromotionType::kKnight:
            move_string.append(1, char('n'));
            break;
        }
    }

    return move_string;
}

int GameState::NegaMax(std::size_t depth, std::size_t *pv_line)
{
    if(!found_any_move_ || !((stop_ && *stop_) || time_out))
    {
        if(depth == 0)
            return active_side_ == Side::kWhite ? evaluator_.Score() : -evaluator_.Score();

        MoveList move_list;

        if(active_side_)
        {
            moves_.GetWhiteAttacksAndPromotions(&move_list);
            moves_.GetWhiteMoves(&move_list);
        }
        else
        {
            moves_.GetBlackAttacksAndPromotions(&move_list);
            moves_.GetBlackMoves(&move_list);
        }

        std::array<std::size_t, kMaxPVLineSize> local_pv_line;

        int score = -kMateScore + static_cast<int>(current_depth_);
        Move *move = nullptr;
        std::size_t best_move = 0;
        int current_score;

        ++current_depth_;

        while((move = move_list.GetNextMove()))
        {

            if(moves_.MakeMove(move->move_))
            {
                current_score = score;
                ++nodes;

                score = std::max(score, -NegaMax(depth - 1, &local_pv_line[0]));

                if(score != current_score)
                {
                    best_move = pv_line[0] = move->move_;

                    std::size_t idx = 1;

                    while((pv_line[idx] = local_pv_line[idx - 1]))
                        ++idx;

                }

                moves_.UnmakeMove(move->move_);
            }
        }

        --current_depth_;

        if(best_move)
            pv_line[0] = best_move;
        else
        {
            if(!moves_.IsKingAttacked(active_side_))
                score = 0;

            pv_line[0] = 0;
        }

        if(!(nodes & 16383))
            time_out = time_out || !TimeManager::GetInstance().CheckTime();

        return score;
    }

    return {};
}

int GameState::AlphaBeta(std::size_t depth, int alpha, int beta, std::size_t *pv_line)
{
    if(!found_any_move_ || !((stop_ && *stop_) || time_out))
    {
        if(depth == 0)
        {
            return QuiescenceSearch(alpha, beta);
        }

        MoveList move_list;

        if(active_side_)
        {
            moves_.GetWhiteAttacksAndPromotions(&move_list);
            move_list.UpdateSortValues(&board_);
            move_list.Sort();
            moves_.GetWhiteMoves(&move_list);
        }
        else
        {
            moves_.GetBlackAttacksAndPromotions(&move_list);
            move_list.UpdateSortValues(&board_);
            move_list.Sort();
            moves_.GetBlackMoves(&move_list);
        }

        std::array<std::size_t, kMaxPVLineSize> local_pv_line = {};

        int score = -kMateScore + static_cast<int>(current_depth_);
        Move *move = nullptr;
        std::size_t best_move = 0;
        bool is_exist = false;
        bool is_draw_rules = false;

        ++current_depth_;

        while((move = move_list.GetNextMove()))
        {
            if(moves_.MakeMove(move->move_))
            {
                is_exist = true;

                if(current_depth_ == 1 && depth == 1)
                    moves_in_root_++;

                hashes_.UpdateHash(move->move_, moves_.GetLastMoveInfo(), en_passant_, castlings_);

                ++nodes;

                bool extend = false;
                if(moves_.IsKingAttacked(active_side_))
                    extend = true;

                score = (is_draw_rules = (fifty_moves_counter_ >= 100 || hashes_.Is3FoldRepetition() || !board_.IsSufficientMaterial())) ? 0 : -AlphaBeta(extend ? depth : depth - 1, -beta, -alpha, &local_pv_line[0]);
                moves_.UnmakeMove(move->move_);

                hashes_.UpdateHash();

                if(score >= beta)
                {
                    --current_depth_;
                    return beta;
                }

                if(score > alpha)
                {
                    alpha = score;

                    best_move = pv_line[0] = move->move_;

                    if(!is_draw_rules)
                    {
                        std::size_t idx = 1;
                        while((pv_line[idx] = local_pv_line[idx - 1]))
                            ++idx;
                    }
                    else
                        pv_line[1] = 0;
                }
            }
        }

        --current_depth_;

        if(best_move)
            pv_line[0] = best_move;
        else if(!is_exist)
        {
            if(!moves_.IsKingAttacked(active_side_))
                alpha = 0;
            else
                alpha = score;

            pv_line[0] = 0;
        }

        if(!(nodes & 32767))
            time_out = time_out || !TimeManager::GetInstance().CheckTime();

        return alpha;
    }

    return {};
}

int GameState::QuiescenceSearch(int alpha, int beta)
{
    MoveList move_list;

    if(active_side_)
    {
        moves_.GetWhiteAttacksAndPromotions(&move_list, false);
        move_list.UpdateSortValues(&board_);
        move_list.Sort();
    }
    else
    {
        moves_.GetBlackAttacksAndPromotions(&move_list, false);
        move_list.UpdateSortValues(&board_);
        move_list.Sort();
    }

    int score = active_side_ == Side::kWhite ? evaluator_.Score() : -evaluator_.Score();

    if(score >= beta)
        return beta;
    if(alpha < score)
        alpha = score;

    Move *move = nullptr;
    //bool is_exist = false;

    while((move = move_list.GetNextMove()))
    {
        if(moves_.MakeMove(move->move_))
        {
            //is_exist = true;
            //hashes_.UpdateHash(*move, moves_.GetLastMoveInfo(), en_passant_, castlings_);

            ++nodes;
            score = -QuiescenceSearch(-beta, -alpha);
            moves_.UnmakeMove(move->move_);

            //hashes_.UpdateHash();

            if(score >= beta)
                return beta;

            if(score > alpha)
                alpha = score;
        }
    }

    return alpha;
}

void GameState::Search(std::size_t depth, std::atomic<bool> *stop)
{
    std::array<std::size_t, kMaxPVLineSize> pv_line{};
    stop_ = stop;
    std::size_t best_move = 0;

    time_out = false;
    nodes = 0;

    int score;

    auto time_start = std::chrono::steady_clock::now();
    found_any_move_ = false;

    if( !found_any_move_ && false/*use_book*/ )
    {
        Signature signature;
        getSignature(&signature);
        Move move;
        if( CTGReader::GetInstance("Perfect2019").GetMove(this, &signature, &move) )
        {
            best_move = move.move_;
            found_any_move_ = true;
        }
    }

    if(!found_any_move_)
        for(std::size_t iterative_depth = 0; iterative_depth <= depth; ++iterative_depth)
        {
            current_depth_ = 0;

            if(iterative_depth == 1)
                moves_in_root_ = 0;

            score = AlphaBeta(iterative_depth, -kMateScore, kMateScore, &pv_line[0]);

            if( found_any_move_ && ((stop_ && *stop_) || time_out))
                break;
            else
            {
                std::ostringstream ss;

                ss << "info depth " << iterative_depth << " time " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time_start).count()  <<
                             " nodes " << nodes << " pv ";

                for(auto move : pv_line)
                {
                    if(!move)
                        break;

                    ss << PrintMove(move) << " ";
                }

                ss << " score ";

                if(score > kHighestScore || score < -kHighestScore)
                {
                    ss << "mate " << (score > 0 ? (kMateScore - score)/2 + 1 : (-score - kMateScore)/ 2) << std::endl;
                }
                else
                    ss << "cp " << score << std::endl;

                std::cout << ss.str();
                std::flush(std::cout);


                best_move = pv_line[0];
                found_any_move_ = true;

                if(!TimeManager::GetInstance().IsInfinite() && iterative_depth == 1 && moves_in_root_ == 1)
                    break;

                if(!TimeManager::GetInstance().IsInfinite() && TimeManager::GetInstance().TimeElapsed() >= 0.5)
                    break;
            }
        }

    if(best_move)
    {
        std::ostringstream ss;
        ss << "bestmove " << PrintMove(best_move) << std::endl;
        std::cout << ss.str();
        std::flush(std::cout);
    }

}

void GameState::getSignature(Signature *signature)
{
    memset(signature, 0, sizeof(Signature));
    int bit_position = 8;
    uint8_t bits = 0, num_bits = 0;

    bool flip_board = active_side_ == Side::kBlack;
    bool mirror_board =  !castlings_ && board_.IsKingOnTheLeftHalf(flip_board ? Side::kBlack : Side::kWhite);

    for (std::size_t file = 0; file < kBoardSize; ++file)
    {
        for (std::size_t rank = 0; rank < kBoardSize; ++rank)
        {
            switch (board_.GetPieceOnSquare((flip_board ? 7 - rank : rank) * 8 + (mirror_board ? 7 - file : file)))
            {
                case KAllPieces:
                    bits = 0x0;
                    num_bits = 1;
                    break;
                case kWhitePawns:
                    if(!flip_board)
                    {
                        bits = 0x3;
                        num_bits = 3;
                    }
                    else
                    {
                        bits = 0x7;
                        num_bits = 3;
                    }
                    break;
                case kBlackPawns:
                    if(!flip_board)
                    {
                        bits = 0x7;
                        num_bits = 3;
                    }
                    else
                    {
                        bits = 0x3;
                        num_bits = 3;
                    }
                    break;
                case kWhiteKnights:
                    if(!flip_board)
                    {
                        bits = 0x9;
                        num_bits = 5;
                    }
                    else
                    {
                        bits = 0x19;
                        num_bits = 5;
                    }
                    break;
                case kBlackKnights:
                    if(!flip_board)
                    {
                        bits = 0x19;
                        num_bits = 5;
                    }
                    else
                    {
                        bits = 0x9;
                        num_bits = 5;
                    }
                    break;
                case kWhiteBishops:
                    if(!flip_board)
                    {
                        bits = 0x5;
                        num_bits = 5;
                    }
                    else
                    {
                        bits = 0x15;
                        num_bits = 5;
                    }
                    break;
                case kBlackBishops:
                    if(!flip_board)
                    {
                        bits = 0x15;
                        num_bits = 5;
                    }
                    else
                    {
                        bits = 0x5;
                        num_bits = 5;
                    }
                    break;
                case kWhiteRooks:
                    if(!flip_board)
                    {
                        bits = 0xD;
                        num_bits = 5;
                    }
                    else
                    {
                        bits = 0x1D;
                        num_bits = 5;
                    }
                    break;
                case kBlackRooks:
                    if(!flip_board)
                    {
                        bits = 0x1D;
                        num_bits = 5;
                    }
                    else
                    {
                        bits = 0xD;
                        num_bits = 5;
                    }
                    break;
                case kWhiteQueens:
                    if(!flip_board)
                    {
                        bits = 0x11;
                        num_bits = 6;
                    }
                    else
                    {
                        bits = 0x31;
                        num_bits = 6;
                    }
                    break;
                case kBlackQueens:
                    if(!flip_board)
                    {
                        bits = 0x31;
                        num_bits = 6;
                    }
                    else
                    {
                        bits = 0x11;
                        num_bits = 6;
                    }
                    break;
                case kWhiteKing:
                    if(!flip_board)
                    {
                        bits = 0x1;
                        num_bits = 6;
                    }
                    else
                    {
                        bits = 0x21;
                        num_bits = 6;
                    }
                    break;
                case kBlackKing:
                    if(!flip_board)
                    {
                        bits = 0x21;
                        num_bits = 6;
                    }
                    else
                    {
                        bits = 0x1;
                        num_bits = 6;
                    }
                    break;
            default: break;
            }
            append_bits_reverse(signature->buf, bits, bit_position, num_bits);
            bit_position += num_bits;
        }
    }

    int ep = -1;
    int flag_bit_length = 0;

    if( is_en_passant_possible() )
    {
        ep = GetLSBPos(en_passant_)%kBoardSize;
        if(mirror_board)
            ep = 7 - ep;
        flag_bit_length = 3;
    }

    int castle = 0;
    if(castlings_ & (flip_board ? kBlackCastling_0_0 : kWhiteCastling_0_0))
        castle += 4;
    if(castlings_ & (flip_board ? kBlackCastling_0_0_0: kWhiteCastling_0_0_0))
        castle += 8;
    if(castlings_  & (flip_board ? kWhiteCastling_0_0: kBlackCastling_0_0))
        castle += 1;
    if(castlings_ & (flip_board ? kWhiteCastling_0_0_0: kBlackCastling_0_0_0))
        castle += 2;
    if(castle)
        flag_bit_length += 4;

    uint8_t flag_bits = castle;

    if (ep != -1)
    {
        flag_bits <<= 3;

        for (int i=0; i<3; ++i, ep>>=1)
            if (ep&1)
                flag_bits |= (1<<(2-i));
    }

    int pad_bits = 0;

    if (8-(bit_position % 8) < flag_bit_length)
    {
        pad_bits = 8 - (bit_position % 8);
        append_bits_reverse(signature->buf, 0, bit_position, pad_bits);
        bit_position += pad_bits;
    }

    pad_bits = 8 - (bit_position % 8) - flag_bit_length;
    if (pad_bits < 0) pad_bits += 8;

    append_bits_reverse(signature->buf, 0, bit_position, pad_bits);
    bit_position += pad_bits;
    append_bits_reverse(signature->buf, flag_bits, bit_position, flag_bit_length);
    bit_position += flag_bit_length;
    signature->length_ = (bit_position + 7) / 8;

    signature->buf[0] = ((uint8_t)(signature->length_));
    if(ep != -1)
        signature->buf[0] |= 1<<5;
    if(castle)
        signature->buf[0] |= 1<<6;
}

bool GameState::is_en_passant_possible()
{
    return board_.IsEnPassantPossible(active_side_, en_passant_);
}

void append_bits_reverse(uint8_t *buf, uint8_t bits, int bit_position, int num_bits)
{
    uint8_t *sig_byte = &buf[bit_position/8];
    int offset = bit_position % 8;
    for (int i=offset; i<num_bits+offset; ++i, bits>>=1)
    {
        if (bits & 1) *sig_byte |= 1 << (7-(i%8));
        if (i%8 == 7) *(++sig_byte) = 0;
    }
}

bool GameState::ByteToMove(uint8_t byte, Move *move)
{
    const char* piece_code =
        "PNxQPQPxQBKxPBRNxxBKPBxxPxQBxBxxxRBQPxBPQQNxxPBQNQBxNxNQQQBQBxxx"
        "xQQxKQxxxxPQNQxxRxRxBPxxxxxxPxxPxQPQxxBKxRBxxxRQxxBxQxxxxBRRPRQR"
        "QRPxxNRRxxNPKxQQxxQxQxPKRRQPxQxBQxQPxRxxxRxQxRQxQPBxxRxQxBxPQQKx"
        "xBBBRRQPPQBPBRxPxPNNxxxQRQNPxxPKNRxRxQPQRNxPPQQRQQxNRBxNQQQQxQQx";

    const int piece_index[256] =
    {
        5, 2, 9, 2, 2, 1, 4, 9, 2, 2, 1, 9, 1, 1, 2, 1,
        9, 9, 1, 1, 8, 1, 9, 9, 7, 9, 2, 1, 9, 2, 9, 9,
        9, 2, 2, 2, 8, 9, 1, 3, 1, 1, 2, 9, 9, 6, 1, 1,
        2, 1, 2, 9, 1, 9, 1, 1, 2, 1, 1, 2, 1, 9, 9, 9,
        9, 2, 1, 9, 1, 1, 9, 9, 9, 9, 8, 1, 2, 2, 9, 9,
        1, 9, 1, 9, 2, 3, 9, 9, 9, 9, 9, 9, 7, 9, 9, 5,
        9, 1, 2, 2, 9, 9, 1, 1, 9, 2, 1, 0, 9, 9, 1, 2,
        9, 9, 2, 9, 1, 9, 9, 9, 9, 2, 1, 2, 3, 2, 1, 1,
        1, 1, 6, 9, 9, 1, 1, 1, 9, 9, 1, 1, 1, 9, 2, 1,
        9, 9, 2, 9, 1, 9, 2, 1, 1, 1, 1, 3, 9, 1, 9, 2,
        2, 9, 1, 8, 9, 2, 9, 9, 9, 2, 9, 2, 9, 2, 2, 9,
        2, 6, 1, 9, 9, 2, 9, 1, 9, 2, 9, 5, 2, 2, 1, 9,
        9, 1, 2, 1, 2, 2, 2, 7, 7, 2, 2, 6, 2, 1, 9, 4,
        9, 2, 2, 2, 9, 9, 9, 1, 2, 1, 1, 1, 9, 9, 5, 1,
        2, 1, 9, 2, 9, 1, 4, 1, 1, 1, 9, 4, 1, 1, 2, 1,
        2, 1, 9, 2, 2, 2, 0, 1, 2, 2, 2, 2, 9, 1, 2, 9
    };

    const int forward[256] =
    {
        1,-1, 9, 0, 1, 1, 1, 9, 0, 6,-1, 9, 1, 3, 0,-1,
        9, 9, 7, 1, 1, 5, 9, 9, 1, 9, 6, 1, 9, 7, 9, 9,
        9, 0, 2, 6, 1, 9, 7, 1, 5, 0,-2, 9, 9, 1, 1, 0,
       -2, 0, 5, 9, 2, 9, 1, 4, 4, 0, 6, 5, 5, 9, 9, 9,
        9, 5, 7, 9,-1, 3, 9, 9, 9, 9, 2, 5, 2, 1, 9, 9,
        6, 9, 0, 9, 1, 1, 9, 9, 9, 9, 9, 9, 1, 9, 9, 2,
        9, 6, 2, 7, 9, 9, 3, 1, 9, 7, 4, 0, 9, 9, 0, 7,
        9, 9, 7, 9, 0, 9, 9, 9, 9, 6, 3, 6, 1, 1, 3, 0,
        6, 1, 1, 9, 9, 2, 0, 5, 9, 9,-2, 1,-1, 9, 2, 0,
        9, 9, 1, 9, 3, 9, 1, 0, 0, 4, 6, 2, 9, 2, 9, 4,
        3, 9, 2, 1, 9, 5, 9, 9, 9, 0, 9, 6, 9, 0, 3, 9,
        4, 2, 6, 9, 9, 0, 9, 5, 9, 3, 9, 1, 0, 2, 0, 9,
        9, 2, 2, 2, 0, 4, 5, 1, 2, 7, 3, 1, 5, 0, 9, 1,
        9, 1, 1, 1, 9, 9, 9, 1, 0, 2,-2, 2, 9, 9, 1, 1,
       -1, 7, 9, 3, 9, 0, 2, 4, 2,-1, 9, 1, 1, 7, 1, 0,
        0, 1, 9, 2, 2, 1, 0, 1, 0, 6, 0, 2, 9, 7, 3, 9
    };

    const int left[256] =
    {
       -1, 2, 9,-2, 0, 0, 1, 9,-4,-6, 0, 9, 1,-3,-3, 2,
        9, 9,-7, 0,-1,-5, 9, 9, 0, 9, 0, 1, 9,-7, 9, 9,
        9,-7, 2,-6, 1, 9, 7, 1,-5,-6,-1, 9, 9,-1,-1,-1,
        1,-3,-5, 9,-1, 9,-2, 0, 4,-5,-6, 5, 5, 9, 9, 9,
        9,-5, 7, 9,-1,-3, 9, 9, 9, 9, 0, 5,-1, 0, 9, 9,
        0, 9,-6, 9, 1, 0, 9, 9, 9, 9, 9, 9,-1, 9, 9, 0,
        9,-6, 0, 7, 9, 9, 3,-1, 9, 0,-4, 0, 9, 9,-5,-7,
        9, 9, 7, 9,-2, 9, 9, 9, 9, 6, 0, 0,-1, 0, 3,-1,
        6, 0, 1, 9, 9, 1,-7, 0, 9, 9,-1,-1, 1, 9, 2,-7,
        9, 9,-1, 9, 0, 9,-1, 1,-3, 0, 0, 0, 9, 0, 9, 4,
        0, 9,-2, 0, 9, 0, 9, 9, 9,-2, 9, 6, 9,-4,-3, 9,
        0, 0, 6, 9, 9,-5, 9, 0, 9,-3, 9, 0,-5, 0,-1, 9,
        9,-2,-2, 2,-1, 0, 0, 1, 0, 0, 3, 0, 5,-2, 9, 0,
        9, 1,-2, 2, 9, 9, 9, 1,-6, 2, 1, 0, 9, 9, 1, 1,
       -2, 0, 9, 0, 9,-4, 0,-4, 0,-2, 9,-1, 0,-7, 1,-4,
       -7,-1, 9, 1, 0,-1, 0, 2,-1, 0,-3,-2, 9, 0, 3, 9
    };

    bool flip_board = active_side_ == Side::kBlack;
    bool mirror_board =  !castlings_ && board_.IsKingOnTheLeftHalf(flip_board ? Side::kBlack : Side::kWhite);
    int file_from = -1, file_to = -1, rank_from = -1, rank_to = -1;

    if (byte == 107)
    {
        file_from = 4;
        file_to = 6;
        rank_from = rank_to = flip_board ? 7 : 0;

        move->move_ = (file_from + rank_from * 8) | ((file_to + rank_to * 8) << 6) | MoveFlags::kCastling;
        return true;
    }

    if (byte == 246)
    {
        file_from = 4;
        file_to = 2;
        rank_from = rank_to = flip_board ? 7 : 0;

        move->move_ = (file_from + rank_from * 8) | ((file_to + rank_to * 8) << 6) | MoveFlags::kCastling;
        return true;
    }

    PieceType pc = PieceType::KAllPieces;

    switch (piece_code[byte])
    {
        case 'P': !flip_board ? pc = PieceType::kWhitePawns : pc = PieceType::kBlackPawns; break;
        case 'N': !flip_board ? pc = PieceType::kWhiteKnights : pc = PieceType::kBlackKnights; break;
        case 'B': !flip_board ? pc = PieceType::kWhiteBishops : pc = PieceType::kBlackBishops; break;
        case 'R': !flip_board ? pc = PieceType::kWhiteRooks : pc = PieceType::kBlackBishops; break;
        case 'Q': !flip_board ? pc = PieceType::kWhiteQueens : pc = PieceType::kBlackQueens; break;
        case 'K': !flip_board ? pc = PieceType::kWhiteKing : pc = PieceType::kBlackKing; break;
        default:;
    }

    int nth_piece = piece_index[byte], piece_count = 0;
    bool found = false;

    for (int file = 0; file < 8 && !found; ++file)
    {
        for (int rank = 0; rank < 8 && !found; ++rank)
        {
            PieceType piece = board_.GetPieceOnSquare((flip_board ? 7 - rank : rank) * 8 + (mirror_board ? 7 - file : file));
            if (piece == pc)
                piece_count++;

            if (piece_count == nth_piece)
            {
                file_from = file;
                rank_from = rank;
                found = true;
            }
        }
    }

    file_to = file_from - left[byte];
    file_to = (file_to + 8) % 8;
    rank_to = rank_from + forward[byte];
    rank_to = (rank_to + 8) % 8;

    if (flip_board)
    {
        rank_from = 7 - rank_from;
        rank_to = 7 - rank_to;
    }

    if (mirror_board)
    {
        file_from = 7 - file_from;
        file_to = 7 - file_to;
    }

    move->move_ = (file_from + rank_from * 8) | ((file_to + rank_to * 8) << 6);

    if(((!flip_board && pc == PieceType::kWhitePawns) || (flip_board && pc == PieceType::kBlackPawns)) && ((flip_board && rank_from == 0 && rank_to == 1) || (!flip_board && rank_from == 6 && rank_to == 7)))
    {
        move->move_ |= (MoveFlags::kPromotion & (PromotionType::kQueen << 14));
    }

    if(((!flip_board && pc == PieceType::kWhitePawns) || (flip_board && pc == PieceType::kBlackPawns)) && ((flip_board && rank_from == 5 && rank_to == 4) || (!flip_board && rank_from == 4 && rank_to == 5)))
    {
        if(board_.GetPieceOnSquare(rank_to * 8 + file_to) == PieceType::KAllPieces && file_to != file_from)
        {
            move->move_ |= (MoveFlags::kEnPassant);
        }
    }

    return true;
}
