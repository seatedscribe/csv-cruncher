# csv-cruncher
C++ flavoured CSV parser with a BNF approach for syntax validation. 

This parser aims at validating most of the CSV formats out there, or dying trying.
It is built on the top of the RFC4180 which to me is a good starting point,
but since it is not considering all the different flavours it needed to be slightly
extended (e.g. ignoring comments)

Just for reference, this is the RFC grammar as provided by the IETF:

RFC 4180

file        = [header CRLF] record *(CRLF record) [EOF] #[CRLF]
header      = name *(COMMA name)
record      = field *(COMMA field)
name        = field
field       = (escaped / non-escaped)
escaped     = DQUOTE *(TEXTDATA / COMMA / CR / LF / 2DQUOTE) DQUOTE
non-escaped = *TEXTDATA
COMMA       = %x2C
CR          = %x0D 
DQUOTE      = %x22 
LF          = %x0A 
CRLF        = CR LF 
TEXTDATA    = %x20-21 / %x23-2B / %x2D-7E

I needed to validate the two most used versions - the "doubling quotes" and 
the "escape characters" - into the same engine, so I slightly modified 
the grammar to better reflect actual code implementation. I also extended it
to validate the case of "escaped characters". This is the result:

CSV CRUNCHER 

stream      = record *(CRLF record) EOF
record      = field *(COMMA field) / comment
field       = quoted / simple
quoted      = DQUOTE *(TEXT / COMMA / CR / LF / 2DQUOTE / 2ESCAPE / ESCAPE TEXT) DQUOTE
simple      = *(TEXT / DQUOTE)
comment     = *(SPACE) COMMENT
COMMA       = %x2C
COMMENT     = %x23
DQUOTE      = %x22
ESCAPE      = %x5C
CR          = %x0D 
LF          = %x0A 
CRLF        = CR LF / CR / LF
SPACE       = %x20
TEXT        = %x20-21 / %x23-2B / %x2D-7E / (empty string)



