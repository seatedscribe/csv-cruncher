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

#include <deque>
#include <string>
#include <sstream>

typedef std::deque<std::string> csvrow;
typedef std::deque<csvrow> csvtable;

class CsvParser {
public:
    CsvParser(std::string filename);
    CsvParser(std::istream &data);
    virtual ~CsvParser() {}
    bool parse();
    void setComment_(std::string comment);
    void setSeparator_(std::string separator);
    void setComment(char c);
    void setSeparator(char c);
//    void ignoreEmptyRows(bool ignore);
    void getParsedData(csvtable& table);
    std::string getError();

private:
    std::istream& in;
    int length;
    std::size_t columns;
    char separator;
    char comment;
    char ch;
    //csvLine tracks the currently parsed row
    std::size_t csvLine;
    csvrow parsedRow;
    csvtable table;
    bool isGood;
    std::stringstream errorMsg;

    void reset();
    void parseFile();
    void parseRecord();
    void parseField();
    std::string parseQuoted();
    std::string parseSimple();
    bool isDoubleQuote(char c);
    bool isSingleQuote(char c);
    bool isQuote(char c);
    bool isDoubleEscape(char c);
    bool isSingleEscape(char c);
    bool isEscape(char c);
    bool isComment();
    bool isCRLF(char c);
    bool isSpace(char c);
    bool isSeparator(char c);
    bool isText(char c);
    bool skipEndline();
    void skipLine();
    bool isFieldTerminator(char c);
    void dumpRow(std::size_t row);

};

#endif //CSVPARSER_H
