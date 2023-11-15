
//----------
// Color functions
//----------

internal uint32
ToU32Color_FlipRBForWin32( uint8 r, uint8 g, uint8 b, uint8 a = 255 ) {
    uint32 Result = ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | ( b << 0 );
    return Result;
}

internal uint32
ToU32Color_FlipRBForWin32( vec4 V4Color ) {
    uint8 r = ( uint8 )( V4Color.r * 255 );
    uint8 g = ( uint8 )( V4Color.g * 255 );
    uint8 b = ( uint8 )( V4Color.b * 255 );
    uint8 a = ( uint8 )( V4Color.a * 255 );
    uint32 Result = ToU32Color_FlipRBForWin32( r, g, b, a );
    return Result;
}

internal uint32
ToU32Color_FlipRBForWin32( vec3 V3Color, flo32 Alpha ) {
    uint32 Result = ToU32Color_FlipRBForWin32( Vec4( V3Color, Alpha ) );
    return Result;
}

internal uint32
ToU32Color( uint8 r, uint8 g, uint8 b, uint8 a = 255 ) {
    uint32 Result = ( a << 24 ) | ( b << 16 ) | ( g << 8 ) | ( r << 0 );
    return Result;
}

internal uint32
ToU32Color( vec4 V4Color ) {
    uint8 r = ( uint8 )( V4Color.r * 255 );
    uint8 g = ( uint8 )( V4Color.g * 255 );
    uint8 b = ( uint8 )( V4Color.b * 255 );
    uint8 a = ( uint8 )( V4Color.a * 255 );
    uint32 Result = ToU32Color( r, g, b, a );
    return Result;
}

internal uint32
ToU32Color( vec3 V3Color, flo32 Alpha ) {
    uint32 Result = ToU32Color( Vec4( V3Color, Alpha ) );
    return Result;
}

internal vec4
ToColor( uint8 r, uint8 g, uint8 b ) {
    vec4 Result = {};
    Result.r = ( flo32 )r / 255.0f;
    Result.g = ( flo32 )g / 255.0f;
    Result.b = ( flo32 )b / 255.0f;
    Result.a = 1.0f;
    return Result;
}

internal vec4
ToV4Color( uint32 Color ) {
    vec4 Result = {};
    Result.r = ( ( flo32 )( ( Color       ) & 0xFF ) ) / 255.0f;
    Result.g = ( ( flo32 )( ( Color >>  8 ) & 0xFF ) ) / 255.0f;
    Result.b = ( ( flo32 )( ( Color >> 16 ) & 0xFF ) ) / 255.0f;
    Result.a = ( ( flo32 )( ( Color >> 24 ) & 0xFF ) ) / 255.0f;
    return Result;
}

internal vec4
TowardWhite( vec4 Color, flo32 t ) {
    vec4 Result = lerp( Color, t, COLOR_WHITE );
    if( Color.a < 1.0f ) {
        Result *= Color.a;
    }
    return Result;
}

internal vec4
TowardBlack( vec4 Color, flo32 t ) {
    vec4 Result = lerp( Color, t, COLOR_BLACK );
    if( Color.a < 1.0f ) {
        Result *= Color.a;
    }
    return Result;
}

internal vec4
TowardGray( flo32 tGray, vec4 Color, flo32 t ) {
    vec4 Result = lerp( Color, t, COLOR_GRAY( tGray ) );
    if( Color.a < 1.0f ) {
        Result *= Color.a;
    }
    return Result;
}