

#define DEBUG_SYSTEM__SAFETY_OBJECT_COUNT  ( 4 )
#define DEBUG_SYSTEM__SAFETY_VERTEX_COUNT  ( 512 )

internal MODEL_DATA
UploadModel( VERTEX1_BUFFER * buffer, VERTEX1 * vertex, uint32 nVertex, uint32 * index, uint32 nIndex ) {
    MODEL_DATA Result = {};
    return Result;
}

internal void
RenderPass( RENDERER * Renderer, RENDER_PASS * Pass, boo32 EnableDepth = false, boo32 EnableBlend = false ) {
    
}

struct PLATFORM {
    boo32 IsRunning;
    
    flo32  TargetSec;
    uint32 FramesPerSec;
    
    KEYBOARD_STATE   Keyboard;
    MOUSE_STATE      Mouse;
    
    MEMORY PermMemory;
    MEMORY TempMemory;
    
    //AUDIO_SYSTEM AudioSystem;
    
    RENDERER Renderer;
    
    READ_FILE       * ReadFile;
    WRITE_FILE      * WriteFile;
    DOES_FILE_EXIST * DoesFileExist;
    DELETE_FILE     * DeleteFile;
};

//----------
// CAMERA
//----------

struct CAMERA_SHAKE {
    vec2 P;
    vec2 Q;
    
    flo32 Timer;
    flo32 UpTime;
    flo32 HoldTime;
    flo32 DownTime;
    
    flo32 t;
    flo32 TargetT;
    
    flo32 Strength;
    vec2  Offset;
};

struct CAMERA_STATE {
    vec2  Pos;
    vec2  Dim;
    vec2  HalfDim;
    flo32 Scale_Ratio;
    
    boo32 Move_Left;
    boo32 Move_Right;
    boo32 Move_Down;
    boo32 Move_Up;
    boo32 Move_Out;
    boo32 Move_In;
};

//----------
// APP STATE
//----------

#define SECONDARY_OBJECTIVE_QUICK_TIMER_TARGET_TIME  ( 7.0f )
enum SECONDARY_OBJECTIVE {
    SecondaryObjective_ExtraItem,
    SecondaryObjective_WhichInSet,
    SecondaryObjective_YesNoInSet,
    SecondaryObjective_QuickTimer,
    
    SecondaryObjective_Count,
    
    SecondaryObjective_None,
};

struct SECONDARY_OBJECTIVE_STATUS {
    SECONDARY_OBJECTIVE SecondaryObjective;
    boo32               IsActive;
};

enum SCORE_UPDATE_TYPE {
    ScoreUpdate_OnePoint,
    ScoreUpdate_OneBonusPoint,
    ScoreUpdate_TwoPoints,
    ScoreUpdate_ThreePoints,
    ScoreUpdate_OneHealth,
    ScoreUpdate_OnePoint_OneHealth,
    ScoreUpdate_OnePoint_TwoHealth,
    ScoreUpdate_TwoPoints_OneHealth,
    ScoreUpdate_LoseHealth,
    
    ScoreUpdate_Count,
};

struct SCORE_UPDATE {
    SCORE_UPDATE_TYPE Type;
    flo32             Timer;
    flo32             TargetTime;
};

struct HIGH_SCORE {
    boo32 IsTargetHighScore;
    int32 Score;
    int32 nItems;
};

internal HIGH_SCORE
HighScoreC( boo32 IsTargetHighScore, int32 Score, int32 nItems ) {
    HIGH_SCORE Result = {};
    Result.IsTargetHighScore = IsTargetHighScore;
    Result.Score  = Score;
    Result.nItems = nItems;
    return Result;
}

struct SAVE_GAME {
#define HIGH_SCORE_MAX_COUNT  ( 10 )
    HIGH_SCORE FullGame_HighScore[ HIGH_SCORE_MAX_COUNT ];
    boo32      Tutorial_IsComplete;
    
#define GAME_MIN_LEVEL  (  3 )
#define GAME_MAX_LEVEL  ( 25 )
#define PRACTICE_MAX_LEVEL  ( GAME_MAX_LEVEL - GAME_MIN_LEVEL + 1 )
    int32      Practice_HighScore[ PRACTICE_MAX_LEVEL ];
};

enum APP_MODE {
    AppMode_None,
    AppMode_Game,
    AppMode_TitleMenu,
    AppMode_Settings,
    AppMode_PracticeMenu,
    
    AppMode_Count,
};

#if USE_FMOD
// NOTE : TEMP!
struct TEMP_DATA {
    int32 iString;
    int32 iEvent;
    
    FMOD_GUID Guid;
    char      String[ 256 ];
};
#endif

struct DUST_PARTICLE {
    vec2  Position;
    vec2  Velocity;
    flo32 Friction;
    flo32 Timer;
    flo32 TargetTime;
    
    flo32  Move_Radians;
    flo32  Move_Speed;
    flo32  Move_Timer;
    flo32  Move_TargetTime;
};

#define WORKER_TARGET_TIME  ( 240.0f / 60.0f )
struct WORKER {
    vec2  Position;
    vec2  Direction;
    flo32 Timer;
};

struct PATH {
    vec2 Start;
    vec2 End;
};

struct WORKER_STATE {
#define PATH_MAX_COUNT  ( 12 )
    int32 nPath;
    PATH   Path[ PATH_MAX_COUNT ];
    
#define WORKER_MAX_COUNT  ( 256 )
    int32 MaxWorker;
    int32   nWorker;
    WORKER   Worker[ WORKER_MAX_COUNT ];
    
#define CELL_ROW_MAX_COUNT  ( 8 )
#define CELL_COL_MAX_COUNT  ( 12 )
#define CELL_MAX_COUNT      ( CELL_ROW_MAX_COUNT * CELL_COL_MAX_COUNT )
#define CELL_TARGET_TIME    ( 240.0f / 60.0f )
    
#define SWEEP_CELL_TARGET_TIME   ( 0.75f )
#define SWEEP_DELAY_TARGET_TIME  ( 7.0f / 60.0f )
#define SWEEP_TARGET_TIME   ( SWEEP_CELL_TARGET_TIME + SWEEP_DELAY_TARGET_TIME * ( flo32 )( CELL_COL_MAX_COUNT - 1 ) )
    
    flo32 Sweep_Timer;
    
    int32 Cell_nActive;
    int32 Cell_MaxActive;
    int32 Cell_Active[ CELL_MAX_COUNT ];
    int32 Cell_Index [ CELL_MAX_COUNT ];
    flo32 Cell_Timer [ CELL_MAX_COUNT ];
};

enum APP_MODE_TRANSITION_TYPE {
    ModeTransitionType_None,
    ModeTransitionType_Enter,
    ModeTransitionType_Exit,
    
    ModeTransitionType_Count,
};

struct APP_MODE_TRANSITION {
    APP_MODE_TRANSITION_TYPE Type;
    flo32                    Timer;
    flo32                    TargetTime;
    APP_MODE                 ToMode;
    boo32                    ToMode_DoEnter;
    flo32                    ToMode_TargetTime;
    APP_MODE                 FromMode;
    boo32                    DoFinalize;
};

struct APP_STATE {
	boo32 IsRunning;
	boo32 IsInitialized;
    
    //AUDIO_SYSTEM * AudioSystem;
    //AUDIO_DATA     AudioList  [ AudioID_Count ];
    
    //#define AUDIO_SCRIPT_MAX_COUNT  ( 16 )
    //AUDIO_ENTRY    AudioScript[ AUDIO_SCRIPT_MAX_COUNT ];
    
    APP_MODE            Mode;
    APP_MODE_TRANSITION ModeTransition;
    
    boo32    StartUp_DoFadeIn;
    flo32    StartUp_tFade;
    flo32    TitleMenu_Delay;
    
    vec2  UI_Dim;
    vec2  UI_HalfDim;
    rect  UI_Bound;
    flo32 dT;
    
#if DEBUG_BUILD
    FONT * Font;
#endif
    
    FONT FontList[ FontID_Count ];
    
#if USE_FMOD
    FMOD_STUDIO_SYSTEM * FMOD_System;
    FMOD_GUID            FMOD_Guid[ SoundID_Count ];
#endif
    
    boo32 HighScore_ShowPractice;
    flo32 HighScore_ShowPractice_tScroll;
    flo32 HighScore_ShowPractice_DoScroll;
    flo32 HighScore_ShowPractice_ScrollY;
    flo32 HighScore_ShowPractice_StripeHeight;
    flo32 HighScore_ShowPractice_InnerHeight;
    int32 HighScore_MostRecent_IsPractice;
    int32 HighScore_MostRecent_iEntry;
    int32 SoundIsActive;
    boo32 DoResetHighScores;
    
    int32 Practice_nMinimum;
    
    flo32 Background_tZoom;
    flo32 Background_dZoom;
    
#define BACKGROUND_PARTICLE_WIND_FRAME_COUNT  ( 10 )
#define BACKGROUND_PARTICLE_MAX_COUNT         ( 256 )
    DUST_PARTICLE  Background_Particle[ BACKGROUND_PARTICLE_MAX_COUNT ];
    vec2           Background_Wind_Pos[ BACKGROUND_PARTICLE_WIND_FRAME_COUNT ];
    vec2           Background_Wind_Vel[ BACKGROUND_PARTICLE_WIND_FRAME_COUNT ];
    
    WORKER_STATE WorkerS;
    
    SAVE_GAME SaveGame;
    
    uint8  Reset_GameMemory_Start;
    
    boo32  ShowPauseMenu;
    
    int32  Item[ ITEM_MAX_COUNT ];
    int32 nItem;
    
#define N_ITEMS_UP_TARGET_TIME  ( 120.0f / 60.0f )
    flo32 nItemsUp_Timer;
    
#define LOCKER_CORRECT_TARGET_TIME  ( 80.0f / 60.0f )
    flo32  Correct_Timer;
    int32  Correct_ItemID;
    vec2   Correct_Position;
    
#define LIGHT_TARGET_TIME  ( 90.0f / 60.0f )
    flo32  Light_Timer;
    boo32  Light_IsCorrect;
    
    boo32  EndGame;
    
    int32  Score;
    int32  nMinimumItems;
    int32  MaxHealth;
    int32  Health;
    flo32  HealthUnit_tTimer[ PLAYER_MAX_HEALTH ];
    flo32  LowHealth_tTimer;
    
#define WIRE_LIGHT_UP_TARGET_TIME  ( 120.0f / 60.0f )
    flo32  Wire_Score_Timer;
    boo32  Wire_Score_DoScore;
    boo32  Wire_Score_DoHealth;
    boo32  Wire_Score_DoOverflow;
    boo32  Wire_Score_DoBonus;
    
#define SCORE_UPDATE_MAX_COUNT  ( 8 )
    int32       nScoreUpdate;
    SCORE_UPDATE ScoreUpdate[ SCORE_UPDATE_MAX_COUNT ];
    
    boo32  Practice_IsActive;
    boo32  MinimumItemsUp_IsActive;
    int32  MinimumItemsUp_ItemsPerLevel;
    
#define TURN_TARGET_TIME  ( 18.0f )
    boo32  Turn_IsActive;
    flo32  Turn_Timer;
    
    rect   Storage_Bound;
    rect   ToStore_Bound;
    rect   ToFind_Return;
    rect   ToFind_Display;
    rect   ScoreUpdate_Display;
    rect   ScorePanel_Display;
    rect   Health_Display;
    //rect   ToFind_Text;
    rect   Turn_Bound;
    rect   Question_Bound;
    
    SECONDARY_OBJECTIVE_STATUS SecondaryObjective_Order[ SecondaryObjective_Count ];
    int32                      SecondaryObjective_iOrder;
    
    SECONDARY_OBJECTIVE SecondaryObjective;
    flo32               SecondaryObjective_Timer;
    int32               SecondaryObjective_ItemID0;
    int32               SecondaryObjective_ItemID1;
    boo32               SecondaryObjective_IsInSet;
    int32               SecondaryObjective_LastItemAsked0;
    int32               SecondaryObjective_LastItemAsked1;
    int32               SecondaryObjective_ToFind;
    boo32               SecondaryObjective_IsSelected;
    flo32               SecondaryObjective_Radians;
    flo32               SecondaryObjective_Alpha;
    boo32               SecondaryObjective_IsActive;
    boo32               SecondaryObjective_FirstOptionWasSelected;
    boo32               SecondaryObjective_WasAnsweredCorrectly;
    boo32               SecondaryObjective_DoTwoBonusPoints;
    flo32               SecondaryObjective_tQuick;
    
#define SECONDARY_OBJECTIVE_VIEW_RESULT_TARGET_TIME  ( 40.0f / 60.0f )
    flo32               SecondaryObjective_ViewResultTimer;
    
    int32  nCorrectItemsReturned_Total;
    int32  nCorrectItemsReturned_ThisLevel;
    int32  nWrongItemsReturned;
    
#define SHOW_TUTORIAL_TARGET_TIME  ( 80.0f / 60.0f )
    boo32  Tutorial_IsActive;
    boo32  Tutorial_PauseForFirstQuestion;
    
    int32  ToFind;
    int32  ToStore;
    boo32  ToStore_IsSelected;
    int32  LastItemStored;
    int32  LastItemFound;
    
    flo32  ToStore_Radians;
    vec2   ToStore_Position;
    vec2   ToStore_Velocity;
    flo32  ToStore_tLight;
    flo32  ToStore_Spotlight_Radians;
    flo32  ToFind_tLight;
    
    int32  ToMove_iLocker;
    vec2   ToMove_Offset;
    
#define LOCKER_MAX_COUNT  ( 64 )
    int32 nExtraLocker;
    int32 nLocker;
    LOCKER Locker[ LOCKER_MAX_COUNT ];
    
    //flo32  ToFind_Particle_Timer;
    //int32  ToFind_nParticle;
#define TO_FIND_PARTICLE_MAX_COUNT  ( 48 )
    //DUST_PARTICLE  ToFind_Particle[ TO_FIND_PARTICLE_MAX_COUNT ];
    
    uint8 Reset_GameMemory_End;
    
    CAMERA_STATE   Camera;
    PARTICLE_STATE ParticleS;
    
    PROFILE_STATE  Profile;
};