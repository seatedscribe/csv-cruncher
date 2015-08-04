/*
 *  Copyright (C) 2015 Enrico Mariotti <enricomariotti@yahoo.it>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <string>

class CsvParser {
public:
    CsvParser(std::istream &data);
    virtual ~CsvParser() {}
    int numRows();

private:
    std::istream& in;
    char ch;
    void parseCsvFile();
    void parseCsvRecord();
    void parseCsvStringList();
    bool isFieldTerminator(char c);
    bool isSpace(char c);
    void parseOptionalSpaces();
    void parseRawString();
    void parseRawField();
    std::string parseQuotedField();
    std::string parseEscapedField();
};

#endif //CSVPARSER_H
