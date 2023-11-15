
//----------
// STRING BUFFER
//----------

struct STRING_BUFFER {
    uint32 nStr;
    uint32 nStrMax;
    char ** str;
    MEMORY  Memory;
};

internal void
Reset( STRING_BUFFER * buffer ) {
    buffer->nStr        = 0;
    buffer->Memory.Used = 0;
}

internal STRING_BUFFER
StringBuffer( MEMORY * parent_Memory, uint32 maxString, uint32 maxSize ) {
    STRING_BUFFER Result = {};
    Result.str     = _PushArray_Clear( parent_Memory, char *, maxString );
    Result.Memory  = SubArena( parent_Memory, maxSize );
    Result.nStrMax = maxString;
    return Result;
}

internal void
StringBuffer_FreeMemory( STRING_BUFFER Buffer, MEMORY * Parent_Memory ) {
    _PopArray( Parent_Memory, char *, Buffer.nStrMax );
    _PopSize ( Parent_Memory, Buffer.Memory.Size );
}

internal char *
AddString( STRING_BUFFER * s, char * String ) {
    char * dest = 0;
    if( s->nStr < s->nStrMax ) {
        uint32 length = ( uint32 )strlen( String );
        dest = ( char * )_PushSize( &s->Memory, length + 1 );
        strcpy( dest, String );
        
        s->str[ s->nStr++ ] = dest;
    }
    
    return dest;
}

//----------
// LOCKER

#define ITEM_WIDTH  ( 140.0f )
#define ITEM_HEIGHT ( 140.0f )
#define ITEM_DIM    ( Vec2( ITEM_WIDTH, ITEM_HEIGHT ) )
#define LOCKER_DIM                ( Vec2( 160.0f, 160.0f ) )

#define LOCKER_SHOW_ITEM_TARGET_TIME  ( 90.0f / 60.0f )

struct LOCKER {
    int32 ItemID;
    vec2  Position;
    
    boo32 DoSpring;
    vec2  PosForSpring;
    vec2  VelForSpring;
    
    flo32 ShowItem_Timer;
};

//----------
// PARTICLES
//----------

struct PARTICLE {
    TEXTURE_ID TextureID;
    
    vec2  Position;
    vec2  Velocity;
    vec2  Accel;
    flo32 Friction;
    
    vec2   Dim;
    vec2  dDim;
    
    flo32  Radians;
    flo32 dRadians;
    
    flo32  Timer;
    flo32  TargetTime;
    vec4   Color;
    vec4  dColor;
};

#define PARTICLE_MAX_COUNT  ( 8192 )
struct PARTICLE_STATE {
    uint32    nParticle;
    PARTICLE   Particle[ PARTICLE_MAX_COUNT ];
};

//----------
// PROFILE
//----------

struct PROFILE_BLOCK {
    char   Label[ 32 ];
    uint64 CycleCounter_Start;
    uint64 CycleCounter_End;
};

#define PROFILE_BLOCK_MAX_COUNT  ( 16 )
struct PROFILE_STATE {
    boo32         ProfilingIsActive;
    uint32       nProfileBlock;
    PROFILE_BLOCK ProfileBlock[ PROFILE_BLOCK_MAX_COUNT ];
};