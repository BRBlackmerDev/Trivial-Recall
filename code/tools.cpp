
//----------
// CLOSEST POINT
//----------

// P : point
// E : Edge
// L : line
// R : ray

internal vec2
ClosestPointToEdge( vec2 C, vec2 A, vec2 B ) {
    vec2 AB = B - A;
    vec2 AC = C - A;
    
    flo32 t      = Clamp01( dot( AC, AB ) / dot( AB, AB ) );
    vec2  Result = A + AB * t;
    return Result;
}

internal flo32
DistPointToEdge( vec2 C, vec2 A, vec2 B ) {
    vec2  P      = ClosestPointToEdge( C, A, B );
    flo32 Result = GetLength( C - P );
    return Result;
}

internal flo32
DistSqPointToEdge( vec2 C, vec2 A, vec2 B ) {
    vec2  P      = ClosestPointToEdge( C, A, B );
    flo32 Result = GetLengthSq( C - P );
    return Result;
}

internal flo32
DistSqPointToTri( vec2 P, vec2 A, vec2 B, vec2 C ) {
    flo32 Result = FLT_MAX;
    Result = MinValue( Result, DistSqPointToEdge( P, A, B ) );
    Result = MinValue( Result, DistSqPointToEdge( P, B, C ) );
    Result = MinValue( Result, DistSqPointToEdge( P, C, A ) );
    
    return Result;
}

internal flo32
DistPointToTri( vec2 P, vec2 A, vec2 B, vec2 C ) {
    flo32 Result = sqrtf( DistSqPointToTri( P, A, B, C ) );
    return Result;
}

internal flo32
DistPointToRect( vec2 P, rect R ) {
    vec2 ClosestPoint = P;
    ClosestPoint.x = MaxValue( ClosestPoint.x, R.Left   );
    ClosestPoint.x = MinValue( ClosestPoint.x, R.Right  );
    ClosestPoint.y = MaxValue( ClosestPoint.y, R.Bottom );
    ClosestPoint.y = MinValue( ClosestPoint.y, R.Top    );
    
    flo32 Result = GetLength( P - ClosestPoint );
    return Result;
}

internal flo32
DistPointToRectPerimeter( vec2 P, rect R ) {
    flo32 Result = FLT_MAX;
    if( IsInRect( P, R ) ) {
        Result = MinValue( fabsf( P.x - R.Left   ), Result );
        Result = MinValue( fabsf( P.y - R.Bottom ), Result );
        Result = MinValue( fabsf( P.x - R.Right  ), Result );
        Result = MinValue( fabsf( P.y - R.Top    ), Result );
    } else {
        Result = DistPointToRect( P, R );
    }
    
    return Result;
}

internal flo32
DistPointToORect( vec2 P, orect R ) {
    flo32 Result = 0.0f;
    if( !IsInORect( P, R ) ) {
        vec2  V     = P - R.Center;
        flo32 DistX = fabsf( dot( V, R.xAxis ) ) - R.HalfDim.x;
        flo32 DistY = fabsf( dot( V, R.yAxis ) ) - R.HalfDim.y;
        Result = sqrtf( DistX * DistX + DistY * DistY );
    }
    
    return Result;
}

internal boo32
IsInEllipse( vec2 P, vec2 Ellipse_Center, vec2 Ellipse_Radius, flo32 Ellipse_Radians ) {
    boo32 Result = false;
    
    vec2  xAxis       = ToDirection2D( Ellipse_Radians );
    flo32 AspectRatio = Ellipse_Radius.y / Ellipse_Radius.x;
    flo32 Radius      = Ellipse_Radius.y;
    flo32 RadiusSq    = Radius * Radius;
    
    P += xAxis * ( dot( P - Ellipse_Center, xAxis ) * ( AspectRatio - 1.0f ) );
    flo32 DistSq = GetLengthSq( P - Ellipse_Center );
    if( DistSq <= RadiusSq ) {
        Result = true;
    }
    
    return Result;
}

internal boo32
IsInCapsule( vec2 Pos, CAPSULE2 Cap ) {
    boo32 Result = false;
    
    vec2  P = ClosestPointToEdge( Pos, Cap.P, Cap.Q );
    flo32 DistSq   = GetLengthSq( Pos - P );
    flo32 RadiusSq = Cap.Radius * Cap.Radius;
    if( DistSq <= RadiusSq ) {
        Result = true;
    }
    return Result;
}

internal vec2
ClosestPointToTriPerimeter( vec2 Pos, vec2 A, vec2 B, vec2 C ) {
    flo32 MaxDist = FLT_MAX;
    vec2  Result  = {};
    
    vec2 Point[  ] = {
        A,
        B,
        C,
        A,
    };
    
    for( uint32 iEdge = 0; iEdge < 3; iEdge++ ) {
        vec2  P    = ClosestPointToEdge( Pos, Point[ iEdge ], Point[ iEdge + 1 ] );
        flo32 Dist = GetLength( P - Pos );
        if( Dist < MaxDist ) {
            MaxDist = Dist;
            Result  = P;
        }
    }
    return Result;
}

internal vec2
ClosestPointToRectPerimeter( vec2 Pos, rect Bound ) {
    flo32 MaxDist = FLT_MAX;
    vec2  Result  = {};
    
    vec2 Point[  ] = {
        GetBL( Bound ),
        GetTL( Bound ),
        GetTR( Bound ),
        GetBR( Bound ),
        GetBL( Bound ),
    };
    
    for( uint32 iEdge = 0; iEdge < 4; iEdge++ ) {
        vec2  P    = ClosestPointToEdge( Pos, Point[ iEdge ], Point[ iEdge + 1 ] );
        flo32 Dist = GetLength( P - Pos );
        if( Dist < MaxDist ) {
            MaxDist = Dist;
            Result  = P;
        }
    }
    return Result;
}

internal boo32
DoIntersectEdgeEdge( vec2 A, vec2 B, vec2 C, vec2 D ) {
    boo32 Result = false;
    
    vec2 AB = B - A;
    vec2 CD = D - C;
    vec2 AC = C - A;
    
    vec2 N = GetNormal( GetPerp( CD ) );
    
    flo32 Denom = dot( N, AB );
    if( Denom != 0.0f ) {
        flo32 t = dot( N, C - A ) / Denom;
        
        if( ( t >= 0.0f ) && ( t <= 1.0f ) ) {
            vec2  P = A + AB * t;
            flo32 s = dot( P - C, CD ) / dot( CD, CD );
            
            if( ( s >= 0.0f ) && ( s <= 1.0f ) ) {
                Result = true;
            }
        }
    }
    return Result;
}

internal boo32
DoIntersectEdgeRect( vec2 A, vec2 B, rect R ) {
    vec2 BL = GetBL( R );
    vec2 TL = GetTL( R );
    vec2 TR = GetTR( R );
    vec2 BR = GetBR( R );
    
    boo32 Result = ( DoIntersectEdgeEdge( A, B, BL, TL ) )
        || ( DoIntersectEdgeEdge( A, B, TL, TR ) )
        || ( DoIntersectEdgeEdge( A, B, TR, BR ) )
        || ( DoIntersectEdgeEdge( A, B, BR, BL ) );
    
    return Result;
}

//----------
// UINT32_PAIR functions
//----------

internal UINT32_PAIR
UInt32Pair( uint32 x, uint32 y ) {
    UINT32_PAIR Result = { x, y };
    return Result;
}

internal boo32
operator==( UINT32_PAIR a, UINT32_PAIR b ) {
    boo32 Result = ( a.x == b.x ) && ( a.y == b.y );
    return Result;
}

internal boo32
operator!=( UINT32_PAIR a, UINT32_PAIR b ) {
    boo32 Result = !( a == b );
    return Result;
}

//----------
// UINT32_TRI functions
//----------

internal UINT32_TRI
UInt32Tri( uint32 x, uint32 y, uint32 z ) {
    UINT32_TRI Result = { x, y, z };
    return Result;
}

internal boo32
operator==( UINT32_TRI a, UINT32_TRI b ) {
    boo32 Result = ( a.x == b.x ) && ( a.y == b.y ) && ( a.z == b.z );
    return Result;
}

internal boo32
operator!=( UINT32_TRI a, UINT32_TRI b ) {
    boo32 Result = !( a == b );
    return Result;
}

//----------
// INT32_PAIR functions
//----------

internal INT32_PAIR
Int32Pair( int32 x, int32 y ) {
    INT32_PAIR Result = { x, y };
    return Result;
}

//----------
// INT32_TRI functions
//----------

internal INT32_TRI
Int32Tri( int32 x, int32 y, int32 z ) {
    INT32_TRI Result = { x, y, z };
    return Result;
}

internal boo32
operator==( INT32_TRI a, INT32_TRI b ) {
    boo32 Result = ( a.x == b.x ) && ( a.y == b.y ) && ( a.z == b.z );
    return Result;
}

internal boo32
operator!=( INT32_TRI a, INT32_TRI b ) {
    boo32 Result = !( a == b );
    return Result;
}

internal UINT32_TRI
ToUInt32Tri( INT32_TRI Tri ) {
    Assert( Tri.x > -1 );
    Assert( Tri.y > -1 );
    Assert( Tri.z > -1 );
    
    UINT32_TRI Result = { ( uint32 )Tri.x, ( uint32 )Tri.y, ( uint32 )Tri.z };
    return Result;
}

//----------
// SAVE/LOAD ENTITY
//----------

struct ENTITY_FILE_HEADER {
    char EntityTag[ 8 ];
    char Version  [ 4 ];
};

internal void
WriteEntityFileHeader( MEMORY * output, char * FileTag, uint32 Version ) {
    WriteSegment( output, FileTag );
    
    char str[ 4 + 1 ] = {};
    sprintf( str, "%04u", Version );
    WriteSegment( output, str );
}

internal boo32
OutputFile( WRITE_FILE * WriteFile, MEMORY * Output, char * SaveDir, char * FileName, char * FileTag ) {
    boo32 IsValid = WriteFile( SaveDir, FileName, FileTag, Output->Base, ( uint32 )Output->Used );
    if( !IsValid ) {
        char str[ 512 ] = {};
        sprintf( str, "ERROR! Error occurred when attempting to save File: %s.%s", FileName, FileTag );
        CONSOLE_ERROR( str );
    }
    
    return IsValid;
}

internal uint32
VerifyEntityHeaderAndGetVersion( uint8 ** ptr, char * FileTag ) {
    ENTITY_FILE_HEADER * header = _addr( *ptr, ENTITY_FILE_HEADER );
    Assert( MatchSegment( header->EntityTag, FileTag, 8 ) );
    
    char str[ 4 + 1 ] = {};
    memcpy( str, header->Version, 4 );
    
    uint32 Result = strtoul( str, 0, 0 );
    return Result;
}

//----------
// CAMERA
//----------

internal void
ZoomCameraToWorldBound( CAMERA_STATE * Camera, rect App_Bound, rect View_Bound, rect World_Bound ) {
    // App_Bound refers to the full screen dimensions (in pixels).
    // View_Bound refers to the window dimensions (in pixels) where the world view should be drawn.
    // World_Bound refers to the dimensions of the play area in game units.
    // The reason these calculations are necessary is because we are always drawing the game to the full screen, then drawing UI and debug panels on top of that. If we were drawing the game to a viewport, these calculations wouldn't be necessary because Direct3D would be handling the composite for us.
    
    vec2 World_Center = GetCenter( World_Bound );
    vec2 View_Center  = GetCenter( View_Bound );
    
    vec2 World_Dim = GetDim( World_Bound );
    
    flo32 View_AspectRatio  = GetWidth( View_Bound  ) / GetHeight( View_Bound  );
    flo32 World_AspectRatio = World_Dim.x / World_Dim.y;
    
    if( View_AspectRatio > World_AspectRatio ) {
        World_Dim.x = World_Dim.y * View_AspectRatio;
    } else {
        World_Dim.y = World_Dim.x / View_AspectRatio;
    }
    World_Bound = RectCD( World_Center, World_Dim );
    
    flo32 tLeft   = ( App_Bound.Left   - View_Center.x ) / ( View_Bound.Left   - View_Center.x );
    flo32 tRight  = ( App_Bound.Right  - View_Center.x ) / ( View_Bound.Right  - View_Center.x );
    flo32 tBottom = ( App_Bound.Bottom - View_Center.y ) / ( View_Bound.Bottom - View_Center.y );
    flo32 tTop    = ( App_Bound.Top    - View_Center.y ) / ( View_Bound.Top    - View_Center.y );
    
    World_Bound.Left   = World_Center.x + ( World_Bound.Left   - World_Center.x ) * tLeft;
    World_Bound.Right  = World_Center.x + ( World_Bound.Right  - World_Center.x ) * tRight;
    World_Bound.Bottom = World_Center.y + ( World_Bound.Bottom - World_Center.y ) * tBottom;
    World_Bound.Top    = World_Center.y + ( World_Bound.Top    - World_Center.y ) * tTop;
    
    Camera->Pos         = GetCenter( World_Bound );
    Camera->Dim         = GetDim   ( World_Bound );
    Camera->HalfDim     = Camera->Dim * 0.5f;
    Camera->Scale_Ratio = Camera->Dim.y / DEV_HEIGHT;
}

internal void
InitCameraShake( CAMERA_SHAKE * Camera, flo32 UpTime, flo32 HoldTime, flo32 DownTime, flo32 Frequency, vec2 OffsetInPixels ) {
    Camera->Timer     = 0.0f;
    Camera->UpTime    = UpTime;
    Camera->HoldTime  = HoldTime;
    Camera->DownTime  = DownTime;
    Camera->t         = 0.0f;
    Camera->TargetT   = 1.0f / Frequency;
    Camera->Strength  = 0.0f;
    Camera->Offset    = OffsetInPixels;
    
    Camera->P.x = RandomF32InRange( -1.0f, 1.0f );
    Camera->P.y = RandomF32InRange( -1.0f, 1.0f );
    Camera->Q.x = RandomF32InRange( -1.0f, 1.0f );
    Camera->Q.y = RandomF32InRange( -1.0f, 1.0f );
}

internal void
UpdateCameraShake( CAMERA_SHAKE * Camera, flo32 dT ) {
    flo32 TargetTime = Camera->UpTime + Camera->HoldTime + Camera->DownTime;
    
    if( Camera->Timer < TargetTime ) {
        Camera->Timer += dT;
        
        flo32 TargetTimeA = Camera->UpTime;
        flo32 TargetTimeB = TargetTimeA + Camera->HoldTime;
        
        if( Camera->Timer < TargetTimeA ) {
            Camera->Strength = Clamp01( Camera->Timer / Camera->UpTime );
        } else if( ( Camera->Timer >= TargetTimeA ) && ( Camera->Timer < TargetTimeB ) ) {
            Camera->Strength = 1.0f;
        } else {
            Camera->Strength = 1.0f - Clamp01( ( Camera->Timer - TargetTimeB ) / Camera->DownTime );
        }
        
        Camera->t += dT;
        while( Camera->t >= Camera->TargetT ) {
            Camera->P = Camera->Q;
            Camera->Q.x = RandomF32InRange( -1.0f, 1.0f );
            Camera->Q.y = RandomF32InRange( -1.0f, 1.0f );
            
            Camera->t -= Camera->TargetT;
        }
    }
}

internal void
UpdateCamera( CAMERA_STATE * Camera, KEYBOARD_STATE * Keyboard, flo32 dT ) {
    if( WasPressed ( Keyboard, KeyCode_num4 ) ) { Camera->Move_Left  = true;  }
    if( WasReleased( Keyboard, KeyCode_num4 ) ) { Camera->Move_Left  = false; }
    if( WasPressed ( Keyboard, KeyCode_num6 ) ) { Camera->Move_Right = true;  }
    if( WasReleased( Keyboard, KeyCode_num6 ) ) { Camera->Move_Right = false; }
    if( WasPressed ( Keyboard, KeyCode_num9 ) ) { Camera->Move_In    = true;  }
    if( WasReleased( Keyboard, KeyCode_num9 ) ) { Camera->Move_In    = false; }
    if( WasPressed ( Keyboard, KeyCode_num7 ) ) { Camera->Move_Out   = true;  }
    if( WasReleased( Keyboard, KeyCode_num7 ) ) { Camera->Move_Out   = false; }
    if( WasPressed ( Keyboard, KeyCode_num8 ) ) { Camera->Move_Up    = true;  }
    if( WasReleased( Keyboard, KeyCode_num8 ) ) { Camera->Move_Up    = false; }
    if( WasPressed ( Keyboard, KeyCode_num5 ) ) { Camera->Move_Down  = true;  }
    if( WasReleased( Keyboard, KeyCode_num5 ) ) { Camera->Move_Down  = false; }
    flo32 Ratio = Camera->Dim.y / DEV_HEIGHT;
    flo32 Speed = 9.0f * Ratio;
    Camera->Scale_Ratio = Ratio;
    
    if( Camera->Move_Left ) {
        Camera->Pos.x -= ( Speed * dT );
    }
    if( Camera->Move_Right ) {
        Camera->Pos.x += ( Speed * dT );
    }
    if( Camera->Move_Down ) {
        Camera->Pos.y -= ( Speed * dT );
    }
    if( Camera->Move_Up ) {
        Camera->Pos.y += ( Speed * dT );
    }
    if( Camera->Move_In ) {
        Camera->Dim    *= 0.99f;
        Camera->HalfDim = Camera->Dim * 0.5f;
    }
    if( Camera->Move_Out ) {
        Camera->Dim    *= 1.01f;
        Camera->HalfDim = Camera->Dim * 0.5f;
    }
    
    DISPLAY_VALUE( vec2, Camera->Pos );
}

internal vec2
GetWorldPos( CAMERA_STATE * Camera, vec2 App_Dim, vec2 Pos ) {
    vec2 t = Clamp01( Pos / App_Dim ) - Vec2( 0.5f, 0.5f );
    vec2 P = Camera->Pos + Camera->Dim * t;
    return P;
}