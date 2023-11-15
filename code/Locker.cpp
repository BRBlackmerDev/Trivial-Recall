
internal void
LogItemToDebug( boo32 ItemWasFound, int32 ItemID ) {
    char * Label[ ITEM_MAX_COUNT ] = {
        "SCISSORS",  "RACECAR",    "BASEBALL",   "SOCCER",     "HOCKEY",     "BASKETBALL",   "GIRAFFE",   "ELEPHANT",
        "MUSHROOM",  "STOPLIGHT",  "STOPSIGN",   "CAR",        "BUS",        "LION",         "TIGER",     "MONKEY",
        "CARROT",    "LETTUCE",    "WATERMELON", "TOMATO",     "COP",        "MAILMAN",      "FIREMAN",   "DOCTOR",
        "CUPCAKE",   "MUG",        "POT",        "SPOON",      "FORK",       "SOCK",         "SHOES",     "SHIRT",
        "MUSIC",     "LAMP",       "CHAIR",      "GUITAR",     "PIANO",      "DRUM",         "TRUMPET",   "HAT",
        "SUN",       "MOON",       "STAR",       "GLOBE",      "HEART",      "TREE",         "FLOWER",    "PENCIL",
        "COW",       "PIG",        "DOG",        "CAT",        "CHICKEN",    "APPLE",        "BANANA",    "CHERRY",
        "HOUSE",     "BALLOON",    "BICYCLE",    "GEAR",       "FIRE",       "WATER",        "LIGHTNING", "ORANGE",
        "SHEEP",     "AIRPLANE",   "TENNIS",     "BENCH",      "BLUE",       "RED",          "GREEN",     "BIRD",
        "FISH",      "DOLPHIN",    "CRAB",       "SNAIL",      "LEMON",      "CLOCK",        "BOAT",      "SCUBA DIVER",
        "HORSE",     "MOUNTAIN",   "COOKIES",    "ICE CREAM",  "CACTUS",     "BOOK",         "ROCKET",    "EAR",
        "PERCENT",   "DOLLAR",     "TEDDY BEAR", "DINOSAUR",   "SKELETON",   "HAND",         "EYE",       "TEETH",
        "MATH",      "KNIGHT",     "SOLDIER",    "QUEEN",      "SUNGLASSES", "UP",           "DOWN",      "HASHTAG",
        "PARACHUTE", "TORNADO",    "LAPTOP",     "HEADPHONES", "CONTROLLER", "BOWLING",      "MOLECULE",  "BEAKER",
        "LADDER",    "PAINTBRUSH", "HAMMER",     "WRENCH",     "SHOVEL",     "WATERING CAN", "BRIDGE",    "SNOWFLAKE",
        "CLOWN",     "GHOST",      "ALIEN",      "WITCH",      "BED",        "HAPPY",        "SAD",       "TOILET",
    };
    
    char Str[ 32 ] = {};
    if( ItemWasFound ) {
        sprintf( Str, "RETURN  %s", Label[ ItemID ] );
    } else {
        sprintf( Str, "STORE   %s", Label[ ItemID ] );
    }
    CONSOLE_STRING( Str );
}

internal void
ResetGameMemory( APP_STATE * AppState ) {
    int32 Size = ( int32 )( &AppState->Reset_GameMemory_End - &AppState->Reset_GameMemory_Start );
    memset( &AppState->Reset_GameMemory_Start, 0, Size );
}

internal void
InitHighScores( APP_STATE * AppState ) {
    SAVE_GAME * SaveGame = &AppState->SaveGame;
    
    SaveGame->FullGame_HighScore[ 0 ] = HighScoreC( true,  90, 10 );
    SaveGame->FullGame_HighScore[ 1 ] = HighScoreC( true,  55,  7 );
    SaveGame->FullGame_HighScore[ 2 ] = HighScoreC( true,  30,  5 );
}

internal SECONDARY_OBJECTIVE
GetNextSecondaryObjective( APP_STATE * AppState ) {
    int32 nActive = 0;
    for( int32 Iter = 0; Iter < SecondaryObjective_Count; Iter++ ) {
        SECONDARY_OBJECTIVE_STATUS Status = AppState->SecondaryObjective_Order[ Iter ];
        if( Status.IsActive ) {
            nActive += 1;
        }
    }
    
    SECONDARY_OBJECTIVE Result = SecondaryObjective_None;
    if( nActive > 0 ) {
        while( Result == SecondaryObjective_None ) {
            SECONDARY_OBJECTIVE_STATUS Status = AppState->SecondaryObjective_Order[ AppState->SecondaryObjective_iOrder ];
            
            if( Status.IsActive ) {
                Result = Status.SecondaryObjective;
            } else {
                AppState->SecondaryObjective_iOrder = ( AppState->SecondaryObjective_iOrder + 1 ) % SecondaryObjective_Count;
            }
        }
        
        AppState->SecondaryObjective_iOrder = ( AppState->SecondaryObjective_iOrder + 1 ) % SecondaryObjective_Count;
    }
    
    return Result;
}

internal SCORE_UPDATE
ScoreUpdateC( SCORE_UPDATE_TYPE Type ) {
    SCORE_UPDATE Result = {};
    Result.Type       = Type;
    
    switch( Type ) {
        case ScoreUpdate_OnePoint: {
            Result.TargetTime = ( 90.0f / 60.0f );
        } break;
        
        case ScoreUpdate_OneBonusPoint: {
            Result.TargetTime = ( 90.0f / 60.0f );
        } break;
        
        case ScoreUpdate_TwoPoints: {
            Result.TargetTime = ( 120.0f / 60.0f );
        } break;
        
        case ScoreUpdate_ThreePoints: {
            Result.TargetTime = ( 120.0f / 60.0f );
        } break;
        
        case ScoreUpdate_OneHealth: {
            Result.TargetTime = ( 90.0f / 60.0f );
        } break;
        
        case ScoreUpdate_OnePoint_OneHealth: {
            Result.TargetTime = ( 120.0f / 60.0f );
        } break;
        
        case ScoreUpdate_OnePoint_TwoHealth: {
            Result.TargetTime = ( 120.0f / 60.0f );
        } break;
        
        case ScoreUpdate_TwoPoints_OneHealth: {
            Result.TargetTime = ( 120.0f / 60.0f );
        } break;
        
        case ScoreUpdate_LoseHealth: {
            Result.TargetTime = ( 90.0f / 60.0f );
        } break;
    }
    
    return Result;
}

internal void
AddScoreUpdate( APP_STATE * AppState, SCORE_UPDATE_TYPE Type ) {
    SCORE_UPDATE ScoreUpdate = ScoreUpdateC( Type );
    
    if( AppState->nScoreUpdate > 0 ) {
        SCORE_UPDATE LastAdded = AppState->ScoreUpdate[ AppState->nScoreUpdate - 1 ];
        ScoreUpdate.Timer = MinValue( LastAdded.Timer - LastAdded.TargetTime, 0.0f );
    }
    
    Assert( AppState->nScoreUpdate < SCORE_UPDATE_MAX_COUNT );
    AppState->ScoreUpdate[ AppState->nScoreUpdate++ ] = ScoreUpdate;
}

internal LOCKER *
GetLockerByItemID( APP_STATE * AppState, int32 ItemID ) {
    Assert( ItemID >= 0 );
    
    LOCKER * Result = 0;
    for( int32 iLocker = 0; ( !Result ) && ( iLocker < AppState->nLocker ); iLocker++ ) {
        LOCKER * Locker = AppState->Locker + iLocker;
        if( Locker->ItemID == ItemID ) {
            Result = Locker;
        }
    }
    
    Assert( Result );
    return Result;
}

internal int32
GetILockerByItemID( APP_STATE * AppState, int32 ItemID ) {
    Assert( ItemID >= 0 );
    
    int32 Result = -1;
    for( int32 iLocker = 0; ( Result <= -1 ) && ( iLocker < AppState->nLocker ); iLocker++ ) {
        LOCKER * Locker = AppState->Locker + iLocker;
        if( Locker->ItemID == ItemID ) {
            Result = iLocker;
        }
    }
    
    Assert( Result >= 0 );
    return Result;
}

internal void
GetItemsForWhichInSetQuestion( MEMORY * TempMemory, APP_STATE * AppState ) {
    AppState->SecondaryObjective_IsInSet = ( boo32 )RandomS32InRange( 0, 1 );
    
    boo32 * IsInSet0 = _PushArray_Clear( TempMemory, boo32, ITEM_MAX_COUNT );
    boo32 * IsInSet1 = _PushArray_Clear( TempMemory, boo32, ITEM_MAX_COUNT );
    int32   Select   = -1;
    
    for( int32 iLocker = 0; iLocker < AppState->nLocker; iLocker++ ) {
        LOCKER Locker = AppState->Locker[ iLocker ];
        IsInSet0[ Locker.ItemID ] = true;
        IsInSet1[ Locker.ItemID ] = true;
    }
    
    int32 ItemID_IsInSet    = 0;
    int32 ItemID_IsNotInSet = 0;
    
    { // Is In Set
        Assert( AppState->ToFind >= 0 );
        IsInSet0[ AppState->ToFind ] = false;
        
        if( AppState->LastItemStored > 0 ) {
            IsInSet0[ AppState->LastItemStored ] = false;
        }
        
        Select = RandomS32InRange( 0, ITEM_MAX_COUNT - 1 );
        while( !IsInSet0[ Select ] ) {
            Select = ( Select + 1 ) % ITEM_MAX_COUNT;
        }
        
        ItemID_IsInSet = Select;
    }
    
    { // Is Not In Set
        Assert( AppState->ToFind >= 0 );
        IsInSet1[ AppState->ToFind ] = true;
        
        Select = RandomS32InRange( 0, ITEM_MAX_COUNT - 1 );
        while( IsInSet1[ Select ] ) {
            Select = ( Select + 1 ) % ITEM_MAX_COUNT;
        }
        
        ItemID_IsNotInSet = Select;
    }
    
    if( AppState->SecondaryObjective_IsInSet ) {
        AppState->SecondaryObjective_ItemID0 = ItemID_IsInSet;
        AppState->SecondaryObjective_ItemID1 = ItemID_IsNotInSet;
    } else {
        AppState->SecondaryObjective_ItemID0 = ItemID_IsNotInSet;
        AppState->SecondaryObjective_ItemID1 = ItemID_IsInSet;
    }
    
    AppState->SecondaryObjective_LastItemAsked0 = AppState->SecondaryObjective_ItemID0;
    AppState->SecondaryObjective_LastItemAsked1 = AppState->SecondaryObjective_ItemID1;
    
    AppState->SecondaryObjective_ToFind = ItemID_IsInSet;
    
    AppState->SecondaryObjective_IsActive = true;
    
    _PopArray( TempMemory, boo32, ITEM_MAX_COUNT );
    _PopArray( TempMemory, boo32, ITEM_MAX_COUNT );
}

internal void
GetItemForYesNoInSetQuestion( MEMORY * TempMemory, APP_STATE * AppState ) {
    AppState->SecondaryObjective_IsInSet = ( boo32 )RandomS32InRange( 0, 1 );
    
    boo32 * IsInSet = _PushArray_Clear( TempMemory, boo32, ITEM_MAX_COUNT );
    int32   Select      = -1;
    
    for( int32 iLocker = 0; iLocker < AppState->nLocker; iLocker++ ) {
        LOCKER Locker = AppState->Locker[ iLocker ];
        IsInSet[ Locker.ItemID ] = true;
    }
    
    if( AppState->SecondaryObjective_IsInSet ) {
        Assert( AppState->ToFind >= 0 );
        IsInSet[ AppState->ToFind ] = false;
        
        if( AppState->LastItemStored >= 0 ) {
            IsInSet[ AppState->LastItemStored ] = false;
        }
        
        Select = RandomS32InRange( 0, ITEM_MAX_COUNT - 1 );
        while( !IsInSet[ Select ] ) {
            Select = ( Select + 1 ) % ITEM_MAX_COUNT;
        }
        
        AppState->SecondaryObjective_ToFind = Select;
    } else {
        Select = RandomS32InRange( 0, ITEM_MAX_COUNT - 1 );
        while( IsInSet[ Select ] ) {
            Select = ( Select + 1 ) % ITEM_MAX_COUNT;
        }
    }
    
    Assert( Select > -1 );
    AppState->SecondaryObjective_ItemID0  = Select;
    AppState->SecondaryObjective_IsActive = true;
    
    _PopArray( TempMemory, boo32, ITEM_MAX_COUNT );
}

internal void
GetItemToFind( MEMORY * TempMemory, APP_STATE * AppState ) {
    Assert( AppState->ToFind <= -1 );
    
    if( AppState->SecondaryObjective_ToFind >= 0 ) {
        AppState->ToFind                    = AppState->SecondaryObjective_ToFind;
        AppState->SecondaryObjective_ToFind = -1;
    } else {
        int32 * iAvailable = _PushArray( TempMemory, int32, AppState->nLocker );
        int32   nAvailable = 0;
        
        for( int32 iLocker = 0; iLocker < AppState->nLocker; iLocker++ ) {
            LOCKER Locker = AppState->Locker[ iLocker ];
            if( Locker.ItemID != AppState->LastItemStored ) {
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
    
    if( AppState->SecondaryObjective == SecondaryObjective_QuickTimer ) {
        AppState->SecondaryObjective_Timer = SECONDARY_OBJECTIVE_QUICK_TIMER_TARGET_TIME;
    }
    if( ( AppState->SecondaryObjective == SecondaryObjective_ExtraItem ) && ( AppState->nExtraLocker >= 1 ) ) {
        AppState->SecondaryObjective_DoTwoBonusPoints = true;
    }
}

internal int32
GetRandomItemFromItemSet( APP_STATE * AppState ) {
    int32 nItemPerBlock = ITEM_MAX_COUNT / 8;
    int32 Select        = RandomS32InRange( 0, nItemPerBlock - 1 );
    
    int32 Result = AppState->Item[ Select ];
    AppState->nItem  -= 1;
    
    int32 PrevSelect = Select;
    int32 bItem      = nItemPerBlock;
    while( ( bItem + nItemPerBlock - 1 ) <= AppState->nItem ) {
        int32 MaxItem = MinValue( bItem + nItemPerBlock - 1, AppState->nItem );
        Select = RandomS32InRange( bItem, MaxItem );
        
        AppState->Item[ PrevSelect ] = AppState->Item[ Select ];
        
        PrevSelect = Select;
        bItem += nItemPerBlock;
    }
    
    AppState->Item[ PrevSelect ] = AppState->Item[ AppState->nItem ];
    
    return Result;
}

internal void
GetItemToStore( APP_STATE * AppState ) {
    Assert( AppState->ToStore <= -1 );
    AppState->ToStore = GetRandomItemFromItemSet( AppState );
    
    AppState->ToStore_Position = GetBC( AppState->ToStore_Bound ) + Vec2( 0.0f, -( ITEM_HEIGHT * 0.5f + 32.0f ) );
    AppState->ToStore_Velocity = Vec2( 0.0f, 800.0f );
}

internal void
GetItemForExtraItem( APP_STATE * AppState ) {
    Assert( AppState->SecondaryObjective_ItemID0 <= -1 );
    AppState->SecondaryObjective_ItemID0  = GetRandomItemFromItemSet( AppState );
    AppState->SecondaryObjective_IsActive = true;
    AppState->SecondaryObjective_Alpha    = 0.0f;
}

//----------
// SAVE/LOAD GAME
//----------

#define FILETAG__SAVEGAME_FILE  ( "SAVEGAME" )
#define FILENAME_SAVEGAME_FILE  ( "MyGame"   )

internal boo32
DoesGameFileExist( PLATFORM * Platform ) {
    char * SaveDir  = 0;
    char * FileTag  = FILETAG__SAVEGAME_FILE;
    char * FileName = FILENAME_SAVEGAME_FILE;
    
    boo32 Result = Platform->DoesFileExist( SaveDir, FileName, FileTag );
    return Result;
}

internal void
SaveGameFile( WRITE_FILE * WriteFile, APP_STATE * AppState, MEMORY * TempMemory ) {
    char * SaveDir  = 0;
    char * FileTag  = FILETAG__SAVEGAME_FILE;
    char * FileName = FILENAME_SAVEGAME_FILE;
    
    MEMORY  _Output = SubArena( TempMemory );
    MEMORY * Output = &_Output;
    
    _writem( Output, SAVE_GAME, AppState->SaveGame );
    
    boo32 IsValid = OutputFile( WriteFile, Output, SaveDir, FileName, FileTag );
    Assert( IsValid );
    
    _PopSize( TempMemory, Output->Size );
}

internal void
LoadGameFile( READ_FILE * ReadFile, APP_STATE * AppState, MEMORY * TempMemory ) {
    char * SaveDir  = 0;
    char * FileTag  = FILETAG__SAVEGAME_FILE;
    char * FileName = "MyGame";
    
    FILE_DATA File = ReadFile( TempMemory, SaveDir, FileName, FileTag );
    if( File.Contents ) {
        uint8 * Ptr = ( uint8 * )File.Contents;
        
        uint32 CheckSize = sizeof( SAVE_GAME );
        Assert( CheckSize == File.Size );
        
        AppState->SaveGame = _read( Ptr, SAVE_GAME );
        
        _PopSize( TempMemory, File.Size );
    }
}

internal void
PlaySound( APP_STATE * AppState, SOUND_ID SoundID ) {
    if( AppState->SoundIsActive ) {
#if USE_FMOD
        FMOD_STUDIO_EVENTDESCRIPTION * EventDescription = 0;
        FMOD_STUDIO_EVENTINSTANCE    * EventInstance    = 0;
        FMOD_GUID                      Guid             = AppState->FMOD_Guid[ SoundID ];
        
        FMOD_RESULT Result = FMOD_Studio_System_GetEventByID( AppState->FMOD_System, &Guid, &EventDescription );
        Assert( Result == FMOD_OK );
        
        Result = FMOD_Studio_EventDescription_CreateInstance( EventDescription, &EventInstance );
        Assert( Result == FMOD_OK );
        
        Result = FMOD_Studio_EventInstance_Start( EventInstance );
        Assert( Result == FMOD_OK );
        
        Result = FMOD_Studio_EventInstance_Release( EventInstance );
        Assert( Result == FMOD_OK );
#endif
    }
}

internal void
StartGame( MEMORY * TempMemory, APP_STATE * AppState, boo32 DoPractice = false ) {
    AppState->nItem = ITEM_MAX_COUNT;
    
    int32 * Temp = _PushArray( TempMemory, int32, AppState->nItem );
    int32  nTemp = AppState->nItem;
    
    for( int32 iItem = 0; iItem < AppState->nItem; iItem++ ) {
        Temp[ iItem ] = iItem;
    }
    for( int32 iItem = 0; iItem < AppState->nItem; iItem++ ) {
        int32 Select = RandomS32InRange( 0, nTemp - 1 );
        
        AppState->Item[ iItem ] = Temp[ Select ];
        
        nTemp -= 1;
        Temp[ Select ] = Temp[ nTemp ];
    }
    
    _PopArray( TempMemory, int32, AppState->nItem );
    
#if 0        
    for( int32 Iter = 0; Iter < 26; Iter++ ) {
        AppState->Item[ Iter ] = Iter + ( int32 )'A';
    }
    for( int32 Iter = 0; Iter < 26; Iter++ ) {
        AppState->Item[ Iter + 26 ] = Iter + ( int32 )'a';
    }
    AppState->nItem = 26;
#endif
    
    if( DoPractice ) {
        AppState->Practice_IsActive            = true;
        AppState->MinimumItemsUp_IsActive      = false;
        AppState->MaxHealth                    = PLAYER_MAX_HEALTH;
        AppState->MinimumItemsUp_ItemsPerLevel = GAME_ITEMS_PER_LEVEL;
        AppState->nMinimumItems                = AppState->Practice_nMinimum;
    } else {
        AppState->Practice_IsActive            = false;
        AppState->MinimumItemsUp_IsActive      = true;
        AppState->MaxHealth                    = PLAYER_MAX_HEALTH;
        AppState->MinimumItemsUp_ItemsPerLevel = GAME_ITEMS_PER_LEVEL;
        AppState->nMinimumItems                = 3;
    }
    
    AppState->ToMove_iLocker  = -1;
    AppState->ToFind          = -1;
    AppState->ToStore         = -1;
    AppState->Correct_ItemID  = -1;
    AppState->Health          = AppState->MaxHealth;
    for( int32 iHealthUnit = 0; iHealthUnit < AppState->Health; iHealthUnit++ ) {
        AppState->HealthUnit_tTimer[ iHealthUnit ] = 1.0f;
    }
    
    AppState->SecondaryObjective                = SecondaryObjective_None;
    AppState->SecondaryObjective_ItemID0        = -1;
    AppState->SecondaryObjective_ItemID1        = -1;
    AppState->SecondaryObjective_LastItemAsked0 = -1;
    AppState->SecondaryObjective_LastItemAsked1 = -1;
    AppState->SecondaryObjective_ToFind         = -1;
    
    { // Randomize Secondary Objective Order
        boo32 IsActive[ SecondaryObjective_Count ] = {};
        IsActive[ SecondaryObjective_QuickTimer ] = false;
        IsActive[ SecondaryObjective_WhichInSet ] = false;
        IsActive[ SecondaryObjective_YesNoInSet ] = false;
        IsActive[ SecondaryObjective_ExtraItem  ] = true;
        
        int32 nList = SecondaryObjective_Count;
        int32  List[ SecondaryObjective_Count ] = {};
        for( int32 Iter = 0; Iter < SecondaryObjective_Count; Iter++ ) {
            List[ Iter ] = Iter;
        }
        
        for( int32 Iter = 0; Iter < SecondaryObjective_Count; Iter++ ) {
            int32 Select = RandomS32InRange( 0, nList - 1 );
            
            SECONDARY_OBJECTIVE_STATUS * Status = AppState->SecondaryObjective_Order + Iter;
            Status->SecondaryObjective = ( SECONDARY_OBJECTIVE )List[ Select ];
            Status->IsActive           = IsActive[ Status->SecondaryObjective ];
            
            nList -= 1;
            List[ Select ] = List[ nList ];
        }
    }
    
    AppState->HighScore_MostRecent_iEntry = -1;
    AppState->Background_dZoom = -( 1.0f / 2.75f );
    
    
    SAVE_GAME * SaveGame = &AppState->SaveGame;
    if( ( !DoPractice ) && ( !SaveGame->Tutorial_IsComplete ) ) {
        AppState->MinimumItemsUp_IsActive        = true;
        AppState->MinimumItemsUp_ItemsPerLevel   = TUTORIAL_ITEMS_PER_LEVEL;
        AppState->nMinimumItems                  = 2;
        AppState->Tutorial_IsActive         = true;
        
        AppState->SecondaryObjective_Order[ 0 ] = { SecondaryObjective_YesNoInSet, true };
        AppState->SecondaryObjective_Order[ 1 ] = { SecondaryObjective_ExtraItem,  true };
        AppState->SecondaryObjective_Order[ 2 ] = { SecondaryObjective_WhichInSet, true };
        AppState->SecondaryObjective_Order[ 3 ] = { SecondaryObjective_QuickTimer, true };
    }
    
    // NOTE : TEMP!
    for( int32 iPractice = 0; iPractice < PRACTICE_MAX_LEVEL; iPractice++ ) {
        SaveGame->Practice_HighScore[ iPractice ] = 1;
    }
    
    { // Init Layout
        rect UI_Bound = AppState->UI_Bound;
        
        rect Storage_Bound = UI_Bound;
        Storage_Bound.Left  += 320.0f;
        Storage_Bound.Right -= 420.0f;
        Storage_Bound.Top   -= 300.0f;
        
        rect ToStore_Bound = Storage_Bound;
        ToStore_Bound.Left  = UI_Bound.Left;
        ToStore_Bound.Right = Storage_Bound.Left;
        
        rect ToFind_Return = UI_Bound;
        ToFind_Return.Left  = Storage_Bound.Right;
        ToFind_Return.Top   = UI_Bound.Bottom + 508.0f;
        
        rect ToFind_Display = RectBCD( GetTC( ToFind_Return ) + Vec2( -60.0f, 20.0f ), LOCKER_DIM );
        rect ToFind_Text    = RectBCD( GetTC( ToFind_Display ) + Vec2( 0.0f, -4.0f ), Vec2( 200.0f, 40.0f ) );
        
        rect ScoreUpdate_Display = RectLCD( GetRC( ToFind_Display ) + Vec2( 36.0f, 8.0f ), Vec2( 100.0f, 112.0f ) );
        
        rect  Turn_Bound = ToFind_Return;
        {
            flo32 FromSides   = 40.0f;
            Turn_Bound.Left  += FromSides;
            Turn_Bound.Right -= ( FromSides + 60.0f );
            Turn_Bound.Bottom = ToFind_Display.Top + 28.0f;
            Turn_Bound.Top    = Turn_Bound.Bottom + 40.0f;
        }
        
        rect Question_Bound = UI_Bound;
        {
            flo32 FromSides = 120.0f;
            Question_Bound.Right  = Storage_Bound.Right - FromSides;
            Question_Bound.Bottom = Storage_Bound.Top;
            Question_Bound.Left   = Storage_Bound.Left + FromSides;
        }
        
        rect ScorePanel_Display = ToFind_Return;
        {
            flo32 FromSides = 48.0f;
            ScorePanel_Display.Left  += FromSides;
            ScorePanel_Display.Right -= FromSides;
            ScorePanel_Display.Top    = UI_Bound.Top - 48.0f;
            ScorePanel_Display.Bottom = ScorePanel_Display.Top - 100.0f;
            
        }
        
        rect Health_Display = RectTCD( GetBC( ScorePanel_Display ) + Vec2( 28.0f, -48.0f ), Vec2( 160.0f, 32.0f ) );
        
        AppState->Storage_Bound       = Storage_Bound;
        AppState->ToStore_Bound       = ToStore_Bound;
        AppState->ToFind_Return       = ToFind_Return;
        AppState->ToFind_Display      = ToFind_Display;
        AppState->ScoreUpdate_Display = ScoreUpdate_Display;
        AppState->ScorePanel_Display  = ScorePanel_Display;
        AppState->Health_Display      = Health_Display;
        //AppState->ToFind_Text         = ToFind_Text;
        AppState->Turn_Bound          = Turn_Bound;
        AppState->Question_Bound      = Question_Bound;
    }
    
    GetItemToStore( AppState );
}

internal void
DrawLockerContents( RENDER_PASS * Pass, rect Bound, int32 ItemID, flo32 Alpha = 1.0f, vec4 Color = COLOR_WHITE ) {
    Assert( ItemID >= 0 );
    
    TEXTURE_ID TextureID = ( TEXTURE_ID )( ( int32 )TextureID_Item + ItemID );
    DrawRect( Pass, TextureID, Bound, Color * Alpha );
}

internal void
DrawLockerContents( RENDER_PASS * Pass, FONT * Font, vec2 Pos, int32 ItemID, flo32 Alpha = 1.0f ) {
    Assert( ItemID >= 0 );
    
    TEXTURE_ID TextureID = ( TEXTURE_ID )( ( int32 )TextureID_Item + ItemID );
    rect Bound = RectCD( Pos, LOCKER_DIM );
    Bound = AddRadius( Bound, -16.0f );
    DrawRect( Pass, TextureID, Bound, COLOR_WHITE * Alpha );
    
#if 0    
    // NOTE : ItemID = ( iColor * 25 ) + ( iShape * 5 ) + ( iNum );
    
    ItemID -= 1;
    int32 iColor = ( ItemID / 25 );
    int32 iShape = ( ItemID % 25 ) / 5;
    int32 iNum   = ( ItemID % 5  ) + 1;
    
    vec4 Color[] = {
        COLOR_WHITE,
        COLOR_YELLOW,
        COLOR_RED,
        COLOR_GREEN,
        COLOR_BLUE,
    };
    
    TEXTURE_ID Shape[] = {
        TextureID_Shape_Circle,
        TextureID_Shape_Cross,
        TextureID_Shape_Triangle,
        TextureID_Shape_Square,
        TextureID_Shape_Star,
    };
    
    vec2 Icon_Offset = Vec2( 40.0f, 40.0f );
    vec2 Icon_Dim    = Vec2( 32.0f, 32.0f );
    
    vec2 P[] = {
        Pos,
        Pos - Icon_Offset,
        Pos + Vec2(  Icon_Offset.x, -Icon_Offset.y ),
        Pos + Icon_Offset,
        Pos + Vec2( -Icon_Offset.x,  Icon_Offset.y ),
    };
    
    boo32 DoDrawShape[ 5 ] = {};
    if( ( iNum == 1 ) || ( iNum == 3 ) || ( iNum == 5 ) ) {
        DoDrawShape[ 0 ] = true;
    }
    if( iNum >= 2 ) {
        DoDrawShape[ 1 ] = true;
        DoDrawShape[ 3 ] = true;
    }
    if( iNum >= 4 ) {
        DoDrawShape[ 2 ] = true;
        DoDrawShape[ 4 ] = true;
    }
    
    for( int32 iIter = 0; iIter < 5; iIter++ ) {
        if( DoDrawShape[ iIter ] ) {
            rect Bound = RectCD( P[ iIter ], Icon_Dim );
            DrawRect( Pass, Shape[ iShape ], Bound, Color[ iColor ] );
        }
    }
#endif
    
#if 0    
    rect Bound = RectCD( Pos, LOCKER_DIM );
    char Str[ 16 ] = {};
    sprintf( Str, "ItemID: %u", ItemID );
    DrawString( Pass, Font, Str, GetT( Bound, Vec2( 0.0f, 1.0f ) ), TextAlign_TopLeft, COLOR_WHITE * Alpha );
    sprintf( Str, "Color: %u", iColor );
    DrawString( Pass, Font, Str, GetT( Bound, Vec2( 0.0f, 0.8f ) ), TextAlign_TopLeft, COLOR_WHITE * Alpha );
    sprintf( Str, "Shape: %u", iShape );
    DrawString( Pass, Font, Str, GetT( Bound, Vec2( 0.0f, 0.6f ) ), TextAlign_TopLeft, COLOR_WHITE * Alpha );
    sprintf( Str, "Num:   %u", iNum   );
    DrawString( Pass, Font, Str, GetT( Bound, Vec2( 0.0f, 0.4f ) ), TextAlign_TopLeft, COLOR_WHITE * Alpha );
#endif
    
    
    //char Str[ 2 ] = {};
    //Str[ 0 ] = ( char )ItemID;
    //DrawString( Pass, Font, Str, Pos, TextAlign_Center, COLOR_WHITE * Alpha );
}

internal boo32
UpdateAndDrawMenuButton( RENDER_PASS * Pass, MOUSE_STATE * Mouse, FONT * Font, char * Label, rect Bound ) {
    boo32 Result = false;
    
    vec4 Background_Color = COLOR_GRAY( 0.05f );
    vec4 Outline_Color    = COLOR_GRAY( 0.1f  );
    
    if( IsInRect( Mouse->Position, Bound ) ) {
        Background_Color = COLOR_GRAY( 0.1f );
        Outline_Color    = COLOR_GRAY( 0.25f );
        
        if( WasPressed( Mouse, MouseButton_Left ) ) {
            Result = true;
            
            Background_Color = COLOR_GRAY( 0.35f );
            Bound            = AddRadius( Bound, -4.0f );
        }
    }
    
    flo32 Outline_Margin = 8.0f;
    DrawRect( Pass, Bound, Outline_Color );
    DrawRect( Pass, AddRadius( Bound, -Outline_Margin ), Background_Color );
    
    DrawString( Pass, Font, Label, GetCenter( Bound ), TextAlign_Center, TEXT_COLOR );
    
    return Result;
}

internal boo32
UpdateAndDrawPauseButton( RENDER_PASS * Pass, MOUSE_STATE * Mouse, FONT * Font, char * Label, rect Bound, boo32 IsActive = true, flo32 Alpha = 1.0f, vec4 Text_Color = TEXT_COLOR ) {
    boo32 Result = false;
    
    vec4 Background_Color = COLOR_GRAY( 0.05f );
    vec4 Outline_Color    = COLOR_GRAY( 0.1f  );
    
    if( ( IsActive ) && ( IsInRect( Mouse->Position, Bound ) ) ) {
        Background_Color = COLOR_GRAY( 0.1f );
        Outline_Color    = COLOR_GRAY( 0.25f );
        
        if( WasPressed( Mouse, MouseButton_Left ) ) {
            Result = true;
            
            Background_Color = COLOR_GRAY( 0.35f );
            Bound            = AddRadius( Bound, -4.0f );
        }
    }
    
    flo32 Outline_Margin = 2.0f;
    DrawRect( Pass, Bound, Outline_Color * Alpha );
    DrawRect( Pass, AddRadius( Bound, -Outline_Margin ), Background_Color * Alpha );
    
    DrawString( Pass, Font, Label, GetCenter( Bound ), TextAlign_Center, Text_Color * Alpha );
    
    return Result;
}

internal boo32
UpdateAndDrawPauseButton( RENDER_PASS * Pass, MOUSE_STATE * Mouse, FONT * Font, char * Label, rect Bound, vec4 Text_Color ) {
    boo32  Result = UpdateAndDrawPauseButton( Pass, Mouse, Font, Label, Bound, true, 1.0f, Text_Color );
    return Result;
}

internal boo32
UpdateAndDrawPauseButton( RENDER_PASS * Pass, MOUSE_STATE * Mouse, TEXTURE_ID TextureID, rect Bound, boo32 IsActive = true, flo32 Alpha = 1.0f ) {
    boo32 Result = false;
    
    vec4 Background_Color = COLOR_GRAY( 0.6f );
    vec4 Outline_Color    = COLOR_GRAY( 0.1f  );
    
    if( ( IsActive ) && ( IsInRect( Mouse->Position, Bound ) ) ) {
        Background_Color = COLOR_GRAY( 0.8f );
        Outline_Color    = COLOR_GRAY( 0.25f );
        
        if( WasPressed( Mouse, MouseButton_Left ) ) {
            Result = true;
            
            Background_Color = COLOR_WHITE;
            Bound            = AddRadius( Bound, -4.0f );
        }
    }
    
    flo32 Outline_Margin = 2.0f;
    DrawRect( Pass, Bound, Outline_Color * Alpha );
    DrawRect( Pass, TextureID, AddRadius( Bound, -Outline_Margin ), Background_Color * Alpha );
    
    return Result;
}

internal boo32
UpdateAndDrawPauseButton( RENDER_PASS * Pass, MOUSE_STATE * Mouse, TEXTURE_ID TextureID, orect Bound, boo32 IsActive = true, flo32 Alpha = 1.0f ) {
    boo32 Result = false;
    
    vec4 Background_Color = COLOR_GRAY( 0.6f );
    vec4 Outline_Color    = COLOR_GRAY( 0.1f  );
    
    if( ( IsActive ) && ( IsInORect( Mouse->Position, Bound ) ) ) {
        Background_Color = COLOR_GRAY( 0.8f );
        Outline_Color    = COLOR_GRAY( 0.25f );
        
        if( WasPressed( Mouse, MouseButton_Left ) ) {
            Result = true;
            
            Background_Color = COLOR_WHITE;
            Bound            = AddRadius( Bound, -4.0f );
        }
    }
    
    flo32 Outline_Margin = 2.0f;
    DrawORect( Pass, Bound, Outline_Color * Alpha );
    DrawORect( Pass, TextureID, AddRadius( Bound, -Outline_Margin ), Background_Color * Alpha );
    
    return Result;
}

internal boo32
UpdateAndDrawQuestionButton( RENDER_PASS * Pass, MOUSE_STATE * Mouse, FONT * Font0, FONT * Font1, char * Label, rect Bound, flo32 Alpha, boo32 IsActive ) {
    boo32 Result = false;
    
    vec2   Dim              = GetDim( Bound );
    vec4   Background_Color = COLOR_GRAY( 0.8f );
    FONT * Font             = Font0;
    
    if( ( IsActive ) && ( IsInRect( Mouse->Position, Bound ) ) ) {
        Background_Color = COLOR_WHITE;
        
        Dim += Vec2( 12.0f, 12.0f );
        Font = Font1;
        
        if( WasPressed( Mouse, MouseButton_Left ) ) {
            Result = true;
        }
    }
    
    DrawRect( Pass, RectCD( GetCenter( Bound ), Dim ), Background_Color * Alpha );
    DrawString( Pass, Font, Label, GetCenter( Bound ), TextAlign_Center, COLOR_BLACK * Alpha );
    
    return Result;
}

internal boo32
UpdateAndDrawItemClickButton( RENDER_PASS * Pass, MOUSE_STATE * Mouse, int32 ItemID, rect Bound, flo32 Alpha, boo32 IsActive ) {
    boo32 Result = false;
    
    vec4 Background_Color = COLOR_GRAY( 0.1f );
    vec4 Outline_Color    = COLOR_GRAY( 0.2f  );
    
    vec4 Color = COLOR_GRAY( 0.8f );
    if( ( IsActive ) && ( IsInRect( Mouse->Position, Bound ) ) ) {
        Background_Color = COLOR_GRAY( 0.2f );
        Outline_Color    = COLOR_GRAY( 0.35f );
        Color            = COLOR_WHITE;
        
        if( WasPressed( Mouse, MouseButton_Left ) ) {
            Result = true;
            
            Background_Color = COLOR_WHITE;
            Bound            = AddRadius( Bound, -4.0f );
        }
        
    }
    
    if( !Result ) {
        DrawRect( Pass, AddRadius( Bound, 2.0f ), TowardBlack( DISPLAY_BORDER_COLOR, 0.5f ) );
    }
    
    flo32 Outline_Margin = 6.0f;
    DrawRect( Pass, Bound, Outline_Color * Alpha );
    DrawRect( Pass, AddRadius( Bound, -Outline_Margin ), Background_Color * Alpha );
    DrawLockerContents( Pass, AddRadius( Bound, -12.0f ), ItemID, Alpha, Color );
    
    return Result;
}

internal boo32
UpdateAndDrawItemGrabButton( RENDER_PASS * Pass, MOUSE_STATE * Mouse, int32 ItemID, rect Draw_Bound, rect Select_Bound, flo32 Alpha, boo32 IsActive ) {
    boo32 Result = false;
    
    vec4 Color = COLOR_GRAY( 0.8f );
    rect Outer = Draw_Bound;
    if( ( IsActive ) && ( IsInRect( Mouse->Position, Select_Bound ) ) ) {
        if( WasPressed( Mouse, MouseButton_Left ) ) {
            Result = true;
        }
        
        Draw_Bound = RectCD( GetCenter( Draw_Bound ), LOCKER_DIM );
        Color = COLOR_WHITE;
        Outer = AddRadius( Draw_Bound, 8.0f );
    }
    
    DrawRect( Pass, TextureID_Glow, AddRadius( Outer, 40.0f ), COLOR_WHITE * Alpha );
    DrawRect( Pass, TextureID_Glow, AddRadius( Outer, 20.0f ), COLOR_WHITE * Alpha );
    DrawRect( Pass, TextureID_Ring, AddRadius( Outer, 28.0f ), COLOR_WHITE * 0.5f * Alpha );
    DrawLockerContents( Pass, AddRadius( Draw_Bound, 3.0f ), ItemID, 1.0f, COLOR_BLACK * 0.5f );
    DrawLockerContents( Pass, Draw_Bound, ItemID, 1.0f, Color );
    
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
DrawLabelS32( RENDER_PASS * Pass, FONT * Font, char * Label, int32 Value, flo32 LabelX, flo32 ValueX, flo32 Y, vec4 Label_Color, vec4 Value_Color ) {
    char Str[ 128 ] = {};
    sprintf( Str, "%d", Value );
    
    DrawString( Pass, Font, Label, Vec2( LabelX, Y ), TextAlign_TopLeft,  Label_Color );
    DrawString( Pass, Font, Str,   Vec2( ValueX, Y ), TextAlign_TopRight, Value_Color );
}

internal void
ScrollPracticeHighScoreTableToCenterMostRecentHighScore( APP_STATE * AppState, SAVE_GAME * SaveGame, int32 iEntry ) {
    AppState->HighScore_MostRecent_IsPractice = true;
    AppState->HighScore_MostRecent_iEntry     = iEntry;
    
    DISPLAY_VALUE( int32, iEntry );
    
    int32 iPracticeToDisplay = -1;
    int32 nPracticeToDisplay = 0;
    for( int32 iPractice = 0; iPractice < PRACTICE_MAX_LEVEL; iPractice++ ) {
        if( SaveGame->Practice_HighScore[ iPractice ] > 0 ) {
            nPracticeToDisplay += 1;
        }
        if( iPractice == AppState->HighScore_MostRecent_iEntry ) {
            iPracticeToDisplay = nPracticeToDisplay;
        }
    }
    
    if( nPracticeToDisplay > 0 ) {
        iPracticeToDisplay = nPracticeToDisplay - iPracticeToDisplay;
        Assert( iPracticeToDisplay >= 0 );
        
        flo32 Stripe_Height = AppState->HighScore_ShowPractice_StripeHeight;
        flo32 Total_Height  = ( flo32 )nPracticeToDisplay * Stripe_Height;
        flo32 Panel_Height  = AppState->HighScore_ShowPractice_InnerHeight;
        if( Total_Height > Panel_Height ) {
            flo32 Remain     = Total_Height - Panel_Height;
            
            flo32 TargetY = ( Panel_Height * 0.5f ) - ( Stripe_Height * 0.5f );
            flo32 TopY    = TargetY - ( Stripe_Height * ( flo32 )iPracticeToDisplay );
            flo32 BottomY = TargetY + ( Stripe_Height * ( flo32 )( nPracticeToDisplay - iPracticeToDisplay ) );
            
            DISPLAY_VALUE( flo32, Remain );
            DISPLAY_VALUE( flo32, TargetY );
            DISPLAY_VALUE( flo32, TopY );
            DISPLAY_VALUE( flo32, BottomY );
            
            flo32 OffsetY = 0.0f;
            if( BottomY < Panel_Height ) {
                TopY    += ( Panel_Height - BottomY );
                BottomY  = Panel_Height;
            }
            if( TopY < 0.0f ) {
                OffsetY = -TopY;
            }
            if( TopY > 0.0f ) {
                OffsetY = 0.0f;
            }
            
            AppState->HighScore_ShowPractice_tScroll = Clamp01( OffsetY / Remain );
        }
    }
}

internal void
UpdateHighScoreAndSave( PLATFORM * Platform, APP_STATE * AppState ) {
    SAVE_GAME * SaveGame = &AppState->SaveGame;
    if( AppState->Practice_IsActive ) {
        int32 iEntry = AppState->nMinimumItems - 3;
        if( AppState->Score > SaveGame->Practice_HighScore[ iEntry ] ) {
            SaveGame->Practice_HighScore[ iEntry ] = AppState->Score;
            
            AppState->HighScore_MostRecent_iEntry     = iEntry;
            AppState->HighScore_MostRecent_IsPractice = true;
            AppState->HighScore_ShowPractice          = true;
            
            int32 iPracticeToDisplay = -1;
            int32 nPracticeToDisplay = 0;
            for( int32 iPractice = 0; iPractice < PRACTICE_MAX_LEVEL; iPractice++ ) {
                if( SaveGame->Practice_HighScore[ iPractice ] > 0 ) {
                    nPracticeToDisplay += 1;
                }
                if( iPractice == AppState->HighScore_MostRecent_iEntry ) {
                    iPracticeToDisplay = nPracticeToDisplay;
                }
            }
            
            if( nPracticeToDisplay > 0 ) {
                iPracticeToDisplay = nPracticeToDisplay - iPracticeToDisplay;
                Assert( iPracticeToDisplay >= 0 );
                
                flo32 Stripe_Height = AppState->HighScore_ShowPractice_StripeHeight;
                flo32 Total_Height  = ( flo32 )nPracticeToDisplay * Stripe_Height;
                flo32 Panel_Height  = AppState->HighScore_ShowPractice_InnerHeight;
                if( Total_Height > Panel_Height ) {
                    flo32 Remain     = Total_Height - Panel_Height;
                    flo32 HighlightY = Stripe_Height * ( ( flo32 )( iPracticeToDisplay ) + 0.5f );
                    
                    flo32 TargetY = ( Panel_Height * 0.5f ) - ( Stripe_Height * 0.5f );
                    flo32 TopY    = TargetY - ( Stripe_Height * ( flo32 )iPracticeToDisplay );
                    flo32 BottomY = TargetY + ( Stripe_Height * ( flo32 )( nPracticeToDisplay - iPracticeToDisplay ) );
                    
                    flo32 OffsetY = 0.0f;
                    if( BottomY < Panel_Height ) {
                        TopY    += ( Panel_Height - BottomY );
                        BottomY  = Panel_Height;
                    }
                    if( TopY < 0.0f ) {
                        OffsetY = -TopY;
                    }
                    if( TopY > 0.0f ) {
                        OffsetY = 0.0f;
                    }
                    
                    AppState->HighScore_ShowPractice_tScroll = Clamp01( OffsetY / Remain );
                }
            }
        }
    } else {
        int32 iHighScore = -1;
        for( int32 Iter = 0; ( iHighScore == -1 ) && ( Iter < HIGH_SCORE_MAX_COUNT ); Iter++ ) {
            HIGH_SCORE HighScore = SaveGame->FullGame_HighScore[ Iter ];
            
            if( AppState->Score > HighScore.Score ) {
                iHighScore = Iter;
            }
            if( ( AppState->Score == HighScore.Score ) && ( AppState->nMinimumItems > HighScore.nItems ) ) {
                iHighScore = Iter;
            }
        }
        
        if( iHighScore >= 0 ) {
            for( int32 Iter = ( HIGH_SCORE_MAX_COUNT - 1 ); Iter > iHighScore; Iter-- ) {
                SaveGame->FullGame_HighScore[ Iter ] = SaveGame->FullGame_HighScore[ Iter - 1 ];
            }
            
            HIGH_SCORE HighScore = {};
            HighScore.Score  = AppState->Score;
            HighScore.nItems = AppState->nMinimumItems;
            SaveGame->FullGame_HighScore[ iHighScore ] = HighScore;
            
            AppState->HighScore_MostRecent_IsPractice = false;
            AppState->HighScore_MostRecent_iEntry     = iHighScore;
            AppState->HighScore_ShowPractice          = false;
        }
    }
    
    SaveGameFile( Platform->WriteFile, AppState, &Platform->TempMemory );
}

internal void
APP_UpdateAndRender( APP_STATE * AppState, PLATFORM * Platform ) {
    RENDERER    * Renderer = &Platform->Renderer;
    RENDER_PASS * Pass_Game  = &Renderer->Pass_Game;
    RENDER_PASS * Pass_Layer = &Renderer->Pass_Layer;
    
    MOUSE_STATE      * Mouse    = &Platform->Mouse;
    KEYBOARD_STATE   * Keyboard = &Platform->Keyboard;
    
    MEMORY * PermMemory = &Platform->PermMemory;
    MEMORY * TempMemory = &Platform->TempMemory;
    
    //AUDIO_SYSTEM * AudioSystem = &Platform->AudioSystem;
    //ASSET_SYSTEM * assetSystem = &Platform->assetSystem;
    
    flo32 dT = Platform->TargetSec;
    AppState->dT = dT;
    
    if( !AppState->IsInitialized ) {
        AppState->IsInitialized    = true;
        
        AppState->UI_Dim     = Vec2( DEV_WIDTH, DEV_HEIGHT );
        AppState->UI_HalfDim = AppState->UI_Dim * 0.5f;
        AppState->UI_Bound   = RectBLD( Vec2( 0.0f, 0.0f ), AppState->UI_Dim );
        
        ZoomCameraToWorldBound( &AppState->Camera, AppState->UI_Bound, AppState->UI_Bound, AppState->UI_Bound );
        
        //AppState->AudioSystem = AudioSystem;
        //AudioSystem->MainVolume = 0.5f;
        
        AppState->SoundIsActive = true;
        
        AppState->Practice_nMinimum = 3;
        AppState->Background_tZoom  = 1.0f;
        
        AppState->StartUp_DoFadeIn  = true;
        AppState->StartUp_tFade     = -0.2f;
        
        if( DoesGameFileExist( Platform ) ) {
            LoadGameFile( Platform->ReadFile, AppState, TempMemory );
        } else {
            InitHighScores( AppState );
        }
        
        // NOTE : This line needs to stay here if we keep the high score display on the title screen. We don't want a high score highlighted when the game starts.
        AppState->HighScore_MostRecent_iEntry = -1;
        
        { // Init Background Dust Particles
            for( int32 iParticle = 0; iParticle < BACKGROUND_PARTICLE_MAX_COUNT; iParticle++ ) {
                DUST_PARTICLE * Particle = AppState->Background_Particle + iParticle;
                Particle->Position = GetRandomPoint( AddDim( AppState->UI_Bound, Vec2( 200.0f, 120.0f ) ) );
                
                flo32 t = RandomF32();
                Particle->Move_Radians    = RandomF32() * TAU;
                Particle->Move_Speed      = lerp( 15.0f, t * t, 30.0f );
                Particle->Move_TargetTime = RandomF32InRange( 1.5f, 3.0f );
                
                Particle->Velocity = ToDirection2D( Particle->Move_Radians ) * Particle->Move_Speed;
            }
        }
        
        InitWorkers( TempMemory, AppState );
        
        AppState->Mode = AppMode_TitleMenu;
        APP_MODE_TRANSITION * ModeTransition = &AppState->ModeTransition;
        ModeTransition->Type       = ModeTransitionType_Enter;
        ModeTransition->Timer      = 0.0f;
        ModeTransition->TargetTime = ( 15.0f / 60.0f );
        ModeTransition->ToMode     = AppMode_None;
        ModeTransition->FromMode   = AppMode_None;
    }
    
    DISPLAY_VALUE( vec2, Mouse->Position );
    vec2 tMouse = GetT01( Mouse->Position, AppState->UI_Bound );
    
    {
        SetFramebuffer( Renderer, Pass_Game, FramebufferID_Main );
        ClearFramebuffer( Pass_Game, COLOR_BLACK );
    }
    
    { //: Background
        AppState->Background_tZoom += AppState->Background_dZoom * dT;
        if( ( AppState->Background_tZoom < 0.0f ) || ( AppState->Background_tZoom > 1.0f ) ) {
            AppState->Background_tZoom = Clamp01( AppState->Background_tZoom );
            AppState->Background_dZoom = 0.0f;
        }
        
        vec2 Dust_MaxToAdd = Vec2( 192.0f, 108.0f );
        Dust_MaxToAdd *= 2.0f;
        vec2 Dust_AddDim       = {};
        Dust_AddDim.x       = lerp( Dust_MaxToAdd.x, AppState->Background_tZoom, 2.0f );
        Dust_AddDim.y       = lerp( Dust_MaxToAdd.y, AppState->Background_tZoom, 2.0f );
        rect Dust_Bound       = AddDim( AppState->UI_Bound, Dust_AddDim );
        
        { // Update Dust Particles
            for( int32 iWind = BACKGROUND_PARTICLE_WIND_FRAME_COUNT - 1; iWind > 0; iWind-- ) {
                AppState->Background_Wind_Pos[ iWind ] = AppState->Background_Wind_Pos[ iWind - 1 ];
                AppState->Background_Wind_Vel[ iWind ] = AppState->Background_Wind_Vel[ iWind - 1 ];
            }
            if( ( AppState->Mode == AppMode_Game ) && ( !AppState->ShowPauseMenu ) ) {
                AppState->Background_Wind_Pos[ 0 ] = {};
                AppState->Background_Wind_Vel[ 0 ] = {};
            } else {
                AppState->Background_Wind_Pos[ 0 ] = Mouse->PrevPosition;
                AppState->Background_Wind_Vel[ 0 ] = Mouse->dPos;
            }
            
            rect AdjReturn = AddRadius( AppState->ToFind_Return, 32.0f );
            
            for( int32 iParticle = 0; iParticle < BACKGROUND_PARTICLE_MAX_COUNT; iParticle++ ) {
                DUST_PARTICLE * Particle = AppState->Background_Particle + iParticle;
                
                if( AppState->Background_dZoom != 0.0f ) {
                    flo32 Scale = -cosf( AppState->Background_tZoom * TAU ) * 0.5f + 0.5f;
                    vec2  tPos  = GetT01( Particle->Position, Dust_Bound ) * Vec2( 2.0f, 2.0f ) + Vec2( -1.0f, -1.0f );
                    vec2 dPos = Dust_MaxToAdd * ( tPos * AppState->Background_dZoom * Scale * dT );
                    Particle->Position += dPos;
                }
                
                for( int32 iWind = 0; iWind < BACKGROUND_PARTICLE_WIND_FRAME_COUNT; iWind++ ) {
                    vec2 Pos = AppState->Background_Wind_Pos[ iWind ];
                    vec2 Vel = AppState->Background_Wind_Vel[ iWind ];
                    
                    flo32  Length = GetLength( Vel );
                    flo32 tLength = GetT01( Length, 1.5f, 60.0f );
                    flo32  Wind_Speed = 800.0f * tLength;
                    vec2   Wind       = GetNormal( Vel ) * Wind_Speed;
                    
                    Assert( ( tLength >= 0.0f ) && ( tLength <= 1.0f ) );
                    
                    flo32  Radius   = lerp( 20.0f, tLength, 60.0f );
                    flo32  RadiusSq = Radius * Radius;
                    
                    vec2 P = Pos;
                    vec2 Q = P + Vel;
                    
                    flo32 DistSq = DistSqPointToEdge( Particle->Position, P, Q );
                    if( DistSq <= RadiusSq ) {
                        flo32  Dist = sqrtf( DistSq );
                        flo32 tDist = Clamp01( Dist / Radius );
                        
                        Particle->Velocity += Wind * ( tDist * dT );
                    }
                }
                
                Particle->Friction = 2.0f;
                
#if 0                
                if( ( !AppState->ShowPauseMenu ) && ( AppState->ToFind >= 0 ) && ( AppState->Health > 0 ) && ( !AppState->Tutorial_PauseForFirstQuestion ) && ( IsInRect( Particle->Position, AdjReturn ) ) ) {
                    vec2  Center = GetCenter( AdjReturn );
                    vec2  V      = GetNormal( Particle->Position - Center );
                    flo32 Speed  = 240.0f;
                    Particle->Friction  = 6.0f;
                    Particle->Velocity += V * ( Speed * dT * Particle->Friction );
                }
#endif
                
                Particle->Move_Timer += dT;
                if( Particle->Move_Timer >= Particle->Move_TargetTime ) {
                    Particle->Move_Timer      = 0.0f;
                    Particle->Move_TargetTime = RandomF32InRange( 1.5f, 3.0f );
                    
                    flo32 t = RandomF32();
                    Particle->Move_Speed = lerp( 15.0f, t * t * t, 80.0f );
                    
                    rect  Bound   = AddDim( AppState->UI_Bound, Vec2( 200.0f, 120.0f ) );
                    flo32 Radians = WrapRadians( Particle->Move_Radians + RandomF32InRange( -PI / 3.0f, PI / 3.0f ) );
                    vec2  Dir     = ToDirection2D( Radians );
                    vec2  Dest    = Particle->Position + Dir * ( Particle->Move_Speed * Particle->Move_TargetTime );
                    if( ( Dest.x < Bound.Left ) || ( Dest.x > Bound.Right ) ) {
                        Dir.x = -Dir.x;
                    }
                    if( ( Dest.y < Bound.Bottom ) || ( Dest.y > Bound.Top ) ) {
                        Dir.y = -Dir.y;
                    }
                    Particle->Move_Radians = ToRadians( Dir );
                }
                
                vec2 Dir   = ToDirection2D( Particle->Move_Radians );
                vec2 Accel = ( Dir * Particle->Move_Speed - Particle->Velocity ) * Particle->Friction;
                
                vec2 dPos = Accel * ( 0.5f * dT * dT ) + Particle->Velocity * dT;
                Particle->Position += dPos;
                Particle->Velocity += Accel * dT;
            }
        }
        
        vec4  Background_Color = lerp( COLOR_GRAY( 0.01f ), AppState->Background_tZoom, COLOR_GRAY( 0.035f ) );
        
        vec4  Dust_Color = lerp( COLOR_GRAY( 0.5f ), AppState->Background_tZoom, COLOR_GRAY( 0.25f ) );
        vec4  Corner_Color = Background_Color;
        
        vec4  Bright_Color = COLOR_GRAY( 0.25f );
        
        AppState->nItemsUp_Timer = MaxValue( AppState->nItemsUp_Timer - dT, 0.0f );
        if( AppState->nItemsUp_Timer > 0.0f ) {
            flo32 tTimer = Clamp01( AppState->nItemsUp_Timer / N_ITEMS_UP_TARGET_TIME );
            Background_Color = lerp( Background_Color, tTimer, Bright_Color );
        }
        
        DrawRect( Pass_Game, AppState->UI_Bound, COLOR_WHITE );
        DrawRect( Pass_Game, AppState->UI_Bound, COLOR_BLACK );
        DrawRect( Pass_Game, TextureID_Background, AppState->UI_Bound, TowardBlack( BONUS_POINTS_COLOR, 0.75f ) * 0.1f );
        //vec2 Background_AddDim = Vec2( 400.0f, 240.0f );
        //vec2 Background_Pos    = GetCenter( AppState->UI_Bound ) + Background_AddDim * ( tMouse * 0.5f - Vec2( 0.5f, 0.5f ) );
        //rect Background_Bound  = RectCD( Background_Pos, GetDim( AppState->UI_Bound ) + Background_AddDim );
        //DrawRect( Pass_Game, TextureID_Cell, Background_Bound, TowardBlack( BONUS_POINTS_COLOR, 0.8f ) * 0.25f );
        //DrawRect( Pass_Game, TextureID_Background,      Background_Bound, Background_Color );
        //DrawRect( Pass_Game, TextureID_Background_Dust, Dust_Bound,       Dust_Color );
        
        if( ( !AppState->ShowPauseMenu ) && ( AppState->Light_Timer > 0.0f ) ) {
            flo32 tLight = Clamp01( AppState->Light_Timer / LIGHT_TARGET_TIME );
            if( AppState->Light_IsCorrect ) {
                Corner_Color = lerp( Corner_Color, tLight, Bright_Color );
            } else {
                Corner_Color = lerp( Corner_Color, tLight, COLOR_GRAY( 0.35f ) * COLOR_RED );
            }
            //DrawRect( Pass_Game, TextureID_Background_Light, Background_Bound, Corner_Color );
            
            //DrawModel( Pass_Game, MeshID_Rect_BRQuad, TextureID_Background, GetCenter( Background_Bound ), GetDim( Background_Bound ) * 0.5f, Corner_Color );
        }
        
        UpdateWorkers( AppState, dT );
        DrawWorkers( Pass_Game, AppState, tMouse );
        
        { // Draw Dust Particles
            vec2 Center = GetCenter( AppState->UI_Bound );
            
            flo32 Speed_Lo = 30.0f;
            flo32 Speed_Hi = 100.0f;
            
            flo32 Zoom = lerp( 0.9f, AppState->Background_tZoom, 1.0f );
            
            for( int32 iParticle = 0; iParticle < BACKGROUND_PARTICLE_MAX_COUNT; iParticle++ ) {
                DUST_PARTICLE Particle = AppState->Background_Particle[ iParticle ];
                
                flo32  Speed = GetLength( Particle.Velocity );
                flo32 tSpeed = GetT01( Speed, Speed_Lo, Speed_Hi );
                
                flo32  Radius = lerp( 4.0f, tSpeed, 6.0f );
                vec4   Color  = lerp( Dust_Color, tSpeed, COLOR_WHITE );
                
                flo32  tDouble = GetT01( tSpeed, 0.25f, 1.0f );
                
                DrawRect( Pass_Game, TextureID_Glow, RectCR( Particle.Position, Radius * Zoom ), Color );
            }
            
#if 0            
            for( int32 iWind = 0; iWind < BACKGROUND_PARTICLE_WIND_FRAME_COUNT; iWind++ ) {
                vec2 Pos = AppState->Background_Wind_Pos[ iWind ];
                vec2 Vel = AppState->Background_Wind_Vel[ iWind ];
                
                vec2 P = Pos;
                vec2 Q = P + Vel;
                
                DrawLine( Pass_Game, P, Q, COLOR_CYAN );
            }
#endif
        }
    }
    
    {
        SetFramebuffer( Renderer, Pass_Game, FramebufferID_Layer );
        ClearFramebuffer( Pass_Game, Vec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
    }
    
    vec2  Layer_Dim   = GetDim( AppState->UI_Bound );
    flo32 Layer_Alpha = 1.0f;
    
    APP_MODE_TRANSITION * ModeTransition = &AppState->ModeTransition;
    boo32                 ModeTransition_DoExit     = false;
    boo32                 ModeTransition_DoEnter    = false;
    boo32                 ModeTransition_IsEntering = false;
    boo32                 ModeTransition_IsExiting  = false;
    { // Finalize Mode Transition from Previous Frame
        if( ModeTransition->DoFinalize ) {
            ModeTransition->DoFinalize = false;
            
            AppState->Mode = ModeTransition->ToMode;
            
            if( ModeTransition->ToMode_DoEnter ) {
                ModeTransition->Type       = ModeTransitionType_Enter;
                ModeTransition->Timer      = 0.0f;
                ModeTransition->TargetTime = ModeTransition->ToMode_TargetTime;
            }
        }
    }
    { // Update Mode Transition
        if( ModeTransition->Type != ModeTransitionType_None ) {
            ModeTransition->Timer += dT;
            
            ModeTransition_IsEntering = ( ModeTransition->Type == ModeTransitionType_Enter );
            ModeTransition_IsExiting  = ( ModeTransition->Type == ModeTransitionType_Exit  );
            
            if( ModeTransition->Timer >= ModeTransition->TargetTime ) {
                ModeTransition_DoEnter = ModeTransition_IsEntering;
                ModeTransition_DoExit  = ModeTransition_IsExiting;
                
                ModeTransition->Type       = ModeTransitionType_None;
                ModeTransition->DoFinalize = ModeTransition_DoExit;
            }
        }
    }
    
    switch( AppState->Mode ) {
        case AppMode_Game: {
            if( ModeTransition_IsEntering ) {
                flo32 tTimer = GetT01( ModeTransition->Timer, 0.0f, ModeTransition->TargetTime );
                Layer_Alpha  = tTimer;
                Layer_Dim   *= lerp( 4.0f, CubeValue( tTimer - 1.0f ) + 1.0f, 1.0f );
            }
            if( ModeTransition_IsExiting ) {
                flo32 tTimer = GetT01( ModeTransition->Timer, 0.0f, ModeTransition->TargetTime );
                Layer_Alpha  = 1.0f - tTimer;
                Layer_Dim   *= lerp( 1.0f, CubeValue( tTimer ), 0.95f );
            }
            boo32 ButtonsAreActive = ( ( !ModeTransition_IsEntering ) && ( !ModeTransition_IsExiting ) );
            
            CAMERA_STATE * Camera = &AppState->Camera;
            
            vec2 MouseP   = Mouse->Position;
            rect UI_Bound = AppState->UI_Bound;
            
            rect  Storage_Bound       = AppState->Storage_Bound;
            rect  ToStore_Bound       = AppState->ToStore_Bound;
            rect  ToFind_Return       = AppState->ToFind_Return;
            rect  ToFind_Display      = AppState->ToFind_Display;
            rect  ScoreUpdate_Display = AppState->ScoreUpdate_Display;
            rect  ScorePanel_Display  = AppState->ScorePanel_Display;
            rect  Health_Display      = AppState->Health_Display;
            //rect  ToFind_Text         = AppState->ToFind_Text;
            rect  Turn_Bound          = AppState->Turn_Bound;
            rect  Question_Bound      = AppState->Question_Bound;
            rect  Question_Display    = AddDim( Question_Bound, Vec2( -80.0f, -20.0f ) );
            
            rect  Storage_AdjBound = AddRadius( Storage_Bound, -( LOCKER_DIM ).x * 0.5f + 20.0f );
            rect  ToFind_AdjReturn = ToFind_Return;
            ToFind_AdjReturn.Left   = Storage_AdjBound.Right;
            ToFind_AdjReturn.Right  += ( LOCKER_DIM ).x;
            ToFind_AdjReturn.Bottom -= ( LOCKER_DIM ).y;
            
            {
                //AppState->Locker_nCol = ( int32 )lerp( 2.0f, tMouse.x, 40.0f );
                //AppState->Locker_nRow = ( int32 )lerp( 2.0f, tMouse.y, 40.0f );
            }
            
            if( AppState->ShowPauseMenu ) {
                vec2  AtPos   = Vec2( 960.0f, 760.0f );
                vec2  Dim     = Vec2( 220.0f, 80.0f );
                flo32 Between = 8.0f;
                
                FONT * Font      = AppState->FontList + FontID_2_50;
                FONT * Text_Font = AppState->FontList + FontID_2_00;
                
                rect Resume = RectTCD( AtPos, Dim );
                AtPos.y -= ( Dim.y + Between );
                if( UpdateAndDrawPauseButton( Pass_Game, Mouse, Font, "Resume", Resume ) ) {
                    PlaySound( AppState, SoundID_Menu_Back );
                    
                    AppState->ShowPauseMenu = false;
                }
                
                AtPos.y -= 80.0f;
                vec2 Sound_Status = AtPos;
                AtPos.y -= ( 36.0f );
                
                rect Sound  = RectTCD( AtPos, Dim * Vec2( 1.5f, 1.0f ) );
                AtPos.y -= ( Dim.y + Between );
                if( UpdateAndDrawPauseButton( Pass_Game, Mouse, Font, "Toggle Sound", Sound ) ) {
                    AppState->SoundIsActive = !AppState->SoundIsActive;
                    
                    if( AppState->SoundIsActive ) {
                        PlaySound( AppState, SoundID_ItemsUp );
                    }
                }
                if( AppState->SoundIsActive ) {
                    DrawString( Pass_Game, Text_Font, "Sound is ON", Sound_Status, TextAlign_TopCenter, TEXT_COLOR );
                } else {
                    DrawString( Pass_Game, Text_Font, "Sound is OFF", Sound_Status, TextAlign_TopCenter, TEXT_COLOR );
                }
                
                AtPos.y -= 108.0f;
                rect Exit = RectTCD( AtPos, Dim );
                if( UpdateAndDrawPauseButton( Pass_Game, Mouse, Font, "Exit Game", Exit ) ) {
                    UpdateHighScoreAndSave( Platform, AppState );
                    PlaySound( AppState, SoundID_LoseGame );
                    
                    AppState->Background_dZoom   = ( 1.0f / 2.0f );
                    
                    AppState->TitleMenu_Delay = ( 60.0f / 60.0f );
                    { // TRANSITION
                        ModeTransition->Type              = ModeTransitionType_Exit;
                        ModeTransition->Timer             = dT;
                        ModeTransition->TargetTime        = ( 30.0f / 60.0f );
                        ModeTransition->ToMode            = AppMode_TitleMenu;
                        ModeTransition->ToMode_DoEnter    = true;
                        ModeTransition->ToMode_TargetTime = ( 110.0f / 60.0f );
                        ModeTransition->FromMode          = AppMode_Game;
                    }
                }
                AtPos.y -= ( Dim.y + Between );
                DrawString( Pass_Game, Text_Font, "High Score will be SAVED.", AtPos, TextAlign_TopCenter, TEXT_COLOR );
            } else {
                { // Update ToFind
                    if( ( AppState->ToFind >= 0 ) && ( AppState->Health > 0 ) && ( !AppState->Tutorial_PauseForFirstQuestion ) ) {
                        flo32 dLight = 1.0f / 1.0f;
                        AppState->ToFind_tLight = MinValue( AppState->ToFind_tLight + dLight * dT, 1.0f );
                    } else {
                        flo32 dLight = 1.0f / 1.25f;
                        AppState->ToFind_tLight = MaxValue( AppState->ToFind_tLight - dLight * dT, 0.0f );
                    }
                }
                
                { // Draw ToStore Fence
                    int32 nRow = 10;
                    int32 nCol = 5;
                    vec2  Fence_Dim = Vec2( 100.0f, 116.0f );
                    
                    vec2 bPos = GetBL( UI_Bound ) - Vec2( 12.0f, 12.0f );
                    for( int32 iRow = 0; iRow < nRow; iRow++ ) {
                        vec2 AtPos = bPos;
                        for( int32 iCol = 0; iCol < nCol; iCol++ ) {
                            rect R = RectBLD( AtPos, Fence_Dim );
                            DrawRect( Pass_Game, TextureID_Fence, R, TowardBlack( FENCE_COLOR, 0.25f ) );
                            
                            AtPos.x += Fence_Dim.x;
                        }
                        
                        bPos.y += Fence_Dim.y;
                    }
                    
                    //rect Top = Bound;
                    //Top.Bottom = Top.Top - 12.0f;
                    //DrawRect( Pass_Game, Top, FENCE_COLOR );
                    
                }
                
                { // Draw Fence between Question Bound and UI Panel
                    int32 nRow = 3;
                    int32 nCol = 2;
                    vec2  Fence_Dim = Vec2( 100.0f, 116.0f );
                    
                    vec2 bPos = GetTR( Question_Bound ) + Vec2( -12.0f, 12.0f );
                    for( int32 iRow = 0; iRow < nRow; iRow++ ) {
                        vec2 AtPos = bPos;
                        for( int32 iCol = 0; iCol < nCol; iCol++ ) {
                            rect R = RectTLD( AtPos, Fence_Dim );
                            DrawRect( Pass_Game, TextureID_Fence, R, TowardBlack( FENCE_COLOR, 0.25f ) );
                            
                            AtPos.x += Fence_Dim.x;
                        }
                        
                        bPos.y -= Fence_Dim.y;
                    }
                }
                
                { // Draw ToFind_Return Fence/Gate
                    rect Outer = AddDim( ToFind_Return, Vec2( -60.0f, -80.0f ) );
                    rect Inner = AddRadius( Outer, -16.0f );
                    
                    vec4 Outer_Color = FENCE_COLOR;
                    {
                        // NOTE: Light up the outer gate if an item is hovering over the tofind_return.
                        if( AppState->ToMove_iLocker >= 0 ) {
                            vec2 Dest = MouseP + AppState->ToMove_Offset;
                            if( IsInRect( Dest, ToFind_AdjReturn ) ) {
                                Outer_Color = lerp( Outer_Color, 0.25f, TEXT_COLOR );
                            }
                        }
                    }
                    if( AppState->Light_Timer > 0.0f ) {
                        flo32 tTimer = Clamp01( AppState->Light_Timer / ( LIGHT_TARGET_TIME - ( 30.0f / 60.0f ) ) );
                        if( AppState->Light_IsCorrect ) {
                            Outer_Color = lerp( Outer_Color, tTimer, TEXT_COLOR );
                        } else {
                            Outer_Color = lerp( Outer_Color, tTimer, DAMAGE_COLOR );
                        }
                    }
                    
                    vec2 Dim0 = GetDim( Outer );
                    
                    flo32 tOffset = GetT01( AppState->ToFind_tLight, 0.0f, 0.85f );
                    tOffset = 1.0f - ( cosf( tOffset * PI ) * 0.5f + 0.5f );
                    flo32 yOffset = ( GetHeight( Outer ) - 40.0f ) * tOffset;
                    
                    { // Rail
                        vec2  Dim        = Vec2( 6.0f, 320.0f );
                        flo32 FromCenter = 80.0f;
                        
                        rect Left  = RectTCD( GetTC( ToFind_Return ) + Vec2( -FromCenter, 0.0f ), Dim );
                        rect Right = RectTCD( GetTC( ToFind_Return ) + Vec2(  FromCenter, 0.0f ), Dim );
                        
                        Left  = AddOffset( Left,  Vec2( 0.0f, yOffset ) );
                        Right = AddOffset( Right, Vec2( 0.0f, yOffset ) );
                        
                        DrawRect( Pass_Game, Left,  TowardBlack( FENCE_COLOR, 0.75f ) );
                        DrawRect( Pass_Game, Right, TowardBlack( FENCE_COLOR, 0.75f ) );
                    }
                    
                    { // Rail
                        vec2  Dim        = Vec2( 10.0f, 60.0f );
                        flo32 FromCenter = 82.0f;
                        
                        rect Left  = RectTCD( GetTC( ToFind_Return ) + Vec2( -FromCenter, 0.0f ), Dim );
                        rect Right = RectTCD( GetTC( ToFind_Return ) + Vec2(  FromCenter, 0.0f ), Dim );
                        
                        DrawRect( Pass_Game, Left,  TowardBlack( FENCE_COLOR, 0.8f ) );
                        DrawRect( Pass_Game, Right, TowardBlack( FENCE_COLOR, 0.8f ) );
                    }
                    
                    Inner = AddOffset( Inner, Vec2( 0.0f, yOffset ) );
                    
                    { // Gate
                        int32 nRow = 3;
                        int32 nCol = 3;
                        rect  AdjInner   = AddRadius( Inner, -2.0f );
                        vec2  Fence_bPos = GetBL( AdjInner );
                        vec2  Fence_Dim  = GetDim( AdjInner ) / ( flo32 )nRow;
                        
                        vec2  bPos = Fence_bPos;
                        for( int32 iRow = 0; iRow < nRow; iRow++ ) {
                            vec2 AtPos = bPos;
                            for( int32 iCol = 0; iCol < nCol; iCol++ ) {
                                rect R = RectBLD( AtPos, Fence_Dim );
                                DrawRect( Pass_Game, TextureID_Fence, R, TowardBlack( FENCE_COLOR, 0.25f ) );
                                
                                AtPos.x += Fence_Dim.x;
                            }
                            
                            bPos.y += Fence_Dim.y;
                        }
                    }
                    
                    { // Fence
                        int32 nRow = 3;
                        int32 nCol = 3;
                        rect  AdjOuter = AddRadius( Outer, -2.0f );
                        vec2  Fence_Dim  = GetDim( AdjOuter ) / ( flo32 )nRow;
                        
                        {
                            vec2 AtPos = GetTL( AdjOuter );
                            for( int32 iTile = 0; iTile < 5; iTile++ ) {
                                rect R = RectBLD( AtPos, Fence_Dim );
                                DrawRect( Pass_Game, TextureID_Fence, R, TowardBlack( FENCE_COLOR, 0.25f ) );
                                
                                AtPos.x += Fence_Dim.x;
                            }
                        }
                        
                        {
                            vec2 AtPos = GetTL( AdjOuter );
                            for( int32 iTile = 0; iTile < 6; iTile++ ) {
                                rect R = RectBRD( AtPos, Fence_Dim );
                                DrawRect( Pass_Game, TextureID_Fence, R, TowardBlack( FENCE_COLOR, 0.25f ) );
                                
                                AtPos.y -= Fence_Dim.y;
                            }
                        }
                        
                        {
                            vec2 AtPos = GetTL( AdjOuter ) + Vec2( 0.0f, -Fence_Dim.y * ( flo32 )nRow );
                            for( int32 iTile = 0; iTile < nCol; iTile++ ) {
                                rect R = RectTLD( AtPos, Fence_Dim );
                                DrawRect( Pass_Game, TextureID_Fence, R, TowardBlack( FENCE_COLOR, 0.25f ) );
                                
                                AtPos.x += Fence_Dim.x;
                            }
                        }
                        
                        {
                            vec2 AtPos = GetTR( AdjOuter );
                            for( int32 iTile = 0; iTile < 5; iTile++ ) {
                                rect R = RectTLD( AtPos, Fence_Dim );
                                DrawRect( Pass_Game, TextureID_Fence, R, TowardBlack( FENCE_COLOR, 0.25f ) );
                                
                                AtPos.y -= Fence_Dim.y;
                            }
                        }
                    }
                    
                    { // ToFind Tray
                        vec2 Dim   = Vec2( 160.0f, 160.0f );
                        rect Bound = RectTCD( GetBC( ToFind_Return ) + Vec2( 0.0f, 68.0f ), Dim );
                        
                        DrawRect( Pass_Game, TextureID_ToStore_Tray, Bound, FENCE_COLOR );
                        
                        orect Arrow = ORectCD( GetTC( Bound ) + Vec2( 0.0f, -20.0f ), Vec2( 40.0f, 80.0f ), PI * 0.5f );
                        DrawORect( Pass_Game, TextureID_ToStore_Arrow, Arrow, TowardBlack( FENCE_COLOR, 0.5f ) );
                    }
                    
                    DrawRect( Pass_Game, TextureID_SolidBoxOutline, Outer, Outer_Color );
                    DrawRect( Pass_Game, TextureID_SolidBoxOutline, Inner, TowardBlack( FENCE_COLOR, 0.05f ) );
                }
                
                { // Draw Storage_Bound
                    //DrawRect( Pass_Game, Storage_Bound, STORAGE_COLOR );
                    
                    //DrawRect( Pass_Game, Storage_Bound, COLOR_GRAY( 0.015f ) );
                    //DrawRect( Pass_Game, TextureID_StorageArea, AddRadius( Storage_Bound, -4.0f ), STORAGE_COLOR );
                    
                    DrawRect( Pass_Game, TextureID_StorageArea_Shadow, Storage_Bound, TowardBlack( STORAGE_COLOR, 0.2f ) );
                    DrawRect( Pass_Game, TextureID_StorageArea_Shadow, AddRadius( Storage_Bound, -12.0f ), STORAGE_COLOR );
                }
                
                { // Update and Draw Store Line
                    if( AppState->ToStore_IsSelected ) {
                        if( WasReleased( Mouse, MouseButton_Left ) ) {
                            AppState->ToStore_IsSelected = false;
                            
                            if( IsInRect( MouseP, Storage_AdjBound ) ) {
                                LOCKER Locker = {};
                                Locker.ItemID   = AppState->ToStore;
                                Locker.Position = MouseP;
                                AppState->Locker[ AppState->nLocker ] = Locker;
                                
                                AppState->LastItemStored = AppState->ToStore;
                                
                                LogItemToDebug( false, AppState->ToStore );
                                
                                AppState->nLocker += 1;
                                AppState->ToStore  = -1;
                                
                                if( ( AppState->nLocker - AppState->nExtraLocker ) >= AppState->nMinimumItems ) {
                                    if( AppState->Tutorial_IsActive ) {
                                        if( ( !AppState->Turn_IsActive ) && ( AppState->nMinimumItems >= 3 ) ) {
                                            AppState->Turn_IsActive = true;
                                            AppState->Turn_Timer    = TURN_TARGET_TIME;
                                        }
                                        if( ( AppState->nMinimumItems == 3 ) && ( AppState->nCorrectItemsReturned_ThisLevel == 4 ) ) {
                                            Assert( AppState->SecondaryObjective == SecondaryObjective_None );
                                            AppState->SecondaryObjective = SecondaryObjective_WhichInSet;
                                            AppState->Tutorial_PauseForFirstQuestion = true;
                                        }
                                    } else if( !AppState->Turn_IsActive ) {
                                        AppState->Turn_IsActive = true;
                                        AppState->Turn_Timer    = TURN_TARGET_TIME;
                                        
                                        AppState->SecondaryObjective = GetNextSecondaryObjective( AppState );
                                        
                                        //AppState->SecondaryObjective = SecondaryObjective_ExtraItem;
                                        //AppState->SecondaryObjective = SecondaryObjective_WhichInSet;
                                        //AppState->SecondaryObjective = SecondaryObjective_YesNoInSet;
                                        //AppState->SecondaryObjective = SecondaryObjective_QuickTimer;
                                    }
                                    
                                    GetItemToFind( TempMemory, AppState );
                                    
                                    if( ( AppState->SecondaryObjective == SecondaryObjective_WhichInSet ) && ( AppState->SecondaryObjective_ItemID0 <= -1 ) ) {
                                        GetItemsForWhichInSetQuestion( TempMemory, AppState );
                                    }
                                    if( ( AppState->SecondaryObjective == SecondaryObjective_YesNoInSet ) && ( AppState->SecondaryObjective_ItemID0 <= -1 ) ) {
                                        GetItemForYesNoInSetQuestion( TempMemory, AppState );
                                    }
                                    if( ( AppState->SecondaryObjective == SecondaryObjective_ExtraItem ) && ( AppState->SecondaryObjective_ItemID0 <= -1 ) && ( AppState->nExtraLocker <= 0 ) ) {
                                        GetItemForExtraItem( AppState );
                                    }
                                } else {
                                    GetItemToStore( AppState );
                                }
                                
                                PlaySound( AppState, SoundID_Item_Drop );
                            }
                        }
                    }
                    
                    if( AppState->ToStore >= 0 ) {
                        
                        
#if 0                    
                        if( !AppState->ToStore_IsSelected ) {
                            vec4 Background_Color = COLOR_GRAY( 0.05f );
                            if( AppState->ShowTutorial ) {
                                AppState->ShowTutorial_Timer = MaxValue( AppState->ShowTutorial_Timer - dT, 0.0f );
                                if( AppState->ShowTutorial_Timer <= 0.0f ) {
                                    AppState->ShowTutorial_Timer = SHOW_TUTORIAL_TARGET_TIME;
                                }
                                flo32 tTimer = Clamp01( AppState->ShowTutorial_Timer / SHOW_TUTORIAL_TARGET_TIME );
                                Background_Color = lerp( Background_Color, tTimer, COLOR_GRAY( 0.25f ) );
                            }
                            if( ( AppState->Health > 0 ) && ( AppState->ToMove_iLocker <= -1 ) && ( !AppState->SecondaryObjective_IsSelected ) && ( IsInRect( MouseP, ToStore_Bound ) ) ) {
                                Background_Color = COLOR_GRAY( 0.25f );
                            }
                            DrawRect( Pass_Game, ToStore_Bound, Background_Color );
                            //DrawLockerContents( Pass_Game, AddRadius( ToStore_Bound, -Slot_Margin ), AppState->ToStore );
                        }
#endif
                    }
                }
                
                { //: ToFind
                    {
                        rect Arrow = ToFind_Return;
                        Arrow.Left   += 120.0f;
                        Arrow.Right  -= 120.0f;
                        Arrow.Bottom += 100.0f;
                        Arrow.Top     = Arrow.Bottom + 80.0f;
                        
                        vec4  Color = COLOR_WHITE;
                        flo32 Alpha = lerp( 0.0f, AppState->ToFind_tLight, 0.75f );
                        
                        //DrawRect( Pass_Game, TextureID_ArrowWithShadow, Arrow, Color * Alpha );
                    }
                    
                    rect ToFind_Panel = ToFind_Return;
                    ToFind_Panel.Top    = UI_Bound.Top;
                    ToFind_Panel.Bottom = ToFind_Return.Top;
                    //DrawRect( Pass_Game, ToFind_Panel, TowardWhite( WALL_COLOR, 0.05f ) );
                    //DrawRect( Pass_Game, TextureID_ToFind_Panel, ToFind_Panel, QUESTION_COLOR );
                    DrawRect( Pass_Game, TextureID_StorageArea_Shadow, ToFind_Panel, PANEL_COLOR );
                }
                
                { // Update and Draw ToFind_Display
                    DrawRect( Pass_Game, AddRadius( ToFind_Display, 2.0f ), TowardBlack( DISPLAY_BORDER_COLOR, 0.5f ) );
                    DrawRect( Pass_Game, ToFind_Display, DISPLAY_BORDER_COLOR );
                    DrawRect( Pass_Game, AddRadius( ToFind_Display, -12.0f ), COLOR_BLACK );
                    
                    rect Item_Bound = AddRadius( ToFind_Display, -20.0f );
                    
                    if( AppState->Health > 0 ) {
                        if( AppState->ToStore >= 0 ) {
                            vec4 Color = TowardBlack( TEXT_COLOR, 0.2f );
                            rect Lock = RectTCD( GetTC( ToFind_Display ) + Vec2( 0.0f, -36.0f ), Vec2( 34.0f, 40.0f ) );
                            DrawRect( Pass_Game, TextureID_Lock, Lock, Color );
                            
                            char Str[ 16 ] = {};
                            int32 nToTake = AppState->nMinimumItems - ( AppState->nLocker - AppState->nExtraLocker );
                            //sprintf( Str, "%u", nToTake );
                            sprintf( Str, "%u/%u", AppState->nLocker, ( AppState->nMinimumItems + AppState->nExtraLocker ) );
                            DrawString( Pass_Game, AppState->FontList + FontID_2_00, Str, GetBC( Lock ) + Vec2( 0.0f, -16.0f ), TextAlign_TopCenter, Color );
                        }
                        if( AppState->ToFind >= 0 ) {
                            flo32 Alpha = 1.0f;
                            if( ( AppState->Tutorial_IsActive ) && ( AppState->Tutorial_PauseForFirstQuestion ) ) {
                                Alpha = 0.0f;
                            }
                            vec4  Color = COLOR_WHITE;
                            DrawLockerContents( Pass_Game, Item_Bound, AppState->ToFind, Alpha, Color );
                        }
                        if( AppState->Correct_ItemID >= 0 ) {
                            flo32 Lo = LOCKER_CORRECT_TARGET_TIME - ( 60.0f / 60.0f );
                            flo32 Hi = Lo + ( 16.0f / 60.0f );
                            flo32 tTimer     = GetT01( AppState->Correct_Timer, Lo, Hi );
                            //DrawLockerContents( Pass_Game, Item_Bound, AppState->Correct_ItemID, tTimer );
                            
                            if( AppState->Correct_Timer >= Lo ) {
                                DrawRect( Pass_Game, AddRadius( ToFind_Display, -12.0f ), COLOR_BLACK * tTimer );
                                orect Checkmark = ORectCD( GetCenter( Item_Bound ) + Vec2( 0.0f, 10.0f ), GetDim( Item_Bound ) + Vec2( -28.0f, -28.0f ), -PI * 0.25f );
                                DrawORect( Pass_Game, TextureID_Checkmark, Checkmark, BONUS_POINTS_COLOR * tTimer );
                            }
                            
                            AppState->Correct_Timer = MaxValue( AppState->Correct_Timer - dT, 0.0f );
                            if( AppState->Correct_Timer <= 0.0f ) {
                                AppState->Correct_ItemID = -1;
                            }
                        }
                    }
                }
                
                { // Update and Draw ToFind_Display Lights
                    AppState->Light_Timer = MaxValue( AppState->Light_Timer - dT, 0.0f );
                    
                    flo32 FromRight = 50.0f;
                    //flo32 Radius    = 22.0f;
                    
                    vec4 Correct_Color = TowardBlack( COLOR_GREEN, 0.75f );
                    vec4 Wrong_Color   = TowardBlack( COLOR_RED,   0.75f );
                    
                    vec2 Correct_Pos = GetT( ToFind_Display, Vec2( 1.0f, 0.75f ) ) + Vec2( FromRight, -6.0f );
                    vec2 Wrong_Pos   = GetT( ToFind_Display, Vec2( 1.0f, 0.25f ) ) + Vec2( FromRight,  6.0f );
                    
                    flo32 Fade_TargetTime = ( 20.0f / 60.0f );
                    flo32 tLight          = Clamp01( AppState->Light_Timer / Fade_TargetTime );
                    
                    if( AppState->Light_Timer > 0.0f ) {
                        if( AppState->Light_IsCorrect ) {
                            Correct_Color = lerp( Correct_Color, tLight, COLOR_GREEN );
                        } else {
                            Wrong_Color   = lerp( Wrong_Color,   tLight, COLOR_RED );
                        }
                    }
                    
                    //DrawCircle( Pass_Game, Correct_Pos, Radius,        COLOR_BLACK );
                    //DrawCircle( Pass_Game, Correct_Pos, Radius * 0.8f, Correct_Color );
                    
                    //DrawCircle( Pass_Game, Wrong_Pos, Radius,        COLOR_BLACK );
                    //DrawCircle( Pass_Game, Wrong_Pos, Radius * 0.8f, Wrong_Color );
                    
                    //vec2 Light_Dim = Vec2( 400.0f, 400.0f );
                    //DrawRect( Pass_Game, TextureID_Glow, RectCD( Correct_Pos, Light_Dim ), Correct_Color );
                    //DrawRect( Pass_Game, TextureID_Glow, RectCD( Wrong_Pos,   Light_Dim ), Wrong_Color   );
                    //DrawRect( Pass_Game, TextureID_Glow, RectCD( Mouse->Position, Light_Dim ), COLOR_WHITE );
                    
                    { // Update and Draw ScoreUpdate_Display
                        DrawRect( Pass_Game, AddRadius( ScoreUpdate_Display, 2.0f ), TowardBlack( DISPLAY_BORDER_COLOR, 0.5f ) );
                        DrawRect( Pass_Game, ScoreUpdate_Display, DISPLAY_BORDER_COLOR );
                        DrawRect( Pass_Game, AddRadius( ScoreUpdate_Display, -8.0f ), COLOR_BLACK );
                    }
                    
                    { // Update Wires
                        AppState->Wire_Score_Timer  = MaxValue( AppState->Wire_Score_Timer - dT, 0.0f );
                    }
                    
                    { // Draw ScoreUpdate_Display Lights
                        flo32 FromBottom = 16.0f;
                        flo32 Radius     = 8.0f;
                        
                        vec4 Color0 = COLOR_GRAY( 0.025f );
                        vec4 Color1 = COLOR_GRAY( 0.025f );
                        vec4 Color2 = COLOR_GRAY( 0.025f );
                        
                        int32 nPoints = 0;
                        
                        if( AppState->ToFind >= 0 ) {
                            nPoints += 1;
                            Color0 = COLOR_YELLOW;
                            
                            if( ( AppState->SecondaryObjective == SecondaryObjective_QuickTimer ) && ( AppState->SecondaryObjective_Timer > 0.0f ) ) {
                                Color1 = COLOR_GREEN;
                                nPoints += 1;
                            }
                            if( AppState->SecondaryObjective == SecondaryObjective_ExtraItem ) {
                                if( AppState->nExtraLocker >= 1 ) {
                                    Color1   = COLOR_GREEN;
                                    nPoints += 1;
                                    
                                    if( AppState->SecondaryObjective_DoTwoBonusPoints ) {
                                        Color2   = COLOR_GREEN;
                                        nPoints += 1;
                                    }
                                }
                            }
                        }
                        
                        vec2 Pos0 = GetT( ScoreUpdate_Display, Vec2( 0.25f, 0.0f ) ) + Vec2( 0.0f, -FromBottom );
                        vec2 Pos1 = GetT( ScoreUpdate_Display, Vec2( 0.50f, 0.0f ) ) + Vec2( 0.0f, -FromBottom );
                        vec2 Pos2 = GetT( ScoreUpdate_Display, Vec2( 0.75f, 0.0f ) ) + Vec2( 0.0f, -FromBottom );
                        
                        DrawRect( Pass_Game, TextureID_Circle, RectCR( Pos0, Radius        ), COLOR_BLACK );
                        DrawRect( Pass_Game, TextureID_Circle, RectCR( Pos0, Radius - 2.0f ), Color0 );
                        
                        DrawRect( Pass_Game, TextureID_Circle, RectCR( Pos1, Radius        ), COLOR_BLACK );
                        DrawRect( Pass_Game, TextureID_Circle, RectCR( Pos1, Radius - 2.0f ), Color1 );
                        
                        DrawRect( Pass_Game, TextureID_Circle, RectCR( Pos2, Radius        ), COLOR_BLACK );
                        DrawRect( Pass_Game, TextureID_Circle, RectCR( Pos2, Radius - 2.0f ), Color2 );
                        
                        vec4 Color = TowardBlack( TEXT_COLOR, 0.2f );
                        char Str[ 8 ] = {};
                        strcpy( Str, "-" );
                        if( nPoints > 0 ) {
                            sprintf( Str, "+%u", nPoints );
                            Color = TEXT_COLOR;
                        }
                        DrawString( Pass_Game, AppState->FontList + FontID_3_00, Str, GetCenter( ScoreUpdate_Display ), TextAlign_Center, Color );
                    }
                }
                
                { // Draw Turn Timer
                    DrawRect( Pass_Game, AddRadius( Turn_Bound, 8.0f + 2.0f ), TowardBlack( DISPLAY_BORDER_COLOR, 0.5f ) );
                    DrawRect( Pass_Game, AddRadius( Turn_Bound, 8.0f ), DISPLAY_BORDER_COLOR );
                    
                    { // Quick Timer
                        flo32 tQuick = Clamp01( SECONDARY_OBJECTIVE_QUICK_TIMER_TARGET_TIME / TURN_TARGET_TIME );
                        rect Bound = Turn_Bound;
                        Bound.Bottom = Bound.Top;
                        Bound.Top   += 40.0f;
                        Bound.Right = lerp( Bound.Left, tQuick, Bound.Right );
                        
                        DrawRect( Pass_Game, AddRadius( Bound, 8.0f + 2.0f ), TowardBlack( DISPLAY_BORDER_COLOR, 0.5f ) );
                        DrawRect( Pass_Game, AddRadius( Bound, 8.0f ), DISPLAY_BORDER_COLOR );
                        
                        // Redraw the Turn Timer bound, so the drop shadow is correct.
                        DrawRect( Pass_Game, AddRadius( Turn_Bound, 8.0f ), DISPLAY_BORDER_COLOR );
                        
                        if( AppState->SecondaryObjective_Timer > 0.0f ) {
                            DrawRect( Pass_Game, AddRadius( Bound, 2.0f ), COLOR_GREEN );
                            DrawRect( Pass_Game, AddRadius( Turn_Bound, 2.0f ), COLOR_GREEN );
                        }
                        
                        DrawRect( Pass_Game, Bound, COLOR_BLACK );
                        
                        rect Inner = Bound;
                        flo32 tTimer = Clamp01( AppState->SecondaryObjective_Timer / SECONDARY_OBJECTIVE_QUICK_TIMER_TARGET_TIME );
                        Inner.Left = lerp( Bound.Right, tTimer, Bound.Left );
                        DrawRect( Pass_Game, Inner, COLOR_GREEN );
                        
                        rect Cover = Bound;
                        Cover.Bottom = lerp( Cover.Bottom, AppState->SecondaryObjective_tQuick, Cover.Top );
                        DrawRect( Pass_Game, Cover, COLOR_GRAY( 0.05f ) );
                    }
                    
                    vec4 Background_Color = COLOR_BLACK;
                    if( ( AppState->Tutorial_IsActive ) && ( !AppState->Turn_IsActive ) ) {
                        Background_Color = COLOR_GRAY( 0.05f );
                    }
                    DrawRect( Pass_Game, Turn_Bound, Background_Color );
                    
                    if( AppState->Turn_IsActive ) {
                        flo32 tTurn = Clamp01( AppState->Turn_Timer / TURN_TARGET_TIME );
                        
                        rect Bound = Turn_Bound;
                        Bound.Left = lerp( Bound.Right, tTurn, Bound.Left );
                        DrawRect( Pass_Game, Bound, COLOR_YELLOW );
                    }
                }
                
                if( AppState->Correct_Timer > 0.0f ) {
                    flo32 tTimer     = Clamp01( AppState->Correct_Timer / LOCKER_CORRECT_TARGET_TIME );
                    flo32 tTimerSq   = tTimer * tTimer;
                    
                    flo32 tPos_Percent = 0.6667f;
                    flo32 tPos         = Clamp01( ( AppState->Correct_Timer - LOCKER_CORRECT_TARGET_TIME * ( 1.0f - tPos_Percent ) ) / ( LOCKER_CORRECT_TARGET_TIME * tPos_Percent ) ); 
                    
                    vec2  Dest  = GetCenter( ToFind_Return );
                    vec2  Pos   = lerp( Dest, tPos * tPos * tPos, AppState->Correct_Position );
                    flo32 Scale = lerp( 0.025f, tTimer, 1.0f );
                    flo32 Alpha = lerp( 0.0f,   tTimer, 1.0f );
                    
                    flo32 Box_LoTargetTime = LOCKER_CORRECT_TARGET_TIME - ( 32.0f / 60.0f );
                    flo32 Box_HiTargetTime = LOCKER_CORRECT_TARGET_TIME - (  0.0f / 60.0f );
                    flo32 Box_Alpha = GetT01( AppState->Correct_Timer, Box_LoTargetTime, Box_HiTargetTime );
                    
                    flo32 Glow_LoTargetTime = 0.0f;
                    flo32 Glow_HiTargetTime = LOCKER_CORRECT_TARGET_TIME;
                    flo32 Glow_tAlpha = GetT01( AppState->Correct_Timer, Glow_LoTargetTime, Glow_HiTargetTime );
                    flo32 Glow_Alpha  = ( -cosf( Glow_tAlpha * TAU ) * 0.5f + 0.5f ) * 0.5f;
                    
                    rect  Bound = RectCD( Pos, ITEM_DIM * Scale );
                    DrawRect( Pass_Game, TextureID_Glow, RectCD( Pos, ITEM_DIM * Scale * 1.125f ), COLOR_WHITE * Glow_Alpha );
                    DrawRect( Pass_Game, TextureID_CardboardBox_Empty, Bound, CARDBOARD_EMPTY_COLOR * Box_Alpha );
                    DrawLockerContents( Pass_Game, Bound, AppState->Correct_ItemID, 1.0f, COLOR_WHITE * ( Alpha * Alpha ) );
                }
                
                { // Update and Draw Find Line
#if 0
                    if( ( IsInRect( MouseP, ToFind_Bound ) ) && ( AppState->ToFind >= 0 ) && ( WasPressed( Mouse, MouseButton_Left ) ) ) {
                        AppState->ToStoreFind_Text_Timer = TO_STORE_FIND_TEXT_TARGET_TIME;
                        AppState->ToStoreFind_IsWrong    = true;
                        AppState->ToStoreFind_Timer      = TO_STORE_FIND_REACT_TARGET_TIME * 0.5f;
                    }
#endif
                    
                    if( AppState->Health > 0 ) {
                        for( int32 iLocker = 0; iLocker < AppState->nLocker; iLocker++ ) {
                            LOCKER * Locker = AppState->Locker + iLocker;
                            Locker->ShowItem_Timer = MaxValue( Locker->ShowItem_Timer - dT, 0.0f );
                        }
                    }
                    
                    //DrawRect( Pass_Game, ToStoreFind_Bound, COLOR_GRAY( 0.025f ) );
                    if( AppState->ToFind >= 0 ) {
#if 0
                        if( AppState->Health > 0 ) {
                            DrawString( Pass_Game, AppState->FontList + FontID_3_00, "FIND", GetTC( ToStoreFind_Bound ) + Vec2( 0.0f, 8.0f ), TextAlign_BottomCenter, COLOR_WHITE );
                        }
#endif
                        
                        //DrawRect( Pass_Game, ToStoreFind_Bound, Background_Color );
                        //DrawLockerContents( Pass_Game, ToFind_Bound, AppState->ToFind );
                    }
                    
                }
                
                { // Update and Draw Question
                    //vec4 Background_Color = QUESTION_COLOR;
                    //vec4 Outline_Color    = TowardWhite( Background_Color, 0.1f );
                    DrawRect( Pass_Game, Question_Bound, TowardBlack( QUESTION_COLOR, 0.5f ) );
                    DrawRect( Pass_Game, TextureID_QuestionArea_Shadow, AddRadius( Question_Bound, -4.0f ), QUESTION_COLOR );
                    
                    DrawRect( Pass_Game, AddRadius( Question_Display, 2.0f ), TowardBlack( DISPLAY_BORDER_COLOR, 0.5f ) );
                    DrawRect( Pass_Game, Question_Display, DISPLAY_BORDER_COLOR );
                    DrawRect( Pass_Game, AddRadius( Question_Display, -12.0f ), COLOR_BLACK );
                    //DrawRect( Pass_Game, TextureID_StorageArea_Shadow, Question_Bound, QUESTION_COLOR );
                    
                    { // Draw Wires
                        flo32 TargetTime = WIRE_LIGHT_UP_TARGET_TIME - ( 40.0f / 60.0f );
                        flo32 tScore  = Clamp01( AppState->Wire_Score_Timer  / TargetTime );
                        
                        vec4  Score_Color    = COLOR_BLACK;
                        vec4  Health_Color   = COLOR_BLACK;
                        vec4  Overflow_Color = COLOR_BLACK;
                        vec4  Bonus_Color    = COLOR_BLACK;
                        
                        if( AppState->Wire_Score_DoScore ) {
                            Score_Color = lerp( COLOR_BLACK, tScore, BONUS_POINTS_COLOR );
                        }
                        if( AppState->Wire_Score_DoHealth ) {
                            Health_Color = lerp( COLOR_BLACK, tScore, BONUS_HEALTH_COLOR );
                        }
                        if( AppState->Wire_Score_DoOverflow ) {
                            Overflow_Color = lerp( COLOR_BLACK, tScore, BONUS_HEALTH_COLOR );
                        }
                        if( AppState->Wire_Score_DoBonus ) {
                            Bonus_Color = lerp( COLOR_BLACK, tScore, BONUS_HEALTH_COLOR );
                        }
                        
                        rect Score = {};
                        Score.Right  = ScoreUpdate_Display.Right - 6.0f;
                        Score.Bottom = ScoreUpdate_Display.Top;
                        Score.Left   = Score.Right - 4.0f;
                        Score.Top    = ScorePanel_Display.Bottom;
                        
                        vec2 P = GetRC( Health_Display ) + Vec2( 0.0f, -6.0f );
                        vec2 Q = GetRC( Health_Display ) + Vec2( 0.0f,  6.0f );
                        
                        rect Health0 = {};
                        Health0.Right  = Score.Left - 8.0f;
                        Health0.Left   = Health0.Right - 4.0f;
                        Health0.Bottom = ScoreUpdate_Display.Top;
                        Health0.Top    = P.y;
                        
                        rect Health1 = {};
                        Health1.Left   = P.x;
                        Health1.Right  = Health0.Right;
                        Health1.Bottom = P.y - 2.0f;
                        Health1.Top    = P.y + 2.0f;
                        
                        rect Overflow0 = {};
                        Overflow0.Right  = Score.Left - 8.0f;
                        Overflow0.Left   = Overflow0.Right - 4.0f;
                        Overflow0.Bottom = Q.y;
                        Overflow0.Top    = ScorePanel_Display.Bottom;
                        
                        rect Overflow1 = {};
                        Overflow1.Left   = Q.x;
                        Overflow1.Right  = Overflow0.Right;
                        Overflow1.Bottom = Q.y - 2.0f;
                        Overflow1.Top    = Q.y + 2.0f;
                        
                        rect Damage = {};
                        Damage.Right  = Turn_Bound.Right - 12.0f;
                        Damage.Left   = Damage.Right - 4.0f;
                        Damage.Bottom = Turn_Bound.Top;
                        Damage.Top    = Health_Display.Bottom;
                        
                        rect Bonus0 = {};
                        Bonus0.Left   = Question_Display.Right;
                        Bonus0.Right  = Health_Display.Left - 100.0f;
                        Bonus0.Bottom = Question_Display.Bottom + 22.0f;
                        Bonus0.Top    = Bonus0.Bottom + 4.0f;
                        
                        rect Bonus2 = {};
                        Bonus2.Left   = Bonus0.Right - 4.0f;
                        Bonus2.Right  = Health_Display.Left;
                        Bonus2.Top    = GetLC( Health_Display ).y + 2.0f;
                        Bonus2.Bottom = Bonus2.Top - 4.0f;
                        
                        rect Bonus1 = RectExtrema( GetTL( Bonus2 ), GetBR( Bonus0 ) );
                        
                        rect Plate  = {};
                        Plate.Left   = Question_Bound.Right;
                        Plate.Right  = ToFind_Return.Left;
                        Plate.Bottom = Bonus0.Bottom - 12.0f;
                        Plate.Top    = Bonus0.Top    + 12.0f;
                        
                        DrawRect( Pass_Game, Plate,   COLOR_GRAY( 0.025f ) );
                        DrawRect( Pass_Game, AddRadius( Plate, -1.0f ),   COLOR_GRAY( 0.05f  ) );
                        DrawRect( Pass_Game, Score,   Score_Color );
                        DrawRect( Pass_Game, Health0, Health_Color );
                        DrawRect( Pass_Game, Health1, Health_Color );
                        DrawRect( Pass_Game, Overflow0, Overflow_Color );
                        DrawRect( Pass_Game, Overflow1, Overflow_Color );
                        DrawRect( Pass_Game, Bonus0,   Bonus_Color );
                        DrawRect( Pass_Game, Bonus1,   Bonus_Color );
                        DrawRect( Pass_Game, Bonus2,   Bonus_Color );
                        
                        DrawRect( Pass_Game, RectCD( GetT( Plate, Vec2( 0.1f, 0.5f ) ), Vec2( 4.0f, 8.0f ) ), COLOR_GRAY( 0.2f ) );
                        DrawRect( Pass_Game, RectCD( GetT( Plate, Vec2( 0.9f, 0.5f ) ), Vec2( 4.0f, 8.0f ) ), COLOR_GRAY( 0.2f ) );
                    }
                    
                    if( AppState->SecondaryObjective == SecondaryObjective_QuickTimer ) {
                        flo32 dQuick = 1.0f / 0.5f;
                        AppState->SecondaryObjective_tQuick = MinValue( AppState->SecondaryObjective_tQuick + dQuick * dT, 1.0f );
                        
                        flo32 dAlpha = 1.0f / 0.35f;
                        AppState->SecondaryObjective_Alpha = MinValue( AppState->SecondaryObjective_Alpha + dAlpha * dT, 1.0f );
                        
                        if( AppState->Health > 0 ) {
                            AppState->SecondaryObjective_Timer = MaxValue( AppState->SecondaryObjective_Timer - dT, 0.0f );
                        }
                        
                        flo32 Alpha      = Clamp01( AppState->SecondaryObjective_Alpha * 1.25f );
                        
                        FONT * Font = AppState->FontList + FontID_3_00;
                        DrawString( Pass_Game, Font, "** QUICK TIMER BONUS **", GetCenter( Question_Bound ), TextAlign_Center, TEXT_COLOR * Alpha );
                    } else {
                        flo32 dQuick = 1.0f / 0.5f;
                        AppState->SecondaryObjective_tQuick = MaxValue( AppState->SecondaryObjective_tQuick - dQuick * dT, 0.0f );
                    }
                    
                    if( ( AppState->SecondaryObjective == SecondaryObjective_WhichInSet ) && ( AppState->SecondaryObjective_ItemID0 >= 0 ) ) {
                        if( AppState->SecondaryObjective_IsActive ) {
                            flo32 dAlpha = 1.0f / 0.35f;
                            AppState->SecondaryObjective_Alpha = MinValue( AppState->SecondaryObjective_Alpha + dAlpha * dT, 1.0f );
                        } else {
                            AppState->SecondaryObjective_ViewResultTimer = MaxValue( AppState->SecondaryObjective_ViewResultTimer - dT, 0.0f );
                            
                            if( AppState->SecondaryObjective_ViewResultTimer <= 0.0f ) {
                                flo32 dAlpha = 1.0f / 0.35f;
                                AppState->SecondaryObjective_Alpha = MaxValue( AppState->SecondaryObjective_Alpha - dAlpha * dT, 0.0f );
                            }
                        }
                        
                        vec2 bPos  = GetCenter( Question_Bound ) + Vec2( 12.0f, 0.0f );
                        rect Bound = RectLCD( bPos, LOCKER_DIM );
                        
                        FONT * Font = AppState->FontList + FontID_2_00;
                        
                        flo32 FromCenter = 340.0f;
                        flo32 Alpha      = Clamp01( AppState->SecondaryObjective_Alpha * 1.25f );
                        
                        DrawString( Pass_Game, Font, "Which memory is in", bPos + Vec2( -FromCenter, 40.0f ), TextAlign_TopLeft, TEXT_COLOR * Alpha );
                        DrawString( Pass_Game, Font, "your storage boxes?",  bPos + Vec2( -FromCenter + 20.0f,  4.0f ), TextAlign_TopLeft, TEXT_COLOR * Alpha );
                        
                        vec2  Button_Dim = ITEM_DIM;
                        vec2  AtPos      = bPos;
                        flo32 InBetween  = 20.0f;
                        
                        rect Item0_Bound = RectLCD( AtPos, Button_Dim );
                        AtPos.x += ( Button_Dim.x + InBetween );
                        rect Item1_Bound = RectLCD( AtPos, Button_Dim );
                        
                        if( AppState->SecondaryObjective_IsActive ) {
                            boo32 UpdateHealthAndScore = false;
                            boo32 IncreaseHealth       = false;
                            if( UpdateAndDrawItemClickButton( Pass_Game, Mouse, AppState->SecondaryObjective_ItemID0, Item0_Bound, Alpha, ( AppState->Health > 0 ) ) ) {
                                UpdateHealthAndScore = true;
                                AppState->SecondaryObjective_FirstOptionWasSelected = true;
                                if( AppState->SecondaryObjective_IsInSet ) {
                                    IncreaseHealth = true;
                                    AppState->SecondaryObjective_WasAnsweredCorrectly = true;
                                } else {
                                    AppState->SecondaryObjective_WasAnsweredCorrectly = false;
                                }
                            }
                            if( UpdateAndDrawItemClickButton( Pass_Game, Mouse, AppState->SecondaryObjective_ItemID1, Item1_Bound, Alpha, ( AppState->Health > 0 ) ) ) {
                                UpdateHealthAndScore = true;
                                AppState->SecondaryObjective_FirstOptionWasSelected = false;
                                if( !AppState->SecondaryObjective_IsInSet ) {
                                    IncreaseHealth = true;
                                    AppState->SecondaryObjective_WasAnsweredCorrectly = true;
                                } else {
                                    AppState->SecondaryObjective_WasAnsweredCorrectly = false;
                                }
                            }
                            
                            if( UpdateHealthAndScore ) {
                                AppState->SecondaryObjective_ViewResultTimer = SECONDARY_OBJECTIVE_VIEW_RESULT_TARGET_TIME;
                                AppState->SecondaryObjective_IsActive = false;
                                
                                int32 ItemIDInSet = AppState->SecondaryObjective_ItemID0;
                                if( !AppState->SecondaryObjective_IsInSet ) {
                                    ItemIDInSet = AppState->SecondaryObjective_ItemID1;
                                }
                                
                                // NOTE: Code to display item from question. This feature kinda breaks the game, especially at low levels, but I'm just commenting out the code instead of deleting it.
                                //int32   iLocker = GetILockerByItemID( AppState, ItemIDInSet );
                                //LOCKER * Locker = AppState->Locker + iLocker;
                                //Locker->ShowItem_Timer = LOCKER_SHOW_ITEM_TARGET_TIME;
                                //_swap( LOCKER, AppState->Locker[ iLocker ], AppState->Locker[ AppState->nLocker - 1 ] );
                                
                                boo32 DoOverflow = false;
                                
                                if( IncreaseHealth ) {
                                    if( AppState->Health < AppState->MaxHealth ) {
                                        AppState->Health += 1;
                                        AddScoreUpdate( AppState, ScoreUpdate_OneHealth );
                                        
                                        PlaySound( AppState, SoundID_Health_Up );
                                    } else {
                                        AppState->Score  += 1;
                                        AddScoreUpdate( AppState, ScoreUpdate_OneBonusPoint );
                                        
                                        PlaySound( AppState, SoundID_AddBonusPoints );
                                        
                                        DoOverflow = true;
                                    }
                                } else {
                                    AppState->Health = MaxValue( AppState->Health - 1, 0 );
                                    AddScoreUpdate( AppState, ScoreUpdate_LoseHealth );
                                    
                                    PlaySound( AppState, SoundID_Health_Down );
                                }
                                
                                AppState->Wire_Score_Timer      = WIRE_LIGHT_UP_TARGET_TIME;
                                AppState->Wire_Score_DoScore    = false;
                                AppState->Wire_Score_DoHealth   = false;
                                AppState->Wire_Score_DoOverflow = DoOverflow;
                                AppState->Wire_Score_DoBonus    = true;
                                
                                if( ( AppState->Tutorial_IsActive ) && ( AppState->Tutorial_PauseForFirstQuestion ) ) {
                                    AppState->Tutorial_PauseForFirstQuestion = false;
                                }
                            }
                        } else {
                            DrawRect( Pass_Game, Item0_Bound, COLOR_GRAY( 0.2f ) * Alpha );
                            DrawRect( Pass_Game, AddRadius( Item0_Bound, -6.0f ), COLOR_GRAY( 0.1f ) * Alpha );
                            
                            DrawRect( Pass_Game, Item1_Bound, COLOR_GRAY( 0.2f ) * Alpha );
                            DrawRect( Pass_Game, AddRadius( Item1_Bound, -6.0f ), COLOR_GRAY( 0.1f ) * Alpha );
                            
                            DrawLockerContents( Pass_Game, AddRadius( Item0_Bound, -12.0f ), AppState->SecondaryObjective_ItemID0, Alpha, COLOR_WHITE );
                            DrawLockerContents( Pass_Game, AddRadius( Item1_Bound, -12.0f ), AppState->SecondaryObjective_ItemID1, Alpha, COLOR_WHITE );
                            
                            if( AppState->SecondaryObjective_IsInSet ) {
                                orect Checkmark = ORectCD( GetCenter( Item0_Bound ) + Vec2( 0.0f, 10.0f ), GetDim( Item0_Bound ) + Vec2( -28.0f, -28.0f ), -PI * 0.25f );
                                DrawORect( Pass_Game, TextureID_Checkmark, Checkmark, BONUS_POINTS_COLOR * Alpha );
                            } else {
                                orect Checkmark = ORectCD( GetCenter( Item1_Bound ) + Vec2( 0.0f, 10.0f ), GetDim( Item1_Bound ) + Vec2( -28.0f, -28.0f ), -PI * 0.25f );
                                DrawORect( Pass_Game, TextureID_Checkmark, Checkmark, BONUS_POINTS_COLOR * Alpha );
                            }
                        }
                        
                        if( ( !AppState->SecondaryObjective_IsActive ) && ( AppState->SecondaryObjective_Alpha <= 0.0f ) ) {
                            AppState->SecondaryObjective_ItemID0 = -1;
                            AppState->SecondaryObjective_ItemID1 = -1;
                        }
                    }
                    
                    if( ( AppState->SecondaryObjective == SecondaryObjective_YesNoInSet ) && ( AppState->SecondaryObjective_ItemID0 >= 0 ) ) {
                        if( AppState->SecondaryObjective_IsActive ) {
                            flo32 dAlpha = 1.0f / 0.35f;
                            AppState->SecondaryObjective_Alpha = MinValue( AppState->SecondaryObjective_Alpha + dAlpha * dT, 1.0f );
                        } else {
                            AppState->SecondaryObjective_ViewResultTimer = MaxValue( AppState->SecondaryObjective_ViewResultTimer - dT, 0.0f );
                            
                            if( AppState->SecondaryObjective_ViewResultTimer <= 0.0f ) {
                                flo32 dAlpha = 1.0f / 0.35f;
                                AppState->SecondaryObjective_Alpha = MaxValue( AppState->SecondaryObjective_Alpha - dAlpha * dT, 0.0f );
                            }
                        }
                        
                        vec2 bPos        = GetCenter( Question_Bound ) + Vec2( 88.0f, 0.0f );
                        rect Item_Display = RectCD( bPos, LOCKER_DIM );
                        
                        FONT * Font  = AppState->FontList + FontID_2_00;
                        FONT * Font0 = AppState->FontList + FontID_2_50;
                        
                        flo32 FromDisplay = 340.0f;
                        flo32 Alpha       = Clamp01( AppState->SecondaryObjective_Alpha * 1.25f );
                        
                        DrawString( Pass_Game, Font, "Is this memory in", GetLC( Item_Display ) + Vec2( -FromDisplay, 40.0f ), TextAlign_TopLeft, TEXT_COLOR * Alpha );
                        DrawString( Pass_Game, Font, "your storage boxes?",  GetLC( Item_Display ) + Vec2( -FromDisplay + 20.0f,  4.0f ), TextAlign_TopLeft, TEXT_COLOR * Alpha );
                        
                        DrawRect( Pass_Game, AddRadius( Item_Display, 2.0f ), TowardBlack( DISPLAY_BORDER_COLOR, 0.5f ) * Alpha );
                        DrawRect( Pass_Game, Item_Display, DISPLAY_BORDER_COLOR * Alpha );
                        DrawRect( Pass_Game, AddRadius( Item_Display, -12.0f ), COLOR_BLACK * Alpha );
                        
                        rect Item_Bound = AddRadius( Item_Display, -20.0f );
                        DrawLockerContents( Pass_Game, Item_Bound, AppState->SecondaryObjective_ItemID0, Alpha, COLOR_WHITE );
                        
                        vec2  Button_Dim = Vec2( 108.0f, 60.0f );
                        flo32 InBetween  = 12.0f;
                        
                        flo32 FromDisplayRight = 32.0f;
                        
                        vec2  AtPos = GetRC( Item_Display );
                        rect  Yes_Bound = RectBLD( AtPos + Vec2( FromDisplayRight,  InBetween * 0.5f ), Button_Dim );
                        rect  No_Bound = RectTLD( AtPos + Vec2( FromDisplayRight, -InBetween * 0.5f ), Button_Dim );
                        
                        if( AppState->SecondaryObjective_IsActive ) {
                            boo32 UpdateHealthAndScore = false;
                            boo32 IncreaseHealth       = false;
                            if( UpdateAndDrawPauseButton( Pass_Game, Mouse, Font0, "Yes", Yes_Bound, ( AppState->Health > 0 ), Alpha ) ) {
                                UpdateHealthAndScore = true;
                                AppState->SecondaryObjective_FirstOptionWasSelected = true;
                                if( AppState->SecondaryObjective_IsInSet ) {
                                    IncreaseHealth = true;
                                    AppState->SecondaryObjective_WasAnsweredCorrectly = true;
                                } else {
                                    AppState->SecondaryObjective_WasAnsweredCorrectly = false;
                                }
                            }
                            if( UpdateAndDrawPauseButton( Pass_Game, Mouse, Font0, "No", No_Bound, ( AppState->Health > 0 ), Alpha ) ) {
                                UpdateHealthAndScore = true;
                                AppState->SecondaryObjective_FirstOptionWasSelected = false;
                                if( !AppState->SecondaryObjective_IsInSet ) {
                                    IncreaseHealth = true;
                                    AppState->SecondaryObjective_WasAnsweredCorrectly = true;
                                } else {
                                    AppState->SecondaryObjective_WasAnsweredCorrectly = false;
                                }
                            }
                            
                            if( UpdateHealthAndScore ) {
                                AppState->SecondaryObjective_ViewResultTimer = SECONDARY_OBJECTIVE_VIEW_RESULT_TARGET_TIME;
                                AppState->SecondaryObjective_IsActive = false;
                                
                                // NOTE: Code to display item from question. This feature kinda breaks the game, especially at low levels, but I'm just commenting out the code instead of deleting it.
                                //if( AppState->SecondaryObjective_IsInSet ) {
                                //int32   iLocker = GetILockerByItemID( AppState, AppState->SecondaryObjective_ItemID0 );
                                //LOCKER * Locker = AppState->Locker + iLocker;
                                //Locker->ShowItem_Timer = LOCKER_SHOW_ITEM_TARGET_TIME;
                                //_swap( LOCKER, AppState->Locker[ iLocker ], AppState->Locker[ AppState->nLocker - 1 ] );
                                //}
                                
                                boo32 DoOverflow = false;
                                
                                if( IncreaseHealth ) {
                                    if( AppState->Health < AppState->MaxHealth ) {
                                        AppState->Health += 1;
                                        AddScoreUpdate( AppState, ScoreUpdate_OneHealth );
                                        
                                        PlaySound( AppState, SoundID_Health_Up );
                                    } else {
                                        AppState->Score  += 1;
                                        AddScoreUpdate( AppState, ScoreUpdate_OneBonusPoint );
                                        
                                        PlaySound( AppState, SoundID_AddBonusPoints );
                                        
                                        DoOverflow = true;
                                    }
                                } else {
                                    AppState->Health = MaxValue( AppState->Health - 1, 0 );
                                    AddScoreUpdate( AppState, ScoreUpdate_LoseHealth );
                                    
                                    PlaySound( AppState, SoundID_Health_Down );
                                }
                                
                                AppState->Wire_Score_Timer      = WIRE_LIGHT_UP_TARGET_TIME;
                                AppState->Wire_Score_DoScore    = false;
                                AppState->Wire_Score_DoHealth   = false;
                                AppState->Wire_Score_DoOverflow = DoOverflow;
                                AppState->Wire_Score_DoBonus    = true;
                            }
                        } else {
                            DrawRect( Pass_Game, Yes_Bound, COLOR_GRAY( 0.2f ) * Alpha );
                            DrawRect( Pass_Game, AddRadius( Yes_Bound, -2.0f ), COLOR_GRAY( 0.1f ) * Alpha );
                            
                            DrawRect( Pass_Game, No_Bound, COLOR_GRAY( 0.2f ) * Alpha );
                            DrawRect( Pass_Game, AddRadius( No_Bound, -2.0f ), COLOR_GRAY( 0.1f ) * Alpha );
                            
                            DrawString( Pass_Game, Font0, "Yes", GetCenter( Yes_Bound ), TextAlign_Center, COLOR_BLACK * Alpha );
                            DrawString( Pass_Game, Font0, "No", GetCenter( No_Bound ), TextAlign_Center, COLOR_BLACK * Alpha );
                            
                            vec2 Dim = Vec2( 1.0f, 1.0f ) * ( GetHeight( Yes_Bound ) + 28.0f );
                            if( AppState->SecondaryObjective_IsInSet ) {
                                orect Checkmark = ORectCD( GetCenter( Yes_Bound ) + Vec2( 0.0f, 10.0f ), Dim, -PI * 0.25f );
                                DrawORect( Pass_Game, TextureID_Checkmark, Checkmark, BONUS_POINTS_COLOR * Alpha );
                            } else {
                                orect Checkmark = ORectCD( GetCenter( No_Bound ) + Vec2( 0.0f, 10.0f ), Dim, -PI * 0.25f );
                                DrawORect( Pass_Game, TextureID_Checkmark, Checkmark, BONUS_POINTS_COLOR * Alpha );
                            }
                        }
                        
                        if( ( !AppState->SecondaryObjective_IsActive ) && ( AppState->SecondaryObjective_Alpha <= 0.0f ) ) {
                            AppState->SecondaryObjective_ItemID0 = -1;
                            AppState->SecondaryObjective_ItemID1 = -1;
                        }
                    }
                    
                    if( ( AppState->SecondaryObjective == SecondaryObjective_ExtraItem ) && ( AppState->SecondaryObjective_ItemID0 >= 0 ) ) {
                        AppState->SecondaryObjective_ViewResultTimer = 0.0f;
                        
                        if( AppState->SecondaryObjective_IsActive ) {
                            flo32 dAlpha = 1.0f / 0.35f;
                            AppState->SecondaryObjective_Alpha = MinValue( AppState->SecondaryObjective_Alpha + dAlpha * dT, 1.0f );
                        } else {
                            flo32 dAlpha = 1.0f / 0.35f;
                            AppState->SecondaryObjective_Alpha = MaxValue( AppState->SecondaryObjective_Alpha - dAlpha * dT, 0.0f );
                        }
                        
                        flo32 dRadians = TAU / 3.0f;
                        AppState->SecondaryObjective_Radians = WrapRadians( AppState->SecondaryObjective_Radians + dRadians * dT );
                        
                        flo32 yOffset = sinf( AppState->SecondaryObjective_Radians ) * 5.0f;
                        vec2  Pos     = GetCenter( Question_Display ) + Vec2( 180.0f, 0.0f );
                        
                        rect    Item_Bound = RectCD( Pos + Vec2( 0.0f, yOffset ), ITEM_DIM );
                        rect  Select_Bound = RectCD( Pos, Vec2( GetHeight( Question_Display ), GetHeight( Question_Display ) ) );
                        Select_Bound = AddRadius( Select_Bound, 12.0f );
                        //DrawRectOutline( Pass_Game, Select_Bound, COLOR_RED );
                        
                        FONT * Font = AppState->FontList + FontID_2_00;
                        flo32 Alpha = AppState->SecondaryObjective_Alpha;
                        
                        vec2 Text_Pos = GetCenter( Question_Display ) + Vec2( 20.0f, -16.0f );
                        DrawString( Pass_Game, Font, "Take extra memory?", Text_Pos, TextAlign_BottomRight, TEXT_COLOR * Alpha );
                        
                        if( !AppState->SecondaryObjective_IsSelected ) {
                            if( UpdateAndDrawItemGrabButton( Pass_Game, Mouse, AppState->SecondaryObjective_ItemID0, Item_Bound, Select_Bound, Alpha, ( AppState->Health > 0 ) ) ) {
                                AppState->SecondaryObjective_IsSelected = true;
                            }
                        }
                        
                        if( AppState->SecondaryObjective_IsSelected ) {
                            if( WasReleased( Mouse, MouseButton_Left ) ) {
                                AppState->SecondaryObjective_IsSelected = false;
                                
                                if( IsInRect( MouseP, Storage_AdjBound ) ) {
                                    LOCKER Locker = {};
                                    Locker.ItemID   = AppState->SecondaryObjective_ItemID0;
                                    Locker.Position = MouseP;
                                    AppState->Locker[ AppState->nLocker ] = Locker;
                                    
                                    LogItemToDebug( false, AppState->SecondaryObjective_ItemID0 );
                                    
                                    AppState->LastItemStored = AppState->SecondaryObjective_ItemID0;
                                    
                                    AppState->nLocker      += 1;
                                    AppState->nExtraLocker += 1;
                                    AppState->SecondaryObjective_ItemID0 = -1;
                                    AppState->SecondaryObjective_DoTwoBonusPoints = true;
                                    
                                    PlaySound( AppState, SoundID_Item_Drop );
                                }
                            }
                        }
                    }
                    
                    if( AppState->nExtraLocker >= 1 ) {
                        FONT * Font = AppState->FontList + FontID_3_00;
                        DrawString( Pass_Game, Font, "** EXTRA ITEM BONUS **", GetCenter( Question_Bound ), TextAlign_Center, TEXT_COLOR );
                    }
                }
                
                { // Update and Draw Lockers
                    for( int32 iLocker = 0; iLocker < AppState->nLocker; iLocker++ ) {
                        LOCKER * Locker = AppState->Locker + iLocker;
                        
                        if( Locker->DoSpring ) {
                            UpdateSpring( &Locker->PosForSpring, &Locker->VelForSpring, Locker->Position, 24.0f, dT );
                            
                            flo32 Radius   = 10.0f;
                            flo32 RadiusSq = Radius * Radius;
                            flo32 DistSq   = GetLengthSq( Locker->PosForSpring - Locker->Position );
                            if( DistSq <= RadiusSq ) {
                                Locker->DoSpring = false;
                            }
                        }
                    }
                    
                    int32 iHover = -1;
                    for( int32 iLocker = 0; iLocker < AppState->nLocker; iLocker++ ) {
                        LOCKER * Locker = AppState->Locker + iLocker;
                        
                        rect Bound = RectCD( Locker->Position, LOCKER_DIM );
                        if( IsInRect( MouseP, Bound ) ) {
                            iHover = iLocker;
                        }
                    }
                    
                    if( AppState->ToMove_iLocker >= 0 ) {
                        iHover = -1;
                    }
                    if( AppState->ToStore_IsSelected ) {
                        iHover = -1;
                    }
                    if( AppState->SecondaryObjective_IsSelected ) {
                        iHover = -1;
                    }
                    if( ( AppState->Tutorial_IsActive ) && ( AppState->Tutorial_PauseForFirstQuestion ) ) {
                        iHover = -1;
                    }
                    if( AppState->Health <= 0.0f ) {
                        iHover = -1;
                    }
                    
                    if( iHover >= 0 ) {
                        if( WasPressed( Mouse, MouseButton_Left ) ) {
                            LOCKER Locker = AppState->Locker[ iHover ];
                            
                            for( int32 iLocker = iHover; iLocker < ( AppState->nLocker - 1 ); iLocker++ ) {
                                AppState->Locker[ iLocker ] = AppState->Locker[ iLocker + 1 ];
                            }
                            AppState->Locker[ AppState->nLocker - 1 ] = Locker;
                            
                            AppState->ToMove_iLocker = AppState->nLocker - 1;
                            AppState->ToMove_Offset  = Locker.Position - MouseP;
                            
                            iHover = -1;
                        }
                    }
                    
                    if( ( AppState->ToMove_iLocker >= 0 ) && ( WasReleased( Mouse, MouseButton_Left ) ) ) {
                        vec2 Dest = MouseP + AppState->ToMove_Offset;
                        if( IsInRect( Dest, Storage_AdjBound ) ) {
                            LOCKER * Locker = AppState->Locker + AppState->ToMove_iLocker;
                            Locker->Position = Dest;
                            
                            PlaySound( AppState, SoundID_Item_Drop );
                        } else if( ( AppState->ToFind  >= 0 ) && ( IsInRect( Dest, ToFind_AdjReturn ) ) ) {
                            LOCKER * Locker = AppState->Locker + AppState->ToMove_iLocker;
                            if( Locker->ItemID == AppState->ToFind ) {
                                AppState->Correct_Timer    = LOCKER_CORRECT_TARGET_TIME;
                                AppState->Correct_ItemID   = Locker->ItemID;
                                AppState->Correct_Position = Dest;
                                
                                LogItemToDebug( true, Locker->ItemID );
                                
                                AppState->Light_Timer     = LIGHT_TARGET_TIME;
                                AppState->Light_IsCorrect = true;
                                
                                int32 Points = 1;
                                int32 Health = 0;
                                
                                boo32 DoHealth   = false;
                                boo32 DoOverflow = false;
                                
                                if( ( AppState->SecondaryObjective == SecondaryObjective_QuickTimer ) && ( AppState->SecondaryObjective_Timer > 0.0f ) ) {
                                    if( AppState->Health < AppState->MaxHealth ) {
                                        Health += 1;
                                        
                                        AddScoreUpdate( AppState, ScoreUpdate_OnePoint_OneHealth );
                                        PlaySound( AppState, SoundID_AddPoints );
                                        PlaySound( AppState, SoundID_Health_Up );
                                        
                                        DoHealth   = true;
                                    } else {
                                        Points += 1;
                                        
                                        AddScoreUpdate( AppState, ScoreUpdate_TwoPoints );
                                        PlaySound( AppState, SoundID_AddBonusPoints );
                                        
                                        DoHealth   = true;
                                        DoOverflow = true;
                                    }
                                } else if( ( AppState->SecondaryObjective == SecondaryObjective_ExtraItem ) && ( AppState->nExtraLocker >= 1 ) ) {
                                    if( AppState->SecondaryObjective_DoTwoBonusPoints ) {
                                        if( AppState->Health >= AppState->MaxHealth ) {
                                            Points += 2;
                                            
                                            AddScoreUpdate( AppState, ScoreUpdate_ThreePoints );
                                            PlaySound( AppState, SoundID_AddPoints );
                                            PlaySound( AppState, SoundID_Health_Up );
                                            
                                            DoHealth   = true;
                                            DoOverflow = true;
                                        } else if( AppState->Health >= ( AppState->MaxHealth - 1 ) ) {
                                            Points += 1;
                                            Health += 1;
                                            
                                            AddScoreUpdate( AppState, ScoreUpdate_TwoPoints_OneHealth );
                                            PlaySound( AppState, SoundID_AddBonusPoints );
                                            PlaySound( AppState, SoundID_Health_Up );
                                            
                                            DoHealth   = true;
                                            DoOverflow = true;
                                        } else {
                                            Health += 2;
                                            
                                            AddScoreUpdate( AppState, ScoreUpdate_OnePoint_TwoHealth );
                                            PlaySound( AppState, SoundID_Health_Up );
                                            
                                            DoHealth   = true;
                                        }
                                    } else {
                                        if( AppState->Health >= AppState->MaxHealth ) {
                                            Points += 1;
                                            
                                            AddScoreUpdate( AppState, ScoreUpdate_TwoPoints );
                                            PlaySound( AppState, SoundID_AddBonusPoints );
                                            
                                            DoHealth   = true;
                                            DoOverflow = true;
                                        } else {
                                            Health += 1;
                                            
                                            AddScoreUpdate( AppState, ScoreUpdate_OnePoint_OneHealth );
                                            PlaySound( AppState, SoundID_AddPoints );
                                            PlaySound( AppState, SoundID_Health_Up );
                                            
                                            DoHealth   = true;
                                        }
                                    }
                                } else {
                                    AddScoreUpdate( AppState, ScoreUpdate_OnePoint );
                                    PlaySound( AppState, SoundID_AddPoints );
                                }
                                
                                AppState->Wire_Score_Timer      = WIRE_LIGHT_UP_TARGET_TIME;
                                AppState->Wire_Score_DoScore    = true;
                                AppState->Wire_Score_DoHealth   = DoHealth;
                                AppState->Wire_Score_DoOverflow = DoOverflow;
                                AppState->Wire_Score_DoBonus    = false;
                                
                                AppState->Score                           += Points;
                                AppState->Health                          += Health;
                                AppState->nCorrectItemsReturned_Total     += 1;
                                AppState->nCorrectItemsReturned_ThisLevel += 1;
                                
                                if( AppState->nCorrectItemsReturned_ThisLevel >= AppState->MinimumItemsUp_ItemsPerLevel ) {
                                    PlaySound( AppState, SoundID_ItemsUp );
                                    
                                    AppState->nCorrectItemsReturned_ThisLevel = 0;
                                    AppState->nExtraLocker                    = 0;
                                    AppState->SecondaryObjective_ItemID0      = -1;
                                    
                                    AppState->nItemsUp_Timer = N_ITEMS_UP_TARGET_TIME;
                                    
                                    if( ( AppState->MinimumItemsUp_IsActive ) && ( AppState->nMinimumItems < GAME_MAX_LEVEL ) ) {
                                        AppState->nMinimumItems += 1;
                                    }
                                    
                                    if( AppState->Tutorial_IsActive ) {
                                        if( AppState->MinimumItemsUp_ItemsPerLevel == TUTORIAL_ITEMS_PER_LEVEL ) {
                                            AppState->MinimumItemsUp_ItemsPerLevel = GAME_ITEMS_PER_LEVEL;
                                        }
                                        
                                        if( ( AppState->nMinimumItems >= 4 ) && ( AppState->SecondaryObjective == SecondaryObjective_WhichInSet ) ) {
                                            AppState->SecondaryObjective = SecondaryObjective_QuickTimer;
                                            AppState->Tutorial_IsActive  = false;
                                            
                                            SAVE_GAME * SaveGame = &AppState->SaveGame;
                                            SaveGame->Tutorial_IsComplete = true;
                                        }
                                    } else {
                                        AppState->SecondaryObjective = GetNextSecondaryObjective( AppState );
                                    }
                                }
                                
                                if( ( AppState->SecondaryObjective == SecondaryObjective_WhichInSet ) || ( AppState->SecondaryObjective == SecondaryObjective_YesNoInSet ) ) {
                                    AppState->SecondaryObjective_ItemID0 = -1;
                                    AppState->SecondaryObjective_ItemID1 = -1;
                                    AppState->SecondaryObjective_Alpha   = 0.0f;
                                }
                                
                                AppState->nLocker -= 1;
                                for( int32 iLocker = AppState->ToMove_iLocker; iLocker < AppState->nLocker; iLocker++ ) {
                                    AppState->Locker[ iLocker ] = AppState->Locker[ iLocker + 1 ];
                                }
                                
                                AppState->LastItemFound           = AppState->ToFind;
                                AppState->Item[ AppState->nItem ] = AppState->ToFind;
                                AppState->nItem += 1;
                                
                                AppState->ToFind = -1;
                                AppState->SecondaryObjective_Timer = 0.0f;
                                
                                int32 nToTake = AppState->nMinimumItems - ( AppState->nLocker - AppState->nExtraLocker );
                                if( nToTake > 0 ) {
                                    GetItemToStore( AppState );
                                } else {
                                    GetItemToFind( TempMemory, AppState );
                                }
                                
                                AppState->Turn_Timer = TURN_TARGET_TIME;
                            } else {
                                Locker->ShowItem_Timer  = LOCKER_SHOW_ITEM_TARGET_TIME;
                                Locker->DoSpring     = true;
                                Locker->PosForSpring = MouseP;
                                Locker->VelForSpring = {};
                                
                                AppState->Light_Timer     = LIGHT_TARGET_TIME;
                                AppState->Light_IsCorrect = false;
                                
                                AppState->nWrongItemsReturned += 1;
                                AppState->Health = MaxValue( AppState->Health - 1, 0 );
                                AddScoreUpdate( AppState, ScoreUpdate_LoseHealth );
                                
                                PlaySound( AppState, SoundID_Health_Down );
                                
                                AppState->SecondaryObjective_DoTwoBonusPoints = false;
                                if( AppState->SecondaryObjective != SecondaryObjective_ExtraItem ) {
                                    AppState->SecondaryObjective_Timer   = 0.0f;
                                    AppState->SecondaryObjective_ItemID0 = -1;
                                    AppState->SecondaryObjective_ItemID1 = -1;
                                    AppState->SecondaryObjective_Alpha   = 0.0f;
                                }
                            }
                        }
                        
                        AppState->ToMove_iLocker = -1;
                    }
                    
                    for( int32 iLocker = 0; iLocker < AppState->nLocker; iLocker++ ) {
                        LOCKER Locker = AppState->Locker[ iLocker ];
                        
                        rect Bound = RectCD( Locker.Position, LOCKER_DIM );
                        
                        flo32 Alpha = 1.0f;
                        if( iLocker == AppState->ToMove_iLocker ) {
                            Alpha = 0.35f;
                        }
                        if( Locker.DoSpring ) {
                            Alpha = 0.35f;
                        }
                        
                        if( Alpha >= 1.0f ) {
                            DrawRect( Pass_Game, TextureID_Shadow, AddRadius( Bound, 6.0f ), COLOR_WHITE * 0.5f );
                        }
                        
                        if( Locker.ShowItem_Timer > 0.0f ) {
                            DrawRect( Pass_Game, TextureID_CardboardBox_Empty, Bound, CARDBOARD_EMPTY_COLOR * Alpha );
                            DrawLockerContents( Pass_Game, AppState->FontList + FontID_1_00, GetCenter( Bound ), Locker.ItemID, Alpha );
                        } else {
                            DrawRect( Pass_Game, TextureID_CardboardBox_Full, Bound, CARDBOARD_COLOR * Alpha );
                        }
                        
                        if( Locker.DoSpring ) {
                            Bound = RectCD( Locker.PosForSpring, LOCKER_DIM );
                            Alpha = 1.0f;
                            
                            if( Locker.ShowItem_Timer > 0.0f ) {
                                DrawRect( Pass_Game, TextureID_CardboardBox_Empty, Bound, CARDBOARD_EMPTY_COLOR * Alpha );
                                DrawLockerContents( Pass_Game, AppState->FontList + FontID_1_00, GetCenter( Bound ), Locker.ItemID, Alpha );
                            } else {
                                DrawRect( Pass_Game, TextureID_CardboardBox_Full, Bound, CARDBOARD_COLOR * Alpha );
                            }
                        }
                    }
                    
                    // Draw Hover
                    if( iHover >= 0 ) {
                        LOCKER Locker = AppState->Locker[ iHover ];
                        rect   Bound  = RectCD( Locker.Position, LOCKER_DIM );
                        DrawRect( Pass_Game, TextureID_CardboardBox_Hover, Bound, TowardWhite( CARDBOARD_COLOR, 0.25f ) );
                    }
                    
                    // NOTE : TEMP!
                    {
                        for( int32 iLocker = 0; iLocker < AppState->nLocker; iLocker++ ) {
                            LOCKER Locker = AppState->Locker[ iLocker ];
                            rect   Bound  = RectCD( Locker.Position, LOCKER_DIM );
                            
                            if( ( IsDown( Mouse, MouseButton_Right ) ) && ( IsInRect( Mouse->Position, Bound ) ) ) {
                                DrawLockerContents( Pass_Game, 0, GetCenter( Bound ), Locker.ItemID );
                            }
                        }
                    }
                }
                
                { //: ToStore
                    if( ( AppState->ToStore >= 0 ) && ( AppState->Health > 0 ) ) {
                        flo32 dLight = 1.0f / 0.25f;
                        AppState->ToStore_tLight = MinValue( AppState->ToStore_tLight + dLight * dT, 1.0f );
                    } else {
                        flo32 dLight = 1.0f / 0.5f;
                        AppState->ToStore_tLight = MaxValue( AppState->ToStore_tLight - dLight * dT, 0.0f );
                    }
                    if( ( AppState->Tutorial_IsActive ) && ( AppState->Tutorial_PauseForFirstQuestion ) ) {
                        flo32 dLight = 1.0f / 0.5f;
                        AppState->ToStore_tLight = MaxValue( AppState->ToStore_tLight - dLight * dT, 0.0f );
                    }
                    
                    { // ToStore Tray
                        vec2 Dim   = Vec2( 180.0f, 340.0f );
                        rect Bound = RectBCD( GetBC( ToStore_Bound ), Dim );
                        
                        Bound.Bottom = Bound.Top - GetWidth( Bound ) * 1.0f;
                        
                        DrawRect( Pass_Game, TextureID_ToStore_Tray, Bound, FENCE_COLOR );
                        
                        orect Arrow = ORectCD( GetTC( Bound ) + Vec2( 0.0f, -20.0f ), Vec2( 40.0f, 80.0f ), -PI * 0.5f );
                        DrawORect( Pass_Game, TextureID_ToStore_Arrow, Arrow, TowardBlack( FENCE_COLOR, 0.5f ) );
                    }
                    
                    { // Spotlight
                        flo32 dRadians = TAU / 8.0f;
                        AppState->ToStore_Spotlight_Radians = WrapRadians( AppState->ToStore_Spotlight_Radians + dRadians * dT );
                        flo32 tRadians0 = Clamp( cosf( AppState->ToStore_Spotlight_Radians             ) * 1.1f, -1.0f, 1.0f );
                        flo32 tRadians1 = Clamp( cosf( AppState->ToStore_Spotlight_Radians + PI * 0.2f ) * 1.1f, -1.0f, 1.0f );
                        flo32 tRadians2 = Clamp( cosf( AppState->ToStore_Spotlight_Radians + PI * 0.45f ) * 1.1f, -1.0f, 1.0f );
                        
                        flo32 Alpha = lerp( 0.0f, AppState->ToStore_tLight, 0.35f );
                        vec2  Dim   = Vec2( GetWidth( ToStore_Bound ) * 0.65f, GetHeight( ToStore_Bound ) * 0.75f );
                        vec4  Color = lerp( COLOR_BLACK, AppState->ToStore_tLight, SPOTLIGHT_COLOR ) * Alpha;
                        //DrawORect( Pass_Game, TextureID_InsideTunnel, ORectTD( GetBC( ToStore_Bound ) + Vec2( 0.0f, -40.0f ), Vec2( 0.5f, 1.0f ), Dim, PI ), Color );
                        DrawORect( Pass_Game, TextureID_Spotlight, ORectTD( GetBC( ToStore_Bound ) + Vec2( 0.0f, -120.0f ), Vec2( 0.5f, 0.0f ), Dim, PI * 0.01f * tRadians2 ), Color );
                        DrawORect( Pass_Game, TextureID_Spotlight, ORectTD( GetBC( ToStore_Bound ) + Vec2( 0.0f, -120.0f ), Vec2( 0.5f, 0.0f ), Dim * Vec2( 1.0f, 0.8f ),  PI * 0.02f * tRadians0 ), Color );
                        DrawORect( Pass_Game, TextureID_Spotlight, ORectTD( GetBC( ToStore_Bound ) + Vec2( 0.0f, -120.0f ), Vec2( 0.5f, 0.0f ), Dim * Vec2( 1.0f, 0.8f ), -PI * 0.02f * tRadians1 ), Color );
                    }
                    
                    if( AppState->ToStore >= 0 ) {
                        flo32 Friction = 2.5f;
                        vec2  Accel    = AppState->ToStore_Velocity * -Friction;
                        
                        vec2  dPos = Accel * ( 0.5f * dT * dT ) + AppState->ToStore_Velocity * dT;
                        AppState->ToStore_Position += dPos;
                        AppState->ToStore_Velocity += Accel * dT;
                        
                        flo32 dRadians = TAU / 3.0f;
                        AppState->ToStore_Radians = WrapRadians( AppState->ToStore_Radians + dRadians * dT );
                        
                        flo32 yOffset = sinf( AppState->ToStore_Radians ) * 8.0f;
                        vec2  Pos     = AppState->ToStore_Position + Vec2( 0.0f, yOffset );
                        rect Bound = RectCD( Pos, ITEM_DIM );
                        vec2 Dim   = GetDim( Bound );
                        
                        if( !AppState->ToStore_IsSelected ) {
                            rect AdjBound = ToStore_Bound;
                            AdjBound.Left   += 20.0f;
                            AdjBound.Right  -= 20.0f;
                            AdjBound.Bottom += 20.0f;
                            AdjBound.Top    -= 340.0f;
                            //DrawRectOutline( Pass_Game, AdjBound, COLOR_RED );
                            
                            if( UpdateAndDrawItemGrabButton( Pass_Game, Mouse, AppState->ToStore, RectCD( GetCenter( Bound ), Dim ), AdjBound, 1.0f, ( AppState->Health > 0 ) ) ) {
                                AppState->ToStore_IsSelected = true;
                            }
                        }
                    }
                }
                
                { // Draw ToStore Item When Selected
                    if( AppState->ToStore_IsSelected ) {
                        flo32 Alpha = 0.65f;
                        
                        if( IsInRect( MouseP, Storage_AdjBound ) ) {
                            Alpha = 1.0f;
                        }
                        
                        rect Bound = RectCD( MouseP, LOCKER_DIM );
                        DrawRect( Pass_Game, TextureID_CardboardBox_Empty, Bound, CARDBOARD_EMPTY_COLOR * Alpha );
                        DrawLockerContents( Pass_Game, AddRadius( Bound, -20.0f ), AppState->ToStore, Alpha );
                    }
                }
                
                { // Draw Extra Item When Selected
                    if( AppState->SecondaryObjective_IsSelected ) {
                        flo32 Alpha = 0.65f;
                        if( IsInRect( MouseP, Storage_AdjBound ) ) {
                            Alpha = 1.0f;
                        }
                        
                        rect Bound = RectCD( MouseP, LOCKER_DIM );
                        DrawRect( Pass_Game, TextureID_CardboardBox_Empty, Bound, CARDBOARD_EMPTY_COLOR * Alpha );
                        DrawLockerContents( Pass_Game, AddRadius( Bound, -20.0f ), AppState->SecondaryObjective_ItemID0, Alpha );
                    }
                }
                
                { // Draw ToMove Item When Active
                    if( AppState->ToMove_iLocker >= 0 ) {
                        vec2  Dest  = MouseP + AppState->ToMove_Offset;
                        flo32 Alpha = 0.5f;
                        if( IsInRect( Dest, Storage_AdjBound ) ) {
                            Alpha = 0.85f;
                        }
                        if( ( AppState->ToFind >= 0 ) && ( IsInRect( Dest, ToFind_AdjReturn ) ) ) {
                            Alpha = 0.85f;
                        }
                        DrawRect( Pass_Game, TextureID_CardboardBox_Full, RectCD( Dest, LOCKER_DIM ), CARDBOARD_COLOR * Alpha );
                    }
                }
                
                { // Finalize Turn Timer
                    if( ( AppState->Health > 0 ) && ( AppState->Turn_IsActive ) && ( !AppState->Tutorial_PauseForFirstQuestion ) ) {
                        AppState->Turn_Timer = MaxValue( AppState->Turn_Timer - dT, 0.0f );
                        if( AppState->Turn_Timer <= 0.0f ) {
                            AppState->Health     = MaxValue( AppState->Health - 1, 0 );
                            AddScoreUpdate( AppState, ScoreUpdate_LoseHealth );
                            
                            AppState->SecondaryObjective_DoTwoBonusPoints = false;
                            
                            PlaySound( AppState, SoundID_Health_Down );
                            
                            if( AppState->Health > 0 ) {
                                AppState->Turn_Timer = TURN_TARGET_TIME;
                            }
                        }
                    }
                    
                    if( ( AppState->Health <= 0 ) && ( !AppState->EndGame ) ) {
                        PlaySound( AppState, SoundID_LoseGame );
                        
                        AppState->EndGame = true;
                        
                        if( AppState->ToFind >= 0 ) {
                            int32 iCorrect = -1;
                            for( int32 iLocker = 0; iLocker < AppState->nLocker; iLocker++ ) {
                                LOCKER * Locker = AppState->Locker + iLocker;
                                Locker->ShowItem_Timer = LOCKER_SHOW_ITEM_TARGET_TIME;
                                
                                if( Locker->ItemID == AppState->ToFind )  {
                                    iCorrect = iLocker;
                                }
                            }
                            Assert( iCorrect >= 0 );
                            
                            LOCKER Locker = AppState->Locker[ iCorrect ];
                            for( int32 iLocker = iCorrect; iLocker < ( AppState->nLocker - 1 ); iLocker++ ) {
                                AppState->Locker[ iLocker ] = AppState->Locker[ iLocker + 1 ];
                            }
                            AppState->Locker[ AppState->nLocker - 1 ] = Locker;
                        }
                        
                        AppState->ToStore_IsSelected            = false;
                        AppState->ToMove_iLocker                = -1;
                        AppState->SecondaryObjective_IsSelected = false;
                        
                        AppState->Background_dZoom = ( 1.0f / 3.5f );
                    }
                }
                
                { //: Score, Health, Menu
                    flo32 FromSides = 48.0f;
                    //rect  Panel     = ToFind_Return;
                    //Panel.Left  += FromSides;
                    //Panel.Right -= FromSides;
                    //Panel.Top    = UI_Bound.Top - 48.0f;
                    //Panel.Bottom = Panel.Top - 100.0f;
                    
                    rect Panel = ScorePanel_Display;
                    DrawRect( Pass_Game, AddRadius( Panel, 8.0f + 2.0f ), TowardBlack( DISPLAY_BORDER_COLOR, 0.5f ) );
                    DrawRect( Pass_Game, AddRadius( Panel, 8.0f ), DISPLAY_BORDER_COLOR );
                    DrawRect( Pass_Game, Panel, COLOR_BLACK );
                    
                    flo32 Left   = Panel.Left  + 20.0f;
                    flo32 Right  = Panel.Right - 48.0f;
                    
                    FONT * Font = AppState->FontList + FontID_2_25;
                    flo32 tItemsUp      = Clamp01( AppState->nItemsUp_Timer / ( N_ITEMS_UP_TARGET_TIME - ( 40.0f / 60.0f ) ) );
                    vec4  ItemsUp_Color = lerp( TEXT_COLOR, tItemsUp, BONUS_POINTS_COLOR );
                    DrawLabelS32( Pass_Game, Font, "Memories", AppState->nMinimumItems, Left, Right, Panel.Top - 13.0f, TEXT_COLOR, ItemsUp_Color );
                    
                    flo32 tScore = 0.0f;
                    if( ( AppState->Wire_Score_DoScore ) || ( AppState->Wire_Score_DoOverflow ) ) {
                        tScore = Clamp01( AppState->Wire_Score_Timer / ( WIRE_LIGHT_UP_TARGET_TIME - ( 40.0f / 60.0f ) ) );
                    }
                    vec4  Score_Color = lerp( TEXT_COLOR, tScore, BONUS_POINTS_COLOR );
                    DrawLabelS32( Pass_Game, Font, "Score", AppState->Score, Left, Right, Panel.Top - 53.0f, TEXT_COLOR, Score_Color );
                    if( AppState->nExtraLocker > 0 ) {
                        DrawString( Pass_Game, AppState->FontList + FontID_2_00, "+1", GetTR( Panel ) + Vec2( -13.0f, -13.0f ), TextAlign_TopRight, COLOR_GRAY( 0.65f ) );
                    }
                    
                    rect Health_Bound = Rect( Left, 0.0f, Right, 0.0f );
                    Health_Bound.Top    = Panel.Bottom - 52.0f;
                    Health_Bound.Bottom = Health_Bound.Top - 32.0f;
                    
                    { // Health Bar
                        if( AppState->Health == 1 ) {
                            flo32 dTimer = 1.0f / 1.0f;
                            AppState->LowHealth_tTimer = MaxValue( AppState->LowHealth_tTimer - dTimer * dT, 0.0f );
                            
                            if( AppState->LowHealth_tTimer <= 0.0f ) {
                                AppState->LowHealth_tTimer = 1.0f;
                            }
                        } else {
                            AppState->LowHealth_tTimer = 0.0f;
                        }
                        
                        vec4 Head_Color = TowardBlack( DISPLAY_BORDER_COLOR, 0.5f );
                        if( AppState->Health <= 0 ) {
                            Head_Color = DAMAGE_COLOR;
                        } else if( AppState->Health <= 1 ) {
                            Head_Color = lerp( Head_Color, AppState->LowHealth_tTimer, DAMAGE_COLOR );
                        }
                        
                        boo32 DoColor[ PLAYER_MAX_HEALTH ] = {};
                        for( int32 iHealthUnit = 1; iHealthUnit <= AppState->MaxHealth; iHealthUnit++ ) {
                            int32 Index = iHealthUnit - 1;
                            
                            if( iHealthUnit <= AppState->Health ) {
                                flo32 dTimer = ( 1.0f / 0.25f );
                                AppState->HealthUnit_tTimer[ Index ] = MinValue( AppState->HealthUnit_tTimer[ Index ] + dTimer * dT, 1.0f );
                            } else {
                                flo32 dTimer = ( 1.0f / 0.5f );
                                AppState->HealthUnit_tTimer[ Index ] = MaxValue( AppState->HealthUnit_tTimer[ Index ] - dTimer * dT, 0.0f );
                                DoColor[ Index ] = true;
                            }
                        }
                        
                        rect Bar  = RectTCD( GetBC( Panel ) + Vec2( 28.0f, -48.0f ), Vec2( 160.0f, 32.0f ) );
                        Bar = Health_Display;
                        rect Head = RectRCD( GetLC( Bar ) + Vec2( -6.0f, 0.0f ), Vec2( 52.0f, 52.0f ) );
                        DrawRect( Pass_Game, AddRadius( Head, 6.0f + 2.0f ), TowardBlack( DISPLAY_BORDER_COLOR, 0.5f ) );
                        
                        DrawRect( Pass_Game, AddRadius( Bar, 6.0f + 2.0f ), TowardBlack( DISPLAY_BORDER_COLOR, 0.5f ) );
                        DrawRect( Pass_Game, AddRadius( Bar, 6.0f ), DISPLAY_BORDER_COLOR );
                        DrawRect( Pass_Game, Bar, COLOR_BLACK );
                        
                        flo32 DistBetween = 10.0f;
                        rect  Health      = AddDim( Bar, Vec2( -DistBetween, -7.0f ) );
                        
                        vec2  Unit_Dim = {};
                        Unit_Dim.x  = GetWidth( Health );
                        Unit_Dim.x -= ( DistBetween * ( flo32 )( AppState->MaxHealth - 1 ) );
                        Unit_Dim.x /= ( flo32 )AppState->MaxHealth;
                        Unit_Dim.y  = GetHeight( Health );
                        DrawRect( Pass_Game, AddRadius( Head, 6.0f ), DISPLAY_BORDER_COLOR );
                        DrawRect( Pass_Game, Head, Head_Color );
                        
                        // NOTE: Redraw black to correctly overlap with the head box.
                        DrawRect( Pass_Game, Bar, COLOR_BLACK );
                        
                        DrawRect( Pass_Game, TextureID_Icon_Health, AddRadius( Head, -2.0f ), COLOR_BLACK );
                        DrawRect( Pass_Game, TextureID_Icon_Health, AddRadius( Head, -4.0f ), COLOR_GRAY( 0.95f ) );
                        
                        vec2  AtPos   = GetBL( Health );
                        for( int32 iHealthUnit = 1; iHealthUnit <= AppState->MaxHealth; iHealthUnit++ ) {
                            int32 Index  = iHealthUnit - 1;
                            flo32 tTimer = AppState->HealthUnit_tTimer[ Index ];
                            vec2  Dim    = {};
                            flo32 Alpha  = 1.0f;
                            vec4  Color  = COLOR_RED;
                            if( tTimer >= 0.8f ) {
                                flo32 t = ( ( tTimer - 0.8f ) / ( 1.0f - 0.8f ) );
                                Dim   = Unit_Dim * lerp( 1.3f, t, 1.0f );
                                
                                if( DoColor[ Index ] ) {
                                    Color = lerp( COLOR_WHITE, t, Color );
                                }
                            } else {
                                flo32 t = ( tTimer / 0.8f );
                                Dim   = Unit_Dim * lerp( 0.5f, tTimer, 1.4f );
                                Alpha = lerp( 0.0f, t, 1.0f );
                                
                                if( DoColor[ Index ] ) {
                                    Color = lerp( TowardBlack( COLOR_RED, 0.5f ), t * t, COLOR_WHITE );
                                }
                            }
                            rect  Bound = RectBLD( AtPos, Unit_Dim );
                            DrawRect( Pass_Game, RectCD( GetCenter( Bound ), Dim ), Color * Alpha );
                            
                            AtPos.x += ( Unit_Dim.x + DistBetween );
                        }
                    }
                    
#if 0                    
                    vec2  nMinimum_Pos = Vec2( lerp( Left, 0.5f, ToFind_Return.Right - FromSides ), Health_Bound.Bottom - 60.0f );
                    
                    DrawString( Pass_Game, AppState->FontList + FontID_2_00, "Memories", nMinimum_Pos, TextAlign_BottomCenter, TEXT_COLOR );
                    
                    char Str[ 16 ] = {};
                    sprintf( Str, "%u", AppState->nMinimumItems );
                    DrawString( Pass_Game, AppState->FontList + FontID_3_00, Str, nMinimum_Pos, TextAlign_TopCenter, TEXT_COLOR );
                    
#endif
                    
                    DISPLAY_VALUE( int32, AppState->nCorrectItemsReturned_Total );
                    DISPLAY_VALUE( int32, AppState->nWrongItemsReturned   );
                    //DrawLabelColonS32( Pass_Game, Font, "Items Returned", AppState->nCorrectItemsReturned, Vec2( 1380.0f, 1000.0f ), 1680.0f, 1780.0f, COLOR_WHITE );
                    //DrawLabelColonS32( Pass_Game, Font, "Mistakes", AppState->nWrongItemsReturned, Vec2( 1380.0f, 960.0f ), 1680.0f, 1780.0f, COLOR_WHITE );
                    
                    { //: Pause Menu Button
                        boo32 IsActive = ( AppState->Health > 0 );
                        flo32 Alpha    = 1.0f;
                        if( !IsActive ) {
                            Alpha = 0.0f;
                        }
                        
                        rect Menu = RectTLD( GetTL( UI_Bound ) + Vec2( 20.0f, -20.0f ), Vec2( 128.0f, 80.0f ) );
                        if( UpdateAndDrawPauseButton( Pass_Game, Mouse, AppState->FontList + FontID_2_25, "Menu", Menu, IsActive, Alpha, TowardBlack( TEXT_COLOR, 0.2f ) ) ) {
                            PlaySound( AppState, SoundID_Menu_Back );
                            
                            AppState->ShowPauseMenu = true;
                        }
                    }
                    
                    { // Update and Draw Score Update
#if 0
                        int32 iType = ( int32 )( tMouse.x * ( flo32 )ScoreUpdate_Count );
                        DISPLAY_VALUE( int32, iType );
                        if( WasPressed( Mouse, MouseButton_Right ) ) {
                            SCORE_UPDATE_TYPE Type = ( SCORE_UPDATE_TYPE )iType;
                            AddScoreUpdate( AppState, Type );
                        }
#endif
                        for( int32 iScoreUpdate = 0; iScoreUpdate < AppState->nScoreUpdate; iScoreUpdate++ ) {
                            SCORE_UPDATE * ScoreUpdate = AppState->ScoreUpdate + iScoreUpdate;
                            ScoreUpdate->Timer += dT;
                        }
                        
                        flo32 FadeIn_TargetTime  = ( 12.0f / 60.0f );
                        flo32 FadeOut_TargetTime = ( 30.0f / 60.0f );
                        
                        // NOTE : TEMP!
                        //for( int32 iScoreUpdate = 0; iScoreUpdate < AppState->nScoreUpdate; iScoreUpdate++ ) {
                        for( int32 iScoreUpdate = 0; iScoreUpdate < 0; iScoreUpdate++ ) {
                            SCORE_UPDATE ScoreUpdate = AppState->ScoreUpdate[ iScoreUpdate ];
                            if( ScoreUpdate.Timer > 0.0f ) {
                                flo32 tTimer = Clamp01( ScoreUpdate.Timer / ScoreUpdate.TargetTime );
                                vec2  Pos    = GetCenter( ScoreUpdate_Display );
                                
                                flo32 Alpha = 1.0f;
                                if( ScoreUpdate.Timer < FadeIn_TargetTime ) {
                                    Alpha = Clamp01( ScoreUpdate.Timer / FadeIn_TargetTime );
                                }
                                if( ScoreUpdate.Timer > ( ScoreUpdate.TargetTime - FadeOut_TargetTime ) ) {
                                    flo32 bTimer = ( ScoreUpdate.TargetTime - FadeOut_TargetTime );
                                    Alpha = 1.0f - Clamp01( ( ScoreUpdate.Timer - bTimer ) / ( FadeOut_TargetTime ) );
                                }
                                
                                switch( ScoreUpdate.Type ) {
                                    case ScoreUpdate_OnePoint: {
                                        DrawString( Pass_Game, AppState->FontList + FontID_3_00, "+1", Pos, TextAlign_Center, TEXT_COLOR * Alpha );
                                    } break;
                                    
                                    case ScoreUpdate_OneBonusPoint: {
                                        DrawString( Pass_Game, AppState->FontList + FontID_3_00, "+1", Pos, TextAlign_Center, BONUS_POINTS_COLOR * Alpha );
                                    } break;
                                    
                                    case ScoreUpdate_TwoPoints: {
                                        DrawString( Pass_Game, AppState->FontList + FontID_3_00, "+2", Pos, TextAlign_Center, BONUS_POINTS_COLOR * Alpha );
                                    } break;
                                    
                                    case ScoreUpdate_ThreePoints: {
                                        DrawString( Pass_Game, AppState->FontList + FontID_3_00, "+3", Pos, TextAlign_Center, BONUS_POINTS_COLOR * Alpha );
                                    } break;
                                    
                                    case ScoreUpdate_OneHealth: {
                                        Pos.x -= 4.0f;
                                        
                                        flo32 FromCenter = 16.0f;
                                        DrawString( Pass_Game, AppState->FontList + FontID_2_50, "+", Pos + Vec2( -FromCenter, 0.0f ), TextAlign_Center, BONUS_HEALTH_COLOR * Alpha );
                                        DrawRect( Pass_Game, TextureID_Icon_Health, RectCD( Pos + Vec2( FromCenter, 0.0f ), Vec2( 40.0f, 40.0f ) ), COLOR_WHITE * Alpha );
                                    } break;
                                    
                                    case ScoreUpdate_OnePoint_OneHealth: {
                                        Pos.x -= 4.0f;
                                        DrawString( Pass_Game, AppState->FontList + FontID_2_25, "+1", Pos + Vec2( 0.0f, 22.0f ), TextAlign_Center, BONUS_POINTS_COLOR * Alpha );
                                        
                                        Pos.y -= 22.0f;
                                        
                                        flo32 FromCenter = 13.0f;
                                        DrawString( Pass_Game, AppState->FontList + FontID_2_25, "+", Pos + Vec2( -FromCenter, 0.0f ), TextAlign_Center, BONUS_HEALTH_COLOR * Alpha );
                                        DrawRect( Pass_Game, TextureID_Icon_Health, RectCD( Pos + Vec2( FromCenter, 0.0f ), Vec2( 32.0f, 32.0f ) ), COLOR_WHITE * Alpha );
                                    } break;
                                    
                                    case ScoreUpdate_OnePoint_TwoHealth: {
                                        Pos.x -= 5.0f;
                                        DrawString( Pass_Game, AppState->FontList + FontID_2_25, "+1", Pos + Vec2( 0.0f, 22.0f ), TextAlign_Center, BONUS_POINTS_COLOR * Alpha );
                                        
                                        Pos.y -= 18.0f;
                                        
                                        flo32 FromCenter = 14.0f;
                                        vec2  Offset     = Vec2( 3.0f, 6.0f );
                                        DrawString( Pass_Game, AppState->FontList + FontID_2_25, "+", Pos + Vec2( -FromCenter, 0.0f ), TextAlign_Center, BONUS_HEALTH_COLOR * Alpha );
                                        DrawRect( Pass_Game, TextureID_Icon_Health, RectCD( Pos + Vec2( FromCenter, -2.0f ) + Offset, Vec2( 28.0f, 28.0f ) ), COLOR_GRAY( 0.8f ) * Alpha );
                                        DrawRect( Pass_Game, TextureID_Icon_Health, RectCD( Pos + Vec2( FromCenter, -2.0f ) - Offset, Vec2( 30.0f, 30.0f ) ), COLOR_GRAY( 0.25f ) * Alpha );
                                        DrawRect( Pass_Game, TextureID_Icon_Health, RectCD( Pos + Vec2( FromCenter, -2.0f ) - Offset, Vec2( 28.0f, 28.0f ) ), COLOR_WHITE * Alpha );
                                    } break;
                                    
                                    case ScoreUpdate_TwoPoints_OneHealth: {
                                        Pos.x -= 4.0f;
                                        DrawString( Pass_Game, AppState->FontList + FontID_2_25, "+2", Pos + Vec2( 0.0f, 22.0f ), TextAlign_Center, BONUS_POINTS_COLOR * Alpha );
                                        
                                        Pos.y -= 22.0f;
                                        
                                        flo32 FromCenter = 13.0f;
                                        DrawString( Pass_Game, AppState->FontList + FontID_2_25, "+", Pos + Vec2( -FromCenter, 0.0f ), TextAlign_Center, BONUS_HEALTH_COLOR * Alpha );
                                        DrawRect( Pass_Game, TextureID_Icon_Health, RectCD( Pos + Vec2( FromCenter, 0.0f ), Vec2( 32.0f, 32.0f ) ), COLOR_WHITE * Alpha );
                                    } break;
                                    
                                    case ScoreUpdate_LoseHealth: {
                                        Pos.x -= 4.0f;
                                        
                                        flo32 FromCenter = 16.0f;
                                        DrawString( Pass_Game, AppState->FontList + FontID_2_50, "-", Pos + Vec2( -FromCenter, 0.0f ), TextAlign_Center, COLOR_RED * Alpha );
                                        DrawRect( Pass_Game, TextureID_Icon_Health, RectCD( Pos + Vec2( FromCenter, 0.0f ), Vec2( 40.0f, 40.0f ) ), COLOR_WHITE * Alpha );
                                    } break;
                                }
                            }
                        }
                        
                        int32 NewCount = 0;
                        for( int32 iScoreUpdate = 0; iScoreUpdate < AppState->nScoreUpdate; iScoreUpdate++ ) {
                            SCORE_UPDATE ScoreUpdate = AppState->ScoreUpdate[ iScoreUpdate ];
                            if( ScoreUpdate.Timer < ScoreUpdate.TargetTime ) {
                                AppState->ScoreUpdate[ NewCount++ ] = ScoreUpdate;
                            }
                        }
                        AppState->nScoreUpdate = NewCount;
                    }
                }
                
#if 0                
                { // Update Dust Particles
                    boo32 IsActive = ( AppState->ToFind >= 0 ) && ( AppState->Health > 0 ) && ( !AppState->Tutorial_PauseForFirstQuestion );
                    
                    flo32 MaxTargetTime = ( 120.0f / 60.0f );
                    if( IsActive ) {
                        AppState->ToFind_Particle_Timer += dT;
                    } else {
                        AppState->ToFind_Particle_Timer  = 0.0f;
                    }
                    AppState->ToFind_Particle_Timer = Clamp( AppState->ToFind_Particle_Timer, 0.0f, MaxTargetTime );
                    AppState->ToFind_nParticle      = ( int32 )( Clamp01( AppState->ToFind_Particle_Timer / MaxTargetTime ) * ( flo32 )TO_FIND_PARTICLE_MAX_COUNT );
                    
                    flo32 MaxDist = 200.0f;
                    for( int32 iParticle = 0; iParticle < TO_FIND_PARTICLE_MAX_COUNT; iParticle++ ) {
                        DUST_PARTICLE * Particle = AppState->ToFind_Particle + iParticle;
                        
                        if( Particle->Timer < Particle->TargetTime ) {
                            rect   Inner = RectCD( GetCenter( ToFind_Return ), Vec2( 60.0f, 90.0f ) );
                            flo32  Dist  = DistPointToRect( Particle->Position, Inner );
                            flo32 tDist  = Clamp01( Dist / MaxDist );
                            
                            flo32 Speed    = lerp( 320.0f, tDist, 200.0f );
                            flo32 Friction = lerp(   8.0f, tDist,   2.0f );
                            
                            vec2  Center = GetCenter( ToFind_Return );
                            vec2  Dir    = GetNormal( Center - Particle->Position );
                            vec2  Accel  = ( Dir * Speed - Particle->Velocity ) * Friction;
                            vec2  dPos   = Accel * ( 0.5f * dT * dT ) + Particle->Velocity * dT;
                            
                            Particle->Position += dPos;
                            Particle->Velocity += Accel * dT;
                            Particle->Timer    += dT;
                        } else if( ( IsActive ) && ( iParticle < AppState->ToFind_nParticle ) ) {
                            vec2  Center = GetCenter( ToFind_Return );
                            vec2  Pos    = GetRandomPointOnPerimeter( AddRadius( ToFind_Return, 20.0f ) );
                            vec2  Dir    = GetNormal( Pos - Center );
                            flo32 Offset = RandomF32InRange( -100.0f, 0.0f );
                            Pos += Dir * Offset;
                            
                            flo32 Radians = ToRadians( -Dir ) + RandomF32InRange( -PI, PI ) * 0.5f;
                            flo32 Speed   = RandomF32InRange( 80.0f, 160.0f );
                            vec2  Vel     = ToDirection2D( Radians ) * Speed;
                            
                            Particle->Position = Pos;
                            Particle->Velocity = Vel;
                            Particle->Timer    = 0.0f;
                            Particle->TargetTime = RandomF32InRange( 1.0f, 1.5f );
                        }
                    }
                }
                
                { // Draw Dust Particles
                    rect Inner = RectCD( GetCenter( ToFind_Return ), Vec2( 60.0f, 90.0f ) );
                    rect Outer = RectCD( GetCenter( ToFind_Return ), Vec2( 240.0f, 320.0f ) );
                    
                    //DrawRectOutline( Pass_Game, Inner, COLOR_RED );
                    //DrawRectOutline( Pass_Game, Outer, COLOR_RED );
                    
                    for( int32 iParticle = 0; iParticle < TO_FIND_PARTICLE_MAX_COUNT; iParticle++ ) {
                        DUST_PARTICLE Particle = AppState->ToFind_Particle[ iParticle ];
                        
                        flo32 Alpha = 1.0f;
                        
                        flo32 FadeIn_TargetTime  = ( 10.0f / 60.0f );
                        flo32 FadeOut_TargetTime = ( 14.0f / 60.0f );
                        if( Particle.Timer <= FadeIn_TargetTime ) {
                            Alpha = Clamp01( Particle.Timer / FadeIn_TargetTime );
                        }
                        flo32 TargetTimeA = ( Particle.TargetTime - FadeOut_TargetTime );
                        if( Particle.Timer >= TargetTimeA ) {
                            Alpha = 1.0f - Clamp01( ( Particle.Timer - TargetTimeA ) / FadeOut_TargetTime );
                        }
                        
                        flo32 Dist  = DistPointToRect( Particle.Position, Inner );
                        flo32 tDist = Clamp01( Dist / 90.0f );
                        
                        vec4 Color  = COLOR_WHITE * Alpha * ( tDist * tDist );
                        
                        flo32 tTimer = GetT01( Particle.Timer, 0.0f, Particle.TargetTime );
                        vec2   Dim   = Vec2( 12.0f, 12.0f ) * lerp( 0.5f, tTimer, 1.0f );
                        
                        rect Bound = RectCD( Particle.Position, Dim );
                        DrawRect( Pass_Game, TextureID_Glow, Bound, Color );
                    }
                }
#endif
                
                { // Draw Tutorial about Storing and Finding Items
                    if( ( AppState->Tutorial_IsActive ) && ( AppState->nMinimumItems <= 2 ) ) {
                        if( ( AppState->ToStore >= 0 ) && ( AppState->nLocker <= 0 ) ) {
                            DrawString( Pass_Game, AppState->FontList + FontID_2_50, "Move the memory into storage.", GetCenter( Question_Bound ), TextAlign_Center, TEXT_COLOR );
                            
                            flo32 tRadians = cosf( AppState->ToStore_Radians * 2.0f ) * 0.5f + 0.5f;
                            flo32 Alpha    = lerp( 0.8f, tRadians, 0.05f );
                            DrawRect( Pass_Game, TextureID_DashedBoxOutline, AddRadius( RectCD( GetCenter( Storage_Bound ), LOCKER_DIM ), 20.0f ), COLOR_WHITE * Alpha );
                        }
                        if( ( AppState->ToFind >= 0 ) && ( AppState->Score <= 0 ) ) {
                            DrawString( Pass_Game, AppState->FontList + FontID_2_50, "Find the memory that matches the picture.", GetCenter( Question_Bound ), TextAlign_Center, TEXT_COLOR );
                        }
                    }
                }
                
                { // Draw Tutorial about Secondary Objectives
                    if( AppState->Tutorial_PauseForFirstQuestion ) {
                        rect Panel = Storage_Bound;
                        Panel.Left   += 200.0f;
                        Panel.Right  -= 200.0f;
                        Panel.Top    -= 40.0f;
                        Panel.Bottom += 240.0f;
                        
                        vec4 Outline_Color    = COLOR_GRAY( 0.075f );
                        vec4 Background_Color = COLOR_GRAY( 0.05f );
                        
                        DrawRect( Pass_Game, Panel, Outline_Color );
                        DrawRect( Pass_Game, AddRadius( Panel, -8.0f ), Background_Color );
                        
                        FONT * Font = AppState->FontList + FontID_2_50;
                        
                        vec2 bPos = GetCenter( Panel ) + Vec2( -20.0f, 92.0f );
                        
                        flo32 Between = 48.0f;
                        DrawString( Pass_Game, Font, "Complete Bonuses to:", bPos + Vec2( -20.0f, Between ), TextAlign_Center, TEXT_COLOR );
                        DrawString( Pass_Game, Font, "1. recover Brain Power", bPos + Vec2( -188.0f, 0.0f ), TextAlign_CenterLeft, TEXT_COLOR );
                        DrawString( Pass_Game, Font, "2. get Bonus Points.", bPos + Vec2( -188.0f, -Between ), TextAlign_CenterLeft, TEXT_COLOR );
                        
                        DrawRect( Pass_Game, TextureID_Icon_Health, RectCD( bPos + Vec2( 256.0f, 4.0f ), Vec2( 40.0f, 40.0f ) ), COLOR_WHITE );
                        
                        DrawString( Pass_Game, AppState->FontList + FontID_2_00, "Bonuses are OPTIONAL.", bPos + Vec2( 0.0f, -128.0f ), TextAlign_Center, BONUS_POINTS_COLOR );
                        DrawString( Pass_Game, AppState->FontList + FontID_1_50, "(except this first one)", bPos + Vec2( 0.0f, -160.0f ), TextAlign_Center, COLOR_GRAY( 0.5f ) );
                        
                        DrawString( Pass_Game, AppState->FontList + FontID_2_00, "Answer the bonus question to continue.", Vec2( GetBC( Panel ).x, Panel.Bottom + 104.0f ), TextAlign_Center, TEXT_COLOR );
                        
#if 0
                        {
                            int32 nArrow = 4;
                            flo32 AdvanceX = GetWidth( Panel ) / ( flo32 )nArrow;
                            
                            nArrow = 3;
                            vec2  AtPos = GetTL( Panel ) + Vec2( AdvanceX, -AdvanceX * 0.35f );
                            
                            vec4 Color = COLOR_WHITE;
                            vec2 Dim   = Vec2( 40.0f, 40.0f );
                            
                            for( int32 Iter = 0; Iter < nArrow; Iter++ ) {
                                DrawRect( Pass_Game, TextureID_Arrow, RectCD( AtPos, Dim ), Color );
                                AtPos.x += AdvanceX;
                            }
                        }
#endif
                    }
                }
                
                { // Update End Game
                    if( AppState->EndGame ) {
                        rect Button = ToFind_Return;
                        Button.Bottom += 140.0f;
                        Button.Top     = Button.Bottom + 120.0f;
                        Button.Left   += 90.0f;
                        Button.Right  -= 90.0f;
                        
                        FONT * Font = AppState->FontList + FontID_3_00;
                        
                        if( UpdateAndDrawMenuButton( Pass_Game, Mouse, Font, "FINISH", Button ) ) {
                            UpdateHighScoreAndSave( Platform, AppState );
                            
                            PlaySound( AppState, SoundID_Menu_Confirm );
                            
                            AppState->TitleMenu_Delay = 0.0f;
                            { // TRANSITION
                                ModeTransition->Type              = ModeTransitionType_Exit;
                                ModeTransition->Timer             = dT;
                                ModeTransition->TargetTime        = ( 30.0f / 60.0f );
                                ModeTransition->ToMode            = AppMode_TitleMenu;
                                ModeTransition->ToMode_DoEnter    = true;
                                ModeTransition->ToMode_TargetTime = ( 50.0f / 60.0f );
                                ModeTransition->FromMode          = AppMode_Game;
                                
                            }
                        }
                    }
                }
            }
        } break;
        
        case AppMode_TitleMenu: {
            boo32 ButtonsAreActive = ( ( !ModeTransition_IsEntering ) && ( !ModeTransition_IsExiting ) );
            
            if( ModeTransition_IsEntering ) {
                if( ModeTransition->FromMode == AppMode_Game ) {
                    flo32 tTimer = GetT01( ModeTransition->Timer, AppState->TitleMenu_Delay, ModeTransition->TargetTime );
                    Layer_Alpha  = tTimer;
                    Layer_Dim   *= lerp( 0.9f, CubeValue( tTimer - 1.0f ) + 1.0f, 1.0f );
                    
                    if( ModeTransition->Timer >= ( ModeTransition->TargetTime - ( 20.0f / 60.0f ) ) ) {
                        ButtonsAreActive = true;
                    }
                } else {
                    Layer_Alpha  = 1.0f;
                }
            }
            if( ModeTransition_IsExiting ) {
                flo32 tTimer = GetT01( ModeTransition->Timer, 0.0f, ModeTransition->TargetTime );
                if( ModeTransition->ToMode == AppMode_Game ) {
                    Layer_Alpha  = 1.0f - tTimer;
                    Layer_Dim   *= lerp( 1.0f, CubeValue( tTimer ), 0.95f );
                }
            }
#if 0            
            flo32 tX = GetT01( Mouse->Position, AppState->UI_Bound ).x;
            int32 iItem = ( int32 )( tX * ( flo32 )ITEM_MAX_COUNT );
            DrawLockerContents( Pass_Game, RectBLD( GetBL( AppState->UI_Bound ) + Vec2( 10.0f, 200.0f ), Vec2( 100.0f, 100.0f ) ), iItem );
            LogItemToDebug( iItem );
#endif
            
            SAVE_GAME * SaveGame = &AppState->SaveGame;
            
#if 0            
            { // NOTE : TEMP!
                for( int32 iHighScore = 0; iHighScore < HIGH_SCORE_MAX_COUNT; iHighScore++ ) {
                    HIGH_SCORE HighScore = {};
                    HighScore.Score  = 100;
                    HighScore.nItems = 10;
                    
                    SaveGame->FullGame_HighScore[ iHighScore ] = HighScore;
                }
                
                for( int32 iPractice = 0; iPractice < 10; iPractice++ ) {
                    SaveGame->Practice_HighScore[ iPractice ] = 100;
                }
                
            }
#endif
            
            boo32 FullGame_IsAvailable = false;
            boo32 Practice_IsAvailable = false;
            
            {
                HIGH_SCORE HighScore = SaveGame->FullGame_HighScore[ 0 ];
                if( HighScore.Score > 0 ) {
                    FullGame_IsAvailable = true;
                }
                
                for( int32 iPractice = 0; ( !Practice_IsAvailable ) && ( iPractice < PRACTICE_MAX_LEVEL ); iPractice++ ) {
                    if( SaveGame->Practice_HighScore[ iPractice ] > 0 ) {
                        Practice_IsAvailable = true;
                    }
                }
            }
            
            
#if 0            
            if( AppState->Score > SaveGame->FullGame_HighScore ) {
                Font = AppState->FontList + FontID_6_00;
                DrawString( Pass_Game, Font, "NEW HIGH SCORE", Vec2( CenterX, AtY ), TextAlign_TopCenter, COLOR_YELLOW );
            }
#endif
            
            if( ( FullGame_IsAvailable ) || ( Practice_IsAvailable ) ) {
                rect Panel = AppState->UI_Bound;
                Panel.Right  -= 200.0f;
                Panel.Left    = Panel.Right - 540.0f;
                Panel.Bottom += 200.0f;
                Panel.Top    -= 200.0f;
                
                vec4 Outline_Color    = TowardBlack( QUESTION_COLOR, 0.5f );
                vec4 Background_Color = QUESTION_COLOR;
                
                DrawRect( Pass_Game, AddRadius( Panel, 4.0f ), Outline_Color );
                DrawRect( Pass_Game, Panel, TowardBlack( Background_Color, 0.35f ) );
                
                rect Inner = AddRadius( Panel, -2.0f );
                Inner.Bottom += 8.0f;
                Inner.Top     = Panel.Top - 112.0f;
                
                
                char Str[ 16 ] = {};
                
                flo32 Stripe_Height  = 54.0f;
                
                flo32 AtY  = Panel.Top - 20.0f;
                FONT * Font = AppState->FontList + FontID_2_50;
                FONT * Label_Font = AppState->FontList + FontID_2_00;
                
                rect Heading = Panel;
                Heading.Bottom = Heading.Top - 60.0f;
                
                rect Heading0 = Heading;
                rect Heading1 = Heading;
                Heading0.Right = lerp( Heading0.Left, 0.5f, Heading0.Right );
                Heading1.Left  = Heading0.Right;
                
                if( ( FullGame_IsAvailable ) && ( Practice_IsAvailable ) ) {
                    if( AppState->HighScore_ShowPractice ) {
                        DrawRect( Pass_Game, Heading0, COLOR_BLACK );
                    } else {
                        DrawRect( Pass_Game, Heading1, COLOR_BLACK );
                    }
                    
                    rect Tab = AddRadius( Heading1, -4.0f );
                    if( AppState->HighScore_ShowPractice ) {
                        Tab = AddRadius( Heading0, -4.0f );
                    }
                    if( IsInRect( Mouse->Position, Tab ) ) {
                        DrawRect( Pass_Game, Tab, COLOR_GRAY( 0.05f ) );
                        
                        if( WasPressed( Mouse, MouseButton_Left ) ) {
                            AppState->HighScore_ShowPractice = !AppState->HighScore_ShowPractice;
                            
                            PlaySound( AppState, SoundID_Item_Drop );
                        }
                    }
                    
                    vec4 Color0 = TEXT_COLOR;
                    vec4 Color1 = COLOR_GRAY( 0.25f );
                    if( AppState->HighScore_ShowPractice ) {
                        _swap( vec4, Color0, Color1 );
                    }
                    
                    DrawString( Pass_Game, Font, "FULL GAME", GetCenter( Heading0 ), TextAlign_Center, Color0 );
                    DrawString( Pass_Game, Font, "PRACTICE",  GetCenter( Heading1 ), TextAlign_Center, Color1 );
                } else {
                    Heading.Bottom += 32.0f;
                }
                
                Inner = AddRadius( Inner, -4.0f );
                DrawRect( Pass_Game, AddRadius( Inner, 2.0f ), TowardBlack( Background_Color, 0.5f ) );
                DrawRect( Pass_Game, Inner, Background_Color );
                
                AppState->HighScore_ShowPractice_InnerHeight  = GetHeight( Inner );
                AppState->HighScore_ShowPractice_StripeHeight = Stripe_Height;
                
                AtY = Inner.Top;
                
                int32 iStripe      = 0;
                vec4  Stripe_Color = COLOR_GRAY( 0.075f );
                flo32 FromTop = 7.0f;
                flo32 LabelY  = AtY + 8.0f;
                
                if( AppState->HighScore_ShowPractice ) {
                    //int32 iEntry = ( int32 )( tMouse.x * ( flo32 )( PRACTICE_MAX_LEVEL - 1 ) );
                    //ScrollPracticeHighScoreTableToCenterMostRecentHighScore( AppState, SaveGame, iEntry );
                    
                    flo32 ItemsX = Panel.Left  + 140.0f;
                    flo32 ScoreX = Panel.Right - 120.0f;
                    
                    DrawString( Pass_Game, Label_Font, "MEMORIES", Vec2( ItemsX + 60.0f, LabelY ), TextAlign_BottomRight, TEXT_COLOR );
                    DrawString( Pass_Game, Label_Font, "HIGH SCORE", Vec2( ScoreX + 50.0f, LabelY ), TextAlign_BottomRight, TEXT_COLOR );
                    
                    flo32 OffsetY = 0.0f;
                    
                    boo32 Scroll_DoDraw = false;
                    vec2  Scroll_Pos    = {};
                    vec2  Scroll_Dim    = {};
                    {
                        int32 nPracticeToDisplay = 0;
                        for( int32 iPractice = 0; iPractice < PRACTICE_MAX_LEVEL; iPractice++ ) {
                            if( SaveGame->Practice_HighScore[ iPractice ] > 0 ) {
                                nPracticeToDisplay += 1;
                            }
                        }
                        
                        if( nPracticeToDisplay > 0 ) {
                            flo32 Total_Height = ( flo32 )nPracticeToDisplay * Stripe_Height;
                            flo32 Panel_Height = GetHeight( Inner );
                            if( Total_Height > Panel_Height ) {
                                flo32 Remain = Total_Height - Panel_Height;
                                
                                if( ( IsInRect( Mouse->Position, Inner ) ) && ( WasPressed( Mouse, MouseButton_Left ) ) ) {
                                    AppState->HighScore_ShowPractice_DoScroll = true;
                                    AppState->HighScore_ShowPractice_ScrollY  = Mouse->Position.y;
                                }
                                if( ( AppState->HighScore_ShowPractice_DoScroll ) && ( WasReleased( Mouse, MouseButton_Left ) ) ) {
                                    AppState->HighScore_ShowPractice_DoScroll = false;
                                    
                                    flo32 dScrollY = Mouse->Position.y - AppState->HighScore_ShowPractice_ScrollY;
                                    flo32 Dir      = dScrollY / fabsf( dScrollY );
                                    flo32 tScrollY = dScrollY / Remain;
                                    
                                    AppState->HighScore_ShowPractice_tScroll = Clamp01( AppState->HighScore_ShowPractice_tScroll - tScrollY );
                                }
                                
                                if( !AppState->HighScore_ShowPractice_DoScroll ) {
                                    AppState->HighScore_ShowPractice_tScroll = Clamp01( AppState->HighScore_ShowPractice_tScroll - 0.05f * ( flo32 )Mouse->WheelClicks );
                                }
                                
                                flo32 tScroll = AppState->HighScore_ShowPractice_tScroll;
                                if( AppState->HighScore_ShowPractice_DoScroll ) {
                                    flo32 dScrollY = Mouse->Position.y - AppState->HighScore_ShowPractice_ScrollY;
                                    flo32 Dir      = dScrollY / fabsf( dScrollY );
                                    flo32 tScrollY = dScrollY / Remain;
                                    
                                    tScroll = Clamp01( tScroll - tScrollY );
                                }
                                
                                OffsetY = tScroll * Remain;
                                
                                Scroll_DoDraw = true;
                                
                                flo32 Scroll_Height  = Panel_Height * ( Panel_Height / Total_Height );
                                flo32 Scroll_OffsetY = tScroll * ( Panel_Height - Scroll_Height );
                                
                                Scroll_Pos = GetTR( Inner ) + Vec2( 0.0f, -Scroll_OffsetY );
                                Scroll_Dim = Vec2( 4.0f, Scroll_Height );
                            }
                        }
                    }
                    
                    AtY += OffsetY;
                    
                    SetClipRect( Renderer, Pass_Game, Inner );
                    for( int32 iPractice = ( PRACTICE_MAX_LEVEL - 1 ); iPractice >= 0; iPractice-- ) {
                        HIGH_SCORE HighScore = SaveGame->FullGame_HighScore[ iPractice ];
                        
                        if( SaveGame->Practice_HighScore[ iPractice ] > 0 ) {
                            if( ( iStripe % 2 ) == 0 ) {
                                rect Stripe = Rect( Inner.Left, AtY - Stripe_Height, Inner.Right, AtY );
                                DrawRect( Pass_Game, Stripe, Stripe_Color );
                            }
                            iStripe += 1;
                            
                            vec4 Color = TEXT_COLOR;
                            if( ( AppState->HighScore_MostRecent_IsPractice ) && ( AppState->HighScore_MostRecent_iEntry == iPractice ) ) {
                                rect Highlight = Rect( Panel.Left + 2.0f, AtY - Stripe_Height, Panel.Right - 2.0f, AtY );
                                DrawRect( Pass_Game, Highlight, TowardBlack( COLOR_YELLOW, 0.2f ) );
                                Color = COLOR_BLACK;
                            }
                            
                            sprintf( Str, "%d", iPractice + 3 );
                            DrawString( Pass_Game, Font, Str, Vec2( ItemsX, AtY - FromTop ), TextAlign_TopRight, Color );
                            
                            sprintf( Str, "%d", SaveGame->Practice_HighScore[ iPractice ] );
                            DrawString( Pass_Game, Font, Str, Vec2( ScoreX, AtY - FromTop ), TextAlign_TopRight, Color );
                            
                            AtY -= Stripe_Height;
                        }
                    }
                    
                    { // Scroll Bar
                        if( Scroll_DoDraw ) {
                            rect Scroll_Bound = RectTRD( Scroll_Pos, Scroll_Dim );
                            DrawRect( Pass_Game, Scroll_Bound, COLOR_BLACK );
                        }
                    }
                    
                    SetClipRect( Renderer, Pass_Game, AppState->UI_Bound );
                } else {
                    flo32 TargetX = Panel.Left + 28.0f;
                    flo32 RankX   = Panel.Left + 80.0f;
                    flo32 ItemsX  = Panel.Left + 280.0f;
                    flo32 ScoreX  = Panel.Right - 52.0f;
                    
                    DrawString( Pass_Game, Label_Font, "RANK", Vec2( RankX - 60.0f, LabelY ), TextAlign_BottomLeft, TEXT_COLOR );
                    DrawString( Pass_Game, Label_Font, "MEMORIES", Vec2( ItemsX + 60.0f, LabelY ), TextAlign_BottomRight, TEXT_COLOR );
                    DrawString( Pass_Game, Label_Font, "SCORE", Vec2( ScoreX - 72.0f, LabelY ), TextAlign_BottomLeft, TEXT_COLOR );
                    
                    for( int32 iHighScore = 0; iHighScore < HIGH_SCORE_MAX_COUNT; iHighScore++ ) {
                        HIGH_SCORE HighScore = SaveGame->FullGame_HighScore[ iHighScore ];
                        
                        if( HighScore.Score > 0 ) {
                            vec4 Color = TEXT_COLOR;
                            
                            if( ( iStripe % 2 ) == 0 ) {
                                rect Stripe = Rect( Inner.Left, AtY - Stripe_Height, Inner.Right, AtY );
                                DrawRect( Pass_Game, Stripe, Stripe_Color );
                            }
                            iStripe += 1;
                            
                            if( HighScore.IsTargetHighScore ) {
                                DrawString( Pass_Game, Font, "*", Vec2( TargetX, AtY - FromTop - 4.0f ), TextAlign_TopCenter, Color );
                            }
                            
                            if( ( !AppState->HighScore_MostRecent_IsPractice ) && ( AppState->HighScore_MostRecent_iEntry == iHighScore ) ) {
                                rect Highlight = Rect( Panel.Left + 2.0f, AtY - Stripe_Height, Panel.Right - 2.0f, AtY );
                                DrawRect( Pass_Game, Highlight, TowardBlack( COLOR_YELLOW, 0.2f ) );
                                
                                Color = COLOR_BLACK;
                            }
                            
                            sprintf( Str, "%d", iHighScore + 1 );
                            DrawString( Pass_Game, Font, Str, Vec2( RankX, AtY - FromTop ), TextAlign_TopRight, Color );
                            
                            sprintf( Str, "%d", HighScore.nItems );
                            DrawString( Pass_Game, Font, Str, Vec2( ItemsX, AtY - FromTop ), TextAlign_TopRight, Color );
                            
                            sprintf( Str, "%d", HighScore.Score );
                            DrawString( Pass_Game, Font, Str, Vec2( ScoreX, AtY - FromTop ), TextAlign_TopRight, Color );
                            
                            AtY -= Stripe_Height;
                        }
                    }
                }
            }
            
            { //: Title Menu
                vec2 Button_Dim = Vec2( 320.0f, 100.0f );
                vec2 AtPos      = Vec2( 600.0f, 740.0f );
                
                FONT * Title_Font  = AppState->FontList + FontID_7_00;
                FONT * Button_Font = AppState->FontList + FontID_3_00;
                
                flo32 Between = 20.0f;
                
                DrawString( Pass_Game, Title_Font, "TRIVIAL", AtPos, TextAlign_BottomCenter, TEXT_COLOR );
                AtPos.y -= ( 92.0f );
                DrawString( Pass_Game, Title_Font, "RECALL",  AtPos, TextAlign_BottomCenter, TEXT_COLOR );
                
                AtPos.y -= ( 72.0f );
                
                rect NewGame = RectTCD( AtPos, Button_Dim );
                AtPos.y -= ( Button_Dim.y + Between );
                if( UpdateAndDrawPauseButton( Pass_Game, Mouse, Button_Font, "New Game", NewGame, ButtonsAreActive ) ) {
                    PlaySound( AppState, SoundID_Menu_Confirm );
                    PlaySound( AppState, SoundID_NewGame );
                    
                    AppState->Background_dZoom   = -( 1.0f / 2.75f );
                    
                    { // TRANSITION
                        ModeTransition->Type              = ModeTransitionType_Exit;
                        ModeTransition->Timer             = dT;
                        ModeTransition->TargetTime        = ( 30.0f / 60.0f );
                        ModeTransition->ToMode            = AppMode_Game;
                        ModeTransition->ToMode_DoEnter    = true;
                        ModeTransition->ToMode_TargetTime = ( 150.0f / 60.0f );
                        ModeTransition->FromMode          = AppMode_TitleMenu;
                    }
                }
                
                if( SaveGame->Tutorial_IsComplete ) {
                    rect Practice = RectTCD( AtPos, Button_Dim );
                    AtPos.y -= ( Button_Dim.y + Between );
                    
                    if( UpdateAndDrawPauseButton( Pass_Game, Mouse, Button_Font, "Practice", Practice, ButtonsAreActive ) ) {
                        AppState->Mode = AppMode_PracticeMenu;
                        
                        PlaySound( AppState, SoundID_Menu_Confirm );
                    }
                }
                
                rect Settings = RectTCD( AtPos, Button_Dim );
                AtPos.y -= ( Button_Dim.y + Between );
                if( UpdateAndDrawPauseButton( Pass_Game, Mouse, Button_Font, "Settings", Settings, ButtonsAreActive ) ) {
                    AppState->Mode = AppMode_Settings;
                    
                    PlaySound( AppState, SoundID_Menu_Confirm );
                }
                
                rect Quit = RectTCD( AtPos, Button_Dim );
                AtPos.y -= ( Button_Dim.y + Between );
                if( UpdateAndDrawPauseButton( Pass_Game, Mouse, Button_Font, "Quit", Quit, ButtonsAreActive ) ) {
                    AppState->StartUp_DoFadeIn = false;
                    
                    PlaySound( AppState, SoundID_Menu_Back );
                    
                    { // TRANSITION
                        ModeTransition->Type              = ModeTransitionType_Exit;
                        ModeTransition->Timer             = dT;
                        ModeTransition->TargetTime        = ( 60.0f / 60.0f );
                        ModeTransition->ToMode            = AppMode_None;
                        ModeTransition->ToMode_DoEnter    = false;
                        ModeTransition->ToMode_TargetTime = 0.0f;
                        ModeTransition->FromMode          = AppMode_TitleMenu;
                    }
                }
                
                if( ModeTransition_DoExit ) {
                    if( ModeTransition->ToMode == AppMode_Game ) {
                        ResetGameMemory( AppState );
                        StartGame( TempMemory, AppState );
                    }
                }
            }
        } break;
        
        case AppMode_PracticeMenu: {
            if( ModeTransition_IsExiting ) {
                flo32 tTimer = GetT01( ModeTransition->Timer, 0.0f, ModeTransition->TargetTime );
                Layer_Alpha  = 1.0f - tTimer;
                Layer_Dim   *= lerp( 1.0f, CubeValue( tTimer ), 0.95f );
            }
            boo32 ButtonsAreActive = ( !ModeTransition_IsExiting );
            
            vec2 AtPos = Vec2( 960.0f, 740.0f );
            
            DrawString( Pass_Game, AppState->FontList + FontID_3_00, "PRACTICE", AtPos, TextAlign_BottomCenter, TEXT_COLOR );
            AtPos.y -= 140.0f;
            
            rect Display = RectTCD( AtPos, ITEM_DIM );
            DrawRect( Pass_Game, AddRadius( Display, 2.0f ), COLOR_GRAY( 0.025f ) );
            DrawRect( Pass_Game, Display, COLOR_GRAY( 0.05f ) );
            DrawRect( Pass_Game, AddRadius( Display, -12.0f ), COLOR_BLACK );
            
            DrawString( Pass_Game, AppState->FontList + FontID_2_00, "How many memories?", GetTC( Display ) + Vec2( 0.0f, 12.0f ), TextAlign_BottomCenter, TEXT_COLOR );
            
            vec2  Dim       = Vec2( 70.0f, 100.0f );
            flo32 FromSides = 12.0f;
            
            rect  Down_Bound = RectRCD( GetLC( Display ) + Vec2( -FromSides, 0.0f ), Dim );
            rect  Up_Bound   = RectLCD( GetRC( Display ) + Vec2(  FromSides, 0.0f ), Dim );
            
            int32 MinItems = 3;
            int32 MaxItems = 25;
            
            orect Down = ORectCD( GetCenter( Down_Bound ), Dim, PI );
            orect Up   = ORectCD( GetCenter( Up_Bound   ), Dim, 0.0f );
            
            if( UpdateAndDrawPauseButton( Pass_Game, Mouse, TextureID_Arrow_Button, Down, ButtonsAreActive ) ) {
                PlaySound( AppState, SoundID_Item_Drop );
                AppState->Practice_nMinimum = WrapS32( AppState->Practice_nMinimum - 1, MinItems, MaxItems );
            }
            if( UpdateAndDrawPauseButton( Pass_Game, Mouse, TextureID_Arrow_Button, Up, ButtonsAreActive ) ) {
                PlaySound( AppState, SoundID_Item_Drop );
                AppState->Practice_nMinimum = WrapS32( AppState->Practice_nMinimum + 1, MinItems, MaxItems );
            }
            AtPos = GetBC( Display );
            
            char Str[ 16 ] = {};
            sprintf( Str, "%d", AppState->Practice_nMinimum );
            DrawString( Pass_Game, AppState->FontList + FontID_3_00, Str, GetCenter( Display ), TextAlign_Center, TEXT_COLOR );
            
            AtPos.y -= 120.0f;
            rect Start = RectTCD( AtPos, Vec2( 160.0f, 100.0f ) );
            if( UpdateAndDrawPauseButton( Pass_Game, Mouse, AppState->FontList + FontID_2_50, "Start", Start, ButtonsAreActive ) ) {
                PlaySound( AppState, SoundID_Menu_Confirm );
                PlaySound( AppState, SoundID_NewGame );
                
                { // TRANSITION
                    ModeTransition->Type              = ModeTransitionType_Exit;
                    ModeTransition->Timer             = dT;
                    ModeTransition->TargetTime        = ( 30.0f / 60.0f );
                    ModeTransition->ToMode            = AppMode_Game;
                    ModeTransition->ToMode_DoEnter    = true;
                    ModeTransition->ToMode_TargetTime = ( 150.0f / 60.0f );
                    ModeTransition->FromMode          = AppMode_TitleMenu;
                }
            }
            
            
            rect Back = RectTLD( GetTL( AppState->UI_Bound ) + Vec2( 32.0f, -32.0f ), Vec2( 128.0f, 80.0f ) );
            if( UpdateAndDrawPauseButton( Pass_Game, Mouse, AppState->FontList + FontID_2_25, "Back", Back ) ) {
                PlaySound( AppState, SoundID_Menu_Back );
                
                AppState->Mode = AppMode_TitleMenu;
            }
            
            if( ModeTransition_DoExit ) {
                if( ModeTransition->ToMode == AppMode_Game ) {
                    ResetGameMemory( AppState );
                    StartGame( TempMemory, AppState, true );
                }
            }
        } break;
        
        case AppMode_Settings: {
            vec2  AtPos   = Vec2( 960.0f, 760.0f );
            vec2  Dim     = Vec2( 220.0f, 80.0f );
            flo32 Between = 8.0f;
            
            FONT * Font      = AppState->FontList + FontID_2_50;
            FONT * Text_Font = AppState->FontList + FontID_2_00;
            
            AtPos.y -= 80.0f;
            vec2 Sound_Status = AtPos;
            AtPos.y -= ( 36.0f );
            
            rect Sound  = RectTCD( AtPos, Dim * Vec2( 1.5f, 1.0f ) );
            AtPos.y = GetBC( Sound ).y;
            if( UpdateAndDrawPauseButton( Pass_Game, Mouse, Font, "Toggle Sound", Sound ) ) {
                AppState->SoundIsActive = !AppState->SoundIsActive;
                
                if( AppState->SoundIsActive ) {
                    PlaySound( AppState, SoundID_ItemsUp );
                }
            }
            if( AppState->SoundIsActive ) {
                DrawString( Pass_Game, Text_Font, "Sound is ON", Sound_Status, TextAlign_TopCenter, TEXT_COLOR );
            } else {
                DrawString( Pass_Game, Text_Font, "Sound is OFF", Sound_Status, TextAlign_TopCenter, TEXT_COLOR );
            }
            
            AtPos.y -= 80.0f;
            rect Reset = RectTCD( AtPos, Dim * Vec2( 1.5f, 1.0f ) );
            AtPos.y -= ( Dim.y + Between );
            if( UpdateAndDrawPauseButton( Pass_Game, Mouse, Font, "Reset High Scores", Reset ) ) {
                AppState->DoResetHighScores = !AppState->DoResetHighScores;
                
                if( AppState->DoResetHighScores ) {
                    PlaySound( AppState, SoundID_Menu_Back );
                } else {
                    PlaySound( AppState, SoundID_Item_Drop );
                }
            }
            
            if( AppState->DoResetHighScores ) {
                rect Confirm = RectLCD( GetRC( Reset ) + Vec2( Between, 0.0f ), Dim );
                if( UpdateAndDrawPauseButton( Pass_Game, Mouse, Font, "Confirm", Confirm, COLOR_RED ) ) {
                    memset( &AppState->SaveGame, 0, sizeof( SAVE_GAME ) );
                    InitHighScores( AppState );
                    SaveGameFile( Platform->WriteFile, AppState, &Platform->TempMemory );
                    
                    AppState->DoResetHighScores = false;
                    PlaySound( AppState, SoundID_LoseGame );
                    
                    WORKER_STATE * WorkerS = &AppState->WorkerS;
                    WorkerS->Sweep_Timer = SWEEP_TARGET_TIME;
                }
            }
            
            
            rect Back = RectTLD( GetTL( AppState->UI_Bound ) + Vec2( 32.0f, -32.0f ), Vec2( 128.0f, 80.0f ) );
            if( UpdateAndDrawPauseButton( Pass_Game, Mouse, AppState->FontList + FontID_2_25, "Back", Back ) ) {
                AppState->DoResetHighScores = false;
                PlaySound( AppState, SoundID_Menu_Back );
                
                AppState->Mode = AppMode_TitleMenu;
            }
        } break;
        
        default: {
            InvalidCodePath;
        } break;
    }
    
    {
        SetFramebuffer( Renderer, Pass_Game, FramebufferID_Main );
        
        { // Layer Zoom
            DrawRect( Pass_Layer, TextureID_Framebuffer_Layer, RectCD( GetCenter( AppState->UI_Bound ), Layer_Dim ), COLOR_WHITE * Layer_Alpha );
        }
        
        { // Fade In At StartUp / Fade Out At Quit
            if( AppState->StartUp_DoFadeIn ) {
                flo32 dFade = 1.0f / 1.0f;
                AppState->StartUp_tFade = MinValue( AppState->StartUp_tFade + dFade * dT, 1.0f );
            } else {
                flo32 dFade = 1.0f / 0.5f;
                AppState->StartUp_tFade = MaxValue( AppState->StartUp_tFade - dFade * dT, 0.0f );
                
                if( AppState->StartUp_tFade <= 0.0f ) {
                    Platform->IsRunning = false;
                }
            }
            
            flo32 Alpha = 1.0f - Clamp01( AppState->StartUp_tFade );
            DrawRect( Pass_Layer, AppState->UI_Bound, COLOR_BLACK * Alpha );
        }
    }
    
#if 0
    {
        DrawRect( Pass_Game, AppState->UI_Bound, COLOR_BLACK );
        
        vec2 PosA = GetTL( AppState->UI_Bound ) + Vec2(  40.0f, -40.0f );
        vec2 PosB = GetTC( AppState->UI_Bound ) + Vec2( -560.0f, -40.0f );
        
        int32 FramesPerSecondA = 60;
        int32 FramesPerSecondB = ( int32 )lerp( 12.0f, tMouse.x, 480.0f );
        
        int32 Friction  = ( int32 )lerp( 1.0f, tMouse.y, 21.0f );
        flo32 FrictionA = ( flo32 )Friction;
        //FramesPerSecondB = 22;
        //FrictionA = 3.0f;
        flo32 FrictionB = CalculateNewFriction( FramesPerSecondB, FrictionA );
        
        flo32 Multiplier = FrictionB / FrictionA;
        DoFramerateTest( Pass_Game, AppState, TempMemory, PosA, FramesPerSecondA, FrictionA, 1.0f );
        DoFramerateTest( Pass_Game, AppState, TempMemory, PosB, FramesPerSecondB, FrictionB, Multiplier );
    }
#endif
    
    DISPLAY_VALUE( uint64, PermMemory->Used );
    DISPLAY_VALUE( uint64, TempMemory->Used );
    Assert( TempMemory->Used < TempMemory->Size );
}