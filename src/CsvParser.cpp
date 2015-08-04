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

//using namespace std;


//TODO: ridefinire peek() e read() copiando il comportamento da Tokenizer

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
    std::cout <<"record: " <<ch;
    if (ch == CsvConst::E_O_F) {
        in.unget();
        ch = '\n';
        std::cout <<" unget: " << ch;
    }
    std::cout <<" after: " << ch <<std::endl;
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


/*

private string parseEscapedField(ICharTokenizer reader) {
      StringBuilder sb = new StringBuilder();

      parseSubField(reader, sb);
      char ch = reader.Read();
      while (processDoubleQuote(reader, ch)) {
        sb.Append('"');
        parseSubField(reader, sb);
        ch = reader.Read();
      }
      reader.Unread(ch);

      return sb.ToString();
    }

    private void parseSubField(ICharTokenizer reader, StringBuilder sb) {
      char ch = reader.Read();
      while ((ch != '"') && (ch != CsvConstants.EOF)) {
        sb.Append(ch);
        ch = reader.Read();
      }
      reader.Unread(ch);
    }

    private bool isBadSimpleFieldChar(char c) {
      return isSpace(c) || isFieldTerminator(c) || (c == '"');
    }

    private string parseSimpleField(ICharTokenizer reader) {

      char ch = reader.Read();
      if (isBadSimpleFieldChar(ch)) {
        reader.Unread(ch);
        return String.Empty;
      }

      StringBuilder sb = new StringBuilder();
      sb.Append(ch);
      ch = reader.Read();
      while (!isBadSimpleFieldChar(ch)) {
        sb.Append(ch);
        ch = reader.Read();
      }
      reader.Unread(ch);

      return sb.ToString();
    }

*/


std::string CsvParser::parseEscapedField()
{
/*    StringBuilder sb = new StringBuilder();

    parseSubField(sb);
    in >> ch;
    while (processDoubleQuote(ch)) {
        sb.Append('"');
        parseSubField(sb);
        in >> ch;
    }
    in.unget();

    return sb.ToString();
    */
}

/*
void CsvParser::parseSubField(StringBuilder sb)
{
    in >> ch;
    while ((ch != '"') && (ch != CsvConst::E_O_F)) {
        sb.Append(ch);
        in >> ch;
    }
    in.unget();
}

bool CsvParser::isBadSimpleFieldChar(char c)
{
    return isSpace(c) || isFieldTerminator(c) || (c == '"');
}

std::string CsvParser::parseSimpleField()
{
    in >> ch;
    if (isBadSimpleFieldChar(ch)) {
        in.unget();
        return String.Empty;
    }

    StringBuilder sb = new StringBuilder();
    sb.Append(ch);
    in >> ch;
    while (!isBadSimpleFieldChar(ch)) {
        sb.Append(ch);
        in >> ch;
    }
    in.unget();

    return sb.ToString();
}

bool CsvParser::processDoubleQuote(char ch)
{
    if ((ch == '"') && (reader.Peek() == '"')) {
        reader.Read(); // discard second quote of double
        return true;
    }
    return false;
}


*/








int CsvParser::numRows() {
    return 18;
}


/*

Tokenizer::Tokenizer(std::string s) {
    m_string = s;
    m_index = 0;
    m_haveUnreadChar = false;
}

void Tokenizer::skipCrInCrLf() {
    if ((m_string[m_index] == '\r') && (m_index + 1 < m_string.length()) && (m_string[m_index + 1] == '\n'))
        m_index++;
}

char Tokenizer::mapCrToLf(char c) {
    if (c == '\r')
        return '\n';
    return c;
}

char Tokenizer::Peek() {
    if (m_haveUnreadChar)
        return m_unreadChar;
    if (m_index < m_string.length())
        return mapCrToLf(m_string[m_index]);
    //TODO: typedef or similar
    return '\0';
}

char Tokenizer::Read() {
    if (m_haveUnreadChar) {
        m_haveUnreadChar = false;
        return m_unreadChar;
    }
    if (m_index < m_string.length()) {
        skipCrInCrLf();
        return mapCrToLf(m_string[m_index++]);
    }
    //TODO: typedef or similar
    return '\0';
}

bool Tokenizer::Unread(char c) {
    if (m_haveUnreadChar) {
        m_errorStr = "Unread() cannot accept more than one pushed back character";
        return false;
    }
    m_haveUnreadChar = true;
    m_unreadChar = c;
    return true;
}
*/
