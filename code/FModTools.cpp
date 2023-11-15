
internal FMOD_GUID
FMODGuid( uint32 Data1, uint32 Data2, uint32 Data3, uint32 Data4 ) {
    FMOD_GUID Result = {};
    Result.Data1 = Data1;
    Result.Data2 = ( uint16 )( ( Data2 >> 16 ) & 0x0000FFFF );
    Result.Data3 = ( uint16 )( ( Data2 >> 0 ) & 0x0000FFFF );
    Result.Data4[ 0 ] = ( uint8 )( ( Data3 >> 24 ) & 0x000000FF );
    Result.Data4[ 1 ] = ( uint8 )( ( Data3 >> 16 ) & 0x000000FF );
    Result.Data4[ 2 ] = ( uint8 )( ( Data3 >>  8 ) & 0x000000FF );
    Result.Data4[ 3 ] = ( uint8 )( ( Data3 >>  0 ) & 0x000000FF );
    Result.Data4[ 4 ] = ( uint8 )( ( Data4 >> 24 ) & 0x000000FF );
    Result.Data4[ 5 ] = ( uint8 )( ( Data4 >> 16 ) & 0x000000FF );
    Result.Data4[ 6 ] = ( uint8 )( ( Data4 >>  8 ) & 0x000000FF );
    Result.Data4[ 7 ] = ( uint8 )( ( Data4 >>  0 ) & 0x000000FF );
    return Result;
}

internal FMOD_GUID
FMODGuid( uint64 Part1, uint64 Part2, uint64 Part3, uint64 Part4, uint64 Part5 ) {
    FMOD_GUID Result = {};
    Result.Data1 = ( uint32 )Part1;
    Result.Data2 = ( uint16 )Part2;
    Result.Data3 = ( uint16 )Part3;
    Result.Data4[ 0 ] = ( uint8 )( ( Part4 >>  8 ) & 0x000000FF );
    Result.Data4[ 1 ] = ( uint8 )( ( Part4 >>  0 ) & 0x000000FF );
    Result.Data4[ 2 ] = ( uint8 )( ( Part5 >> 40 ) & 0x000000FF );
    Result.Data4[ 3 ] = ( uint8 )( ( Part5 >> 32 ) & 0x000000FF );
    Result.Data4[ 4 ] = ( uint8 )( ( Part5 >> 24 ) & 0x000000FF );
    Result.Data4[ 5 ] = ( uint8 )( ( Part5 >> 16 ) & 0x000000FF );
    Result.Data4[ 6 ] = ( uint8 )( ( Part5 >>  8 ) & 0x000000FF );
    Result.Data4[ 7 ] = ( uint8 )( ( Part5 >>  0 ) & 0x000000FF );
    return Result;
}