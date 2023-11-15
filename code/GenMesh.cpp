
struct MESH {
    vec3  * Vertex;
    vec2  * UV;
    int32 * Index;
    
    int32  nVertex;
    int32  nUV;
    int32  nIndex;
};

typedef MESH ( GEN_MESH )( MEMORY * Memory );

internal MESH
InitMesh( MEMORY * Memory, int32 nVertex, int32 nIndex ) {
    MESH Result = {};
    Result.Vertex  = _PushArray_Clear( Memory, vec3,  nVertex );
    Result.UV      = _PushArray_Clear( Memory, vec2,  nVertex );
    Result.Index   = _PushArray_Clear( Memory, int32, nIndex  );
    Result.nVertex = nVertex;
    Result.nUV     = nVertex;
    Result.nIndex  = nIndex;
    return Result;
}

internal MESH
GenMesh_Line( MEMORY * Memory ) {
    MESH Result = InitMesh( Memory, 2, 2 );
    
    Result.Vertex[ 0 ] = Vec3( 0.0f, 0.0f, 0.0f );
    Result.Vertex[ 1 ] = Vec3( 1.0f, 0.0f, 0.0f );
    
    Result.UV[ 0 ] = Vec2( 0.0f, 0.0f );
    Result.UV[ 1 ] = Vec2( 0.0f, 0.0f );
    
    Result.Index[ 0 ] = 0;
    Result.Index[ 1 ] = 1;
    
    return Result;
}

internal MESH
GenMesh_Rect( MEMORY * Memory ) {
    MESH Result = InitMesh( Memory, 4, 6 );
    
    Result.Vertex[ 0 ] = Vec3( -1.0f, -1.0f, 0.0f );
    Result.Vertex[ 1 ] = Vec3(  1.0f, -1.0f, 0.0f );
    Result.Vertex[ 2 ] = Vec3( -1.0f,  1.0f, 0.0f );
    Result.Vertex[ 3 ] = Vec3(  1.0f,  1.0f, 0.0f );
    
    Result.UV[ 0 ] = Vec2( 0.0f, 0.0f );
    Result.UV[ 1 ] = Vec2( 1.0f, 0.0f );
    Result.UV[ 2 ] = Vec2( 0.0f, 1.0f );
    Result.UV[ 3 ] = Vec2( 1.0f, 1.0f );
    
    Result.Index[ 0 ] = 0;
    Result.Index[ 1 ] = 1;
    Result.Index[ 2 ] = 2;
    Result.Index[ 3 ] = 3;
    Result.Index[ 4 ] = 2;
    Result.Index[ 5 ] = 1;
    
    return Result;
}

internal MESH
GenMesh_Rect_BRQuad( MEMORY * Memory ) {
    MESH Result = InitMesh( Memory, 4, 6 );
    
    Result.Vertex[ 0 ] = Vec3(  0.0f, -1.0f, 0.0f );
    Result.Vertex[ 1 ] = Vec3(  1.0f, -1.0f, 0.0f );
    Result.Vertex[ 2 ] = Vec3(  0.0f,  0.0f, 0.0f );
    Result.Vertex[ 3 ] = Vec3(  1.0f,  0.0f, 0.0f );
    
    Result.UV[ 0 ] = Vec2( 0.5f, 0.0f );
    Result.UV[ 1 ] = Vec2( 1.0f, 0.0f );
    Result.UV[ 2 ] = Vec2( 0.5f, 0.5f );
    Result.UV[ 3 ] = Vec2( 1.0f, 0.5f );
    
    Result.Index[ 0 ] = 0;
    Result.Index[ 1 ] = 1;
    Result.Index[ 2 ] = 2;
    Result.Index[ 3 ] = 3;
    Result.Index[ 4 ] = 2;
    Result.Index[ 5 ] = 1;
    
    return Result;
}

internal MESH
GenMesh_RectOutline( MEMORY * Memory ) {
    MESH Result = InitMesh( Memory, 4, 5 );
    
    Result.Vertex[ 0 ] = Vec3( -1.0f, -1.0f, 0.0f );
    Result.Vertex[ 1 ] = Vec3(  1.0f, -1.0f, 0.0f );
    Result.Vertex[ 2 ] = Vec3( -1.0f,  1.0f, 0.0f );
    Result.Vertex[ 3 ] = Vec3(  1.0f,  1.0f, 0.0f );
    
    Result.UV[ 0 ] = Vec2( 0.0f, 0.0f );
    Result.UV[ 1 ] = Vec2( 1.0f, 0.0f );
    Result.UV[ 2 ] = Vec2( 0.0f, 1.0f );
    Result.UV[ 3 ] = Vec2( 1.0f, 1.0f );
    
    Result.Index[ 0 ] = 0;
    Result.Index[ 1 ] = 1;
    Result.Index[ 2 ] = 3;
    Result.Index[ 3 ] = 2;
    Result.Index[ 4 ] = 0;
    
    return Result;
}

internal MESH
GenMesh_Circle( MEMORY * Memory ) {
    int32 nSegment = 64;
    
    MESH Result = InitMesh( Memory, nSegment + 1, nSegment * 3 );
    
    for( int32 iSegment = 0; iSegment < nSegment; iSegment++ ) {
        flo32 Radians = ( ( flo32 )iSegment / ( flo32 )nSegment ) * TAU;
        flo32 X = cosf( Radians );
        flo32 Y = sinf( Radians );
        
        Result.Vertex[ iSegment ] = Vec3( X, Y, 0.0f );
        Result.UV    [ iSegment ] = Vec2( 0.0f, 0.0f );
    }
    
    for( int32 iSegment = 0; iSegment < nSegment; iSegment++ ) {
        int32 bIndex = iSegment * 3;
        Result.Index[ bIndex + 0 ] = Result.nVertex;
        Result.Index[ bIndex + 1 ] = iSegment;                    // [0..63]
        Result.Index[ bIndex + 2 ] = ( iSegment + 1 ) % nSegment; // [1..64]
    }
    
    return Result;
}

internal MESH
GenMesh_CircleOutline( MEMORY * Memory ) {
    int32 nSegment = 64;
    
    MESH Result = InitMesh( Memory, nSegment, nSegment + 1 );
    
    for( int32 iSegment = 0; iSegment < nSegment; iSegment++ ) {
        flo32 Radians = ( ( flo32 )iSegment / ( flo32 )nSegment ) * TAU;
        flo32 X = cosf( Radians );
        flo32 Y = sinf( Radians );
        
        Result.Vertex[ iSegment ] = Vec3( X, Y, 0.0f );
        Result.UV    [ iSegment ] = Vec2( 0.0f, 0.0f );
    }
    
    for( int32 iSegment = 0; iSegment < nSegment; iSegment++ ) {
        Result.Index[ iSegment ] = iSegment;
    }
    
    return Result;
}

internal MESH
GenMesh_Triangle( MEMORY * Memory ) {
    MESH Result = InitMesh( Memory, 3, 3 );
    
    Result.Vertex[ 0 ] = Vec3( -1.0f, -1.0f, 0.0f );
    Result.Vertex[ 1 ] = Vec3(  1.0f, -1.0f, 0.0f );
    Result.Vertex[ 2 ] = Vec3(  0.0f,  1.0f, 0.0f );
    
    Result.UV[ 0 ] = Vec2( 0.0f, 0.0f );
    Result.UV[ 1 ] = Vec2( 1.0f, 0.0f );
    Result.UV[ 2 ] = Vec2( 0.5f, 1.0f );
    
    Result.Index[ 0 ] = 0;
    Result.Index[ 1 ] = 1;
    Result.Index[ 2 ] = 2;
    
    return Result;
}

internal MESH
GenMesh_Triangle_FlipUV( MEMORY * Memory ) {
    MESH Result = InitMesh( Memory, 3, 3 );
    
    Result.Vertex[ 0 ] = Vec3( -1.0f, -1.0f, 0.0f );
    Result.Vertex[ 1 ] = Vec3(  1.0f, -1.0f, 0.0f );
    Result.Vertex[ 2 ] = Vec3(  0.0f,  1.0f, 0.0f );
    
    Result.UV[ 0 ] = Vec2( 0.0f, 1.0f );
    Result.UV[ 1 ] = Vec2( 1.0f, 1.0f );
    Result.UV[ 2 ] = Vec2( 0.5f, 0.0f );
    
    Result.Index[ 0 ] = 0;
    Result.Index[ 1 ] = 1;
    Result.Index[ 2 ] = 2;
    
    return Result;
}

internal MESH
GenMesh_TriangleOutline( MEMORY * Memory ) {
    MESH Result = InitMesh( Memory, 3, 4 );
    
    Result.Vertex[ 0 ] = Vec3( -1.0f, -1.0f, 0.0f );
    Result.Vertex[ 1 ] = Vec3(  1.0f, -1.0f, 0.0f );
    Result.Vertex[ 2 ] = Vec3(  0.0f,  1.0f, 0.0f );
    
    Result.UV[ 0 ] = Vec2( 0.0f, 0.0f );
    Result.UV[ 1 ] = Vec2( 0.0f, 0.0f );
    Result.UV[ 2 ] = Vec2( 0.0f, 0.0f );
    
    Result.Index[ 0 ] = 0;
    Result.Index[ 1 ] = 1;
    Result.Index[ 2 ] = 2;
    Result.Index[ 3 ] = 0;
    
    return Result;
}