#include<takeapoll.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using namespace eosio;
using namespace std;
using namespace simtheorist;

class takeapoll : public contract
{
    using contract::contract;

    public:
    takeapoll(account_name self) : contract(self), _polls(_self, _self), _polloptions(_self, _self), _votes(_self, self){}

    // @abi action
    void savevote(name sender, string& voter, string& pollName, string& vote)
    {
        string keyVal = pollName + "_" + voter;
        // Make sure this poll exists
        auto p = _polls.find(stn(pollName));
        if (p != _polls.end())
        {
            // Make sure this isn't a duplicate vote (same poll and voter)
            auto v = _votes.find(stn(keyVal));
            if (v == _votes.end())
            {
                // Make sure this is a valid option for this poll
                string optionKey = p->pollName + "_" + vote;
                auto po = _polloptions.find(stn(optionKey));
                if (po != _polloptions.end())
                {                
                    _votes.emplace(sender, [&](auto& row)
                    {
                        row.voteKey = stn(keyVal);
                        row.sender = sender;
                        row.voter = voter;
                        row.pollName = pollName;
                        row.vote = vote;
                    } );
                }
                else { print("Not a Valid Option"); }
            }
            else { print("Vote Already Cast"); }
        }
        else { print("Poll Not Found"); }

    }

    // @abi action
    void saveoption(name sender, string& pollName, string& option)
    {
        require_auth(sender);

        string keyVal = pollName + "_" + option;

        auto i = _polls.find(stn(pollName));
        if (i != _polls.end())
        {
            name pollSender = i->sender;
            eosio_assert(sender == pollSender, "Only the poll creator can add valid options");

            string pn = "PollName: " + i->pollName;
            print(pn.data());

            auto j = _polloptions.find(stn(keyVal));
            if (j == _polloptions.end())
            {
                _polloptions.emplace(sender, [&](auto& optrow )
                {
                    optrow.sender = sender;
                    optrow.pollName = pollName;
                    optrow.option = option;
                    optrow.optionKey = stn(keyVal);
                } );
            }
            else { print(" Poll Option Already Exists"); }
            
        }
        else { print(" Poll Not Found"); }

    }

    // @abi action
    void deleteoption(name sender, string& pollName, string& option)
    {
        print("Checking Auth   ");
        require_auth(sender);
        string keyVal = pollName + "_" + option;

        auto po = _polloptions.find(stn(keyVal));
        if (po != _polloptions.end())
        { 
            _polloptions.erase(po);         
        }
        else { print("Poll Option Not Found"); }
    }

    // @abi action
    void deletepoll (name sender, string& pollName)
    {
        print("Checking Auth   ");
        require_auth(sender);

        auto i = _polls.find(stn(pollName));
        if (i != _polls.end())
        { _polls.erase(i); }
        else { print("Poll Not Found"); }    
    }
    
    // @abi action
    void savepoll (name sender, string& pollName, uint8_t isOpen, uint8_t isPublic, uint8_t isActive, 
        uint8_t pollType)
    {
        require_auth(sender);
        
        auto i = _polls.find(stn(pollName));
        if (i == _polls.end())
        {
            _polls.emplace(sender, [&](auto& row ) 
            {
                row.pollKey = stn(pollName);
                row.pollName = pollName;
                row.sender = sender;
                row.isOpen = isOpen;
                row.isPublic = isPublic;
                row.isActive = isActive;
                row.pollType = pollType;
            });
        }
        else 
        {
            _polls.modify (i, 0, [&](auto& row)
            { 
                row.sender = sender;
                row.isOpen = isOpen;
                row.isPublic = isPublic;
                row.isActive = isActive;
                row.pollType = pollType;
            } );
        }
        print (stn(pollName));
    };

    //multi_index<N(polls), polls< indexed_by<N(pollName)>, const_mem_fun<polls, uint64_t, &polls::by_pollName> _polls;
    // typedef eosio::multi_index< N(polls), polls,
    //      indexed_by< pollName, const_mem_fun<polls, uint64_t, &polls::by_pollName> >
    //   > _polls;
    void abihack(){}

private:
    
    multi_index<N(polls), polls> _polls;
    multi_index<N(polloptions), polloptions> _polloptions;
    multi_index<N(votes), votes> _votes;

    static uint64_t stn(const std::string& s) {
        return eosio::string_to_name(s.c_str());
    };

/*
    static constexpr uint64_t stn( string& str ) {

      uint32_t len = 0;
      while( str[len] ) ++len;

      uint64_t value = 0;

      for( uint32_t i = 0; i <= 12; ++i ) {
         uint64_t c = 0;
         if( i < len && i <= 12 ) c = uint64_t(char_to_symbol( str[i] ));

         if( i < 12 ) {
            c &= 0x1f;
            c <<= 64-5*(i+1);
         }
         else {
            c &= 0x0f;
         }

         value |= c;
      }
      return value;
   }
*/
};


//EOSIO_ABI (takeapoll, (savepoll)(abihack))
EOSIO_ABI (takeapoll, (savepoll)(deletepoll)(saveoption)(deleteoption)(savevote))
