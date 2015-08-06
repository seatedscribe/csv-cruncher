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
#include <deque>


typedef std::deque<std::string> csvrow;
typedef std::deque<csvrow> csvtable;

class CsvParser {
public:
    CsvParser(std::istream &data);
    virtual ~CsvParser() {}
    void parse();
    void setComment(std::string comment);
    void setSeparator(std::string separator);
    bool getParsedData(csvtable& table);
private:
    std::istream& in;
    int length;
    char delimiter;
    char comment;
    char ch;
    //currentLine does not count comments
    int currentLine;
    csvrow parsedRow;
    csvtable table;
    bool isGood;

    void parseFile();
    void parseRecord();
    void parseField();
    std::string parseQuoted();
    std::string parseSimple();
    //TODO: isComma --> isDelimiter()
    bool isComma(char c);
    bool isDoubleQuote(char c);
    bool isSingleQuote(char c);
    bool isQuote(char c);
    bool isDoubleEscape(char c);
    bool isSingleEscape(char c);
    bool isEscape(char c);
    bool isComment();
    bool isCRLF(char c);
    bool isSpace(char c);
    bool isDelimiter(char c);
    bool isText(char c);
    bool skipEndline();
    void skipLine();
    bool isFieldTerminator(char c);

    void checkCoherence();
    void dumpLastRow();

};

#endif //CSVPARSER_H
