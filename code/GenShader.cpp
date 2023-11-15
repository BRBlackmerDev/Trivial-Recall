
internal OPENGL_SHADER
GenShader_Texture() {
    char * HeaderCode = R"FOO(
// Header Code
                 #version 150
    )FOO";
    
    char * VertexCode = R"FOO(
    // Vertex Code
    uniform mat4x4 Transform;
in vec4 VertPos;
in vec2 VertUV;
in vec4 VertColor;
    smooth out vec2 FragUV;
    smooth out vec4 FragColor;
    void main( void ) {
gl_Position = Transform * VertPos;
    
        FragUV    = VertUV;
        FragColor = VertColor;
    }
    )FOO";
    
    char * FragmentCode = R"FOO(
    // Fragment Code
    uniform sampler2D TextureSampler;
    out vec4 ResultColor;
    smooth in vec2 FragUV;
    smooth in vec4 FragColor;
    void main( void ) {
        vec4 TexSample = texture( TextureSampler, FragUV );
ResultColor = FragColor * TexSample;
    }
    )FOO";
    
    OPENGL_SHADER Shader = {};
    Shader.ProgramID        = OpenGLCreateProgram( HeaderCode, VertexCode, FragmentCode );
    Shader.TransformID      = glGetUniformLocation( Shader.ProgramID, "Transform" );
    Shader.TextureSamplerID = glGetUniformLocation( Shader.ProgramID, "TextureSampler" );
    Shader.VertPosID        = glGetAttribLocation ( Shader.ProgramID, "VertPos"   );
    Shader.VertUVID         = glGetAttribLocation ( Shader.ProgramID, "VertUV"    );
    Shader.VertColorID      = glGetAttribLocation ( Shader.ProgramID, "VertColor" );
    return Shader;
}


#if 0
internal OPENGL_SHADER
GenShader_Blur() {
    char * HeaderCode = R"FOO(
// Header Code
                 #version 150
    )FOO";
    
    char * VertexCode = R"FOO(
    // Vertex Code
    uniform mat4x4 Transform;
in vec4 VertPos;
in vec2 VertUV;
in vec4 VertColor;
    smooth out vec2 FragUV;
    smooth out vec4 FragColor;
    void main( void ) {
gl_Position = Transform * VertPos;
    
        FragUV    = VertUV;
        FragColor = VertColor;
    }
    )FOO";
    
    char * FragmentCode = R"FOO(
    // Fragment Code
    uniform sampler2D TextureSampler;
    out vec4 ResultColor;
    smooth in vec2 FragUV;
    smooth in vec4 FragColor;
uniform float Offset[ 3 ] = float[]( 0.0f,          1.3846153846f, 3.2307692308f );
uniform float Weight[ 3 ] = float[]( 0.2270270270f, 0.3162162162f, 0.0702702703f );
    void main( void ) {
vec4 Sample = vec4( 0.0f, 0.0f, 0.0f, 0.0f );
		 Sample += texture( TextureSampler, FragUV ) * Weight[ 0 ];
	for( int Iter = 1; Iter < 3; Iter++ ) {
		 Sample += texture( TextureSampler, ( FragUV + vec2( Offset[ Iter ] / 1920.0f, 0.0f ) ) ) * Weight[ Iter ];
		 Sample += texture( TextureSampler, ( FragUV - vec2( Offset[ Iter ] / 1920.0f, 0.0f ) ) ) * Weight[ Iter ];
		 Sample += texture( TextureSampler, ( FragUV + vec2( 0.0f, Offset[ Iter ] / 1080.0f ) ) ) * Weight[ Iter ];
		 Sample += texture( TextureSampler, ( FragUV - vec2( 0.0f, Offset[ Iter ] / 1080.0f ) ) ) * Weight[ Iter ];
	}
ResultColor = Sample;
    }
    )FOO";
    
    OPENGL_SHADER Shader = {};
    Shader.ProgramID        = OpenGLCreateProgram( HeaderCode, VertexCode, FragmentCode );
    Shader.TransformID      = glGetUniformLocation( Shader.ProgramID, "Transform" );
    Shader.TextureSamplerID = glGetUniformLocation( Shader.ProgramID, "TextureSampler" );
    Shader.VertPosID        = glGetAttribLocation ( Shader.ProgramID, "VertPos"   );
    Shader.VertUVID         = glGetAttribLocation ( Shader.ProgramID, "VertUV"    );
    Shader.VertColorID      = glGetAttribLocation ( Shader.ProgramID, "VertColor" );
    return Shader;
}
#endif
