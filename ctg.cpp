#include "bits_functions.h"
#include "ctg.h"
#include "logger.h"

#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

#define read_24(buf, pos)   \
    ((buf[pos]<<16) + (buf[(pos)+1]<<8) + (buf[(pos)+2]))
#define read_32(buf, pos)   \
    ((buf[pos]<<24) + (buf[pos+1]<<16) + (buf[(pos)+2]<<8) + (buf[(pos+3)+2]))

CTGReader::CTGReader(std::string name) : name_(name)
{
    if(!name_.empty())
    {
        ctg_file_stream_ = std::make_unique<std::ifstream>();

        if(ctg_file_stream_)
            ctg_file_stream_->open(name_ + ".ctg", std::ios::in | std::ios::binary);

        if( !ctg_file_stream_ || !ctg_file_stream_->is_open() )
        {
            std::cerr << "Could not open the file: " << name_ + ".ctg" << std::endl;
            exit (EXIT_FAILURE);
        }
        else
            ctg_input_ = ctg_file_stream_.get();

        ctb_file_stream_ = std::make_unique<std::ifstream>();

        if(ctb_file_stream_)
            ctb_file_stream_->open(name_ + ".ctb", std::ios::in | std::ios::binary);

        if( !ctb_file_stream_ || !ctb_file_stream_->is_open() )
        {
            std::cerr << "Could not open the file: " << name_ + ".ctb" << std::endl;
            exit (EXIT_FAILURE);
        }
        else
            ctb_input_ = ctb_file_stream_.get();

        cto_file_stream_ = std::make_unique<std::ifstream>();

        if(cto_file_stream_)
            cto_file_stream_->open(name_ + ".cto", std::ios::in | std::ios::binary);

        if( !cto_file_stream_ || !cto_file_stream_->is_open() )
        {
            std::cerr << "Could not open the file: " << name_ + ".cto" << std::endl;
            exit (EXIT_FAILURE);
        }
        else
            cto_input_ = cto_file_stream_.get();

        ctb_input_->read((char*)&bounds_, 12);

        bounds_.low = Flip32bitsVertically(bounds_.low);
        bounds_.high = Flip32bitsVertically(bounds_.high);
    }
}

int32_t CTGReader::ConvertSignatureToHash(Signature *signature)
{
    static const uint32_t hash_bits[64] =
    {
        0x3100d2bf, 0x3118e3de, 0x34ab1372, 0x2807a847,
        0x1633f566, 0x2143b359, 0x26d56488, 0x3b9e6f59,
        0x37755656, 0x3089ca7b, 0x18e92d85, 0x0cd0e9d8,
        0x1a9e3b54, 0x3eaa902f, 0x0d9bfaae, 0x2f32b45b,
        0x31ed6102, 0x3d3c8398, 0x146660e3, 0x0f8d4b76,
        0x02c77a5f, 0x146c8799, 0x1c47f51f, 0x249f8f36,
        0x24772043, 0x1fbc1e4d, 0x1e86b3fa, 0x37df36a6,
        0x16ed30e4, 0x02c3148e, 0x216e5929, 0x0636b34e,
        0x317f9f56, 0x15f09d70, 0x131026fb, 0x38c784b1,
        0x29ac3305, 0x2b485dc5, 0x3c049ddc, 0x35a9fbcd,
        0x31d5373b, 0x2b246799, 0x0a2923d3, 0x08a96e9d,
        0x30031a9f, 0x08f525b5, 0x33611c06, 0x2409db98,
        0x0ca4feb2, 0x1000b71e, 0x30566e32, 0x39447d31,
        0x194e3752, 0x08233a95, 0x0f38fe36, 0x29c7cd57,
        0x0f7b3a39, 0x328e8a16, 0x1e7d1388, 0x0fba78f5,
        0x274c7e7c, 0x1e8be65c, 0x2fa0b0bb, 0x1eb6c371
    };

    int32_t hash = 0;
    int16_t tmp = 0;

    for (int i=0; i < signature->length_; ++i)
    {
        int8_t byte = signature->buf[i];
        tmp += ((0x0f - (byte & 0x0f)) << 2) + 1;
        hash += hash_bits[tmp & 0x3f];
        tmp += ((0xf0 - (byte & 0xf0)) >> 2) + 1;
        hash += hash_bits[tmp & 0x3f];
    }

    return hash;
}

bool CTGReader::GetEntry(int page_index, Signature *signature, Entry *entry)
{
    // Pages are a uniform 4096 bytes.
    uint8_t buf[4096];

    uint64_t idx = 4096*(static_cast<uint64_t>(page_index) + 1);

    ctg_input_->seekg(idx);

    if (!ctg_input_->read((char*)buf, 4096))
        return false;

    int num_positions = (buf[0]<<8) + buf[1];

    int pos = 4;
    for (int i=0; i<num_positions; ++i)
    {
        int entry_size = buf[pos] % 32;
        bool equal = true;
        if (signature->length_ != entry_size) equal = false;

        for (int j=0; j < signature->length_ && equal; ++j)
        {
            if (buf[pos+j] != signature->buf[j]) equal = false;
        }
        if (!equal) {
            pos += entry_size + buf[pos+entry_size] + 33;
            continue;
        }

        pos += entry_size;
        entry_size = buf[pos];
        for (int j=1; j<entry_size; ++j) entry->moves[j-1] = buf[pos+j];
        entry->num_moves = (entry_size - 1)/2;
        pos += entry_size;
        entry->total = read_24(buf, pos);
        pos += 3;
        entry->losses = read_24(buf, pos);
        pos += 3;
        entry->wins = read_24(buf, pos);
        pos += 3;
        entry->draws = read_24(buf, pos);
        pos += 3;
        entry->unknown1 = read_32(buf, pos);
        pos += 4;
        entry->avg_rating_games = read_24(buf, pos);
        pos += 3;
        entry->avg_rating_score = read_32(buf, pos);
        pos += 4;
        entry->perf_rating_games = read_24(buf, pos);
        pos += 3;
        entry->perf_rating_score = read_32(buf, pos);
        pos += 4;
        entry->recommendation = buf[pos];
        pos += 1;
        entry->unknown2 = buf[pos];
        pos += 1;
        entry->comment = buf[pos];
        return true;
    }

    return false;
}

int CTGReader::GetPageIndex(int32_t hash)
{
    uint32_t key = 0;
    int page_index;

    for (int mask = 1; key <= (uint32_t)bounds_.high; mask = (mask << 1) + 1)
    {
        key = (hash & mask) + mask;
        if (key >= (uint32_t)bounds_.low)
        {
            cto_input_->seekg(16 + key*4);
            cto_input_->read((char*)&page_index, 4);
            page_index = Flip32bitsVertically(page_index);

            if(page_index >= 0)
                break;
        }
    }

    return page_index;
}

CTGReader::~CTGReader()
{
    ctg_file_stream_->close();

    if(ctg_file_stream_->is_open())
    {
        std::cerr << "Could not close the file" << std::endl;
        exit (EXIT_FAILURE);
    }

    ctb_file_stream_->close();

    if(ctb_file_stream_->is_open())
    {
        std::cerr << "Could not close the file" << std::endl;
        exit (EXIT_FAILURE);
    }

    cto_file_stream_->close();

    if(cto_file_stream_->is_open())
    {
        std::cerr << "Could not close the file" << std::endl;
        exit (EXIT_FAILURE);
    }
}

bool CTGReader::GetMove(GameState *state, Signature *signature, Move *move)
{
    Entry entry;

    int page_index = GetPageIndex(ConvertSignatureToHash(signature));

    if(page_index >= 0 && GetEntry(page_index, signature, &entry))
    {

        if(PickMove(state, &entry, move))
        {
            return true;
        }
    }

    return false;
}



bool CTGReader::PickMove(GameState *state, Entry *entry, Move *move)
{
    Move moves[50];
    int64_t weights[50];
    bool recommended[50];
    int64_t total_weight = 0;
    bool have_recommendations = false;

    for (int i=0; i<2*entry->num_moves; i += 2)
    {
        uint8_t byte = entry->moves[i];
        if( state->ByteToMove(byte, &moves[i/2]) )
        {
            weights[i/2] = MoveWeight(state, moves[i/2], entry->moves[i+1], &recommended[i/2]);

            if (recommended[i/2])
                have_recommendations = true;
        }
        else
            break;
    }

    for (int i = 0; i < entry->num_moves; ++i)
    {
        if( have_recommendations && !recommended[i] )
            weights[i] = 0;

        total_weight += weights[i];
        weights[i] = total_weight;
    }

    if (total_weight == 0)
        return false;

    int64_t nw = total_weight + 50;

    std::mt19937_64 rng;
    rng.seed(std::random_device{}());

    if(int64_t(rng() % nw) > total_weight)
        return false;

    int64_t choice = rng() % total_weight;
    int64_t i;

    for (i=0; choice >= weights[i]; ++i) {}

    *move = moves[i];

    return true;
}

int64_t CTGReader::MoveWeight(GameState *state, Move move, uint8_t annotation, bool *recommended)
{
    int64_t weight = 0;

    std::string fen_before = state->GetFen();

    if(state->GetMoves()->MakeMove(move.move_))
    {
        Entry entry;
        Signature signature;
        state->getSignature(&signature);

        int page_index = GetPageIndex(ConvertSignatureToHash(&signature));

        if(page_index >= 0 && GetEntry(page_index, &signature, &entry))
        {
            *recommended = false;
            int64_t half_points = 2*entry.wins + entry.draws;
            int64_t games = entry.wins + entry.draws + entry.losses;
            weight = (games < 3) ? 0 : half_points * half_points / games;
            if (entry.recommendation == 64) weight = 0;
            if (entry.recommendation == 128) *recommended = true;

            switch (annotation)
            {
                case 0x01: weight *=  8; break;                         //  !
                case 0x02: weight  =  0; *recommended = false; break;   //  ?
                case 0x03: weight *= 32; break;                         // !!
                case 0x04: weight  =  0; *recommended = false; break;   // ??
                case 0x05: weight /=  2; *recommended = false; break;   // !?
                case 0x06: weight /=  8; *recommended = false; break;   // ?!
                case 0x08: weight = INT32_MAX; break;                   // Only move
                case 0x16: break;                                       // Zugzwang
                default: break;
            }
        }

        state->GetMoves()->UnmakeMove(move.move_);
    }

    return weight;
}
