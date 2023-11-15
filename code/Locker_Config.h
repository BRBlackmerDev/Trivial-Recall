
//----------
//----------

//#define APP_PermMemorySize _MB( 256 )
#define APP_PermMemorySize   _GB( 1 )
#define APP_TempMemorySize   _MB( 256 )

#define APP_isFullscreen  ( 0 )
#define LEVEL_SAVE_DIRECTORY  ( "../../asset/entity" )
#define ART_SAVE_DIRECTORY    ( "../../asset/art" )
#define AUDIO_SAVE_DIRECTORY  ( "../../asset/audio" )
#define MUSIC_SAVE_DIRECTORY  ( "../../asset/audio/music" )
#define CODE_DIRECTORY        ( "../../code" )

#if ( DEBUG_BUILD )
#define FONT_SAVE_DIRECTORY   ( "../../asset/" )
#else
#define FONT_SAVE_DIRECTORY   ( 0 )
#endif

#define DEV_WIDTH   ( 1920.0f )
#define DEV_HEIGHT  ( 1080.0f )

#define WALL_COLOR            ( COLOR_GRAY( 0.1f ) )
#define TUNNEL_DARK_COLOR     ( COLOR_GRAY( 0.1f ) )
#define TUNNEL_LIGHT_COLOR    ( COLOR_GRAY( 0.35f ) )
#define SPOTLIGHT_COLOR       ( COLOR_GRAY( 0.35f ) )
#define STORAGE_COLOR         ( COLOR_GRAY( 0.2f ) )
#define QUESTION_COLOR        ( COLOR_GRAY( 0.10f ) )
#define DISPLAY_BORDER_COLOR  ( COLOR_GRAY( 0.0875f ) )
#define PANEL_COLOR           ( COLOR_GRAY( 0.075f ) )
#define FENCE_COLOR           ( COLOR_GRAY( 0.2f ) )
#define DAMAGE_COLOR          ( COLOR_RED )
#define CARDBOARD_COLOR       ( Vec4( 0.25f, 0.1667f, 0.05f, 1.0f ) )
#define CARDBOARD_EMPTY_COLOR ( TowardBlack( CARDBOARD_COLOR, 0.2f ) )

#define ENABLE_TUTORIAL  ( 0 )

#define TEXT_COLOR   ( Vec4( 0.9f, 0.9f, 0.9f, 1.0f ) )
#define BONUS_POINTS_COLOR  ( TowardBlack( COLOR_YELLOW, 0.25f ) )
#define BONUS_HEALTH_COLOR  ( TowardBlack( COLOR_GREEN,  0.25f ) )

#define USE_FMOD       ( 1 )
#define USE_DIRECTX12  ( 0 )

#define ITEM_MAX_COUNT     ( 128 )
#define PLAYER_MAX_HEALTH  ( 5 )
#define GAME_ITEMS_PER_LEVEL      ( 8 )
#define TUTORIAL_ITEMS_PER_LEVEL  ( 3 )