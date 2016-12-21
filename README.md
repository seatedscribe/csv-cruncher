# csv-cruncher
C++ flavoured CSV parser with a BNF approach for syntax validation. 

This parser aims at validating most of the CSV formats out there, or dying trying.
It is built on the top of the RFC4180 which to me is a good starting point,
but since it is not considering all the different flavours actually used in the real world,
it needed to be slightly extended (e.g. ignoring comments)

Just for reference, this is the RFC grammar as provided by the IETF:

RFC 4180

file        = [header CRLF] record *(CRLF record) [EOF] #[CRLF]
header      = name *(COMMA name)
record      = field *(COMMA field)
name        = field
field       = (escaped | non-escaped)
escaped     = DQUOTE *(TEXTDATA | COMMA | CR | LF | 2DQUOTE) DQUOTE
non-escaped = *TEXTDATA
COMMA       = %x2C
CR          = %x0D 
DQUOTE      = %x22 
LF          = %x0A 
CRLF        = CR LF 
TEXTDATA    = %x20-21 | %x23-2B | %x2D-7E

I needed to validate the two most used versions - the "doubling quotes" and 
the "escape character" - into the same engine, so I slightly modified 
the grammar to better reflect actual code implementation. I also extended it
to validate the case of "escaped characters". This is the result:

CSV CRUNCHER 

file        = record *(CRLF record) EOF
record      = field *(separator field) | comment --e.g. ,,A,B has 2 empty fields and is valid
field       = simple | quoted
simple      = TEXT -- e.g. f""ie"l"""d is OK
quoted      = qualifier escaped qualifier
escaped     = ESC_TEXT *(escape_mark ESC_TEXT)
escape_mark = (2qualifier | BACKSLASH qualifier)
qualifier   = one of {DQUOTE, COLON, BACKSLASH}
separator   = one of {COMMA, COLON, SEMICOLON}
comment     = *(SPACE | TAB) COMMENT
COMMA       = %x2C --i.e. ','
COLON       = %x58 --i.e. ':'
SEMICOLON   = %x59 --i.e. ';'
COMMENT     = %x23 --i.e. '#'
DQUOTE      = %x22 --i.e. '"'
BACKSLASH   = %x5C --i.e. '\'
CR          = %x0D 
LF          = %x0A 
SPACE       = %x20
CRLF        = CR LF | LF
TEXT        = *(any char except CRLF, separator, EOF) (also TEXT="" shall be valid) --was: +(%x20-21 | %x23-7E)
ESC_TEXT    = +(any char except qualifier, EOF)

