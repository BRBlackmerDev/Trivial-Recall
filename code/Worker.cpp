
internal WORKER
RandomWorker( WORKER_STATE * WorkerS ) {
    WORKER Worker = {};
    
    int32 iPath = RandomS32InRange( 0, WorkerS->nPath - 1 );
    flo32 tPath = RandomF32();
    PATH   Path = WorkerS->Path[ iPath ];
    Worker.Position  = lerp( Path.Start, tPath, Path.End );
    Worker.Direction = GetNormal( Path.End - Path.Start );
    Worker.Timer     = WORKER_TARGET_TIME;
    
    return Worker;
}

internal void
InitWorkers( MEMORY * TempMemory, APP_STATE * AppState ) {
    WORKER_STATE * WorkerS = &AppState->WorkerS;
    
    rect R = AppState->UI_Bound;
    //R = AddDim( R, Vec2( 240.0f, 160.0f ) );
    
    int32 nPath = 0;
    WorkerS->Path[ nPath++ ] = { GetT( R, Vec2( 0.0f, 0.2f ) ), GetT( R, Vec2( 1.0f, 0.2f ) ) };
    WorkerS->Path[ nPath++ ] = { GetT( R, Vec2( 1.0f, 0.4f ) ), GetT( R, Vec2( 0.0f, 0.4f ) ) };
    WorkerS->Path[ nPath++ ] = { GetT( R, Vec2( 0.0f, 0.6f ) ), GetT( R, Vec2( 1.0f, 0.6f ) ) };
    WorkerS->Path[ nPath++ ] = { GetT( R, Vec2( 1.0f, 0.8f ) ), GetT( R, Vec2( 0.0f, 0.8f ) ) };
    WorkerS->Path[ nPath++ ] = { GetT( R, Vec2( 0.12f, 0.0f ) ), GetT( R, Vec2( 0.12f, 1.0f ) ) };
    WorkerS->Path[ nPath++ ] = { GetT( R, Vec2( 0.27f, 1.0f ) ), GetT( R, Vec2( 0.27f, 0.0f ) ) };
    WorkerS->Path[ nPath++ ] = { GetT( R, Vec2( 0.42f, 0.0f ) ), GetT( R, Vec2( 0.42f, 1.0f ) ) };
    WorkerS->Path[ nPath++ ] = { GetT( R, Vec2( 0.57f, 1.0f ) ), GetT( R, Vec2( 0.57f, 0.0f ) ) };
    WorkerS->Path[ nPath++ ] = { GetT( R, Vec2( 0.72f, 0.0f ) ), GetT( R, Vec2( 0.72f, 1.0f ) ) };
    WorkerS->Path[ nPath++ ] = { GetT( R, Vec2( 0.87f, 1.0f ) ), GetT( R, Vec2( 0.88f, 0.0f ) ) };
    Assert( nPath < PATH_MAX_COUNT );
    WorkerS->nPath = nPath;
    
    WorkerS->MaxWorker = WORKER_MAX_COUNT;
    WorkerS->nWorker   = WorkerS->MaxWorker;
    for( int32 iWorker = 0; iWorker < WorkerS->MaxWorker; iWorker++ ) {
        WorkerS->Worker[ iWorker ]       = RandomWorker( WorkerS );
        WorkerS->Worker[ iWorker ].Timer = RandomF32() * WORKER_TARGET_TIME;
    }
    
    WorkerS->Cell_MaxActive = MinValue( 32, CELL_MAX_COUNT );
    int32 * Cell_Index0 = _PushArray( TempMemory, int32, CELL_MAX_COUNT );
    for( int32 iCell = 0; iCell < CELL_MAX_COUNT; iCell++ ) {
        Cell_Index0[ iCell ] = iCell;
    }
    
    for( int32 iCell = 0; iCell < CELL_MAX_COUNT; iCell++ ) {
        int32 nRemain = CELL_MAX_COUNT - iCell;
        int32 Select  = RandomS32InRange( 0, nRemain - 1 );
        
        WorkerS->Cell_Index[ iCell ] = Cell_Index0[ Select ];
        Cell_Index0[ Select ] = Cell_Index0[ nRemain - 1 ];
    }
    
    _PopArray( TempMemory, int32, CELL_MAX_COUNT );
    
    { // Activate New Cells
        int32 nInactive = CELL_MAX_COUNT - WorkerS->Cell_nActive;
        while( WorkerS->Cell_nActive < WorkerS->Cell_MaxActive ) {
            int32 Select = RandomS32InRange( 0, MinValue( 16 - 1, nInactive - 1 ) );
            int32 Index  = WorkerS->Cell_Index[ Select ];
            
            nInactive -= 1;
            WorkerS->Cell_Index[ Select ] = WorkerS->Cell_Index[ nInactive ];
            
            WorkerS->Cell_Timer[ Index ] = RandomF32() * CELL_TARGET_TIME;
            WorkerS->Cell_Active[ WorkerS->Cell_nActive ] = Index;
            
            WorkerS->Cell_nActive += 1;
        }
    }
}

internal void
UpdateWorkers( APP_STATE * AppState, flo32 dT ) {
    WORKER_STATE * WorkerS = &AppState->WorkerS;
    
    WorkerS->Sweep_Timer = MaxValue( WorkerS->Sweep_Timer - dT, 0.0f );
    
    flo32 Speed = 80.0f;
    
    for( int32 iWorker = 0; iWorker < WorkerS->nWorker; iWorker++ ) {
        WORKER * Worker = WorkerS->Worker + iWorker;
        
        Worker->Position += Worker->Direction * ( Speed * dT );
        
        Worker->Timer -= dT;
        if( Worker->Timer <= 0.0f ) {
            *Worker = RandomWorker( WorkerS );
        }
    }
    
    { // Activate New Cells
        int32 nInactive = CELL_MAX_COUNT - WorkerS->Cell_nActive;
        while( WorkerS->Cell_nActive < WorkerS->Cell_MaxActive ) {
            int32 Select = RandomS32InRange( 0, nInactive - 1 );
            int32 Index  = WorkerS->Cell_Index[ Select ];
            
            nInactive -= 1;
            WorkerS->Cell_Index[ Select ] = WorkerS->Cell_Index[ nInactive ];
            
            WorkerS->Cell_Timer[ Index ] = CELL_TARGET_TIME;
            WorkerS->Cell_Active[ WorkerS->Cell_nActive ] = Index;
            
            WorkerS->Cell_nActive += 1;
        }
    }
    
    for( int32 iCell = 0; iCell < CELL_MAX_COUNT; iCell++ ) {
        WorkerS->Cell_Timer[ iCell ] = MaxValue( WorkerS->Cell_Timer[ iCell ] - dT, 0.0f );
    }
    
    { // Deactivate Old Cells
        int32 NewCount  = 0;
        int32 nInactive = CELL_MAX_COUNT - WorkerS->Cell_nActive;
        for( int32 iActive = 0; iActive < WorkerS->Cell_nActive; iActive++ ) {
            int32 Index = WorkerS->Cell_Active[ iActive ];
            if( WorkerS->Cell_Timer[ Index ] <= 0.0f ) {
                WorkerS->Cell_Index[ nInactive ] = Index;
                nInactive += 1;
            } else {
                WorkerS->Cell_Active[ NewCount ] = Index;
                NewCount  += 1;
            }
        }
        WorkerS->Cell_nActive = NewCount;
    }
}

internal void
DrawWorkers( RENDER_PASS * Pass, APP_STATE * AppState, vec2 tMouse ) {
    WORKER_STATE * WorkerS = &AppState->WorkerS;
    
    flo32 tZoom   = -cosf( AppState->Background_tZoom * PI ) * 0.5f + 0.5f;
    flo32  Zoom   = lerp( 0.5f, tZoom, 1.0f );
    vec2   Center = GetCenter( AppState->UI_Bound );
    
    for( int32 iPath = 0; iPath < WorkerS->nPath; iPath++ ) {
        PATH Path = WorkerS->Path[ iPath ];
        //DrawLine( Pass, Path.Start, Path.End, COLOR_RED );
    }
    
    { // Workers
        flo32 Alpha_LoTimer = ( 60.0f / 60.0f );
        flo32 Alpha_HiTimer = WORKER_TARGET_TIME - ( 60.0f / 60.0f );
        
        rect  Alpha_Bound   = AddDim( AppState->UI_Bound, Vec2( -180.0f, -160.0f ) );
        flo32 Alpha_MaxDist = 120.0f;
        
        flo32 Radius = 6.0f * 0.5f * Zoom;
        for( int32 iWorker = 0; iWorker < WorkerS->nWorker; iWorker++ ) {
            WORKER Worker = WorkerS->Worker[ iWorker ];
            vec2   Pos    = Center + ( Worker.Position - Center ) * Zoom;
            rect   Bound  = RectCR( Pos, Radius );
            
            
            
            flo32  Alpha = 1.0f;
            if( Worker.Timer <= Alpha_LoTimer ) {
                Alpha = GetT01( Worker.Timer, 0.0f, Alpha_LoTimer );
            }
            if( Worker.Timer >= Alpha_HiTimer ) {
                Alpha = 1.0f - GetT01( Worker.Timer, Alpha_HiTimer, WORKER_TARGET_TIME );
            }
            
            if( !IsInRect( Worker.Position, Alpha_Bound ) ) {
                flo32  Dist = DistPointToRect( Worker.Position, Alpha_Bound );
                flo32 tDist = Clamp01( Dist / Alpha_MaxDist );
                Alpha *= ( 1.0f - tDist );
            }
            //DrawRect( Pass, Bound, COLOR_GRAY( 0.15f ) * Alpha );
            DrawRect( Pass, TextureID_Glow, Bound, BONUS_POINTS_COLOR * Alpha );
        }
    }
    
    { // Cells
        int32 nRow = 8;
        int32 nCol = 12;
        vec2  bPos    = GetT( AppState->UI_Bound, Vec2( 0.12f, 0.2f ) );
        vec2  Advance = ( GetDim( AppState->UI_Bound ) * Vec2( 0.15f, 0.2f ) * 0.5f );
        vec2  Dim     = ( Advance + Vec2( -32.0f, -32.0f ) ) * Zoom;
        
        flo32 Alpha_LoTimer = ( 40.0f / 60.0f );
        flo32 Alpha_HiTimer = CELL_TARGET_TIME - ( 40.0f / 60.0f );
        
#if 0        
        flo32 Worker_LoTimer = 0.0f;
        flo32 Worker_HiTimer = CELL_TARGET_TIME - ( 30.0f / 60.0f );
        
        vec2  Offset0 = Vec2( 1.0f, 0.0f ) * Advance * 0.5f * Zoom * 0.5f;
        vec2  Offset1 = GetPerp( Offset0 ) * 8.0f;
        flo32 Radius  = 6.0f * 0.5f * Zoom;
        
        flo32 Move_tThreshold = 0.125f;
#endif
        
        flo32 Sweep_TargetTime = SWEEP_CELL_TARGET_TIME;
        flo32 Delay            = SWEEP_DELAY_TARGET_TIME;
        //flo32 Total_TargetTime = Sweep_TargetTime + Delay * ( flo32 )( CELL_COL_MAX_COUNT - 1 );
        
        for( int32 iRow = 0; iRow < nRow; iRow++ ) {
            for( int32 iCol = 0; iCol < nCol; iCol++ ) {
                int32 iCell = iRow * CELL_COL_MAX_COUNT + iCol;
                flo32 Timer = WorkerS->Cell_Timer[ iCell ];
                
                vec2 Pos   = bPos + Advance * Vec2( ( flo32 )iCol, ( flo32 )iRow );
                Pos -= ( Advance * 0.5f );
                Pos  = Center + ( Pos - Center ) * Zoom;
                
                flo32 tAlpha = 1.0f;
                if( Timer <= Alpha_LoTimer ) {
                    tAlpha *= ( GetT01( Timer, 0.0f, Alpha_LoTimer ) );
                }
                if( Timer >= Alpha_HiTimer ) {
                    tAlpha *= ( 1.0f - GetT01( Timer, Alpha_HiTimer, CELL_TARGET_TIME ) );
                }
                
                flo32 LoTimer = Delay * ( flo32 )iCol;
                flo32 HiTimer = LoTimer + Sweep_TargetTime;
                flo32  tTimer = GetT01( SWEEP_TARGET_TIME - WorkerS->Sweep_Timer, LoTimer, HiTimer );
                flo32  tAlpha0 = -cosf( tTimer * TAU ) * 0.5f + 0.5f;
                tAlpha = MaxValue( tAlpha, tAlpha0 * 2.0f );
                
                flo32 Alpha = lerp( 0.0f,  tAlpha, 0.5f );
                flo32 Scale = lerp( 0.95f, tAlpha, 1.0f  );
                
#if 0                
                {
                    flo32 tWorker = 1.0f - GetT01( Timer, Worker_LoTimer, Worker_HiTimer );
                    flo32 tMove0  = GetT01( tWorker, 0.0f, Move_tThreshold );
                    flo32 tMove1  = GetT01( tWorker, Move_tThreshold, 1.0f );
                    
                    Alpha = GetT01( Alpha, 0.0f, 0.25f );
                    
                    vec2  P       = Pos + Offset0 * ( 1.0f + tMove0 ) + Offset1 * tMove1;
                    rect  Bound   = RectCR( P, Radius );
                    DrawRect( Pass, TextureID_Glow, Bound, BONUS_POINTS_COLOR * Alpha * tMove0 );
                }
#endif
                
                rect Bound = RectCD( Pos, Dim * Scale );
                DrawRect( Pass, TextureID_Cell, Bound, COLOR_GRAY( 0.1f ) * Alpha );
            }
        }
    }
    
    { // Draw Black Rectangle
        flo32 Alpha = lerp( 0.5f, AppState->Background_tZoom, 0.0f );
        DrawRect( Pass, AppState->UI_Bound, COLOR_BLACK * Alpha );
    }
}