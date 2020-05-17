#include "bits_functions.h"
#include "board.h"
#include "constants.h"
#include "logger.h"

#include <algorithm>
#include <bitset>
#include <iostream>

Board::Board()
{

}

char Board::GetPieceSymbolAtPosition(std::size_t position) const
{
    return (white_pawns_ & GetBitSet(position) ? 'P' :
           (white_rooks_ & white_bishops_ & GetBitSet(position) ? 'Q' :
           (white_bishops_ & GetBitSet(position) ? 'B' :
           (white_knights_ & GetBitSet(position) ? 'N' :
           (white_rooks_ & GetBitSet(position) ? 'R' :
           (white_king_ & GetBitSet(position) ? 'K' :
           (black_pawns_ & GetBitSet(position) ? 'p' :
           (black_rooks_ & black_bishops_ & GetBitSet(position) ? 'q' :
           (black_bishops_ & GetBitSet(position) ? 'b' :
           (black_knights_ & GetBitSet(position) ? 'n' :
           (black_rooks_ & GetBitSet(position) ? 'r' :
           (black_king_ & GetBitSet(position) ? 'k' : 0))))))))))));
}

bool Board::SetFen(const std::string &fen_string)
{
    std::size_t row = 0;
    std::size_t column = 0;

    bool result = true;

    black_pawns_ = black_knights_  = black_bishops_ =
            black_rooks_  = black_king_ = 0;
    white_pawns_ = white_knights_  = white_bishops_  =
            white_rooks_ = white_king_ = 0;

    for(auto&& symbol : fen_string)
    {
        if(symbol >= '1' && symbol <= '8')
            column += static_cast<std::size_t>(symbol) - '1' + 1;
        else if(symbol == '/')
        {
            if(column == kBoardSize)
            {
                column = 0;
                ++row;
            }
            else
            {
                result = false;
                break;
            }

            if(row >= kBoardSize)
            {
                result = false;
                break;
            }
        }
        else if(symbol == ' ')
            break;
        else
        {
            std::size_t bit_number = (kBoardSize - 1 - row) * kBoardSize + column;

            switch(symbol)
            {
                case 'p': black_pawns_ |= GetBitSet(bit_number); ++black_pawns_count; break;
                case 'n': black_knights_ |= GetBitSet(bit_number); ++black_knights_count; break;
                case 'b': black_bishops_ |= GetBitSet(bit_number); ++black_bishops_count; break;
                case 'r': black_rooks_ |= GetBitSet(bit_number); ++black_rooks_count; break;
                case 'q': black_rooks_ |= GetBitSet(bit_number);
                          black_bishops_ |= GetBitSet(bit_number); ++black_queens_count; break;
                case 'k': black_king_ |= GetBitSet(bit_number); break;
                case 'P': white_pawns_ |= GetBitSet(bit_number); ++white_pawns_count; break;
                case 'N': white_knights_ |= GetBitSet(bit_number); ++white_knights_count; break;
                case 'B': white_bishops_ |= GetBitSet(bit_number); ++white_bishops_count; break;
                case 'R': white_rooks_ |= GetBitSet(bit_number); ++white_rooks_count; break;
                case 'Q': white_rooks_ |= GetBitSet(bit_number);
                          white_bishops_ |= GetBitSet(bit_number); ++white_queens_count; break;
                case 'K': white_king_ |= GetBitSet(bit_number); break;
                default : result = false; break;
            }

            if(result)
                ++column;
            else
                break;
        }
    }

    if(result)
    {
        whites_ = white_pawns_ | white_knights_ | white_bishops_ | white_rooks_ | white_king_;
        blacks_ = black_pawns_ | black_knights_ | black_bishops_ | black_rooks_ | black_king_;
        occupied_ = whites_ | blacks_;
        empty_ = ~occupied_;
    }

//    std::cout << black_pawns_ << std::endl;

//    Logger lg;
//    lg << black_pawns_;

    return result;
}

std::string Board::GetFen() const
{
    std::string fen_string;

    std::bitset<64> binary_number(occupied_);
    std::size_t empty_in_row = 0;

    std::size_t row = 0;
    std::size_t column = 0;
    std::size_t bit_number = 0;

    while(row < kBoardSize)
    {
        bit_number = (kBoardSize - 1 - row) * kBoardSize + column;

        if(GetBitSet(bit_number) & occupied_)
        {
            if(empty_in_row)
                fen_string.append(std::to_string(empty_in_row));

            fen_string.append(1, GetPieceSymbolAtPosition(bit_number));
            empty_in_row = 0;
        }
        else
            ++empty_in_row;

        if(!((bit_number + 1) % kBoardSize))
        {
            if(empty_in_row)
            {
                fen_string.append(std::to_string(empty_in_row));
                empty_in_row = 0;
            }

            if(bit_number + 1 != kBoardSize)
                fen_string.append(1, '/');
        }

        if(++column == kBoardSize)
        {
            ++row;
            column = 0;
        }
    }

    return fen_string;
}

bool Board::IsSufficientMaterial() const
{
    if(black_pawns_count == 0 && black_queens_count == 0 && black_rooks_count == 0 &&
            white_pawns_count == 0 && white_queens_count == 0 && white_rooks_count == 0)
    {
        return white_bishops_count >= 2 || (white_bishops_ == 1 && white_knights_ >= 1) || white_knights_count >= 3 ||
                black_bishops_count >= 2 || (black_bishops_ == 1 && black_knights_ >= 1) || black_knights_count >= 3;
    }

    return true;
}

void Board::AddPiece(PieceType piece)
{
    switch(piece)
    {
    case PieceType::kWhiteQueens:
        ++white_queens_count;
        break;
    case PieceType::kBlackQueens:
        ++black_queens_count;
        break;
    case PieceType::kWhiteRooks:
        ++white_rooks_count;
        break;
    case PieceType::kBlackRooks:
        ++black_rooks_count;
        break;
    case PieceType::kWhiteBishops:
        ++white_bishops_count;
        break;
    case PieceType::kBlackBishops:
        ++black_bishops_count;
        break;
    case PieceType::kWhiteKnights:
        ++white_knights_count;
        break;
    case PieceType::kBlackKnights:
        ++black_knights_count;
        break;
    default:
        break;
    }
}

void Board::RemovePiece(PieceType piece)
{
    switch(piece)
    {
    case PieceType::kWhitePawns:
        --white_pawns_count;
        break;
    case PieceType::kBlackPawns:
        --black_pawns_count;
        break;
    case PieceType::kWhiteQueens:
        --white_queens_count;
        break;
    case PieceType::kBlackQueens:
        --black_queens_count;
        break;
    case PieceType::kWhiteRooks:
        --white_rooks_count;
        break;
    case PieceType::kBlackRooks:
        --black_rooks_count;
        break;
    case PieceType::kWhiteBishops:
        --white_bishops_count;
        break;
    case PieceType::kBlackBishops:
        --black_bishops_count;
        break;
    case PieceType::kWhiteKnights:
        --white_knights_count;
        break;
    case PieceType::kBlackKnights:
        --black_knights_count;
        break;
    default:
        break;
    }
}

bool Board::operator!=(const Board &b) const
{
    return b.occupied_ != occupied_ ||
            b.whites_ != whites_ ||
            b.white_pawns_ != white_pawns_ ||
            b.white_bishops_ != white_bishops_ ||
            b.white_knights_ != white_knights_ ||
            b.white_rooks_ != white_rooks_ ||
            b.white_king_ != white_king_ ||
            b.blacks_ != blacks_ ||
            b.black_pawns_ != black_pawns_ ||
            b.black_bishops_ != black_bishops_ ||
            b.black_knights_ != black_knights_ ||
            b.black_rooks_ != black_rooks_ ||
            b.black_king_ != black_king_;
}

PieceType Board::GetPieceOnSquare(std::size_t square) const
{
    uint64_t bit_board = GetBitSet(square);

    if(bit_board & black_pawns_)
        return PieceType::kBlackPawns;

    if(bit_board & black_bishops_)
    {
        if(bit_board & black_rooks_)
            return PieceType::kBlackQueens;
        else
            return PieceType::kBlackBishops;
    }

    if(bit_board & black_rooks_)
        return PieceType::kBlackRooks;

    if(bit_board & black_knights_)
        return PieceType::kBlackKnights;

    if(bit_board & black_king_)
        return PieceType::kBlackKing;

    if(bit_board & white_pawns_)
        return PieceType::kWhitePawns;

    if(bit_board & white_bishops_)
    {
        if(bit_board & white_rooks_)
            return PieceType::kWhiteQueens;
        else
            return PieceType::kWhiteBishops;
    }

    if(bit_board & white_rooks_)
        return PieceType::kWhiteRooks;

    if(bit_board & white_knights_)
        return PieceType::kWhiteKnights;

    if(bit_board & white_king_)
        return PieceType::kWhiteKing;

    return PieceType::KAllPieces;
}

void Board::InvertPieces()
{
    std::swap(blacks_, whites_);
    std::swap(black_pawns_, white_pawns_);
    std::swap(black_knights_, white_knights_);
    std::swap(black_bishops_, white_bishops_);
    std::swap(black_rooks_, white_rooks_);
    std::swap(black_king_, white_king_);
}

void Board::FlipVertically()
{
     whites_ = FlipBitboardVertically(whites_);
     white_pawns_ = FlipBitboardVertically(white_pawns_);
     white_knights_ = FlipBitboardVertically(white_knights_);
     white_bishops_ = FlipBitboardVertically(white_bishops_);
     white_rooks_ = FlipBitboardVertically(white_rooks_);
     white_king_ = FlipBitboardVertically(white_king_);

     blacks_ = FlipBitboardVertically(blacks_);
     black_pawns_ = FlipBitboardVertically(black_pawns_);
     black_knights_ = FlipBitboardVertically(black_knights_);
     black_bishops_ = FlipBitboardVertically(black_bishops_);
     black_bishops_ = FlipBitboardVertically(black_rooks_);
     black_king_ = FlipBitboardVertically(black_king_);

     occupied_ = FlipBitboardVertically(occupied_);
     empty_ = FlipBitboardVertically(empty_);
}

void Board::FlipHorizontally()
{
    whites_ = FlipBitboardHorizontally(whites_);
    white_pawns_ = FlipBitboardHorizontally(white_pawns_);
    white_knights_ = FlipBitboardHorizontally(white_knights_);
    white_bishops_ = FlipBitboardHorizontally(white_bishops_);
    white_rooks_ = FlipBitboardHorizontally(white_rooks_);
    white_king_ = FlipBitboardHorizontally(white_king_);

    blacks_ = FlipBitboardHorizontally(blacks_);
    black_pawns_ = FlipBitboardHorizontally(black_pawns_);
    black_knights_ = FlipBitboardHorizontally(black_knights_);
    black_bishops_ = FlipBitboardHorizontally(black_bishops_);
    black_bishops_ = FlipBitboardHorizontally(black_rooks_);
    black_king_ = FlipBitboardHorizontally(black_king_);

    occupied_ = FlipBitboardHorizontally(occupied_);
    empty_ = FlipBitboardHorizontally(empty_);
}

bool Board::IsKingOnTheLeftHalf(Side side) const
{
    return (side == Side::kWhite ? white_king_ : black_king_) & kLeftHalf;
}

bool Board::IsEnPassantPossible(Side side, uint64_t en_passant) const
{
    bool result = false;

    if( side == Side::kWhite )
    {
        result |= (white_pawns_ << kMoveRight) & kEmptyLeft & en_passant;
        result |= (white_pawns_ << kMoveLeft) & kEmptyRight & en_passant;
    }
    else
    {
        result |= (black_pawns_ >> kMoveRight) & kEmptyRight & en_passant;
        result |= (black_pawns_ >> kMoveLeft) & kEmptyLeft & en_passant;
    }

    return result;
}

std::string Board::GetPositionFromBitBoard(const uint64_t &bb) const
{
    std::string position;
    std::size_t bit_number = GetLSBPos(bb);

    position.append(1, char('a' + bit_number % 8));
    position.append(1, char('1' + bit_number / 8));

    return position;
}
