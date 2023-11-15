


#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_LINK_STATUS                    0x8B82
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRAMEBUFFER                    0x8D40
// NOTE : Some of these values are defined in gl-enums.h, for future reference.

#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB               0x2093
#define WGL_CONTEXT_FLAGS_ARB                     0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126

#define WGL_CONTEXT_DEBUG_BIT_ARB                 0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB    0x0002

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

typedef char GLchar;

typedef GLuint ( GLFUNC_glCreateShader       )( GLenum type );
typedef void   ( GLFUNC_glAttachShader       )( GLuint program, GLuint shader );
typedef void   ( GLFUNC_glCompileShader      )( GLuint shader );
typedef GLuint ( GLFUNC_glCreateProgram      )( void );
typedef GLuint ( GLFUNC_glCreateShader       )( GLenum type );
typedef void   ( GLFUNC_glLinkProgram        )( GLuint program );
typedef void   ( GLFUNC_glShaderSource       )( GLuint shader, GLsizei count, GLchar ** string, GLint * length );
typedef void   ( GLFUNC_glUseProgram         )( GLuint program );
typedef void   ( GLFUNC_glGetProgramInfoLog  )( GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog );
typedef void   ( GLFUNC_glGetShaderInfoLog   )( GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
typedef void   ( GLFUNC_glValidateProgram    )( GLuint program );
typedef void   ( GLFUNC_glGetProgramiv       )( GLuint program, GLenum pname, GLint * params );
typedef GLint  ( GLFUNC_glGetUniformLocation )( GLuint program, const GLchar * name );
//typedef void gl_uniform_4fv(GLint location, GLsizei count, const GLfloat *value);
typedef void  ( GLFUNC_glUniformMatrix4fv )( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );
typedef void  ( GLFUNC_glEnableVertexAttribArray  )( GLuint index);
typedef void  ( GLFUNC_glDisableVertexAttribArray )( GLuint index);
typedef GLint ( GLFUNC_glGetAttribLocation        )( GLuint program, const GLchar * name);
typedef void  ( GLFUNC_glVertexAttribPointer      )( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer );
typedef void  ( GLFUNC_glUniform1i )( GLint location, GLint v0 );
typedef const char * ( GLFUNC_wglGetExtensionsStringARB )( void );

typedef HGLRC ( WGL_CREATE_CONTEXT_ATTRIBS_ARB )( HDC WindowDC, HGLRC ShareContext,
                                                 const int32 * AttribList );
typedef BOOL ( GLFUNC_wglSwapIntervalEXT )( int32 Interval );
typedef void ( GLFUNC_glGenFramebuffers )( GLsizei n, GLuint * ids );
typedef void ( GLFUNC_glBindFramebuffer )( GLenum target, GLuint framebuffer );
typedef void ( GLFUNC_glFramebufferTexture2D )( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level );
typedef GLenum ( GLFUNC_glCheckFramebufferStatus )( GLenum target );

#define OpenGLGlobalFunction( Name ) global_variable GLFUNC_##Name * Name;
OpenGLGlobalFunction( glAttachShader             );
OpenGLGlobalFunction( glCompileShader            );
OpenGLGlobalFunction( glCreateProgram            );
OpenGLGlobalFunction( glCreateShader             );
OpenGLGlobalFunction( glLinkProgram              );
OpenGLGlobalFunction( glShaderSource             );
OpenGLGlobalFunction( glUseProgram               );
OpenGLGlobalFunction( glGetProgramInfoLog        );
OpenGLGlobalFunction( glGetShaderInfoLog         );
OpenGLGlobalFunction( glValidateProgram          );
OpenGLGlobalFunction( glGetProgramiv             );
OpenGLGlobalFunction( glGetUniformLocation       );
OpenGLGlobalFunction( glEnableVertexAttribArray  );
OpenGLGlobalFunction( glDisableVertexAttribArray );
OpenGLGlobalFunction( glGetAttribLocation        );
OpenGLGlobalFunction( glVertexAttribPointer      );
OpenGLGlobalFunction( glUniform1i                );
OpenGLGlobalFunction( glUniformMatrix4fv         );
OpenGLGlobalFunction( wglSwapIntervalEXT         );
OpenGLGlobalFunction( wglGetExtensionsStringARB  );
OpenGLGlobalFunction( glGenFramebuffers          );
OpenGLGlobalFunction( glBindFramebuffer          );
OpenGLGlobalFunction( glFramebufferTexture2D     );
OpenGLGlobalFunction( glCheckFramebufferStatus   );
//global_variable gl_uniform_4fv *glUniform4fv;

struct VERTEX1 {
    vec3 Position;
    vec2 TexCoord;
};

internal VERTEX1
Vertex1( vec3 pos, vec2 tex ) {
    VERTEX1 Result = {};
    Result.Position = pos;
    Result.TexCoord = tex;
    return Result;
}

struct VERTEX1_BUFFER {
    uint32 maxVertex;
    uint32 maxIndex;
    uint32 nVertex;
    uint32 nIndex;
    VERTEX1 * vertex_Start;
    uint32  *  index_Start;
};

struct RENDER_OBJECT {
    MODEL_ID   ModelID;
    TEXTURE_ID TextureID;
    mat4       transform;
    vec4       modColor;
};

struct RENDER_OBJECT_LIST {
    uint32 nObjects;
    uint32 maxObjects;
    RENDER_OBJECT * object;
};

struct TEXT_RENDER_OBJECT {
    TEXTURE_ID TextureID;
    uint32     bVertex;
    uint32     nVertex;
    vec4       modColor;
};

struct TEXT_RENDER_OBJECT_LIST {
    uint32 nObjects;
    uint32 maxObjects;
    TEXT_RENDER_OBJECT * object;
};

struct MODEL_DATA {
    uint32 nIndex;
    uint32 bIndex;
    uint32 bVertex;
};

struct OPENGL_VERTEX {
    vec3   Position;
    vec2   UV;
    uint32 Color;
};

internal OPENGL_VERTEX
OpenGLVertex( vec3 Position, vec2 UV, uint32 Color ) {
    OPENGL_VERTEX Result = {};
    Result.Position = Position;
    Result.UV       = UV;
    Result.Color    = Color;
    return Result;
}

struct OPENGL_DRAW_OBJECT {
    int32      nPrimitive;
    TEXTURE_ID  TextureID;
};

struct OPENGL_DRAW_LIST {
    OPENGL_VERTEX      * Vertex;
    OPENGL_DRAW_OBJECT * Object;
    int32 MaxObject;
    int32   nObject;
    
    int32 MaxVertex;
    int32   nVertex;
};

struct OPENGL_SHADER {
    GLuint ProgramID;
    GLuint TransformID;
    GLuint TextureSamplerID;
    
    GLuint VertPosID;
    GLuint VertUVID;
    GLuint VertColorID;
};

internal void
_AddObjectToDrawList( OPENGL_DRAW_LIST * List, TEXTURE_ID TextureID ) {
    boo32 DoAdd = true;
    if( List->nObject > 0 ) {
        OPENGL_DRAW_OBJECT * LastObject = List->Object + ( List->nObject - 1 );
        if( LastObject->TextureID == TextureID ) {
            LastObject->nPrimitive += 1;
            DoAdd = false;
        }
    }
    if( DoAdd ) {
        OPENGL_DRAW_OBJECT Object = {};
        Object.nPrimitive = 1;
        Object.TextureID  = TextureID;
        List->Object[ List->nObject ] = Object;
        
        List->nObject += 1;
    }
}

struct OPENGL_DISPLAY_SETTINGS {
    uint32 Width;
    uint32 Height;
    uint32 RefreshRate_Numer;
    uint32 RefreshRate_Denom;
};

struct RENDER_COMMAND_CLEAR_FRAMEBUFFER {
    vec4 Color;
};

enum FRAMEBUFFER_ID {
    FramebufferID_Main,
    FramebufferID_Layer,
    
    FramebufferID_Count,
};

struct OPENGL_FRAMEBUFFER {
    GLuint FramebufferID;
    GLuint TextureID;
    int32  Width;
    int32  Height;
    // TODO : HasDepth
    // TODO : HasStencil
};

struct RENDER_COMMAND_SET_FRAMEBUFFER {
    OPENGL_FRAMEBUFFER Framebuffer;
};

struct RENDER_COMMAND_SET_SHADER {
    OPENGL_SHADER Shader;
};

struct RENDER_COMMAND_SET_CLIP_RECT {
    rect ClipRect;
};

struct RENDER_COMMAND_TEXTURED_QUADS {
    int32 nObject;
    int32 bVertex;
};

enum RENDER_COMMAND {
    RenderCommand_ClearFramebuffer,
    RenderCommand_SetFramebuffer,
    RenderCommand_SetShader,
    RenderCommand_SetClipRect,
    RenderCommand_TexturedQuads,
};

struct RENDER_COMMAND_HEADER {
    RENDER_COMMAND Type;
};

struct OPENGL_RENDER_PASS {
    MEMORY CommandBuffer;
    
    mat4   Camera_Transform;
    
    RENDER_COMMAND                  LastCommand;
    OPENGL_DRAW_OBJECT            * LastObject;
    RENDER_COMMAND_TEXTURED_QUADS * LastQuads;
    
    OPENGL_VERTEX      * Vertex;
    int32             MaxVertex;
    int32               nVertex;
    
    //OPENGL_DRAW_LIST Quad;
};

internal void
AddObjectToRenderPass( OPENGL_RENDER_PASS * Pass, TEXTURE_ID TextureID ) {
    if( Pass->LastCommand != RenderCommand_TexturedQuads ) {
        RENDER_COMMAND_HEADER * Header = _PushType( &Pass->CommandBuffer, RENDER_COMMAND_HEADER );
        Header->Type = RenderCommand_TexturedQuads;
        
        Pass->LastCommand = RenderCommand_TexturedQuads;
        Pass->LastObject  = 0;
        Pass->LastQuads   = _PushType_Clear( &Pass->CommandBuffer, RENDER_COMMAND_TEXTURED_QUADS );
        Pass->LastQuads->bVertex = Pass->nVertex;
        Pass->LastQuads->nObject = 0;
    }
    
    OPENGL_DRAW_OBJECT * LastObject = Pass->LastObject;
    if( ( LastObject ) && ( LastObject->TextureID == TextureID ) ) {
        LastObject->nPrimitive += 1;
    } else {
        Assert( Pass->LastQuads );
        Pass->LastQuads->nObject += 1;
        
        LastObject = _PushType( &Pass->CommandBuffer, OPENGL_DRAW_OBJECT );
        LastObject->nPrimitive = 1;
        LastObject->TextureID  = TextureID;
        Pass->LastObject = LastObject;
    }
}

enum SHADER_ID {
    ShaderID_Texture,
    ShaderID_Blur,
    
    ShaderID_Count,
};

struct OPENGL_RENDERER {
    HWND  Window;
    
    boo32 IsValid;
    boo32 IsModernContext;
    boo32 HasVSync;
    
    char * Vendor;
    char * Renderer;
    char * Version;
    char * ShadingLanguageVersion;
    char * Extensions;
    char * wglExtensions;
    
    OPENGL_SHADER Shader[ ShaderID_Count ];
    
    boo32  Texture_IsLoaded[ TextureID_Count ];
    GLuint Texture_Handle  [ TextureID_Count ];
    
    MODEL_DATA     ModelData[ MeshID_Count ];
    
    OPENGL_RENDER_PASS Pass_Game;
    OPENGL_RENDER_PASS Pass_Layer;
    OPENGL_RENDER_PASS Pass_Debug;
    
    OPENGL_FRAMEBUFFER Framebuffer[ FramebufferID_Count ];
};

internal void
InitPass( OPENGL_RENDERER * Renderer, OPENGL_RENDER_PASS * Pass, MEMORY * Memory, int32 MaxQuad ) {
    int32 CommandBuffer_Size = ( sizeof( OPENGL_DRAW_OBJECT ) + sizeof( RENDER_COMMAND_HEADER ) ) * MaxQuad + _KB( 16 );
    Pass->CommandBuffer      = SubArena( Memory, CommandBuffer_Size, false );
    Pass->Vertex             = _PushArray( Memory, OPENGL_VERTEX,      MaxQuad * 4 );
    Pass->MaxVertex          = MaxQuad * 4;
}

#if 0
internal void
RenderPass( OPENGL_RENDERER * Renderer, OPENGL_RENDER_PASS * Pass ) {
    glUniformMatrix4fv( Renderer->TransformID, 1, GL_FALSE, Pass->Camera_Transform.elem );
    
    OPENGL_DRAW_LIST * List = &Pass->Quad;
    
    uint8 * bVertex = ( uint8 * )List->Vertex;
    glVertexAttribPointer( Renderer->VertPosID,   3, GL_FLOAT, false, sizeof( OPENGL_VERTEX ), bVertex + OffsetOf( OPENGL_VERTEX, Position )  );
    glVertexAttribPointer( Renderer->VertUVID,    2, GL_FLOAT, false, sizeof( OPENGL_VERTEX ), bVertex + OffsetOf( OPENGL_VERTEX, UV ) );
    glVertexAttribPointer( Renderer->VertColorID, 4, GL_UNSIGNED_BYTE, true, sizeof( OPENGL_VERTEX ), bVertex + OffsetOf( OPENGL_VERTEX, Color ) );
    
    int32 AtVertex = 0;
    for( int32 iObject = 0; iObject < List->nObject; iObject++ ) {
        OPENGL_DRAW_OBJECT Object = List->Object[ iObject ];
        
        TEXTURE_ID TextureID = Object.TextureID;
        Assert( Renderer->Texture_IsLoaded[ TextureID ] );
        glBindTexture( GL_TEXTURE_2D, ( GLuint )Renderer->Texture_Handle[ TextureID ] );
        
        int32 nVertexPerPrimitive = 4;
        int32 nVertex = Object.nPrimitive * nVertexPerPrimitive;
        glDrawArrays( GL_QUADS, AtVertex, nVertex );
        AtVertex += nVertex;
    }
    
    List->nObject = 0;
    List->nVertex = 0;
}
#endif

internal void
RenderPass( OPENGL_RENDERER * Renderer, OPENGL_RENDER_PASS * Pass, OPENGL_SHADER * CurrentShader  ) {
    MEMORY * Memory = &Pass->CommandBuffer;
    uint8  * Start  = ( uint8 * )Memory->Base;
    uint8  * At     = Start;
    
    while( ( uint64 )( At - Start ) < Memory->Used ) {
        RENDER_COMMAND_HEADER * Header = _addr( At, RENDER_COMMAND_HEADER );
        
        switch( Header->Type ) {
            case RenderCommand_ClearFramebuffer: {
                RENDER_COMMAND_CLEAR_FRAMEBUFFER * Command = _addr( At, RENDER_COMMAND_CLEAR_FRAMEBUFFER );
                
                vec4 Color = Command->Color;
                
                glClearColor( Color.r, Color.g, Color.b, Color.a );
                glClear( GL_COLOR_BUFFER_BIT );
            } break;
            
            case RenderCommand_SetFramebuffer: {
                RENDER_COMMAND_SET_FRAMEBUFFER * Command = _addr( At, RENDER_COMMAND_SET_FRAMEBUFFER );
                
                OPENGL_FRAMEBUFFER Framebuffer = Command->Framebuffer;
                //glBindFramebuffer( GL_FRAMEBUFFER, 0 );
                glBindFramebuffer( GL_FRAMEBUFFER, Framebuffer.FramebufferID );
                glViewport( 0, 0, Framebuffer.Width, Framebuffer.Height );
                
                //glEnable    ( GL_TEXTURE_2D );
                //glEnable    ( GL_SCISSOR_TEST );
                //glScissor   ( 0, 0, ( int32 )( ( flo32 )Screen_Width * tMouseX ), Screen_Height );
                
                //glEnable( GL_BLEND );
                //glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
                //glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
                
                //glDisable( GL_BLEND );
            } break;
            
            case RenderCommand_SetShader: {
                RENDER_COMMAND_SET_SHADER Command = _read( At, RENDER_COMMAND_SET_SHADER );
                
                OPENGL_SHADER Shader = Command.Shader;
                glUseProgram( Shader.ProgramID );
                glUniform1i ( Shader.TextureSamplerID, 0 );
                glEnableVertexAttribArray( Shader.VertPosID   );
                glEnableVertexAttribArray( Shader.VertUVID    );
                glEnableVertexAttribArray( Shader.VertColorID );
                
                *( CurrentShader ) = Shader;
            } break;
            
            case RenderCommand_SetClipRect: {
                RENDER_COMMAND_SET_CLIP_RECT Command = _read( At, RENDER_COMMAND_SET_CLIP_RECT );
                
                rect ClipRect = Command.ClipRect;
                glScissor( ( int32 )ClipRect.Left, ( int32 )ClipRect.Bottom, ( int32 )GetWidth( ClipRect ), ( int32 )GetHeight( ClipRect ) );
            } break;
            
            case RenderCommand_TexturedQuads: {
                RENDER_COMMAND_TEXTURED_QUADS Command = _read( At, RENDER_COMMAND_TEXTURED_QUADS );
                
                glUniformMatrix4fv( CurrentShader->TransformID, 1, GL_FALSE, Pass->Camera_Transform.elem );
                
                uint8 * Vertex = ( uint8 * )Pass->Vertex;
                glVertexAttribPointer( CurrentShader->VertPosID,   3, GL_FLOAT, false, sizeof( OPENGL_VERTEX ), Vertex + OffsetOf( OPENGL_VERTEX, Position )  );
                glVertexAttribPointer( CurrentShader->VertUVID,    2, GL_FLOAT, false, sizeof( OPENGL_VERTEX ), Vertex + OffsetOf( OPENGL_VERTEX, UV ) );
                glVertexAttribPointer( CurrentShader->VertColorID, 4, GL_UNSIGNED_BYTE, true, sizeof( OPENGL_VERTEX ), Vertex + OffsetOf( OPENGL_VERTEX, Color ) );
                
                OPENGL_DRAW_OBJECT * ObjectArray = _addra( At, OPENGL_DRAW_OBJECT, Command.nObject );
                
                int32 AtVertex = Command.bVertex;
                for( int32 iObject = 0; iObject < Command.nObject; iObject++ ) {
                    OPENGL_DRAW_OBJECT Object = ObjectArray[ iObject ];
                    
                    TEXTURE_ID TextureID = Object.TextureID;
                    Assert( Renderer->Texture_IsLoaded[ TextureID ] );
                    glBindTexture( GL_TEXTURE_2D, ( GLuint )Renderer->Texture_Handle[ TextureID ] );
                    
                    int32 nVertexPerPrimitive = 4;
                    int32 nVertex = Object.nPrimitive * nVertexPerPrimitive;
                    glDrawArrays( GL_QUADS, AtVertex, nVertex );
                    
                    AtVertex += nVertex;
                }
            } break;
            
            default: {
                InvalidCodePath;
            } break;
        }
    }
    
    Pass->CommandBuffer.Used = 0;
    Pass->nVertex     = 0;
    Pass->LastCommand = ( RENDER_COMMAND )0;
    Pass->LastObject  = 0;
    Pass->LastQuads   = 0;
}

internal GLuint
OpenGLCreateProgram( char * HeaderCode, char * VertexCode, char * FragmentCode ) {
    GLuint   VertexShaderID        = glCreateShader( GL_VERTEX_SHADER );
    GLchar * VertexShaderCode[ 2 ] =
    {
        HeaderCode,
        VertexCode,
    };
    glShaderSource ( VertexShaderID, _ArrayCount( VertexShaderCode ), VertexShaderCode, 0 );
    glCompileShader( VertexShaderID);
    
    GLuint   FragmentShaderID        = glCreateShader( GL_FRAGMENT_SHADER );
    GLchar * FragmentShaderCode[ 2 ] =
    {
        HeaderCode,
        FragmentCode,
    };
    glShaderSource ( FragmentShaderID, _ArrayCount( FragmentShaderCode ), FragmentShaderCode, 0 );
    glCompileShader( FragmentShaderID );
    
    GLuint ProgramID = glCreateProgram();
    glAttachShader( ProgramID, VertexShaderID );
    glAttachShader( ProgramID, FragmentShaderID );
    glLinkProgram ( ProgramID);
    
    glValidateProgram( ProgramID );
    GLint IsLinked = false;
    glGetProgramiv( ProgramID, GL_LINK_STATUS, &IsLinked );
    if( !IsLinked ) {
        GLsizei Ignored = {};
        char VertexErrors  [ 4096 ];
        char FragmentErrors[ 4096 ];
        char ProgramErrors [ 4096 ];
        glGetShaderInfoLog ( VertexShaderID,   sizeof( VertexErrors   ), &Ignored, VertexErrors   );
        glGetShaderInfoLog ( FragmentShaderID, sizeof( FragmentErrors ), &Ignored, FragmentErrors );
        glGetProgramInfoLog( ProgramID,        sizeof( ProgramErrors  ), &Ignored, ProgramErrors  );
        
        InvalidCodePath;
    }
    
    return ProgramID;
}

#include "GenShader.cpp"

internal OPENGL_RENDERER
InitOpenGL( HWND Window, MEMORY * Memory, OPENGL_DISPLAY_SETTINGS DisplaySettings ) {
    OPENGL_RENDERER Renderer = {};
    
    HDC WindowDC = GetDC( Window );
    Renderer.Window = Window;
    
    PIXELFORMATDESCRIPTOR DesiredPixelFormat = {};
    DesiredPixelFormat.nSize      = sizeof( DesiredPixelFormat );
    DesiredPixelFormat.nVersion   = 1;
    DesiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
    DesiredPixelFormat.dwFlags    = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    DesiredPixelFormat.cColorBits = 32;
    DesiredPixelFormat.cAlphaBits = 8;
    DesiredPixelFormat.iLayerType = PFD_MAIN_PLANE;
    
    int32 iSuggestedPixelFormat = ChoosePixelFormat( WindowDC, &DesiredPixelFormat );
    PIXELFORMATDESCRIPTOR SuggestedPixelFormat = {};
    DescribePixelFormat( WindowDC, iSuggestedPixelFormat,
                        sizeof( SuggestedPixelFormat ), &SuggestedPixelFormat );
    SetPixelFormat( WindowDC, iSuggestedPixelFormat, &SuggestedPixelFormat );
    
    HGLRC OpenGLRC      = wglCreateContext( WindowDC );
    boo32 OpenGLIsValid = false;
    if( wglMakeCurrent( WindowDC, OpenGLRC ) ) {
        boo32 IsModernContext = false;
        
        WGL_CREATE_CONTEXT_ATTRIBS_ARB * wglCreateContextAttribsARB =
            ( WGL_CREATE_CONTEXT_ATTRIBS_ARB * )wglGetProcAddress( "wglCreateContextAttribsARB" );
        if( wglCreateContextAttribsARB ) {
            int Attribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 0,
                WGL_CONTEXT_FLAGS_ARB, 0
#if 0
                | WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#endif
#if DEBUG_BUILD
                | WGL_CONTEXT_DEBUG_BIT_ARB
#endif
                ,
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                0,
            };
            
            HGLRC ModernGLRC = wglCreateContextAttribsARB( WindowDC, 0, Attribs );
            if( ModernGLRC ) {
                if( wglMakeCurrent( WindowDC, ModernGLRC ) ) {
                    IsModernContext = true;
                    wglDeleteContext( OpenGLRC );
                    OpenGLRC = ModernGLRC;
                    
                    OpenGLIsValid = true;
                }
            }
        } else {
            InvalidCodePath;
        }
        
        {
            
            Renderer.IsModernContext = IsModernContext;
            Renderer.Vendor   = ( char * )glGetString( GL_VENDOR   );
            Renderer.Renderer = ( char * )glGetString( GL_RENDERER );
            Renderer.Version  = ( char * )glGetString( GL_VERSION  );
            if( Renderer.IsModernContext ) {
                Renderer.ShadingLanguageVersion = ( char * )glGetString( GL_SHADING_LANGUAGE_VERSION );
            }
            
            Renderer.Extensions    = ( char * )glGetString( GL_EXTENSIONS );
            
#define Win32GetOpenGLFunction( Name ) Name = ( GLFUNC_##Name * )wglGetProcAddress( #Name )
            
            Win32GetOpenGLFunction( wglGetExtensionsStringARB );
            if( wglGetExtensionsStringARB ) {
                Renderer.wglExtensions = ( char * )wglGetExtensionsStringARB();
            }
            
            FILE_PARSER  _glParser = FileParser( Renderer.Extensions,    ( int32 )strlen( Renderer.Extensions    ) );
            FILE_PARSER _wglParser = FileParser( Renderer.wglExtensions, ( int32 )strlen( Renderer.wglExtensions ) );
            FILE_PARSER *  glParser = &_glParser;
            FILE_PARSER * wglParser = &_wglParser;
            
            while( HasTextRemaining( wglParser ) ) {
                STRING Token = ParseToken( wglParser );
                if( MatchString( Token, "WGL_EXT_swap_control" ) ) {
                    Renderer.HasVSync = true;
                }
            }
#if 0            
            char *At = Renderer.Extensions;
            while(*At)
            {
                while(IsWhitespace(*At)) {++At;}
                char *End = At;
                while(*End && !IsWhitespace(*End)) {++End;}
                
                umm Count = End - At;        
                
                if(0) {}
                else if(StringsAreEqual(Count, At, "GL_EXT_texture_sRGB")) {Renderer.GL_EXT_texture_sRGB=true;}
                else if(StringsAreEqual(Count, At, "GL_EXT_framebuffer_sRGB")) {Renderer.GL_EXT_framebuffer_sRGB=true;}
                
                At = End;
            }
#endif
        }
        
        if( Renderer.HasVSync ) {
            Win32GetOpenGLFunction( wglSwapIntervalEXT );
            if( wglSwapIntervalEXT ) {
                wglSwapIntervalEXT( 1 );
            } else {
                Renderer.HasVSync = false;
            }
        }
        
#if 0
        if( OpenGLIsValid ) {
            int32        MaxObject    = 32768;
            int32        MaxVertex    = MaxObject * 4;
            OPENGL_VERTEX * Vertex    = _PushArray_Clear( Memory, OPENGL_VERTEX, MaxVertex );
            TEXTURE_ID    * TextureID = _PushArray_Clear( Memory, TEXTURE_ID, MaxObject );
            
            Renderer.MaxObject = MaxObject;
            Renderer.Vertex    = Vertex;
            Renderer.TextureID = TextureID;
        }
#endif
        
        if( OpenGLIsValid ) {
            Win32GetOpenGLFunction( glAttachShader             );
            Win32GetOpenGLFunction( glCompileShader            );
            Win32GetOpenGLFunction( glCreateProgram            );
            Win32GetOpenGLFunction( glCreateShader             );
            Win32GetOpenGLFunction( glLinkProgram              );
            Win32GetOpenGLFunction( glShaderSource             );
            Win32GetOpenGLFunction( glUseProgram               );
            Win32GetOpenGLFunction( glGetProgramInfoLog        );
            Win32GetOpenGLFunction( glGetShaderInfoLog         );
            Win32GetOpenGLFunction( glValidateProgram          );
            Win32GetOpenGLFunction( glGetProgramiv             );
            Win32GetOpenGLFunction( glGetUniformLocation       );
            Win32GetOpenGLFunction( glEnableVertexAttribArray  );
            Win32GetOpenGLFunction( glDisableVertexAttribArray );
            Win32GetOpenGLFunction( glGetAttribLocation        );
            Win32GetOpenGLFunction( glVertexAttribPointer      );
            Win32GetOpenGLFunction( glUniform1i                );
            Win32GetOpenGLFunction( glUniformMatrix4fv         );
            Win32GetOpenGLFunction( glGenFramebuffers          );
            Win32GetOpenGLFunction( glBindFramebuffer          );
            Win32GetOpenGLFunction( glFramebufferTexture2D     );
            Win32GetOpenGLFunction( glCheckFramebufferStatus   );
        }
        
        if( OpenGLIsValid ) {
            Renderer.Shader[ ShaderID_Texture ] = GenShader_Texture();
            //Renderer.Shader[ ShaderID_Blur    ] = GenShader_Blur();
        }
        
        if( OpenGLIsValid ) {
            OPENGL_FRAMEBUFFER Framebuffer = {};
            Framebuffer.FramebufferID = 0;
            Framebuffer.TextureID     = 0;
            Framebuffer.Width         = DisplaySettings.Width;
            Framebuffer.Height        = DisplaySettings.Height;
            Renderer.Framebuffer[ FramebufferID_Main ] = Framebuffer;
        }
        
        if( OpenGLIsValid ) {
            GLuint FramebufferID = {};
            glGenFramebuffers( 1, &FramebufferID );
            glBindFramebuffer( GL_FRAMEBUFFER, FramebufferID );
            
            int32 Width  = DisplaySettings.Width;
            int32 Height = DisplaySettings.Height;
            
            GLuint TextureID = {};
            glGenTextures( 1, &TextureID );
            glBindTexture( GL_TEXTURE_2D, TextureID );
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
            
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
            glBindTexture( GL_TEXTURE_2D, 0 );
            
            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureID, 0 );
            
            if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE ) {
                OPENGL_FRAMEBUFFER Framebuffer = {};
                Framebuffer.FramebufferID = FramebufferID;
                Framebuffer.TextureID     = TextureID;
                Framebuffer.Width         = DisplaySettings.Width;
                Framebuffer.Height        = DisplaySettings.Height;
                Renderer.Framebuffer[ FramebufferID_Layer ] = Framebuffer;
            } else {
                InvalidCodePath;
                OpenGLIsValid = false;
            }
            
            glBindFramebuffer( GL_FRAMEBUFFER, 0 );
            
            int32 BreakHere = 10;
        }
    } else {
        InvalidCodePath;
    }
    
    ReleaseDC( Window, WindowDC );
    
    return Renderer;
}

internal void
UploadTexture( OPENGL_RENDERER * Renderer, TEXTURE_ID TextureID, uint32 Width, uint32 Height, void * Data ) {
    GLuint Handle = {};
    glGenTextures( 1, &Handle );
    glBindTexture( GL_TEXTURE_2D, Handle );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data );
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    
    glBindTexture( GL_TEXTURE_2D, 0 );
    
    Renderer->Texture_IsLoaded[ TextureID ] = true;
    Renderer->Texture_Handle  [ TextureID ] = Handle;
}

internal void
ClearFramebuffer( OPENGL_RENDER_PASS * Pass, vec4 Color ) {
    MEMORY * Memory = &Pass->CommandBuffer;
    
    RENDER_COMMAND_HEADER * Header = _PushType_Clear( Memory, RENDER_COMMAND_HEADER );
    Header->Type = RenderCommand_ClearFramebuffer;
    
    RENDER_COMMAND_CLEAR_FRAMEBUFFER * Command = _PushType_Clear( Memory, RENDER_COMMAND_CLEAR_FRAMEBUFFER );
    Command->Color = Color;
    
    Pass->LastCommand = RenderCommand_ClearFramebuffer;
    Pass->LastObject  = 0;
    Pass->LastQuads   = 0;
}

internal void
SetFramebuffer( OPENGL_RENDERER * Renderer, OPENGL_RENDER_PASS * Pass, FRAMEBUFFER_ID FramebufferID ) {
    MEMORY * Memory = &Pass->CommandBuffer;
    
    RENDER_COMMAND_HEADER * Header = _PushType_Clear( Memory, RENDER_COMMAND_HEADER );
    Header->Type = RenderCommand_SetFramebuffer;
    
    RENDER_COMMAND_SET_FRAMEBUFFER * Command = _PushType_Clear( Memory, RENDER_COMMAND_SET_FRAMEBUFFER );
    Command->Framebuffer = Renderer->Framebuffer[ FramebufferID ];
    
    Pass->LastCommand = RenderCommand_SetFramebuffer;
    Pass->LastObject  = 0;
    Pass->LastQuads   = 0;
}

internal void
SetShader( OPENGL_RENDERER * Renderer, OPENGL_RENDER_PASS * Pass, SHADER_ID ShaderID ) {
    MEMORY * Memory = &Pass->CommandBuffer;
    
    RENDER_COMMAND_HEADER * Header = _PushType_Clear( Memory, RENDER_COMMAND_HEADER );
    Header->Type = RenderCommand_SetShader;
    
    RENDER_COMMAND_SET_SHADER * Command = _PushType_Clear( Memory, RENDER_COMMAND_SET_SHADER );
    Command->Shader = Renderer->Shader[ ShaderID ];
    
    Pass->LastCommand = RenderCommand_SetShader;
    Pass->LastObject  = 0;
    Pass->LastQuads   = 0;
}

internal void
SetClipRect( OPENGL_RENDERER * Renderer, OPENGL_RENDER_PASS * Pass, rect ClipRect ) {
    MEMORY * Memory = &Pass->CommandBuffer;
    
    RENDER_COMMAND_HEADER * Header = _PushType_Clear( Memory, RENDER_COMMAND_HEADER );
    Header->Type = RenderCommand_SetClipRect;
    
    RENDER_COMMAND_SET_CLIP_RECT * Command = _PushType_Clear( Memory, RENDER_COMMAND_SET_CLIP_RECT );
    Command->ClipRect = ClipRect;
    
    Pass->LastCommand = RenderCommand_SetClipRect;
    Pass->LastObject  = 0;
    Pass->LastQuads   = 0;
}

internal void
RenderGame( OPENGL_DISPLAY_SETTINGS DisplaySettings, OPENGL_RENDERER * Renderer ) {
    //glViewport  ( 0, 0, DisplaySettings.Width, DisplaySettings.Height );
    //glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    //glClear( GL_COLOR_BUFFER_BIT );
    
    glViewport  ( 0, 0, DisplaySettings.Width, DisplaySettings.Height );
    
    glEnable    ( GL_TEXTURE_2D );
    glEnable    ( GL_SCISSOR_TEST );
    glScissor   ( 0, 0, DisplaySettings.Width, DisplaySettings.Height );
    
    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
    //glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    OPENGL_SHADER DefaultShader = Renderer->Shader[ ShaderID_Texture ];
    glUseProgram( DefaultShader.ProgramID );
    glUniform1i ( DefaultShader.TextureSamplerID, 0 );
    glEnableVertexAttribArray( DefaultShader.VertPosID   );
    glEnableVertexAttribArray( DefaultShader.VertUVID    );
    glEnableVertexAttribArray( DefaultShader.VertColorID );
    
    OPENGL_SHADER CurrentShader = DefaultShader;
    RenderPass( Renderer, &Renderer->Pass_Game,  &CurrentShader );
    RenderPass( Renderer, &Renderer->Pass_Layer, &CurrentShader );
    RenderPass( Renderer, &Renderer->Pass_Debug, &CurrentShader );
    
    glUseProgram( 0 );
    glDisableVertexAttribArray( CurrentShader.VertPosID   );
    glDisableVertexAttribArray( CurrentShader.VertUVID    );
    glDisableVertexAttribArray( CurrentShader.VertColorID );
    
    glDisable( GL_SCISSOR_TEST );
    glDisable( GL_BLEND );
}

typedef OPENGL_RENDERER             RENDERER;
typedef OPENGL_RENDER_PASS          RENDER_PASS;
typedef OPENGL_DRAW_LIST            DRAW_LIST;
typedef OPENGL_DISPLAY_SETTINGS     DISPLAY_SETTINGS;