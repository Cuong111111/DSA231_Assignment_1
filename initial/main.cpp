#include<fstream>
#include "main.h"

#include "Restaurant.cpp"
void simulate(string filename, imp_res* r)
{
    ifstream ss(filename);
    string str, maxsize, name, energy, num;
    int i = 1;
    while(ss >> str)
    {
        if(str == "MAXSIZE")
        {
            ss >> maxsize;
            MAXSIZE = stoi(maxsize);
        }
        else if(str == "RED") // RED <NAME> <ENERGY>
        {
            ss >> name;
            ss >> energy;
            r->RED(name, stoi(energy));
        }

        else if(str == "BLUE") // BLUE <NUM>
        {
            ss >> num;
            r->BLUE(stoi(num));
        }
        else if(str == "PURPLE") // PURPLE
        {
            r->PURPLE();
        }
        else if(str == "REVERSAL") // REVERSAL
        {
            r->REVERSAL();
        }
        else if(str == "UNLIMITED_VOID") // UNLIMITED_VOID
        {
            cout << str << " " << "line " << i << endl;
            r->UNLIMITED_VOID();
        }
        else if(str == "DOMAIN_EXPANSION") // DOMAIN_EXPANSION
        {
            cout << str << " " << "line " << i << endl;
            r->DOMAIN_EXPANSION();
        }
        else // LIGHT <NUM>
        {
            ss >> num;
            cout << str << " " << num << " " << "line " << i << endl;
            r->LIGHT(stoi(num));
        }
        i++;
    }
}

int main(int argc, char* argv[]) {
    //if (argc < 2)
    //	return 1;
    //  string fileName = argv[1];
    imp_res* r = new imp_res();
    string fileName = "test.txt";
    simulate(fileName, r);
    delete r;
    return 0;
}