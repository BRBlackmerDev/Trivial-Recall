
//----------
// STRING
//----------

struct STRING {
    char * String;
    uint32 nChar;
};

internal boo32
MatchString( char * String0, char * String1 ) {
    boo32 doMatch = true;
    while( ( *String0 ) && ( *String1 ) ) {
        if( *String0 != *String1 ) { doMatch = false; }
        String0++;
        String1++;
    }
    if( ( *String0 ) || ( *String1 ) ) { doMatch = false; }
    return doMatch;
}

internal boo32
MatchSegment( char * String0, char * String1, uint32 nChar ) {
    boo32 doMatch = true;
    for( uint32 iChar = 0; ( doMatch ) && ( iChar < nChar ); iChar++ ) {
        if( String0[ iChar ] != String1[ iChar ] ) { doMatch = false; }
    }
    return doMatch;
}

internal boo32
MatchSegmentNoCase( char * StringA, char * StringB, uint32 nChar ) {
    boo32 DoMatch = true;
    for( uint32 iChar = 0; ( DoMatch ) && ( iChar < nChar ); iChar++ ) {
        uint32 CharA = ( uint32 )StringA[ iChar ];
        uint32 CharB = ( uint32 )StringB[ iChar ];
        if( CharA < 'a' ) { CharA += 0X00000020; }
        if( CharB < 'a' ) { CharB += 0x00000020; }
        if( CharA != CharB ) { DoMatch = false; }
    }
    return DoMatch;
}

internal void
WriteSegment( MEMORY * Memory, char * Str, int32 Length = -1 ) {
    if( Length == -1 ) {
        Length = ( int32 )strlen( Str );
    }
    uint8 * Dest = ( uint8 * )_PushSize( Memory, Length );
    memcpy( Dest, Str, Length );
}

internal void
WriteString( MEMORY * Memory, char * str, uint32 nChar ) {
    uint8 * dest = ( uint8 * )_PushSize( Memory, nChar );
    memcpy( dest, str, nChar );
    
    _writem( Memory, uint8, 0 );
}

internal void
WriteString( MEMORY * Memory, char * str ) {
    uint32 nChar = ( uint32 )strlen( str );
    WriteString( Memory, str, nChar );
}

internal boo32
MatchString( STRING strA, STRING strB ) {
    boo32 Result = ( strA.nChar == strB.nChar );
    if( Result ) {
        Result = MatchSegment( strA.String, strB.String, strA.nChar );
    }
    return Result;
}

internal boo32
MatchString( STRING strA, char * str ) {
    uint32 nChar = ( uint32 )strlen( str );
    boo32 Result = ( strA.nChar == nChar );
    if( Result ) {
        Result = MatchSegment( strA.String, str, strA.nChar );
    }
    return Result;
}

//----------
// FILE_PARSER

struct FILE_PARSER {
    char * Start;
    char * At;
    int32  Size;
};

internal FILE_PARSER
FileParser( void * Memory, int32 Size ) {
    FILE_PARSER Result = {};
    Result.Start = ( char * )Memory;
    Result.At    = Result.Start;
    Result.Size  = Size;
    return Result;
}

internal FILE_PARSER
FileParser( STRING Str ) {
    FILE_PARSER Result = {};
    Result.Start = Str.String;
    Result.At    = Str.String;
    Result.Size  = Str.nChar;
    return Result;
}

internal boo32
HasTextRemaining( FILE_PARSER * Parser ) {
    boo32 Result = ( ( Parser->At - Parser->Start ) < Parser->Size );
    return Result;
}

internal boo32
IsWhiteSpace( char c ) {
    boo32 Result = ( c == 0 ) || ( c == ' ' ) || ( c == '\n' ) || ( c == '\t' ) || ( c == '\r' );
    return Result;
}

internal void
EatWhiteSpace( FILE_PARSER * Parser ) {
    while( HasTextRemaining( Parser ) && ( IsWhiteSpace( *Parser->At ) ) ) { Parser->At++; }
}

internal void
EatNonWhiteSpace( FILE_PARSER * Parser ) {
    while( HasTextRemaining( Parser ) && ( !IsWhiteSpace( *Parser->At ) ) ) { Parser->At++; }
}

internal char *
GetEnd( char * str ) {
    Assert( ( str ) && ( str[ 0 ] ) );
    
    char * Result = str;
    while( Result[ 0 ] ) { Result++; }
    return Result;
}

internal STRING
ParseToken( FILE_PARSER * Parser ) {
    Assert( Parser->Size > 0 );
    
    STRING Result = {};
    
    EatWhiteSpace( Parser );
    
    if( HasTextRemaining( Parser ) ) {
        Result.String = Parser->At;
    }
    
    EatNonWhiteSpace( Parser );
    Result.nChar = ( uint32 )( Parser->At - Result.String );
    
    EatWhiteSpace( Parser );
    
    return Result;
}

internal STRING
ParseLine( FILE_PARSER * Parser ) {
    STRING Result = {};
    
    EatWhiteSpace( Parser );
    
    if( HasTextRemaining( Parser ) ) {
        Result.String = Parser->At;
    }
    
    while( ( *Parser->At != 0 ) && ( *Parser->At != '\n' ) && ( *Parser->At != '\r' ) ) { Parser->At++; }
    Result.nChar = ( uint32 )( Parser->At - Result.String );
    
    EatWhiteSpace( Parser );
    return Result;
}

internal char
ParseChar( FILE_PARSER * Parser ) {
    Assert( Parser->At );
    
    char Result = ( char )( Parser->At[ 0 ] );
    Parser->At++;
    return Result;
}

internal uint8
ParseU8( STRING Token ) {
    uint8 Result = ( uint8 )strtoul( Token.String, 0, 0 );
    return Result;
}

internal uint8
ParseU8( FILE_PARSER * Parser ) {
    STRING Token  = ParseToken( Parser );
    uint8  Result = ParseU8( Token );
    return Result;
}

internal uint16
ParseU16( STRING Token ) {
    uint16 Result = ( uint16 )strtoul( Token.String, 0, 0 );
    return Result;
}

internal uint32
ParseU32( STRING Token ) {
    uint32 Result = strtoul( Token.String, 0, 0 );
    return Result;
}

internal uint32
ParseU32( FILE_PARSER * Parser ) {
    STRING Token  = ParseToken( Parser );
    uint32 Result = ParseU32( Token );
    return Result;
}

internal uint64
ParseU64_Hex( FILE_PARSER * Parser ) {
    STRING Token  = ParseToken( Parser );
    uint64 Result = strtoull( Token.String, 0, 16 );
    return Result;
}

internal int32
ParseS32( STRING Token ) {
    int32  Result = strtol( Token.String, 0, 0 );
    return Result;
}

internal int32
ParseS32( FILE_PARSER * Parser ) {
    STRING Token  = ParseToken( Parser );
    int32  Result = ParseS32( Token );
    return Result;
}

internal flo32
ParseF32( STRING Token ) {
    flo32  Result = strtof( Token.String, 0 );
    return Result;
}

internal flo32
ParseF32( FILE_PARSER * Parser ) {
    STRING Token  = ParseToken( Parser );
    flo32  Result = ParseF32( Token );
    return Result;
}

internal vec2
ParseV2( STRING Token ) {
    FILE_PARSER Parser = FileParser( Token.String, Token.nChar );
    
    vec2 Result = {};
    for( uint32 iElem = 0; iElem < 2; iElem++ ) {
        Result.elem[ iElem ] = ParseF32( &Parser );
    }
    return Result;
}

internal vec3
ParseV3( STRING Token ) {
    FILE_PARSER Parser = FileParser( Token.String, Token.nChar );
    
    vec3 Result = {};
    for( uint32 iElem = 0; iElem < 3; iElem++ ) {
        Result.elem[ iElem ] = ParseF32( &Parser );
    }
    return Result;
}

internal vec4
ParseV4( STRING Token ) {
    FILE_PARSER Parser = FileParser( Token.String, Token.nChar );
    
    vec4 Result = {};
    for( uint32 iElem = 0; iElem < 4; iElem++ ) {
        Result.elem[ iElem ] = ParseF32( &Parser );
    }
    return Result;
}

internal quat
ParseQuat( STRING Token ) {
    FILE_PARSER Parser = FileParser( Token.String, Token.nChar );
    
    quat Result = {};
    for( uint32 iElem = 0; iElem < 4; iElem++ ) {
        Result.elem[ iElem ] = ParseF32( &Parser );
    }
    return Result;
}