

#include <math.h> // sqrtf, cosf, sinf
#include <stdint.h> // Type definitions
#include <stdlib.h> // rand
#include <time.h> // time
#include <float.h> // FLT_MAX
#include <cstdio> // sprintf
#include <shlwapi.h>
#include <windows.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_trueType.h"

#include "TypeDef.h"
#include "math.cpp"

#define 	STD_INCLUDE_DECL
#include "Vector.cpp"
#include "Memory.cpp"
#include "Font.cpp"
#undef	STD_INCLUDE_DECL

#define	STD_INCLUDE_FUNC
#include "Vector.cpp"
#include "Memory.cpp"
#include "Font.cpp"
#undef	STD_INCLUDE_FUNC

#include "Locker_Config.h"
#include "AssetID.h"

#define AUDIO__BYTES_PER_SAMPLE    ( 2 )
#define AUDIO__CHANNEL_COUNT       ( 2 )

struct AUDIO_DATA {
    uint8 * Audio;
    int32  nSamples;
};

#include "win32_platform.h"
#include "Entity.h"
#include "AssetPackerTools.cpp"
#include "GenMesh.cpp"
#include "Parser.cpp"
#include "bmp.cpp"
#include "X.cpp"
#include "wav.cpp"

#if USE_FMOD
#include "fmod_studio.h"
#include "FModTools.cpp"
#endif

struct ASSET_OUTPUT {
    MEMORY * Data_Output;
    MEMORY * AssetID_Output;
    
    int32 nTexture;
    int32 nMesh;
    int32 nLevel;
    int32 nAudio;
};

internal FILE_DATA
ReadFile( MEMORY * Memory, char * File_Path ) {
    FILE_DATA Result = {};
    
    FILE * File = fopen( File_Path, "rb" );
    if( File ) {
        fseek( File, 0, SEEK_END );
        int32 File_Bytes = ftell( File );
        fseek( File, 0, SEEK_SET );
        
        uint8 * Dest = ( uint8 * )_PushSize( Memory, File_Bytes );
        int32 BytesRead = ( int32 )fread( Dest, 1, File_Bytes, File );
        if( File_Bytes != BytesRead ) {
            printf( "ERROR! Error reading file!  ( File_Bytes != BytesRead )" );
        } else {
            Result.Contents = Dest;
            Result.Size     = File_Bytes;
        }
        
        fclose( File );
    }
    
    return Result;
}

internal FILE_DATA
ReadFile( MEMORY * Memory, char * SaveDir, char * FileName, char * Extension ) {
	char FilePath[ 2048 ] = {};
    GenFilePath( FilePath, SaveDir, FileName, Extension );
    
    FILE_DATA Result = ReadFile( Memory, FilePath );
    return Result;
}

internal void
WriteFile( char * FilePath, void * Data, uint64 Size ) {
	FILE * File = fopen( FilePath, "wb" );
	if( File ) {
        char Str[ 1024 ] = {};
        
		int32 BytesWritten = ( int32 )fwrite( Data, 1, Size, File );
		if( BytesWritten != Size ) {
			sprintf( Str, "ERROR. Unable to write File: %s", FilePath );
		} else {
            sprintf( Str, "SUCCESS. Wrote File: %s", FilePath );
        }
        printf( Str );
        printf( "\n" );
        
        fclose( File );
    }
}

struct ASSET_TEXTURE {
#define ASSET_TEXTURE_NAME_CHAR_MAX_COUNT  ( 64 )
    char     Name[ ASSET_TEXTURE_NAME_CHAR_MAX_COUNT ];
    int32    Width;
    int32    Height;
    uint32 * Texture_Data;
};

internal void
PackTextureID( MEMORY * AssetID_Output, char * TextureID ) {
    WriteSegment( AssetID_Output, "   TextureID_" );
    WriteSegment( AssetID_Output, TextureID );
    WriteSegment( AssetID_Output, ",\n" );
}

internal void
PackTexture( ASSET_OUTPUT * AssetOutput, char * TextureID, int32 Width, int32 Height, uint32 * Texture_Data ) {
    _writem( AssetOutput->Data_Output, int32, Width  );
    _writem( AssetOutput->Data_Output, int32, Height );
    
    if( Texture_Data ) {
        _writeb( AssetOutput->Data_Output, Texture_Data, Width * Height * sizeof( uint32 ) );
    }
    
    PackTextureID( AssetOutput->AssetID_Output, TextureID );
    
    AssetOutput->nTexture += 1;
}

internal void
PackBMP( ASSET_OUTPUT * AssetOutput, MEMORY * Memory, char * FileName ) {
    int32 Memory_Size = _MB( 32 );
    MEMORY  _DestMemory = SubArena( Memory, Memory_Size );
    MEMORY  _TempMemory = SubArena( Memory, Memory_Size );
    
    MEMORY * DestMemory    = &_DestMemory;
    MEMORY * TempMemory    = &_TempMemory;
    
    TEXTURE_DATA Texture = BMP_ReadFile( ReadFile, DestMemory, TempMemory, ART_SAVE_DIRECTORY, FileName );
    if( Texture.TexelData ) {
        PackTexture( AssetOutput, FileName, Texture.Width, Texture.Height, ( uint32 * )Texture.TexelData );
    }
}

internal void
DivideAndPackSpriteSheet( ASSET_OUTPUT * AssetOutput, MEMORY * Memory, char * FileName, int32 nRow, int32 nCol ) {
    int32 Memory_Size = _MB( 8 );
    MEMORY  _DestMemory = SubArena( Memory, Memory_Size );
    MEMORY  _TempMemory = SubArena( Memory, Memory_Size );
    
    MEMORY * DestMemory    = &_DestMemory;
    MEMORY * TempMemory    = &_TempMemory;
    
    TEXTURE_DATA Texture = BMP_ReadFile( ReadFile, DestMemory, TempMemory, ART_SAVE_DIRECTORY, FileName );
    if( Texture.TexelData ) {
        Assert( ( Texture.Width % nCol ) == 0 );
        Assert( ( Texture.Width % nRow ) == 0 );
        
        int32 xPixel   = Texture.Width  / nCol;
        int32 yPixel   = Texture.Height / nRow;
        int32 nPixel   = xPixel * yPixel;
        int32 nTexture = nRow * nCol;
        
        uint32 * TextureToPack = _PushArray( Memory, uint32, nPixel );
        
        uint32 * TexelData = ( uint32 * )Texture.TexelData;
        for( int32 iRow = 0; iRow < nRow; iRow++ ) {
            uint32 * Src  = ( uint32 * )Texture.TexelData + ( Texture.Width * yPixel * iRow );
            
            for( int32 iCol = 0; iCol < nCol; iCol++ ) {
                
                for( int32 iTexelY = 0; iTexelY < yPixel; iTexelY++ ) {
                    for( int32 iTexelX = 0; iTexelX < xPixel; iTexelX++ ) {
                        TextureToPack[ xPixel * iTexelY + iTexelX ] = Src[ Texture.Width * iTexelY + iTexelX ];
                    }
                }
                
                Src += xPixel;
                
                PackTexture( AssetOutput, "TEMP", xPixel, yPixel, TextureToPack );
            }
        }
        
        _PopArray( Memory, uint32, nPixel );
        _PopSize ( Memory, Memory_Size );
        _PopSize ( Memory, Memory_Size );
    }
}

internal void
PackMesh( ASSET_OUTPUT * AssetOutput, MODEL_ID MeshID, MESH Mesh ) {
    _writem( AssetOutput->Data_Output, MODEL_ID, MeshID );
    
    _writem( AssetOutput->Data_Output, int32, Mesh.nVertex );
    _writem( AssetOutput->Data_Output, int32, Mesh.nIndex  );
    
    for( int32 iVertex = 0; iVertex < Mesh.nVertex; iVertex++ ) {
        _writem( AssetOutput->Data_Output, vec3, Mesh.Vertex[ iVertex ] );
        _writem( AssetOutput->Data_Output, vec2, Mesh.UV    [ iVertex ] );
    }
    
    _writea( AssetOutput->Data_Output, Mesh.Index, int32, Mesh.nIndex );
    
    AssetOutput->nMesh += 1;
}

internal void
PackGeneratedMesh( ASSET_OUTPUT * AssetOutput, MEMORY * TempMemory, MODEL_ID MeshID, GEN_MESH * GenMesh ) {
    MESH Mesh = GenMesh( TempMemory );
    
    PackMesh( AssetOutput, MeshID, Mesh );
    
    _PopArray( TempMemory, vec3,  Mesh.nVertex );
    _PopArray( TempMemory, vec2,  Mesh.nUV     );
    _PopArray( TempMemory, int32, Mesh.nIndex  );
}

internal void
PackXFile( ASSET_OUTPUT * AssetOutput, READ_FILE * ReadFile, MEMORY * TempMemory, char * SaveDir, char * FileName, MODEL_ID MeshID ) {
    int32  Memory_Size = _MB( 1 );
    MEMORY Memory      = SubArena( TempMemory, Memory_Size );
    MESH   Mesh   = LoadXFile( ReadFile, &Memory, TempMemory, SaveDir, FileName, MeshID );
    
    PackMesh( AssetOutput, MeshID, Mesh );
    
    _PopSize ( TempMemory, Memory_Size );
}

internal void
PackAudio( ASSET_OUTPUT * AssetOutput, AUDIO_ID AudioID, AUDIO_DATA Audio ) {
    _writem( AssetOutput->Data_Output, AUDIO_ID, AudioID );
    _writem( AssetOutput->Data_Output, int32, Audio.nSamples );
    
    int32 SampleSize = AUDIO__BYTES_PER_SAMPLE * AUDIO__CHANNEL_COUNT;
    _writeb( AssetOutput->Data_Output, Audio.Audio, Audio.nSamples * SampleSize );
    
    AssetOutput->nAudio += 1;
}

internal void
PackWavFile( ASSET_OUTPUT * AssetOutput, READ_FILE * ReadFile, MEMORY * TempMemory, char * SaveDir, char * FileName, AUDIO_ID AudioID ) {
    int32  Memory_Size = _MB( 50 );
    MEMORY Memory      = SubArena( TempMemory, Memory_Size );
    
    AUDIO_DATA Audio = WAV_ReadFile( ReadFile, &Memory, TempMemory, SaveDir, FileName );
    
    PackAudio( AssetOutput, AudioID, Audio );
    
    _PopSize( TempMemory, Memory_Size );
}

internal void
PackSoundID( MEMORY * SoundID_Output, char * SoundID, int32 nChar = -1 ) {
    WriteSegment( SoundID_Output, "   SoundID_" );
    WriteSegment( SoundID_Output, SoundID, nChar );
    WriteSegment( SoundID_Output, ",\n" );
}

int32 main() {
    int32    Data_Output_Size = _MB( 512 );
    MEMORY  _Data_Output      = Memory( calloc( Data_Output_Size, 1 ), Data_Output_Size );
    MEMORY * Data_Output      = &_Data_Output;
    
    int32    AssetID_Output_Size = _MB( 64 );
    MEMORY  _AssetID_Output      = Memory( calloc( AssetID_Output_Size, 1 ), AssetID_Output_Size );
    MEMORY * AssetID_Output      = &_AssetID_Output;
    
    int32    SoundID_Output_Size = _MB( 1 );
    MEMORY  _SoundID_Output      = Memory( calloc( SoundID_Output_Size, 1 ), SoundID_Output_Size );
    MEMORY * SoundID_Output      = &_SoundID_Output;
    
    int32    TempMemory_Size = _MB( 512 );
    MEMORY  _TempMemory      = Memory( calloc( TempMemory_Size, 1 ), TempMemory_Size );
    MEMORY * TempMemory      = &_TempMemory;
    
    ASSET_OUTPUT  _AssetOutput  = {};
    ASSET_OUTPUT * AssetOutput  = &_AssetOutput;
    AssetOutput->Data_Output    = Data_Output;
    AssetOutput->AssetID_Output = AssetID_Output;
    
    int32 * nTexture = _PushType( Data_Output, int32 );
    
    WriteSegment( AssetID_Output, "\n" );
    WriteSegment( AssetID_Output, "enum TEXTURE_ID {\n" );
    
    { // create and Upload default Debug Texture
        uint32 Width  = 256;
        uint32 Height = 256;
        uint32 nTexel = Width * Height;
        
        uint32 * DefaultTexture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( uint32 iter = 0; iter < nTexel; iter++ ) {
            uint32 iRow = ( iter / 8192 );
            uint32 iCol = ( iter / 32 ) % 8;
            
            if( ( ( ( iRow + iCol ) % 2 ) == 0 ) ) {
                DefaultTexture[ iter ] = 0xFFFF00FF;
            } else {
                DefaultTexture[ iter ] = 0xFF444444;
            }
        }
        PackTexture( AssetOutput, "DefaultTexture", Width, Height, DefaultTexture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // create and Upload white Texture
        uint32 WhiteTexture = 0xFFFFFFFF;
        PackTexture( AssetOutput, "WhiteTexture", 1, 1, &WhiteTexture );
    }
    
    { // Create and Upload Circle Texture
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        vec2  Center   = Vec2( ( flo32 )Width, ( flo32 )Height ) * 0.5f;
        flo32 Radius   = ( flo32 )Width * 0.5f;
        flo32 RadiusSq = Radius * Radius;
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos    = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            flo32 DistSq = GetLengthSq( Pos - Center );
            if( DistSq <= RadiusSq ) {
                Texture[ iTexel ] = ToU32Color( COLOR_WHITE );
            }
        }
        PackTexture( AssetOutput, "Circle", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Arrow Texture
        // NOTE: Arrow points up.
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        vec2 A = Vec2( 0.0f, ( flo32 )Height * 0.5f );
        vec2 B = Vec2( ( flo32 )Width, ( flo32 )Height * 0.5f );
        vec2 C = Vec2( ( flo32 )Width * 0.5f, ( flo32 )Height );
        
        rect Bound = RectBCD( Vec2( ( flo32 )Width * 0.5f, 0.0f ), Vec2( ( flo32 )Width * 0.5f, ( flo32 )Height * 0.5f ) );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos   = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            if( ( IsInTri( Pos, A, B, C ) ) || ( IsInRect( Pos, Bound ) ) ) {
                Texture[ iTexel ] = ToU32Color( COLOR_WHITE );
            }
        }
        PackTexture( AssetOutput, "Arrow", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Arrow With Shadow Texture
        // NOTE: Arrow points up.
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        vec2 A = Vec2( 0.0f, ( flo32 )Height * 0.5f );
        vec2 B = Vec2( ( flo32 )Width, ( flo32 )Height * 0.5f );
        vec2 C = Vec2( ( flo32 )Width * 0.5f, ( flo32 )Height );
        
        rect Bound = RectBCD( Vec2( ( flo32 )Width * 0.5f, 0.0f ), Vec2( ( flo32 )Width * 0.5f, ( flo32 )Height * 0.5f ) );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos   = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            flo32 tGray = Clamp01( Pos.y / ( flo32 )Height );
            tGray = lerp( 1.0f, tGray, 0.1f );
            if( ( IsInTri( Pos, A, B, C ) ) || ( IsInRect( Pos, Bound ) ) ) {
                Texture[ iTexel ] = ToU32Color( COLOR_GRAY( tGray ) );
            }
        }
        PackTexture( AssetOutput, "ArrowWithShadow", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Arrow Button Texture
        // NOTE: Arrow points right.
        int32 Width  = 128;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        rect Bound = RectBLD( Vec2( 0.0f, 0.0f ), Vec2( ( flo32 )Width, ( flo32 )Height ) );
        Bound = AddRadius( Bound, -32.0f );
        
        vec2 A = GetT( Bound, Vec2( 0.0f, 0.0f ) );
        vec2 B = GetT( Bound, Vec2( 1.0f, 0.5f ) );
        vec2 C = GetT( Bound, Vec2( 0.0f, 1.0f ) );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos   = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            if( IsInTri( Pos, A, B, C ) ) {
                Texture[ iTexel ] = ToU32Color( COLOR_GRAY( 0.5f ) );
            } else {
                Texture[ iTexel ] = ToU32Color( COLOR_GRAY( 0.1f ) );
            }
        }
        PackTexture( AssetOutput, "Arrow_Button", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Spotlight Texture
        // NOTE: Spotlight points up.
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        vec2 A = Vec2( ( flo32 )Width * 0.5f, 0.0f );
        vec2 B = Vec2( ( flo32 )Width, ( flo32 )Height );
        vec2 C = Vec2( 0.0f, ( flo32 )Height );
        
        flo32 Margin = 8.0f;
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos   = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            if( IsInTri( Pos, A, B, C ) ) {
                flo32 Alpha = 1.0f - GetT01( Pos.y, ( flo32 )Height * 0.5f, ( flo32 )Height );
                
                flo32 Dist0 = DistPointToEdge( Pos, A, C );
                flo32 Dist1 = DistPointToEdge( Pos, A, B );
                flo32 Dist  = MinValue( Dist0, Dist1 );
                flo32 Alph  = Clamp01( Dist / Margin );
                
                Texture[ iTexel ] = ToU32Color( COLOR_WHITE * ( Alpha * Alpha ) * Alph );
            }
        }
        PackTexture( AssetOutput, "Spotlight", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Glow Texture
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        vec2  Center   = Vec2( ( flo32 )Width, ( flo32 )Height ) * 0.5f;
        flo32 Radius   = ( flo32 )Width * 0.5f;
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos    = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            flo32 Dist   = GetLength( Pos - Center );
            
            if( Dist <= Radius ) {
                flo32 tDist = 1.0f - Clamp01( Dist / Radius );
                Texture[ iTexel ] = ToU32Color( COLOR_WHITE * tDist );
            }
        }
        PackTexture( AssetOutput, "Glow", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Ring Texture
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        flo32 Margin = 32.0f;
        flo32 Radius = ( ( flo32 )Width * 0.5f ) - Margin;
        
        vec2  Center   = Vec2( ( flo32 )Width, ( flo32 )Height ) * 0.5f;
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos    = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            flo32 Dist   = GetLength( Pos - Center );
            flo32 tDist  = 1.0f - Clamp01( fabsf( Dist - Radius ) / Margin );
            
            Texture[ iTexel ] = ToU32Color( COLOR_WHITE * tDist );
        }
        PackTexture( AssetOutput, "Ring", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Pause Menu Button Texture
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        vec4 Background_Color = COLOR_GRAY( 0.25f );
        vec4 Line_Color       = COLOR_GRAY( 0.1f );
        vec2 Line_Dim         = Vec2( 164.0f, 12.0f );
        
        flo32 Margin = 8.0f;
        rect Bound = Rect( 0.0f, 0.0f, ( flo32 )Width, ( flo32 )Height );
        rect Line0 = RectCD( GetT( Bound, Vec2( 0.5f, 0.725f ) ), Line_Dim );
        rect Line1 = RectCD( GetT( Bound, Vec2( 0.5f, 0.5f  ) ), Line_Dim );
        rect Line2 = RectCD( GetT( Bound, Vec2( 0.5f, 0.275f ) ), Line_Dim );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            
            vec4  Color = Background_Color;
            
            flo32 Dist  = DistPointToRectPerimeter( Pos, Bound );
            flo32 tDist = Clamp01( Dist / Margin );
            Color.rgb *= lerp( 0.35f, tDist, 1.0f );
            
            if( IsInRect( Pos, Line0 ) ) {
                Color = Line_Color;
            }
            if( IsInRect( Pos, Line1 ) ) {
                Color = Line_Color;
            }
            if( IsInRect( Pos, Line2 ) ) {
                Color = Line_Color;
            }
            
            Texture[ iTexel ] = ToU32Color( Color );
        }
        PackTexture( AssetOutput, "PauseMenuButton", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Dashed Box Outline Texture
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        flo32 Radius_Outer = 16.0f;
        flo32 Radius_Inner =  8.0f;
        
        rect Bound = Rect( 0.0f, 0.0f, ( flo32 )Width, ( flo32 )Height );
        Bound = AddRadius( Bound, -Radius_Outer );
        
        int32 nStripePerSide = 8;
        flo32  Stripe_Width  = 12.0f;
        flo32  Advance       = ( flo32 )Width / ( flo32 )nStripePerSide;
        
        int32 nStripe       = 0;
        rect   Stripe[ 32 ] = {};
        
        vec2  AtPos = {};
        for( int32 iStripe = 0; iStripe < ( nStripePerSide - 1 ); iStripe++ ) {
            AtPos.y += Advance;
            Stripe[ nStripe++ ] = RectLCD( AtPos, Vec2( ( flo32 )Width, Stripe_Width ) );
        }
        AtPos = {};
        for( int32 iStripe = 0; iStripe < ( nStripePerSide - 1 ); iStripe++ ) {
            AtPos.x += Advance;
            Stripe[ nStripe++ ] = RectBCD( AtPos, Vec2( Stripe_Width, ( flo32 )Height ) );
        }
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos  = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            if( !IsInRect( Pos, Bound ) ) {
                flo32 Dist = DistPointToRect( Pos, Bound );
                if( ( Dist >= Radius_Inner ) && ( Dist <= Radius_Outer ) ) {
                    boo32 IsValid = true;
                    for( int32 iStripe = 0; ( IsValid ) && ( iStripe < nStripe ); iStripe++ ) {
                        if( IsInRect( Pos, Stripe[ iStripe ] ) ) {
                            IsValid = false;
                        }
                    }
                    
                    if( IsValid ) {
                        Texture[ iTexel ] = ToU32Color( COLOR_WHITE );
                    }
                }
            }
        }
        PackTexture( AssetOutput, "DashedBoxOutline", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Solid Box Outline Texture
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        flo32 Radius_Outer = 16.0f;
        flo32 Radius_Inner =  8.0f;
        
        rect Bound = Rect( 0.0f, 0.0f, ( flo32 )Width, ( flo32 )Height );
        Bound = AddRadius( Bound, -Radius_Outer );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos  = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            if( !IsInRect( Pos, Bound ) ) {
                flo32 Dist = DistPointToRect( Pos, Bound );
                if( ( Dist >= Radius_Inner ) && ( Dist <= Radius_Outer ) ) {
                    Texture[ iTexel ] = ToU32Color( COLOR_WHITE );
                }
            }
        }
        PackTexture( AssetOutput, "SolidBoxOutline", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload ToStore Tray Texture
        int32 Width  = 128;
        int32 Height = 128;
        int32 nTexel = Width * Height;
        
        flo32 Margin = 3.0f;
        flo32 Radius = 12.0f;
        
        rect Bound = Rect( 0.0f, 0.0f, ( flo32 )Width, ( flo32 )Height );
        Bound.Left   += ( Margin + Radius );
        Bound.Right  -= ( Margin + Radius );
        Bound.Top    -= ( Margin + Radius );
        Bound.Bottom += ( Margin + Radius );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos  = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            if( IsInRect( Pos, Bound ) ) {
                Texture[ iTexel ] = ToU32Color( COLOR_WHITE );
            } else {
                flo32 Dist = DistPointToRect( Pos, Bound );
                if( Dist <= ( Radius + Margin ) ) {
                    flo32 tMargin = GetT01( Dist, Radius, Radius + Margin );
                    vec4  Color   = lerp( COLOR_WHITE, tMargin, COLOR_GRAY( 0.25f ) );
                    Texture[ iTexel ] = ToU32Color( Color );
                }
            }
        }
        PackTexture( AssetOutput, "ToStore_Tray", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload ToStore Arrow Texture
        // NOTE: Arrow points right.
        int32 Width  = 128;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        rect Bound = RectBLD( Vec2( 0.0f, 0.0f ), Vec2( ( flo32 )Width, ( flo32 )Height ) );
        Bound = AddRadius( Bound, -32.0f );
        
        vec2 A = GetT( Bound, Vec2( 0.0f, 0.0f ) );
        vec2 B = GetT( Bound, Vec2( 1.0f, 0.5f ) );
        vec2 C = GetT( Bound, Vec2( 0.0f, 1.0f ) );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos   = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            if( IsInTri( Pos, A, B, C ) ) {
                Texture[ iTexel ] = ToU32Color( COLOR_WHITE );
            }
        }
        PackTexture( AssetOutput, "ToStore_Arrow", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Lock Texture
        int32 Width  = 64;
        int32 Height = 64;
        int32 nTexel = Width * Height;
        
        flo32 Radius_Outer = ( flo32 )Width * 0.5f;
        flo32 Radius_Inner = Radius_Outer - 10.0f;
        vec2  Center       = Vec2( ( flo32 )Width, ( flo32 )Height ) * 0.5f;
        
        rect  Top    = Rect( 0.0f, ( flo32 )Height * 0.5f, ( flo32 )Width, ( flo32 )Height );
        rect  Bottom = Rect( 0.0f, 0.0f, ( flo32 )Width, ( flo32 )Height * 0.5f - 2.0f );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos   = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            flo32 Dist  = GetLength( Pos - Center );
            
            if( IsInRect( Pos, Bottom ) ) {
                Texture[ iTexel ] = ToU32Color( COLOR_WHITE );
            } else if( ( IsInRect( Pos, Top ) ) && ( Dist <= Radius_Outer ) && ( Dist >= Radius_Inner ) ) {
                Texture[ iTexel ] = ToU32Color( COLOR_WHITE );
            }
        }
        PackTexture( AssetOutput, "Lock", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Checkmark Texture
        int32 Width  = 128;
        int32 Height = 128;
        int32 nTexel = Width * Height;
        
        flo32 Radius    = 4.0f;
        flo32 Thickness = 32.0f;
        flo32 Base      = 72.0f;
        
        flo32 Left  = ( ( flo32 )Width - Base ) * 0.5f;
        flo32 Right = ( flo32 )Width - Left;
        
        rect  Box0 = Rect( Left, 0.0f, Right, Thickness );
        rect  Box1 = Rect( Right - Thickness, 0.0f, Right, ( flo32 )Height );
        
        Box0 = AddRadius( Box0, -Radius );
        Box1 = AddRadius( Box1, -Radius );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos   = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            flo32 Dist0 = DistPointToRect( Pos, Box0 );
            flo32 Dist1 = DistPointToRect( Pos, Box1 );
            
            if( ( Dist0 <= Radius ) || ( Dist1 <= Radius ) ) {
                Texture[ iTexel ] = ToU32Color( COLOR_WHITE );
            }
        }
        PackTexture( AssetOutput, "Checkmark", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Cell Texture
        int32 Width  = 128;
        int32 Height = 128;
        int32 nTexel = Width * Height;
        
        flo32 Edge   = 2.0f;
        flo32 Margin = 32.0f;
        rect  Bound  = Rect( 0.0f, 0.0f, ( flo32 )Width, ( flo32 )Height );
        Bound = AddRadius( Bound, -Edge );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2   Pos  = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            flo32  Dist = DistPointToRectPerimeter( Pos, Bound );
            flo32 tDist = 1.0f - Clamp01( Dist / Margin );
            if( !IsInRect( Pos, Bound ) ) {
                tDist = 1.0f - Clamp01( Dist / Edge );
            }
            
            Texture[ iTexel ] = ToU32Color( COLOR_WHITE * tDist );
        }
        PackTexture( AssetOutput, "Cell", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Cardboard Box Empty Texture
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        flo32 Margin       = 32.0f;
        rect  Front_Bound  = Rect(  0.0f,  0.0f, ( flo32 )Width,         ( flo32 )Height         );
        rect  Back_Bound   = AddRadius( Front_Bound, -Margin );
        vec4  Front_Color  = COLOR_GRAY( 0.8f );
        vec4  Back_Color   = COLOR_GRAY( 0.4f );
        
        vec2 V0 = GetBL( Back_Bound ) - GetBL( Front_Bound );
        vec2 V1 = GetBR( Back_Bound ) - GetBR( Front_Bound );
        vec2 V2 = GetTR( Back_Bound ) - GetTR( Front_Bound );
        vec2 V3 = GetTL( Back_Bound ) - GetTL( Front_Bound );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2 Pos = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            if( IsInRect( Pos, Back_Bound ) ) {
                Texture[ iTexel ] = ToU32Color( Back_Color );
            } else {
                flo32  Dist = DistPointToRect( Pos, Back_Bound );
                flo32 tDist = 1.0f - Clamp01( Dist / Margin );
                
                vec4  Color = lerp( Front_Color, tDist, TowardBlack( Back_Color, 0.1f ) );
                Texture[ iTexel ] = ToU32Color( Color );
            }
        }
        PackTexture( AssetOutput, "CardboardBox_Empty", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Cardboard Box Full Texture
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        rect Box_Bound    = Rect(  0.0f,  0.0f, ( flo32 )Width,         ( flo32 )Height         );
        rect Crease_Bound = RectCD( GetCenter( Box_Bound ), Vec2( ( flo32 )Width, 6.0f ) );
        vec4 Box_Color    = COLOR_WHITE;
        vec4 Crease_Color = COLOR_GRAY( 0.9f );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2 Pos = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            
            vec4  Color = Box_Color;
            flo32 Dist  = DistPointToRectPerimeter( Pos, Box_Bound );
            flo32 tDist = 1.0f - Clamp01( Dist / 8.0f );
            
            if( IsInRect( Pos, Crease_Bound ) ) {
                Color = Crease_Color;
            }
            
            Color = TowardBlack( Color, tDist * 0.5f );
            
            Texture[ iTexel ] = ToU32Color( Color );
        }
        PackTexture( AssetOutput, "CardboardBox_Full", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Cardboard Box Hover Texture
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        flo32 Margin = 20.0f;
        rect  Bound  = Rect(  0.0f,  0.0f, ( flo32 )Width,         ( flo32 )Height         );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2   Pos  = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            flo32  Dist = DistPointToRectPerimeter( Pos, Bound );
            flo32 tDist = 1.0f - Clamp01( Dist / Margin );
            
            Texture[ iTexel ] = ToU32Color( COLOR_WHITE * tDist );
        }
        PackTexture( AssetOutput, "CardboardBox_Hover", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Increase Lockers Icon Texture
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        flo32 Dim  = 128.0f;
        flo32 xMin = ( ( flo32 )Width  - Dim ) * 0.5f;
        flo32 xMax = ( ( flo32 )Width  + Dim ) * 0.5f;
        flo32 yMin = ( ( flo32 )Height - Dim ) * 0.5f;
        flo32 yMax = ( ( flo32 )Height + Dim ) * 0.5f;
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2 Pos = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            if( ( Pos.x >= xMin ) && ( Pos.x <= xMax ) ) {
                Texture[ iTexel ] = ToU32Color( COLOR_WHITE );
            }
            if( ( Pos.y >= yMin ) && ( Pos.y <= yMax ) ) {
                Texture[ iTexel ] = ToU32Color( COLOR_WHITE );
            }
        }
        PackTexture( AssetOutput, "Icon_LockerUp", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Decrease Lockers Icon Texture
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        vec2  Dim   = Vec2( 128.0f, 64.0f );
        rect  Bound = RectCD( Vec2( ( flo32 )Width * 0.5f, ( flo32 )Height * 0.5f ), Dim );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2 Pos = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            if( IsInRect( Pos, Bound ) ) {
                Texture[ iTexel ] = ToU32Color( COLOR_WHITE );
            }
        }
        PackTexture( AssetOutput, "Icon_LockerDown", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Icon Shape : Circle
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        vec2  Center   = Vec2( ( flo32 )Width, ( flo32 )Height ) * 0.5f;
        flo32 Radius   = ( flo32 )Width * 0.5f;
        flo32 RadiusSq = Radius * Radius;
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos    = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            flo32 DistSq = GetLengthSq( Pos - Center );
            if( DistSq <= RadiusSq ) {
                Texture[ iTexel ] = ToU32Color( COLOR_WHITE );
            }
        }
        PackTexture( AssetOutput, "Shape_Circle", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Icon Shape : Cross
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        vec2  Center = Vec2( ( flo32 )Width, ( flo32 )Height ) * 0.5f;
        
        flo32 Offset = 48.0f;
        
        vec2 A = Vec2( Offset, 0.0f );
        vec2 B = Vec2( 0.0f, Offset );
        vec2 C = Vec2( ( flo32 )Width, ( flo32 )Height - Offset );
        
        vec2 Dim = {
            GetLength( C - A ),
            GetLength( B - A ),
        };
        
        orect Bound0 = ORectCD( Center, Dim,  PI * 0.25f );
        orect Bound1 = ORectCD( Center, Dim, -PI * 0.25f );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos    = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            if( ( IsInORect( Pos, Bound0 ) ) || ( IsInORect( Pos, Bound1 ) ) ) {
                Texture[ iTexel ] = ToU32Color( COLOR_WHITE );
            }
        }
        PackTexture( AssetOutput, "Shape_Cross", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Icon Shape : Triangle
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        vec2 A = Vec2( 0.0f, 0.0f );
        vec2 B = Vec2( ( flo32 )Width, 0.0f );
        vec2 C = Vec2( ( flo32 )Width * 0.5f, ( flo32 )Height );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos    = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            if( IsInTri( Pos, A, B, C ) ) {
                Texture[ iTexel ] = ToU32Color( COLOR_WHITE );
            }
        }
        PackTexture( AssetOutput, "Shape_Triangle", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Icon Shape : Square
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            Texture[ iTexel ] = ToU32Color( COLOR_WHITE );
        }
        PackTexture( AssetOutput, "Shape_Square", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Icon Shape : Star
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        flo32 Radius = ( flo32 )Height / ( 1.0f + cosf( ToRadians( 36.0f ) ) );
        flo32 Base   = Radius * 0.5f;
        vec2  Center = Vec2( ( flo32 )Width * 0.5f, ( flo32 )Height - Radius );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 Row = iTexel / Width;
            int32 Col = iTexel % Width;
            
            vec2  Pos = Vec2( ( flo32 )Col, ( flo32 )Row ) + Vec2( 0.5f, 0.5f );
            
            for( int32 iTri = 0; iTri < 5; iTri++ ) {
                vec2 yAxis = ToDirection2D( PI * 0.5f + ( TAU / 5.0f ) * ( flo32 )iTri ) * Radius;
                vec2 xAxis = -GetNormal( GetPerp( yAxis ) ) * Base;
                
                vec2 A = Center - xAxis;
                vec2 B = Center + xAxis;
                vec2 C = Center + yAxis;
                
                if( IsInTri( Pos, A, B, C ) ) {
                    Texture[ iTexel ] = ToU32Color( COLOR_WHITE );
                }
            }
        }
        PackTexture( AssetOutput, "Shape_Star", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Texture for Inside Tunnel
        // Bright center is at tPos = { 0.5f, 1.0f }
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        
        vec2  Center  = Vec2( ( flo32 )Width, ( flo32 )Height ) * 0.5f;
        flo32 Radius  = ( flo32 )Width * 0.5f;
        vec2  CenterA = Vec2( ( flo32 )Width * 0.5f, ( flo32 )Height );
        flo32 RadiusA = Radius * 1.9f;
        
        flo32 GrayA = 1.0f;
        flo32 GrayB = 0.0f;
        
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 iRow = ( iTexel / Width );
            int32 iCol = ( iTexel % Width );
            
            vec2  Pos  = Vec2( ( flo32 )iCol, ( flo32 )iRow ) + Vec2( 0.5f, 0.5f );
            flo32 Dist = GetLength( Pos - Center );
            if( Dist <= Radius ) {
                flo32 DistA = GetLength( Pos - CenterA );
                flo32 t     = Clamp01( DistA / RadiusA );
                flo32 tGray = lerp( GrayA, t, GrayB );
                vec4  Color = COLOR_GRAY( tGray );
                
                Texture[ iTexel ] = ToU32Color( Color );
            }
        }
        PackTexture( AssetOutput, "InsideTunnel", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Texture for Shadow
        // Shadow for Item boxes
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        flo32 Margin = 16.0f;
        rect  Outer  = Rect( 0.0f, 0.0f, ( flo32 )Width, ( flo32 )Height );
        rect  Inner  = AddRadius( Outer, -Margin );
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 iRow = ( iTexel / Width );
            int32 iCol = ( iTexel % Width );
            
            vec2   Pos  = Vec2( ( flo32 )iCol, ( flo32 )iRow ) + Vec2( 0.5f, 0.5f );
            flo32  Dist = DistPointToRect( Pos, Inner );
            flo32 tDist = 1.0f - Clamp01( Dist / Margin );
            
            if( Dist <= Margin ) {
                Texture[ iTexel ] = ToU32Color( COLOR_BLACK * tDist );
            }
        }
        PackTexture( AssetOutput, "Shadow", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
#if 0    
    { // Create and Upload Texture for Display Border
        int32 Width  = 256;
        int32 Height = 256;
        int32 nTexel = Width * Height;
        
        int32 nBlock   = 8;
        int32 PerBlock = Width / nBlock;
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 iRow = ( iTexel / Width );
            int32 iCol = ( iTexel % Width );
            
            int32 xBlock = iCol / PerBlock;
            int32 yBlock = iRow / PerBlock;
            
            vec4 Color0 = COLOR_GRAY( 1.0f );
            vec4 Color1 = COLOR_GRAY( 0.75f );
            
            if( ( ( xBlock + yBlock ) % 2 ) == 0 ) {
                _swap( vec4, Color0, Color1 );
            }
            
            vec2  Pos  = Vec2( ( flo32 )iCol, ( flo32 )iRow ) + Vec2( 0.5f, 0.5f );
            vec2 bPos  = Vec2( ( flo32 )xBlock, ( flo32 )yBlock ) * ( flo32 )PerBlock + Vec2( 0.5f, 0.5f );
            vec2  V    = Pos - bPos;
            flo32 t    = V.x / V.y;
            
            if( t > 1.0f ) {
                Texture[ iTexel ] = ToU32Color( Color0 );
            } else {
                Texture[ iTexel ] = ToU32Color( Color1 );
            }
        }
        PackTexture( AssetOutput, "DisplayBorder", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
#endif
    
    PackBMP( AssetOutput, TempMemory, "Icon_Health" );
    //PackBMP( AssetOutput, TempMemory, "Background" );
    //PackBMP( AssetOutput, TempMemory, "Background_Light" );
    PackBMP( AssetOutput, TempMemory, "Fence" );
    //PackBMP( AssetOutput, TempMemory, "StorageArea" );
    //PackBMP( AssetOutput, TempMemory, "ToFind_Panel" );
    
    
#if 0    
    { // Create and Upload Texture for Background Dust
        int32 Width  = 1920;
        int32 Height = 1080;
        int32 nTexel = Width * Height;
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        
        //for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
        //Texture[ iTexel ] = ToU32Color( COLOR_BLACK );
        //}
        
        int32 nStar = 256;
        for( int32 iStar = 0; iStar < nStar; iStar++ ) {
            int32 iRow = RandomS32InRange( 0, Height - 1 );
            int32 iCol = RandomS32InRange( 0, Width  - 1 );
            
            flo32 tGray = RandomF32InRange( 0.1f, 0.75f );
            vec4  Color = COLOR_GRAY( tGray );
            
            int32 iTexel = iRow * Width + iCol;
            Texture[ iTexel ] = ToU32Color( Color );
        }
        
        PackTexture( AssetOutput, "Background_Dust", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
#endif
    
    { // Create and Upload Background Texture
        // NOTE: Center is near the bottom-right of the texture.
        int32 Width  = 1024;
        int32 Height = 1024;
        int32 nTexel = Width * Height;
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        
        vec2  Center       = Vec2( ( flo32 )Width + 32.0f, -( 128.0f + 64.0f ) );
        flo32 Radius_Outer = ( flo32 )Width * 2.0f;
        flo32 Radius_Inner = Radius_Outer * 0.25f;
        
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 iRow = ( iTexel / Width );
            int32 iCol = ( iTexel % Width );
            
            vec2   Pos  = Vec2( ( flo32 )iCol, ( flo32 )iRow ) + Vec2( 0.5f, 0.5f );
            flo32  Dist = GetLength( Pos - Center );
            flo32 tDist = GetT01( Dist, Radius_Inner, Radius_Outer );
            flo32 tGray = lerp( 0.0f, tDist, 1.0f );
            
            Texture[ iTexel ] = ToU32Color( COLOR_GRAY( tGray ) );
        }
        
        PackTexture( AssetOutput, "Background", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Texture for Storage Area Shadow
        // NOTE: Center is near the bottom-right of the texture.
        int32 Width  = 512;
        int32 Height = 512;
        int32 nTexel = Width * Height;
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        
        vec2  Center       = Vec2( ( flo32 )Width + 16.0f, -( 64.0f + 32.0f ) );
        flo32 Radius_Outer = ( flo32 )Width + 64.0f;
        flo32 Radius_Inner = Radius_Outer * 0.25f;
        
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 iRow = ( iTexel / Width );
            int32 iCol = ( iTexel % Width );
            
            vec2   Pos  = Vec2( ( flo32 )iCol, ( flo32 )iRow ) + Vec2( 0.5f, 0.5f );
            flo32  Dist = GetLength( Pos - Center );
            flo32 tDist = GetT01( Dist, Radius_Inner, Radius_Outer );
            flo32 tGray = lerp( 1.0f, tDist, 0.75f );
            
            Texture[ iTexel ] = ToU32Color( COLOR_GRAY( tGray ) );
        }
        
        PackTexture( AssetOutput, "StorageArea_Shadow", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    { // Create and Upload Texture for Question Area Shadow
        // NOTE: Center is near the top-center of the texture.
        int32 Width  = 512;
        int32 Height = 512;
        int32 nTexel = Width * Height;
        
        uint32 * Texture = _PushArray_Clear( TempMemory, uint32, nTexel );
        
        vec2  Center       = Vec2( ( flo32 )Width * 0.5f, ( flo32 )Height + 128.0f );
        flo32 Radius_Outer = ( flo32 )Height + 64.0f;
        flo32 Radius_Inner = Radius_Outer * 0.25f;
        
        for( int32 iTexel = 0; iTexel < nTexel; iTexel++ ) {
            int32 iRow = ( iTexel / Width );
            int32 iCol = ( iTexel % Width );
            
            vec2   Pos  = Vec2( ( flo32 )iCol, ( flo32 )iRow ) + Vec2( 0.5f, 0.5f );
            flo32  Dist = GetLength( Pos - Center );
            flo32 tDist = GetT01( Dist, Radius_Inner, Radius_Outer );
            flo32 tGray = lerp( 1.0f, tDist, 0.75f );
            
            Texture[ iTexel ] = ToU32Color( COLOR_GRAY( tGray ) );
        }
        
        PackTexture( AssetOutput, "QuestionArea_Shadow", Width, Height, Texture );
        
        _PopArray( TempMemory, uint32, nTexel );
    }
    
    DivideAndPackSpriteSheet( AssetOutput, TempMemory, "Icon_Clipart_Page01", 8, 8 );
    DivideAndPackSpriteSheet( AssetOutput, TempMemory, "Icon_Clipart_Page02", 8, 8 );
    
    WriteSegment( AssetID_Output, "\n" );
    WriteSegment( AssetID_Output, "   TextureID_Count,\n" );
    WriteSegment( AssetID_Output, "};\n\n" );
    
    *nTexture = AssetOutput->nTexture;
    
    { // MESHES
        int32 * nMesh = _PushType( Data_Output, int32 );
        
        PackGeneratedMesh( AssetOutput, TempMemory, MeshID_Line,            GenMesh_Line            );
        PackGeneratedMesh( AssetOutput, TempMemory, MeshID_Rect,            GenMesh_Rect            );
        PackGeneratedMesh( AssetOutput, TempMemory, MeshID_RectOutline,     GenMesh_RectOutline     );
        PackGeneratedMesh( AssetOutput, TempMemory, MeshID_Circle,          GenMesh_Circle          );
        PackGeneratedMesh( AssetOutput, TempMemory, MeshID_CircleOutline,   GenMesh_CircleOutline   );
        PackGeneratedMesh( AssetOutput, TempMemory, MeshID_Triangle,        GenMesh_Triangle        );
        PackGeneratedMesh( AssetOutput, TempMemory, MeshID_Triangle_FlipUV, GenMesh_Triangle_FlipUV );
        PackGeneratedMesh( AssetOutput, TempMemory, MeshID_TriangleOutline, GenMesh_TriangleOutline );
        PackGeneratedMesh( AssetOutput, TempMemory, MeshID_Rect_BRQuad,     GenMesh_Rect_BRQuad );
        
        *nMesh = AssetOutput->nMesh;
    }
    
    { // MUSIC
#if USE_FMOD
        FILE_DATA Master = ReadFile( TempMemory, "Master.bank" );
        if( Master.Contents ) {
            _writem( AssetOutput->Data_Output, int32, Master.Size );
            _writeb( AssetOutput->Data_Output, Master.Contents, Master.Size );
            _PopSize( TempMemory, Master.Size );
        } else {
            printf( "ERROR! Unable to pack FMOD Master.bank\n" );
        }
        
        FILE_DATA SoundEffects = ReadFile( TempMemory, "Sound Effects.bank" );
        if( SoundEffects.Contents ) {
            _writem( AssetOutput->Data_Output, int32, SoundEffects.Size );
            _writeb( AssetOutput->Data_Output, SoundEffects.Contents, SoundEffects.Size );
            _PopSize( TempMemory, SoundEffects.Size );
            
            {
                int32 * nSoundID = _PushType( AssetOutput->Data_Output, int32 );
                int32   nSound   = 0;
                
                FILE_DATA Guid_File = ReadFile( TempMemory, "GUIDs.txt" );
                if( Guid_File.Contents ) {
                    FILE_PARSER  _Parser = FileParser( Guid_File.Contents, Guid_File.Size );
                    FILE_PARSER * Parser = &_Parser;
                    
                    WriteSegment( SoundID_Output, "\nenum SOUND_ID {\n" );
                    
                    while( HasTextRemaining( Parser ) ) {
                        EatWhiteSpace( Parser );
                        Assert( ( char )( Parser->At[ 0 ] ) == '{' );
                        
                        char * Ptr = Parser->At;
                        while( *Ptr != ' ' ) {
                            if( ( *Ptr == '{' ) || ( *Ptr == '}' ) || ( *Ptr == '-' ) ) {
                                *Ptr = ' ';
                            }
                            Ptr++;
                        }
                        
                        uint64 Part[ 5 ] = {};
                        for( int32 iPart = 0; iPart < 5; iPart++ ) {
                            EatWhiteSpace( Parser );
                            Part[ iPart ] = ParseU64_Hex( Parser );
                        }
                        
                        FMOD_GUID Guid = FMODGuid( Part[ 0 ], Part[ 1 ], Part[ 2 ], Part[ 3 ], Part[ 4 ] );
                        
                        EatWhiteSpace( Parser );
                        
                        STRING Str = ParseLine( Parser );
                        for( uint32 iChar = 0; iChar < Str.nChar; iChar++ ) {
                            if( ( Str.String[ iChar ] == ':' ) || ( Str.String[ iChar ] == '/' ) ) {
                                Str.String[ iChar ] = ' ';
                            }
                        }
                        
                        FILE_PARSER  _NameParser = FileParser( Str.String, Str.nChar );
                        FILE_PARSER * NameParser = &_NameParser;
                        
                        STRING Type = ParseToken( NameParser );
                        STRING Name = ParseToken( NameParser );
                        
                        if( MatchString( Type, "event" ) ) {
                            PackSoundID( SoundID_Output, Name.String, Name.nChar );
                            
                            _writem( AssetOutput->Data_Output, FMOD_GUID, Guid );
                            nSound++;
                        }
                        
                        EatWhiteSpace( Parser );
                    }
                    
                    WriteSegment( SoundID_Output, "\n   SoundID_Count,\n};\n\n" );
                    *nSoundID = nSound;
                } else {
                    printf( "ERROR! Unable to open GUID File : GUIDs.txt\n" );
                }
            }
        } else {
            printf( "ERROR! Unable to pack FMOD SoundEffects.bank\n" );
        }
#else
        int32 * nAudio = _PushType( Data_Output, int32 );
        *nAudio = AssetOutput->nAudio;
#endif
    }
    
    WriteFile( "Asset.pak",                    Data_Output->Base,    Data_Output->Used );
    WriteFile( "../../code/AssetID_Temp.h", AssetID_Output->Base, AssetID_Output->Used );
    
    if( SoundID_Output->Used > 0 ) {
        WriteFile( "../../code/SoundID.h",      SoundID_Output->Base, SoundID_Output->Used );
    }
    
    return 0;
}