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

#define DBG_MSG false
//TODO: currently, separator and comment are both a single char
//TODO: ignore empty rows
void CsvParser::setComment_(std::string comment) { std::string c(comment); }
void CsvParser::setSeparator_(std::string separator) { std::string c(separator); }


CsvParser::CsvParser(std::istream& data)
    : in(data)
    , separator(',')
    , comment('#')
{
    in.seekg (0, in.end);
    length = in.tellg();
    in.seekg (0, in.beg);
}

/*void CsvParser::loadContent()
{
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
}
*/



void CsvParser::reset()
{
    in.clear();
    in.seekg(0, std::ios::beg);
    csvLine = 1;
    columns = 0;
    isGood = true;
    ignoreEmpty = true;
    //while (table.size())
    //    table.pop_back();
    table.clear();
    errorMsg.str("");
    errorMsg.clear();
}

bool CsvParser::parse()
{
    reset();
    if (length <= 0) {
        errorMsg <<"file seems empty; skipping..." <<std::endl;
        return false;
    }
    parseFile();
    int rows=table.size();
    size_t columns=table.at(0).size();
    errorMsg
      <<length <<" bytes; "
      <<rows <<" rows, " <<columns <<" columns" <<std::endl;

    return isGood;
}

void CsvParser::parseFile()
{
    parseRecord();
    ch = in.peek();
    while (ch != EOF) {
        if (!skipEndline()) {
            errorMsg <<"*** ABORTING ***\nCRITICAL PARSE ERROR @line "
                     <<csvLine << " - wrong separator?" <<std::endl;
            dumpRow(table.size()-1);
            isGood = false;
            return;
        }
        csvLine++;
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
        } while (ch == separator);
        in.unget();

        if (isEmptyRow()) {
            parsedRow.clear();
            if (DBG_MSG) {
                errorMsg <<"DBG -->@" <<csvLine
                         <<" skp empty row" <<std::endl;
            }
            return;
        }

        if ((0 == columns)) {
            columns = parsedRow.size();
            if (DBG_MSG) {
                std::cout<<"DBG: header @line "<< csvLine <<std::endl;
            }
        }
        else if (parsedRow.size() != columns) {
            errorMsg
               <<"*** WARNING: row " <<csvLine
               <<" has different size than header ("
               <<parsedRow.size() <<" instead of " <<columns <<")"
               <<std::endl;
            isGood = false;
        }

        table.push_back(parsedRow);
        parsedRow.clear();
        if (DBG_MSG) {
            errorMsg <<"DBG +->";
            dumpRow(table.size()-1);
//            std::cout <<errorMsg.str()<<std::endl;
        }
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

void CsvParser::ignoreEmptyRows(bool ignore)
{
    ignoreEmpty = ignore;
}

std::string CsvParser::parseQuoted()
{
    std::stringstream ss;
    //skip quote
    in.get();

/*
private string parseQuotedField(ICharTokenizer reader) {
  reader.Read(); // read and discard initial quote

  string field = parseEscapedField(reader);

  char ch = reader.Read();
  if (ch != '"') {
    reader.Unread(ch);
    throw new CsvParserNoTermQuoteException(
        "Quoted field has no terminating double quote");
  }
  return field;
}*/

    in.get(ch);
    //TODO: isSingleQuote should be isQualifier (see keepass csv importer)
    while (!in.eof() && !isSingleQuote(ch) &&
             ( isText(ch)          ||
               isSeparator(ch)     ||
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

    if (ch == comment) {
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
    if (isSeparator(c))
        return false;
    if ( ((c == 0x20) || (c == 0x21)) ||
         ((c >= 0x23) && (c <= 0x7E))
       )
          return true;
    return false;
}

bool CsvParser::isEmptyRow()
{
    if (parsedRow.at(0).size() == 0)
        return true;
    return false;
}

void CsvParser::skipLine()
{
    std::string s;
    getline(in, s);
    if (DBG_MSG) {
        errorMsg <<"DBG -->@" <<csvLine
                 <<" skp |"
                 <<s.substr(0,s.length()-4) <<"...|" <<std::endl;
    }
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

bool CsvParser::isSeparator(char c)
{
    return (c == separator);
}

bool CsvParser::isFieldTerminator(char c)
{
    return (isSeparator(c) || (c == '\n') || (c == EOF));
}

void CsvParser::setSeparator(char c)
{
    separator = c;
}

void CsvParser::setComment(char c)
{
    comment = c;
}



void CsvParser::getParsedData(csvtable& table)
{
    table = this->table;
}

void CsvParser::dumpRow(std::size_t row) {
    if (row >= table.size()) {
        errorMsg << "Internal error, index too large\n";
        return;
    }
    csvrow::iterator it = table.at(row).begin();
    errorMsg <<"@" <<csvLine <<" ";
    while (it != table.at(row).end())
        errorMsg <<"|" <<*it++;
    errorMsg <<"|\n";
}

std::string CsvParser::getError()
{
    return errorMsg.str();
}


