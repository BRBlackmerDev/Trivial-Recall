
internal vec2
CalculateDestination( int32 nIter, flo32 InitPosition, flo32 InitVelocity, flo32 Friction, flo32 dT ) {
    flo32 Pos = InitPosition;
    flo32 Vel = InitVelocity;
    
    for( int32 iIter = 0; iIter < nIter; iIter++ ) {
        flo32 Accel = Vel * -Friction;
        flo32 dPos  = Accel * ( 0.5f * dT * dT ) + Vel * dT;
        
        Pos += dPos;
        Vel += Accel * dT;
    }
    
    vec2   Result = Vec2( Pos, Vel );
    return Result;
}

internal flo32
CalculateNewFriction( int32 FramesPerSecond, flo32 bFriction ) {
    int32 bFramesPerSecond = 60;
    flo32 dT               = 1.0f / ( flo32 )bFramesPerSecond;
    
    flo32 bPosition        = 0.0f;
    flo32 bVelocity        = 800.0f;
    int32 nIter            = bFramesPerSecond;
    
    vec2  bDestination = CalculateDestination( nIter, bPosition, bVelocity, bFriction, dT );
    
    flo32 MinFriction     = 0.0f;
    flo32 MaxFriction     = 30.0f;
    flo32 Result          = 0.0f;
    
    int32 MaxIter = 1000;
    int32 iIter   = 0;
    boo32 DoTest  = true;
    while( DoTest ) {
        flo32 Friction    = lerp( MinFriction, 0.5f, MaxFriction );
        if( fabsf( MaxFriction - MinFriction ) <= 0.00000001f ) {
            DoTest = false;
            Result = Friction;
        } else {
            vec2  Destination = CalculateDestination( FramesPerSecond, bPosition, bVelocity, Friction, 1.0f / ( flo32 )FramesPerSecond );
            if( 0 ) {
            } else if( Destination.x < bDestination.x ) {
                MaxFriction = Friction;
            } else if( Destination.x > bDestination.x ) {
                MinFriction = Friction;
            } else if( Destination.x == bDestination.x ) {
                DoTest = false;
                Result = Friction;
            }
        }
        
        if( iIter >= MaxIter ) {
            DoTest = false;
            Result = Friction;
        }
        
        iIter += 1;
    }
    
    return Result;
}

internal void
DoFramerateTest( RENDER_PASS * Pass, APP_STATE * AppState, MEMORY * TempMemory, vec2 bPos, int32 FramesPerSecond, flo32 Friction, flo32 Multiplier ) {
    
    int32 nIter = 2 * FramesPerSecond;
    flo32 dT    = 1.0f / ( flo32 )FramesPerSecond;
    
    flo32 * P = _PushArray_Clear( TempMemory, flo32, nIter );
    flo32 * V = _PushArray_Clear( TempMemory, flo32, nIter );
    
    flo32 Velocity  = 800.0f;
    flo32 Position  = 0.0f;
    
    char Str[ 64 ] = {};
    sprintf( Str, "FPS: %d, Friction: %f (* %f)", FramesPerSecond, Friction, Multiplier );
    
    FONT * Font = AppState->Font;
    DrawString( Pass, Font, Str, bPos + Vec2( 8.0f, 8.0f ), TextAlign_BottomLeft, COLOR_WHITE );
    
    for( int32 iIter = 0; iIter < nIter; iIter++ ) {
        flo32 Accel = Velocity * -Friction;
        flo32 dPos  = Accel * ( 0.5f * dT * dT ) + Velocity * dT;
        
        Position += dPos;
        Velocity += Accel * dT;
        
        P[ iIter ] = Position;
        V[ iIter ] = Velocity;
    }
    
    int32 nEntriesPerLine = 60;
    
    flo32 AdvanceX = 60.0f;
    flo32 AdvanceY = 16.0f;
    vec2  AtPos    = bPos;
    
    int32 AtLine = 0;
    for( int32 iIter = 0; iIter < nIter; iIter++ ) {
        vec2 A = AtPos;
        vec2 B = A + Vec2( AdvanceX, 0.0f );
        
        
        sprintf( Str, "%6.01f", P[ iIter ] );
        DrawString( Pass, Font, Str, A, TextAlign_TopLeft, COLOR_WHITE );
        
        sprintf( Str, "%6.01f", V[ iIter ] );
        DrawString( Pass, Font, Str, B, TextAlign_TopLeft, COLOR_WHITE );
        
        AtPos.y -= AdvanceY;
        
        AtLine += 1;
        if( AtLine >= nEntriesPerLine ) {
            AtLine = 0;
            
            bPos.x += AdvanceX * 2.5f;
            AtPos   = bPos;
        }
    }
    
    _PopArray( TempMemory, flo32, nIter );
    _PopArray( TempMemory, flo32, nIter );
}