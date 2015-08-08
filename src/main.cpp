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
#include <vector>
#include "CsvParser.h"



int main()
{

    csvtable parsed;
    std::vector<std::string> filenames;
//    filenames.push_back("onerow_emptylines.csv");
//    filenames.push_back("onefield.csv");
//    filenames.push_back("onerow.csv");
//    filenames.push_back("test.csv");
    filenames.push_back("sol049.csv");

    while (filenames.size()) {
        std::string current("../examples/");
        current+=filenames.back();
        filenames.pop_back();
        std::ifstream file(current.c_str(), std::ios::in | std::ios::binary);
        if (!file) {
            std::cout <<"File not found: " <<current.c_str() <<std::endl;
            continue;
        }

        CsvParser csv(file);
        if (!file.is_open()) {
            std::cout <<"File " <<current.c_str() <<" not open!" <<std::endl;
            continue;
        }

        std::cout <<"Parsing " <<current.c_str() <<std::endl;
        if(!csv.parse()) {
            std::cout<<"WARNING: errors found in source data!"<<std::endl;
            std::cout<<csv.getError()<<std::endl;

            std::cout<<"Trying with another separator..."<<std::endl;
            csv.setSeparator(';');
            if (csv.parse())
                std::cout<<"Good!"<<std::endl;
            else std::cout<<csv.getError()<<std::endl;
        }

        file.close();
        std::cout<<"****++++****++++****++++\n";
        csv.getParsedData(parsed);

        csvtable::iterator it = parsed.begin();
        int rows=parsed.size();
        std::size_t columns=(*it).size();
        std::cout<<rows <<" rows, " <<columns <<" columns" <<std::endl;
        for (;it != parsed.end(); ++it) {
//                std::cout<<(*it).at(0)<<std::endl;
        }
    }
    return 0;
}

