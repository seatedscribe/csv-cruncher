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

class Tokenizer {
public:
    Tokenizer(std::string s);
    virtual ~Tokenizer() {}
    char Read();
    char Peek();
    bool Unread(char c);
private:
   std::string m_string;
   std::string m_errorStr;
   bool m_error;
   bool m_haveUnreadChar;
   char m_unreadChar;
   int m_index;
   void skipCrInCrLf();
   char mapCrToLf(char c);
};

class StringBuilder;

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
    /*
    void parseSubField(StringBuilder sb);
    bool isBadSimpleFieldChar(char c);
    std::string parseSimpleField();
    void setCommentDelimiter(std::string delimiter);
    */
//    Tokenizer reader;
};

#endif //CSVPARSER_H
