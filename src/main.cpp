/*
* #  Copyright (C) 2015 Enrico Mariotti <enricomariotti@yahoo.it>
* #
* #  This program is free software: you can redistribute it and/or modify
* #  it under the terms of the GNU General Public License as published by
* #  the Free Software Foundation, either version 2 or (at your option)
* #  version 3 of the License.
* #
* #  This program is distributed in the hope that it will be useful,
* #  but WITHOUT ANY WARRANTY; without even the implied warranty of
* #  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* #  GNU General Public License for more details.
* #
* #  You should have received a copy of the GNU General Public License
* #  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <fstream>
#include <iostream>
#include <deque>
#include "CsvParser.h"

void tryParse(std::ifstream &file);
csvtable parsed;

int main()
{

    std::deque<std::string> filenames;
/*    filenames.push_back("u_diffcolumns.csv");
    filenames.push_back("d_diffcolumns.csv");

    filenames.push_back("u_sol049.csv");
    filenames.push_back("d_sol049.csv");

    filenames.push_back("u_corner.csv");
    filenames.push_back("d_corner.csv");

    filenames.push_back("u_onerow.csv");
    filenames.push_back("d_onerow.csv");

    filenames.push_back("u_onefield.csv");
    filenames.push_back("d_onefield.csv");

    filenames.push_back("u_emptylines.csv");
    filenames.push_back("d_emptylines.csv");

    filenames.push_back("u_empty.csv");
    filenames.push_back("d_empty.csv");
*/
    filenames.push_back("u_malquoted.csv");
    filenames.push_back("d_malquoted.csv");

    while (filenames.size()) {
        std::string current("../examples/");
        current+=filenames.front();
        filenames.pop_front();

        std::ifstream file(current.c_str(), std::ios::in | std::ios::binary);

        if (!file) {
            std::cout <<"File not found: " <<current.c_str() <<std::endl;
            continue;
        }

        if (!file.is_open()) {
            std::cout <<"File " <<current.c_str() <<" not open!" <<std::endl;
            continue;
        }

        std::cout <<"\nParsing " <<current.c_str() <<std::endl;
        tryParse(file);
        file.close();
    }
    return 0;
}

void tryParse(std::ifstream& file)
{

    CsvParser csv(file);
    csv.setSeparator(',');
    if(!csv.parse()) {
        std::cout<<"Errors found in source data!\n"
                  <<csv.getError()<<std::endl
                 <<"Let's try with another separator... [;]"<<std::endl;
        csv.setSeparator(';');
        if (csv.parse())
            std::cout<<"Good at second shot!"<<std::endl;
        else std::cout<<csv.getError()<<std::endl;
    }
    else std::cout<<"No formal errors; verify that you expected:"<<std::endl;

    csv.getParsedData(parsed);

    csvtable::iterator it = parsed.begin();
    int rows=parsed.size();
    if (rows == 0)
        return;
    std::size_t columns=(*it).size();
    std::cout<<rows <<" rows, " <<columns <<" columns" <<std::endl;
    for (;it != parsed.end(); ++it) {
       //process data
    }
}
