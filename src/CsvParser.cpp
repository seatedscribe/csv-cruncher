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

#include <iostream>
#include <fstream>
#include "CsvParser.h"


namespace CsvConst {
    char delimiter=',';
    char E_O_F='\0';
}

CsvParser::CsvParser(std::istream& data)
    : in(data)
{
    parseCsvFile();
}

void CsvParser::parseCsvFile()
{
    int i=0;
    while (in.peek() != CsvConst::E_O_F) {
        parseCsvRecord();
        std::cout << i++<<std::endl;
    }
}

void CsvParser::parseCsvRecord()
{
    parseCsvStringList();

    in >> ch;
    std::cout <<"record: " <<std::hex <<ch <<std::endl;
    if (ch == CsvConst::E_O_F) {
        in.unget();
        ch = '\n';
        std::cout <<"unget: " <<std::hex << ch <<std::endl;
    }
    std::cout <<"after: " << std::hex << ch <<std::endl;
    if (ch != '\n') {
        std::cout << "End of record was expected but more data exists." <<std::endl;
        return;
    }
}

void CsvParser::parseCsvStringList()
{
    do {
        parseRawString();
        in >> ch;
        std::cout << ch <<std::endl;
    } while (ch == CsvConst::delimiter);
    in.unget();
}

bool CsvParser::isFieldTerminator(char c)
{
    return ((c == CsvConst::delimiter) || (c == '\n') || (c == CsvConst::E_O_F));
}

bool CsvParser::isSpace(char c)
{
    return ((c == ' ') | (c == '\t'));
}

void CsvParser::parseOptionalSpaces()
{
    do {
        in >> ch;
    } while (isSpace(ch));
    in.unget();
}

void CsvParser::parseRawString()
{
    parseOptionalSpaces();
    parseRawField();
    if (!isFieldTerminator(in.peek()))
        parseOptionalSpaces();
}

void CsvParser::parseRawField()
{
    std::string fieldValue;

    ch = in.peek();
    if (!isFieldTerminator(ch)) {
        if (ch == '"')
            fieldValue = parseQuotedField();
//        else
//            fieldValue = parseSimpleField();
    }
}

std::string CsvParser::parseQuotedField()
{
    in >> ch; // read and discard initial quote

    std::string field = parseEscapedField();

    in >> ch;
    if (ch != '"') {
        in.unget();
        std::cout <<"Quoted field has no terminating double quote" <<std::endl;
        return std::string();
    }
    return field;
}


std::string CsvParser::parseEscapedField()
{

}

int CsvParser::numRows() {
    return 18;
}

