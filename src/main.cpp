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
    filenames.push_back("test.csv");

    while (filenames.size()) {
        std::string current("../examples/");
        current+=filenames.back();
        filenames.pop_back();
        std::ifstream file(current.c_str(), std::ios::in);
        std::cout <<"Filepath: " <<current.c_str() <<std::endl;
        if (!file)
            return 1;

        CsvParser csv(file);
        if (!file.is_open()) {
            std::cout <<"File " <<current.c_str() <<" is closed!" <<std::endl;
        }
        std::cout <<"Parsing " <<current.c_str() <<std::endl;
        csv.parse();
        file.close();

        std::cout<<"****++++****++++****++++\n";
        if (!csv.getParsedData(parsed))
            std::cout<<"errors are present in source data"<<std::endl;

        csvtable::iterator it = parsed.begin();
        int rows=parsed.size();
        size_t columns=(*it).size();
        int cr=0;
        std::cout<<rows <<" rows, " <<columns <<" columns" <<std::endl;
        for (;it != parsed.end(); ++it) {
            cr++;
//            if ((*it).size() != columns) {
                std::cout<<(*it).at(0)<<std::endl;
//            }
        }
    }
    return 0;
}

//best fast method to read from file to a string:
//std::string get_file_contents(const char *filename)
//{
//  std::ifstream in(filename, std::ios::in | std::ios::binary);
//  if (in)
//  {
//    std::string contents;
//    in.seekg(0, std::ios::end);
//    contents.resize(in.tellg());
//    in.seekg(0, std::ios::beg);
//    in.read(&contents[0], contents.size());
//    in.close();
//    return(contents);
//  }
//  throw(errno);
//}
