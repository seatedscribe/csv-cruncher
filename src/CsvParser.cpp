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
#include <sstream>
#include <fstream>
#include "CsvParser.h"


//TODO: currently, delimiter and comment are both a single char
void CsvParser::setComment(std::string comment) {}
void CsvParser::setSeparator(std::string separator) {}

void CsvParser::checkCoherence()
{
    bool result = true;
    csvtable::iterator it = table.begin();
    int rows=table.size();
    size_t columns=(*it).size();
    int cr=0; //current row
    std::cout
      <<length <<" bytes; "
      <<rows <<" rows, " <<columns <<" columns" <<std::endl;
    for (;it != table.end(); ++it) {
        cr++;
        if ((*it).size() != columns) {
            std::cout
               <<"*** WARNING: row " <<cr
               <<" has different size than header ("
               <<(*it).size() <<" instead of " <<columns <<")"
               <<std::endl;
            result = false;
        }
    }
    isGood = result;
}

bool CsvParser::getParsedData(csvtable& table)
{
    table = this->table;
    return isGood;
}

CsvParser::CsvParser(std::istream& data)
    : in(data)
    , delimiter(',')
    , comment('#')
    , ch()
    , currentLine(1)
    , isGood(false)
{
    in.seekg (0, in.end);
    length = in.tellg();
    in.seekg (0, in.beg);
}

void CsvParser::parse()
{
    parseFile();
    checkCoherence();
}

void CsvParser::parseFile()
{
    parseRecord();
    ch = in.peek();
    while (ch != EOF) {
        if (!skipEndline()) {
            std::cout <<"\n!\n!!\n!!!\n*******\nparse error @line "
                     <<currentLine << std::endl;
            dumpLastRow();
            return;
        }
        currentLine++;
        parseRecord();
    }
}

void CsvParser::parseRecord()
{
    if (isComment()) {
        skipLine();
    }
    else {
        do {
            parseField();
            in.get(ch);
        } while (ch == delimiter);
        in.unget();
        table.push_back(parsedRow);
        parsedRow.clear();
        dumpLastRow();
    }
}

void CsvParser::parseField()
{
    std::string fieldValue;
    ch = in.peek();
    if (!isFieldTerminator(ch)) {
        if (isQuote(ch))
            fieldValue = parseQuoted();
        else
            fieldValue = parseSimple();
    }
    parsedRow.push_back(fieldValue);
    //std::cout<<fieldValue<<std::endl;
}

std::string CsvParser::parseQuoted()
{
    std::stringstream ss;
    //skip quote
    in.get();

    in.get(ch);
    //TODO: isSingleQuote should be isQualifier (see keepass csv importer)
    while (!in.eof() && !isSingleQuote(ch) &&
             ( isText(ch)          ||
               isComma(ch)         ||
               isCRLF(ch)          ||
               isDoubleQuote(ch)   ||
               isDoubleEscape(ch)  ||
               isSingleEscape(ch)))
    {
        if (isSingleEscape(ch) ||
            isDoubleEscape(ch) ||
            isDoubleQuote(ch))
        {
            in.get(ch);
        }

        ss << ch;
        in.get(ch);
    }
    //just skipped last quote
    return ss.str();
}

std::string CsvParser::parseSimple()
{
    std::stringstream ss;
    in.get(ch);
    while ((isText(ch) || isQuote(ch)) && !in.eof()) {
        ss << ch;
        in.get(ch);
    }
    in.unget();
    return ss.str();
}

bool CsvParser::isComma(char c)
{
    if (c == 0x2C)
       return true;
    return false;
}

bool CsvParser::isDoubleEscape(char c)
{
    if (!isEscape(c))
        return false;
    if (!isEscape(in.peek()))
        return false;
    return true;
}

bool CsvParser::isSingleEscape(char c)
{
    if (!isEscape(c))
        return false;
    if (isEscape(in.peek()))
        return false;
    return true;
}

bool CsvParser::isEscape(char c)
{
    if (c == 0x5C)
       return true;
    return false;
}

bool CsvParser::isComment()
{
    bool result = false;
    int pos = in.tellg();

    do {
        ch = in.get();
    } while (isSpace(ch) && !in.eof());

    if (ch == 0x23) {
           result = true;
    }
    in.seekg(pos);
    return result;
}

bool CsvParser::isDoubleQuote(char c)
{
    if (!isQuote(c))
        return false;
    if (!isQuote(in.peek()))
        return false;
    return true;
}

bool CsvParser::isSingleQuote(char c)
{
    if (!isQuote(c))
        return false;
    if (isQuote(in.peek()))
        return false;
    return true;
}

bool CsvParser::isQuote(char c)
{
    if (c == 0x22)
       return true;
    return false;
}

bool CsvParser::isText(char c)
{
    if ( ((c == 0x20) || (c == 0x21)) ||
         ((c >= 0x23) && (c <= 0x2B)) ||
         ((c >= 0x2D) && (c <= 0x7E))
       )
          return true;
    return false;
}

void CsvParser::skipLine()
{
    std::string s;
    getline(in, s);
    std::cout <<"skipping |" <<s.substr(0,s.length()-4) <<"...|" <<std::endl;
    int p=in.tellg();
    //should work with every EOL format
    in.seekg(p-1);
}

bool CsvParser::skipEndline()
{
    in.get(ch);
    if (ch == '\r') {
        in.get(ch);
        if (ch == '\n') {
            return true;
        }
    }
    else if (ch == '\n') {
        return true;
    }
    return false;
}

bool CsvParser::isCRLF(char c)
{
    // according to my grammar
    // CRLF = CR LF / CR / LF
    if ((c == '\r') || (c == '\n'))
        return true;
    return false;
}

bool CsvParser::isSpace(char c)
{
    if (c == 0x20)
       return true;
    return false;
}

bool CsvParser::isDelimiter(char c)
{
    return (c == delimiter);
}

bool CsvParser::isFieldTerminator(char c)
{
    return (isDelimiter(c) || (c == '\n') || (c == EOF));
}

void CsvParser::dumpLastRow() {
    csvrow::iterator it = table.back().begin();
//TODO:   table.at(currentLine).begin()
    std::cout <<"@" <<currentLine <<" ";
    while (it != table.back().end())
        std::cout <<"|" <<*it++;
    std::cout<<"|"<<std::endl;
}
