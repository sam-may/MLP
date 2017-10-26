#include "TString.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
using namespace std;

class Parser
{
    public:
        // Map to hold the row number to mlp outputs
        std::map<int, float> map_mlp;

        // Open the file and parse the mlp outputs to map;
        void load(TString filename)
        {
            string line;
            ifstream myfile(filename.Data());
            int eventid;
            float mlpoutput;
            if (myfile.is_open())
            {
                while (getline(myfile, line))
                {
                    std::stringstream ss(line);
                    ss >> eventid >> mlpoutput;
                    map_mlp[eventid] = mlpoutput;
                }
            }
        }

        // Constructor/destructor
        Parser(TString filename) { load(filename); }
        ~Parser() {}

        // Accessing values
        float mlp(int eventid)
        {
            std::pair<std::map<int, float>::iterator, bool> ret;
            ret = map_mlp.insert( {eventid, -999} );
            if (ret.second == false)
                return map_mlp[eventid];
            else
                return -999;
        }


};
