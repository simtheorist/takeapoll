#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using namespace eosio;
using namespace std;

//typedef eosio::generic_currency<eosio::token<N(my.currenc)
namespace simtheorist
{
    //enum votetype {truefalse = 0, multichoice = 1};

    //@abi table
    struct votes
    {
        uint64_t voteKey;
        name sender;
        string pollName;
        string voter;
        string vote;

        uint64_t primary_key() const { return voteKey; }
        EOSLIB_SERIALIZE(votes, (voteKey)(sender)(pollName)(voter)(vote))
    };

    // @abi table
    struct polls
    {
        uint64_t pollKey;
        name sender;
        string pollName;
        uint8_t isOpen;
        uint8_t isPublic;
        uint8_t isActive;
        uint8_t pollType;

        uint64_t primary_key() const { return pollKey; }
        EOSLIB_SERIALIZE(polls, (pollKey)(sender)(pollName)(isOpen)(isPublic)(isActive)(pollType))
        
    };   

    // @abi table
    struct polloptions
    {
        uint64_t optionKey;
        name sender;        
        string pollName;
        string option;
        
        uint64_t primary_key() const { return optionKey; }
        EOSLIB_SERIALIZE(polloptions, (optionKey)(sender)(pollName)(option))
    };
}

    
    


