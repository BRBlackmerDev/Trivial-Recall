
//----------
// This is old code from the first two iterations of the game. I most likely don't need it, but I'm storing it here just in case.

internal void
DrawLockerContents( RENDER_PASS * Pass, FONT * Font, vec2 Pos, int32 ItemID, flo32 Alpha = 1.0f ) {
    char Str[ 2 ] = {};
    char C = ( char )( ItemID - 1 ) + 'A';
    Str[ 0 ] = C;
    
    DrawString( Pass, Font, Str, Pos, TextAlign_Center, COLOR_WHITE * Alpha );
}

internal boo32
UpdateAndDrawLockerButton( RENDER_PASS * Pass, MOUSE_STATE * Mouse, TEXTURE_ID TextureID, rect Bound, boo32 IsActive ) {
    boo32 Result = false;
    
    if( IsActive ) {
        vec4  Background_Color = COLOR_GRAY( 0.1f );
        vec4  Icon_Color       = COLOR_GRAY( 0.2f );
        flo32 Icon_Margin      = 4.0f;
        
        if( IsInRect( Mouse->Position, Bound ) ) {
            Background_Color = COLOR_GRAY( 0.25f );
            Icon_Color       = COLOR_WHITE;
            if( WasPressed( Mouse, MouseButton_Left ) ) {
                Result = true;
            }
        }
        
        DrawRect( Pass, Bound, Background_Color );
        DrawRect( Pass, TextureID, AddRadius( Bound, -Icon_Margin ), Icon_Color );
    }
    
    return Result;
}

internal void
DrawLabelColonS32( RENDER_PASS * Pass, FONT * Font, char * Label, int32 Value, vec2 AtPos, flo32 ColonX, flo32 ValueX, vec4 Color ) {
    char Str[ 128 ] = {};
    sprintf( Str, "%d", Value );
    
    DrawString( Pass, Font, Label, AtPos,                   TextAlign_TopLeft,   Color );
    DrawString( Pass, Font, ":",   Vec2( ColonX, AtPos.y ), TextAlign_TopCenter, Color );
    DrawString( Pass, Font, Str,   Vec2( ValueX, AtPos.y ), TextAlign_TopRight,  Color );
}

internal void
APP_UpdateAndRender( APP_STATE * AppState, PLATFORM * Platform ) {
    RENDERER    * Renderer = &Platform->Renderer;
    RENDER_PASS * Pass_Game  = &Renderer->Pass_Game;
    
	MOUSE_STATE      * Mouse    = &Platform->Mouse;
	KEYBOARD_STATE   * Keyboard = &Platform->Keyboard;
	
	MEMORY * PermMemory = &Platform->PermMemory;
	MEMORY * TempMemory = &Platform->TempMemory;
	
    AUDIO_SYSTEM * AudioSystem = &Platform->AudioSystem;
	//ASSET_SYSTEM * assetSystem = &Platform->assetSystem;
    
    flo32 dT = Platform->TargetSec;
    AppState->dT = dT;
    
    DrawString( Pass_Game, AppState->FontList + FontID_1_00, "Why does the first drawn character flicker?", Vec2( 960.0f, -540.0f ), TextAlign_Center, Vec2( 1.0f, 1.0f ), COLOR_WHITE );
    
    if( !AppState->IsInitialized ) {
        AppState->IsInitialized    = true;
        
        AppState->UI_Dim     = Vec2( DEV_WIDTH, DEV_HEIGHT );
        AppState->UI_HalfDim = AppState->UI_Dim * 0.5f;
        AppState->UI_Bound   = RectBLD( Vec2( 0.0f, 0.0f ), AppState->UI_Dim );
        
        ZoomCameraToWorldBound( &AppState->Camera, AppState->UI_Bound, AppState->UI_Bound, AppState->UI_Bound );
        
        AppState->AudioSystem = AudioSystem;
        AudioSystem->MainVolume = 0.5f;
        
        AppState->Mode = AppMode_Game;
        
        AppState->Locker_nRow = 2;
        AppState->Locker_nCol = 2;
        
        AppState->nItem = ITEM_MAX_COUNT;
        for( int32 Iter = 0; Iter < AppState->nItem; Iter++ ) {
            AppState->Item[ Iter ] = Iter + 1;
        }
        
        AppState->ToMove_iLocker = -1;
        AppState->nMinimumItems = 3;
    }
    
    switch( AppState->Mode ) {
        case AppMode_Game: {
            CAMERA_STATE * Camera = &AppState->Camera;
            
            vec2 MouseP   = Mouse->Position;
            rect UI_Bound = AppState->UI_Bound;
            rect Locker_TotalBound = {};
            
            rect Storage_Bound = Rect( 100.0f, 100.0f, 900.0f, 980.0f );
            DrawRectOutline( Pass_Game, Storage_Bound, COLOR_RED );
            
            vec2 Slot_Dim      = Vec2(  180.0f, 180.0f );
            vec2 ToStore_Pos   = Vec2( 1100.0f, 200.0f );
            vec2 ToFind_Pos    = Vec2( 1100.0f, 400.0f );
            rect ToStore_Bound = RectCD( ToStore_Pos, Slot_Dim );
            rect ToFind_Bound  = RectCD( ToFind_Pos,  Slot_Dim );
            
            {
                vec2 tMouse = GetT01( MouseP, UI_Bound );
                
                //AppState->Locker_nCol = ( int32 )lerp( 2.0f, tMouse.x, 40.0f );
                //AppState->Locker_nRow = ( int32 )lerp( 2.0f, tMouse.y, 40.0f );
            }
            
            { // Draw Counter
                rect Bound = Rect( 1040.0f, 200.0f, 1080.0f, 880.0f );
                DrawRect( Pass_Game, Bound, COLOR_GRAY( 0.1f ) );
            }
            
            { // Update and Draw Store Line
                boo32 CanStore = true;
                if( AppState->nLocker >= ITEM_MAX_COUNT ) {
                    CanStore = false;
                }
                if( ( CanStore ) && ( AppState->ToStore <= -1 ) ) {
                    int32 Select = RandomS32InRange( 0, AppState->nItem - 1 );
                    AppState->ToStore = AppState->Item[ Select ];
                    AppState->nItem  -= 1;
                    AppState->Item[ Select ] = AppState->Item[ AppState->nItem ];
                }
                
                if( AppState->ToStore > 0 ) {
                    if( ( WasPressed( Mouse, MouseButton_Left ) ) && ( IsInRect( MouseP, ToStore_Bound ) ) ) {
                        AppState->ToStore_IsSelected = true;
                    }
                }
                
                if( AppState->ToStore_IsSelected ) {
                    if( WasReleased( Mouse, MouseButton_Left ) ) {
                        AppState->ToStore_IsSelected = false;
                        
                        if( IsInRect( MouseP, Storage_Bound ) ) {
                            LOCKER Locker = {};
                            Locker.ItemID   = AppState->ToStore;
                            Locker.Position = MouseP;
                            AppState->Locker[ AppState->nLocker ] = Locker;
                            
                            AppState->nLocker += 1;
                            AppState->ToStore  = 0;
                        }
                    }
                }
                
                DrawRect( Pass_Game, ToStore_Bound, COLOR_GRAY( 0.025f ) );
                if( ( AppState->ToStore > 0 ) && ( !AppState->ToStore_IsSelected ) ) {
                    vec4 Background_Color = COLOR_GRAY( 0.05f );
                    if( IsInRect( MouseP, ToStore_Bound ) ) {
                        Background_Color = COLOR_GRAY( 0.25f );
                    }
                    DrawRect( Pass_Game, ToStore_Bound, Background_Color );
                    DrawLockerContents( Pass_Game, AppState->FontList + FontID_6_00, ToStore_Pos, AppState->ToStore );
                }
            }
            
            { // Update and Draw Find Line
                boo32 CanFind = true;
                if( AppState->nLocker <= 0 ) {
                    CanFind = false;
                }
                
                {
                    for( int32 iLocker = 0; iLocker < AppState->nLocker; iLocker++ ) {
                        LOCKER * Locker = AppState->Locker + iLocker;
                        Locker->Find_Timer  = MaxValue( Locker->Find_Timer  - dT, 0.0f );
                        Locker->ShowItem_Timer = MaxValue( Locker->ShowItem_Timer - dT, 0.0f );
                    }
                }
                
                if( ( CanFind ) && ( AppState->ToFind <= 0 ) ) {
                    int32 * iAvailable = _PushArray( TempMemory, int32, AppState->nLocker );
                    int32   nAvailable = 0;
                    
                    for( int32 iLocker = 0; iLocker < AppState->nLocker; iLocker++ ) {
                        LOCKER Locker = AppState->Locker[ iLocker ];
                        if( Locker.Find_Timer <= 0.0f ) {
                            iAvailable[ nAvailable++ ] = iLocker;
                        }
                    }
                    
                    if( nAvailable > 0 ) {
                        int32  Select = RandomS32InRange( 0, nAvailable - 1 );
                        LOCKER Locker = AppState->Locker[ iAvailable[ Select ] ];
                        
                        AppState->ToFind = Locker.ItemID;
                    }
                    
                    _PopArray( TempMemory, int32, AppState->nLocker );
                }
                
                DrawRect( Pass_Game, ToFind_Bound, COLOR_GRAY( 0.025f ) );
                if( AppState->ToFind > 0 ) {
                    vec4 Background_Color = COLOR_GRAY( 0.05f );
                    if( AppState->ToMove_iLocker > -1 ) {
                        vec2 Dest = MouseP + AppState->ToMove_Offset;
                        if( IsInRect( Dest, ToFind_Bound ) ) {
                            Background_Color = COLOR_GRAY( 0.25f );
                        }
                    }
                    DrawRect( Pass_Game, ToFind_Bound, Background_Color );
                    DrawLockerContents( Pass_Game, AppState->FontList + FontID_6_00, ToFind_Pos, AppState->ToFind );
                }
            }
            
            { // Update and Draw Lockers
                int32 iHover = -1;
                for( int32 iLocker = 0; iLocker < AppState->nLocker; iLocker++ ) {
                    LOCKER * Locker = AppState->Locker + iLocker;
                    
                    rect Bound = RectCD( Locker->Position, LOCKER_DIM );
                    if( IsInRect( MouseP, Bound ) ) {
                        iHover = iLocker;
                    }
                }
                
                if( AppState->ToMove_iLocker > -1 ) {
                    iHover = -1;
                }
                
                if( iHover > -1 ) {
                    if( WasPressed( Mouse, MouseButton_Left ) ) {
                        LOCKER Locker = AppState->Locker[ iHover ];
                        AppState->ToMove_iLocker = iHover;
                        AppState->ToMove_Offset  = Locker.Position - MouseP;
                    }
                }
                
                if( ( AppState->ToMove_iLocker > -1 ) && ( WasReleased( Mouse, MouseButton_Left ) ) ) {
                    boo32 DoSwap   = false;
                    boo32 DoRemove = false;
                    
                    vec2 Dest = MouseP + AppState->ToMove_Offset;
                    if( IsInRect( Dest, Storage_Bound ) ) {
                        DoSwap = true;
                        
                        LOCKER * Locker = AppState->Locker + ( AppState->nLocker - 1 );
                        Locker->Position = Dest;
                    } else if( ( AppState->ToFind > 0 ) && ( IsInRect( Dest, ToFind_Bound ) ) ) {
                        LOCKER * Locker = AppState->Locker + AppState->ToMove_iLocker;
                        if( Locker->ItemID == AppState->ToFind ) {
                            AppState->Correct_Timer  = LOCKER_CORRECT_TARGET_TIME;
                            AppState->Correct_ItemID = Locker->ItemID;
                            
                            AppState->Item[ AppState->nItem ] = Locker->ItemID;
                            AppState->nItem += 1;
                            
                            int32 Points = 1;
                            AppState->Score                 += Points;
                            AppState->nCorrectItemsReturned += 1;
                            AppState->LastPoints             = Points;
                            AppState->LastPoints_Timer       = LAST_POINTS_TARGET_TIME;
                            
                            DoSwap   = true;
                            DoRemove = true;
                            
                            AppState->ToFind = 0;
                        } else {
                            Locker->ShowItem_Timer = LOCKER_SHOW_ITEM_TARGET_TIME;
                            
                            AppState->nWrongItemsReturned += 1;
                            
                            DoSwap = true;
                        }
                    }
                    
                    if( DoSwap ) {
                        _swap( LOCKER, AppState->Locker[ AppState->ToMove_iLocker ], AppState->Locker[ AppState->nLocker - 1 ] );
                    }
                    if( DoRemove ) {
                        AppState->nLocker -= 1;
                    }
                    AppState->ToMove_iLocker = -1;
                }
                
                int32 nLocker = AppState->nLocker;
                for( int32 iLocker = 0; iLocker < nLocker; iLocker++ ) {
                    LOCKER Locker = AppState->Locker[ iLocker ];
                    
                    rect Bound = RectCD( Locker.Position, LOCKER_DIM );
                    
                    if( iLocker == iHover ) {
                        DrawRectOutline( Pass_Game, Bound, COLOR_WHITE );
                    }
                    
                    flo32 Alpha = 1.0f;
                    if( iLocker == AppState->ToMove_iLocker ) {
                        Alpha = 0.15f;
                    }
                    
                    if( Locker.ShowItem_Timer > 0.0f ) {
                        vec4  Color      = COLOR_WHITE;
                        flo32 CycleTimer = LOCKER_SHOW_ITEM_TARGET_TIME / 5.0f;
                        if( ( ( int32 )( Locker.ShowItem_Timer / CycleTimer ) % 2 ) == 0 ) {
                            Color = COLOR_RED;
                        }
                        DrawRect( Pass_Game, TextureID_CardboardBox_Empty, Bound, Color * Alpha );
                        DrawLockerContents( Pass_Game, AppState->FontList + FontID_6_00, GetCenter( Bound ), Locker.ItemID, Alpha );
                    } else {
                        DrawRect( Pass_Game, TextureID_Locker_Full, Bound, COLOR_WHITE * Alpha );
                    }
                }
            }
            
            { // Update and Draw Correct Item
                if( AppState->Correct_ItemID > 0 ) {
                    rect Bound = RectRCD( Vec2( 1052.0f, 540.0f ), Vec2( 180.0f, 180.0f ) );
                    
                    flo32 CycleTimer = ( 6.0f / 60.0f );
                    vec4  Color      = TowardBlack( COLOR_YELLOW, 0.75f );
                    if( ( ( int32 )( AppState->Correct_Timer / CycleTimer ) % 2 ) == 0 ) {
                        Color = COLOR_GRAY( 0.05f );
                    }
                    DrawRect( Pass_Game, Bound, Color );
                    
                    DrawLockerContents( Pass_Game, AppState->FontList + FontID_6_00, GetCenter( Bound ), AppState->Correct_ItemID );
                    
                    AppState->Correct_Timer = MaxValue( AppState->Correct_Timer - dT, 0.0f );
                    if( AppState->Correct_Timer <= 0.0f ) {
                        AppState->Correct_ItemID = 0;
                    }
                }
            }
            
            { // Draw ToStore Item When Selected
                if( AppState->ToStore_IsSelected ) {
                    flo32 Alpha = 0.15f;
                    if( IsInRect( MouseP, Storage_Bound ) ) {
                        Alpha = 1.0f;
                    }
                    
                    DrawRect( Pass_Game, TextureID_CardboardBox_Empty, RectCD( MouseP, LOCKER_DIM ), COLOR_WHITE * Alpha );
                    DrawLockerContents( Pass_Game, AppState->FontList + FontID_6_00, MouseP, AppState->ToStore, Alpha );
                }
            }
            
            { // Draw ToMove Item When Active
                if( AppState->ToMove_iLocker > -1 ) {
                    vec2  Pos   = MouseP + AppState->ToMove_Offset;
                    flo32 Alpha = 0.15f;
                    if( IsInRect( Pos, Storage_Bound ) ) {
                        Alpha = 0.75f;
                    }
                    DrawRect( Pass_Game, TextureID_Locker_Full, RectCD( Pos, LOCKER_DIM ), COLOR_WHITE * Alpha );
                }
            }
            
            
#if 0            
            { // Draw Customer In/Out
                vec2  Slot_Dim    = Vec2( 180.0f, 180.0f );
                flo32 DistBetween = 40.0f;
                
                flo32 X       = 1120.0f + Slot_Dim.x * 0.5f;
                flo32 yOffset = ( DistBetween + Slot_Dim.y ) * 0.5f;
                
                vec2 bPos = Vec2( X, 540.0f );
                vec2 In_Pos  = bPos + Vec2( 0.0f,  yOffset );
                vec2 Out_Pos = bPos + Vec2( 0.0f, -yOffset );
                
                rect In_Bound  = RectCD( In_Pos,  Slot_Dim );
                rect Out_Bound = RectCD( Out_Pos, Slot_Dim );
                
                DrawRect( Pass_Game, In_Bound,  COLOR_GRAY( 0.025f ) );
                DrawRect( Pass_Game, Out_Bound, COLOR_GRAY( 0.025f ) );
                
                FONT * Font = AppState->FontList + FontID_6_00;
                
                if( AppState->ToStore > 0 ) {
                    DrawLockerContents( Pass_Game, Font, GetCenter( In_Bound  ), AppState->ToStore );
                }
                if( AppState->ToFind > 0 ) {
                    DrawLockerContents( Pass_Game, Font, GetCenter( Out_Bound ), AppState->ToFind );
                    
                    // Draw Time Bonus Meter
                    if( AppState->TimeBonus_Timer > 0.0f ) {
                        rect Bar_Bound = Out_Bound;
                        Bar_Bound.Left   = Bar_Bound.Right;
                        Bar_Bound.Right += 8.0f;
                        
                        flo32 tTimer = Clamp01( AppState->TimeBonus_Timer / TIME_BONUS_TARGET_TIME );
                        rect Inner_Bound = AddRadius( Bar_Bound, -1.0f );
                        Inner_Bound.Top = lerp( Inner_Bound.Bottom, tTimer, Inner_Bound.Top );
                        
                        DrawRect( Pass_Game, Bar_Bound,   COLOR_GRAY( 0.05f ) );
                        DrawRect( Pass_Game, Inner_Bound, TowardBlack( COLOR_GREEN, 0.5f ) );
                    }
                }
            }
            
            { // Update and Draw Lockers
                AppState->ToFind_ShowItem_Timer = MaxValue( AppState->ToFind_ShowItem_Timer - dT, 0.0f );
                if( AppState->ToFind_ShowItem_Timer <= 0.0f ) {
                    AppState->ToFind_Wrong_iLocker = -1;
                }
                
                flo32 MaxDim         = 180.0f;
                flo32 MaxDistBetween = 8.0f;
                flo32 MaxMargin      = 8.0f;
                
                Locker_TotalBound        = Rect( 100.0f, 100.0f, 960.0f, 980.0f );
                flo32 TotalBound_InnerMargin =  8.0f;
                flo32 TotalBound_OuterMargin = 12.0f;
                
                rect  Locker_Bound       = AddRadius( Locker_TotalBound, -( TotalBound_OuterMargin + TotalBound_InnerMargin ) );
                vec2  Locker_Dim         = Vec2( MaxDim, MaxDim );
                flo32 Locker_DistBetween = MaxDistBetween;
                flo32 Locker_Margin      = MaxMargin;
                
                flo32 Width  = ( ( Locker_Dim.x + Locker_DistBetween ) * ( flo32 )AppState->Locker_nCol ) - Locker_DistBetween;
                flo32 Height = ( ( Locker_Dim.y + Locker_DistBetween ) * ( flo32 )AppState->Locker_nRow ) - Locker_DistBetween;
                
                if( ( Width > GetWidth( Locker_Bound ) ) || ( Height > GetHeight( Locker_Bound ) ) ) {
                    flo32 Wid = GetWidth ( Locker_Bound );
                    flo32 Hei = GetHeight( Locker_Bound );
                    
                    flo32 Dim         = MaxDim;
                    flo32 DistBetween = MaxDistBetween;
                    flo32 Margin      = MaxMargin;
                    
                    if( Width > Wid ) {
                        flo32 xRatio = Wid / Width;
                        
                        Dim         = MinValue( MaxDim         * xRatio, Dim );
                        DistBetween = MinValue( MaxDistBetween * xRatio, DistBetween );
                        Margin      = MinValue( MaxMargin      * xRatio, MaxMargin );
                    }
                    if( Height > Hei ) {
                        flo32 yRatio = Hei / Height;
                        
                        Dim         = MinValue( MaxDim         * yRatio, Dim );
                        DistBetween = MinValue( MaxDistBetween * yRatio, DistBetween );
                        Margin      = MinValue( MaxMargin      * yRatio, MaxMargin );
                    }
                    
                    // Recalculate Locker Dim
                    Locker_Dim         = Vec2( Dim, Dim );
                    Locker_DistBetween = DistBetween;
                    Locker_Margin      = Margin;
                    
                    Width  = ( ( Locker_Dim.x + Locker_DistBetween ) * ( flo32 )AppState->Locker_nCol ) - Locker_DistBetween;
                    Height = ( ( Locker_Dim.y + Locker_DistBetween ) * ( flo32 )AppState->Locker_nRow ) - Locker_DistBetween;
                }
                
                {
                    vec4 Outline_Color    = COLOR_GRAY( 0.025f );
                    vec4 Background_Color = COLOR_GRAY( 0.05f  );
                    
                    if( ( AppState->ToStore > 0 ) || ( AppState->ToFind > 0 ) ) {
                        Outline_Color    = COLOR_GRAY( 0.05f );
                        Background_Color = COLOR_GRAY( 0.075f );
                    }
                    
                    rect Inner_Bound = RectCD( GetCenter( Locker_Bound ), Vec2( Width + TotalBound_InnerMargin * 2.0f, Height + TotalBound_InnerMargin * 2.0f ) );
                    rect Outer_Bound = AddRadius( Inner_Bound, TotalBound_OuterMargin );
                    
                    DrawRect( Pass_Game, Outer_Bound, Outline_Color );
                    DrawRect( Pass_Game, Inner_Bound, Background_Color );
                }
                
                vec2 bPos = GetCenter( Locker_Bound ) - Vec2( Width, Height ) * 0.5f;
                
                int32 nLocker = AppState->Locker_nRow * AppState->Locker_nCol;
                for( int32 iLocker = 0; iLocker < nLocker; iLocker++ ) {
                    LOCKER * Locker = AppState->Locker + iLocker;
                    
                    int32 iRow = iLocker / AppState->Locker_nCol;
                    int32 iCol = iLocker % AppState->Locker_nCol;
                    
                    vec2 Pos = bPos;
                    Pos.x += ( Locker_Dim.x + Locker_DistBetween ) * ( flo32 )iCol;
                    Pos.y += ( Locker_Dim.y + Locker_DistBetween ) * ( flo32 )iRow;
                    
                    rect Bound = RectBLD( Pos, Locker_Dim );
                    
                    vec4 Outline_Color = COLOR_GRAY( 0.1f );
                    boo32 CanSelect = false;
                    if( IsInRect( MouseP, Bound ) ) {
                        if( ( AppState->ToStore > 0 ) && ( Locker->ItemID == 0 ) ) {
                            CanSelect = true;
                        }
                        if( ( AppState->ToFind  > 0 ) && ( Locker->ItemID >  0 ) ) {
                            CanSelect = true;
                        }
                    }
                    
                    if( CanSelect ) {
                        Outline_Color = COLOR_GRAY( 0.5f );
                        
                        if( WasPressed( Mouse, MouseButton_Left ) ) {
                            if( AppState->ToStore > 0 ) {
                                Locker->ItemID    = AppState->ToStore;
                                AppState->ToStore = 0;
                                
                                AppState->Locker_nFull += 1;
                            }
                            if( AppState->ToFind > 0 ) {
                                if( Locker->ItemID == AppState->ToFind ) {
                                    AppState->Correct_Timer  = LOCKER_CORRECT_TARGET_TIME;
                                    AppState->Correct_ItemID = Locker->ItemID;
                                    
                                    AppState->Item[ AppState->nItem ] = Locker->ItemID;
                                    AppState->nItem += 1;
                                    
                                    AppState->Locker_nFull -= 1;
                                    
                                    // Calculate Points
                                    int32 Points = 1 + AppState->Locker_nFull;
                                    if( AppState->TimeBonus_Timer > 0.0f ) {
                                        Points *= 2;
                                    }
                                    AppState->Score                 += Points;
                                    AppState->nCorrectItemsReturned += 1;
                                    AppState->LastPoints             = Points;
                                    AppState->LastPoints_Timer       = LAST_POINTS_TARGET_TIME;
                                    
                                    Locker->ItemID   = 0;
                                    AppState->ToFind = 0;
                                    
                                    AppState->ToFind_Wrong_iLocker = -1;
                                    AppState->ToFind_ShowItem_Timer   = 0.0f;
                                } else {
                                    AppState->ToFind_Wrong_iLocker = iLocker;
                                    AppState->ToFind_ShowItem_Timer   = LOCKER_SHOW_ITEM_TARGET_TIME;
                                    
                                    AppState->nWrongItemsReturned += 1;
                                }
                            }
                        }
                    }
                    DrawRect( Pass_Game, Bound, Outline_Color );
                    
                    TEXTURE_ID TextureID = TextureID_CardboardBox_Empty;
                    if( Locker->ItemID > 0 ) {
                        TextureID = TextureID_Locker_Full;
                    }
                    
                    vec4 Locker_Color = COLOR_GRAY( 0.5f );
                    if( ( AppState->ToStore > 0 ) || ( AppState->ToFind > 0 ) ) {
                        Locker_Color = COLOR_WHITE;
                    }
                    DrawRect( Pass_Game, TextureID, AddRadius( Bound, -Locker_Margin ), Locker_Color );
                    
                    if( ( AppState->ToFind_Wrong_iLocker == iLocker ) && ( AppState->ToFind_ShowItem_Timer > 0.0f ) ) {
                        vec4  Color      = COLOR_WHITE;
                        flo32 CycleTimer = LOCKER_SHOW_ITEM_TARGET_TIME / 5.0f;
                        if( ( ( int32 )( AppState->ToFind_ShowItem_Timer / CycleTimer ) % 2 ) == 0 ) {
                            Color = COLOR_RED;
                        }
                        DrawRect( Pass_Game, TextureID_CardboardBox_Empty, AddRadius( Bound, -Locker_Margin ), Color );
                        
                        DrawLockerContents( Pass_Game, AppState->FontList + FontID_6_00, GetCenter( Bound ), Locker->ItemID );
                    }
                }
            }
            
            { // Update and Draw Correct Item
                if( AppState->Correct_ItemID > 0 ) {
                    rect Bound = RectRCD( Vec2( 1052.0f, 540.0f ), Vec2( 180.0f, 180.0f ) );
                    
                    flo32 CycleTimer = ( 6.0f / 60.0f );
                    vec4  Color      = TowardBlack( COLOR_YELLOW, 0.75f );
                    if( ( ( int32 )( AppState->Correct_Timer / CycleTimer ) % 2 ) == 0 ) {
                        Color = COLOR_GRAY( 0.05f );
                    }
                    DrawRect( Pass_Game, Bound, Color );
                    
                    DrawLockerContents( Pass_Game, AppState->FontList + FontID_6_00, GetCenter( Bound ), AppState->Correct_ItemID );
                    
                    AppState->Correct_Timer = MaxValue( AppState->Correct_Timer - dT, 0.0f );
                    if( AppState->Correct_Timer <= 0.0f ) {
                        AppState->Correct_ItemID = 0;
                    }
                }
            }
            
            { // Update and Draw Increase/Decrease Lockers Buttons
                flo32 DistBetween = 20.0f;
                flo32 DistFrom    = 12.0f;
                vec2  Button_Dim  = Vec2( 48.0f, 48.0f );
                
                vec2  yOffset = Vec2( 0.0f, ( DistBetween + Button_Dim.y ) * 0.5f );
                vec2  xOffset = Vec2( ( DistBetween + Button_Dim.x ) * 0.5f, 0.0f );
                
                vec2 Col_bPos = GetT( Locker_TotalBound, Vec2( 0.0f, 0.5f ) ) + Vec2( -( DistFrom + Button_Dim.x * 0.5f ), 0.0f );
                vec2 Row_bPos = GetT( Locker_TotalBound, Vec2( 0.5f, 1.0f ) ) + Vec2( 0.0f, ( DistFrom + Button_Dim.y * 0.5f ) );
                vec2 ColUp_Pos   = Col_bPos + yOffset;
                vec2 ColDown_Pos = Col_bPos - yOffset;
                vec2 RowUp_Pos   = Row_bPos + xOffset;
                vec2 RowDown_Pos = Row_bPos - xOffset;
                
                boo32 ColUp_IsValid   = true;
                boo32 ColDown_IsValid = true;
                boo32 RowUp_IsValid   = true;
                boo32 RowDown_IsValid = true;
                
                if( AppState->Locker_nFull > 0 ) {
                    ColUp_IsValid   = false;
                    ColDown_IsValid = false;
                    RowUp_IsValid   = false;
                    RowDown_IsValid = false;
                }
                if( ( ( AppState->Locker_nCol + 1 ) * AppState->Locker_nRow ) > ITEM_MAX_COUNT ) {
                    ColUp_IsValid = false;
                }
                if( ( AppState->Locker_nCol * ( AppState->Locker_nRow + 1 ) ) > ITEM_MAX_COUNT ) {
                    RowUp_IsValid = false;
                }
                if( AppState->Locker_nCol <= 1 ) {
                    ColDown_IsValid = false;
                }
                if( AppState->Locker_nRow <= 1 ) {
                    RowDown_IsValid = false;
                }
                
                
                if( UpdateAndDrawLockerButton( Pass_Game, Mouse, TextureID_Icon_LockerUp, RectCD( ColUp_Pos, Button_Dim ), ColUp_IsValid ) ) {
                    AppState->Locker_nCol += 1;
                }
                if( UpdateAndDrawLockerButton( Pass_Game, Mouse, TextureID_Icon_LockerDown, RectCD( ColDown_Pos, Button_Dim ), ColDown_IsValid ) ) {
                    AppState->Locker_nCol -= 1;
                    
                }
                if( UpdateAndDrawLockerButton( Pass_Game, Mouse, TextureID_Icon_LockerUp, RectCD( RowUp_Pos, Button_Dim ), RowUp_IsValid ) ) {
                    AppState->Locker_nRow += 1;
                }
                if( UpdateAndDrawLockerButton( Pass_Game, Mouse, TextureID_Icon_LockerDown, RectCD( RowDown_Pos, Button_Dim ), RowDown_IsValid ) ) {
                    AppState->Locker_nRow -= 1;
                }
            }
            
            { // Update and Draw In/Out Buttons
                boo32 CanStore = true;
                boo32 CanFind  = true;
                
                int32 nMax = AppState->Locker_nRow * AppState->Locker_nCol;
                if( AppState->Locker_nFull >= nMax ) {
                    CanStore = false;
                }
                if( AppState->Locker_nFull <= 0 ) {
                    CanFind  = false;
                }
                if( ( AppState->ToStore > 0 ) || ( AppState->ToFind > 0 ) ) {
                    CanStore = false;
                    CanFind  = false;
                }
                if( AppState->nItem <= 0 ) {
                    CanStore = false;
                }
                
                vec2  Button_Dim    = Vec2( 200.0f, 80.0f );
                flo32 Button_Margin = 8.0f;
                vec2  Arrow_Dim     = Vec2( 52.0f, 60.0f );
                flo32 DistBetween   = 80.0f;
                flo32 DistFromRight = 200.0f;
                vec2  bPos = Vec2( UI_Bound.Right - DistFromRight - Button_Dim.x * 0.5f, 540.0f );
                
                vec2  Offset  = Vec2( 0.0f, ( DistBetween + Button_Dim.y ) * 0.5f );
                vec2  In_Pos  = bPos + Offset;
                vec2  Out_Pos = bPos - Offset;
                rect In_Bound  = RectCD( In_Pos,  Button_Dim );
                rect Out_Bound = RectCD( Out_Pos, Button_Dim );
                
                vec4 In_Color  = COLOR_GREEN;
                vec4 Out_Color = COLOR_RED;
                
                {
                    vec4 Outline_Color = COLOR_GRAY( 0.05f );
                    vec4 Arrow_Color   = TowardBlack( In_Color, 0.9f );
                    if( CanStore ) {
                        Outline_Color = COLOR_GRAY( 0.25f );
                        Arrow_Color   = In_Color;
                        
                        if( IsInRect( MouseP, In_Bound ) ) {
                            Outline_Color = COLOR_GRAY( 0.5f );
                            if( WasPressed( Mouse, MouseButton_Left ) ) {
                                int32 Select = RandomS32InRange( 0, AppState->nItem - 1 );
                                AppState->ToStore = AppState->Item[ Select ];
                                AppState->nItem  -= 1;
                                AppState->Item[ Select ] = AppState->Item[ AppState->nItem ];
                            }
                        }
                    }
                    
                    DrawRect( Pass_Game, In_Bound, Outline_Color );
                    DrawRect( Pass_Game, AddRadius( In_Bound, -Button_Margin ), COLOR_BLACK );
                    
                    orect Bound = ORectCD( GetCenter( In_Bound ), Arrow_Dim, PI * 0.5f );
                    DrawORect( Pass_Game, TextureID_Arrow, Bound,  Arrow_Color );
                }
                
                {
                    vec4 Outline_Color = COLOR_GRAY( 0.05f );
                    vec4 Arrow_Color   = TowardBlack( Out_Color, 0.9f );
                    if( CanFind ) {
                        Outline_Color = COLOR_GRAY( 0.25f );
                        Arrow_Color   = Out_Color;
                        
                        if( IsInRect( MouseP, Out_Bound ) ) {
                            Outline_Color = COLOR_GRAY( 0.5f );
                            if( WasPressed( Mouse, MouseButton_Left ) ) {
                                int32 nLocker = AppState->Locker_nRow * AppState->Locker_nCol;
                                int32 Select  = RandomS32InRange( 0, nLocker - 1 );
                                
                                LOCKER * Locker = AppState->Locker + Select;
                                while( Locker->ItemID == 0 ) {
                                    Select = ( Select + 1 ) % nLocker;
                                    Locker = AppState->Locker + Select;
                                }
                                
                                AppState->ToFind = Locker->ItemID;
                                
                                AppState->TimeBonus_Timer = TIME_BONUS_TARGET_TIME;
                            }
                        }
                    }
                    
                    DrawRect( Pass_Game, Out_Bound, Outline_Color );
                    DrawRect( Pass_Game, AddRadius( Out_Bound, -Button_Margin ), COLOR_BLACK );
                    
                    orect Bound = ORectCD( GetCenter( Out_Bound ), Arrow_Dim, -PI * 0.5f );
                    DrawORect( Pass_Game, TextureID_Arrow, Bound,  Arrow_Color );
                }
            }
#endif
            
            { // Update and Draw Score
                AppState->LastPoints_Timer = MaxValue( AppState->LastPoints_Timer - dT, 0.0f );
                
                FONT * Font = AppState->FontList + FontID_2_00;
                DrawLabelColonS32( Pass_Game, Font, "Score", AppState->Score, Vec2( 980.0f, 1000.0f ), 1080.0f, 1180.0f, COLOR_WHITE );
                
                if( AppState->LastPoints_Timer > 0.0f ) {
                    flo32 Alpha = Clamp01( AppState->LastPoints_Timer / ( 20.0f / 60.0f ) );
                    
                    char Str[ 32 ] = {};
                    sprintf( Str, "+%d", AppState->LastPoints );
                    DrawString( Pass_Game, Font, Str, Vec2( 1220.0f, 1000.0f ), TextAlign_TopLeft, COLOR_YELLOW * Alpha );
                    if( AppState->TimeBonus_Timer > 0.0f ) {
                        flo32 Width = GetDimOfString( Font, Str ).Dim.x;
                        DrawString( Pass_Game, Font, "(2x)", Vec2( 1240.0f + Width, 1000.0f ), TextAlign_TopLeft, COLOR_GREEN * Alpha );
                    }
                }
                
                DrawLabelColonS32( Pass_Game, Font, "Items Returned", AppState->nCorrectItemsReturned, Vec2( 1380.0f, 1000.0f ), 1680.0f, 1780.0f, COLOR_WHITE );
                DrawLabelColonS32( Pass_Game, Font, "Mistakes", AppState->nWrongItemsReturned, Vec2( 1380.0f, 960.0f ), 1680.0f, 1780.0f, COLOR_WHITE );
            }
            
            { // Finalize Time Bonus
                AppState->TimeBonus_Timer = MaxValue( AppState->TimeBonus_Timer - dT, 0.0f );
            }
        } break;
        
        default: {
            InvalidCodePath;
        } break;
    }
    
    DISPLAY_VALUE( uint64, PermMemory->Used );
    DISPLAY_VALUE( uint64, TempMemory->Used );
    Assert( TempMemory->Used < TempMemory->Size );
}