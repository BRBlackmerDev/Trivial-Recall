

//----------
// DRAW
//----------

#if 0
internal void
DrawModel( RENDER_OBJECT_LIST * objectList, MODEL_ID ModelID, TEXTURE_ID TextureID, mat4 transform, vec4 modColor ) {
    if( objectList->nObjects < objectList->maxObjects ) {
        RENDER_OBJECT object = {};
        object.ModelID   = ModelID;
        object.TextureID = TextureID;
        object.transform = transform;
        object.modColor  = modColor;
        
        objectList->object[ objectList->nObjects++ ] = object;
    } else {
        char str[ 512 ] = {};
        sprintf( str, "ERROR! Render Object List ran out of objects for rendering Models! Max Count = %u", objectList->maxObjects );
        CONSOLE_ERROR( str );
    }
}

internal void
DrawModel( RENDER_PASS * Pass, MODEL_ID ModelID, TEXTURE_ID TextureID, mat4 Transform, vec4 modColor ) {
    DrawModel( &Pass->TriObjectList, ModelID, TextureID, Transform, modColor );
}

internal void
DrawModel( RENDER_PASS * Pass, MODEL_ID ModelID, TEXTURE_ID TextureID, vec3 Position, vec4 modColor ) {
    mat4 transform = mat4_translate( Position );
    
    DrawModel( &Pass->TriObjectList, ModelID, TextureID, transform, modColor );
}

internal void
DrawModel( RENDER_PASS * Pass, MODEL_ID ModelID, TEXTURE_ID TextureID, vec3 Position, vec3 Scale, quat Orientation, vec4 modColor ) {
    mat4 transform = mat4_translate( Position ) * mat4_quaternion( Orientation ) * mat4_Scale( Scale );
    
    DrawModel( &Pass->TriObjectList, ModelID, TextureID, transform, modColor );
}

internal void
DrawModel( RENDER_PASS * Pass, MODEL_ID ModelID, TEXTURE_ID TextureID, vec2 Position, vec2 Scale, vec4 modColor ) {
    mat4 transform = mat4_translate( Position, 0.0f ) * mat4_Scale( Scale, 1.0f );
    
    DrawModel( &Pass->TriObjectList, ModelID, TextureID, transform, modColor );
}

internal void
DrawModel( RENDER_PASS * Pass, MODEL_ID ModelID, TEXTURE_ID TextureID, vec2 Position, vec2 Scale, flo32 Radians, vec4 modColor ) {
    mat4 transform = mat4_translate( Position, 0.0f ) * mat4_rotate( Vec3( 0.0f, 0.0f, 1.0f ), Radians ) * mat4_Scale( Scale, 1.0f );
    
    DrawModel( &Pass->TriObjectList, ModelID, TextureID, transform, modColor );
}

internal void
DrawQuad( RENDER_PASS * Pass, TEXTURE_ID TextureID, vec2 Position, vec2 xAxis, vec2 yAxis, vec4 modColor ) {
    mat4 transform = mat4_translate( Position, 0.0f ) * mat4_column( Vec3( xAxis, 0.0f ), Vec3( yAxis, 0.0f ), Vec3( 0.0f, 0.0f, 1.0f ) );
    DrawModel( &Pass->TriObjectList, MeshID_Rect, TextureID, transform, modColor );
}

internal void
DrawQuadOutline( RENDER_PASS * Pass, vec2 Position, vec2 xAxis, vec2 yAxis, vec4 modColor ) {
    mat4 transform = mat4_translate( Position, 0.0f ) * mat4_column( Vec3( xAxis, 0.0f ), Vec3( yAxis, 0.0f ), Vec3( 0.0f, 0.0f, 1.0f ) );
    DrawModel( &Pass->LineObjectList, MeshID_RectOutline, TextureID_WhiteTexture, transform, modColor );
}

internal void
DrawQuad( RENDER_PASS * Pass, TEXTURE_ID TextureID, vec2 Position, flo32 radians, vec2 Scale, vec4 modColor ) {
    vec2 xAxis = Vec2( cosf( radians ), sinf( radians ) );
    vec2 yAxis = GetPerp( xAxis );
    
    xAxis *= Scale.x;
    yAxis *= Scale.y;
    
    DrawQuad( Pass, TextureID, Position, xAxis, yAxis, modColor );
}

internal void
DrawQuad( RENDER_PASS * Pass, TEXTURE_ID TextureID, vec2 Position, vec2 Scale, vec4 modColor ) {
    mat4 transform = mat4_translate( Position, 0.0f ) * mat4_Scale( Scale, 1.0f );
    DrawModel( &Pass->TriObjectList, MeshID_Rect, TextureID, transform, modColor );
}

internal void
DrawQuadOutline( RENDER_PASS * Pass, vec2 Position, vec2 Scale, vec4 modColor ) {
    mat4 transform = mat4_translate( Position, 0.0f ) * mat4_Scale( Scale, 1.0f );
    DrawModel( &Pass->LineObjectList, MeshID_RectOutline, TextureID_WhiteTexture, transform, modColor );
}
#endif

internal void
DrawLine( RENDER_PASS * Pass, vec2 P, vec2 Q, vec4 Color ) {
#if 0    
    DRAW_LIST * List = &Pass->Line;
    Assert( List->nObject < List->MaxObject );
    
    OPENGL_VERTEX * Vertex = List->Vertex + List->nVertex;
    Vertex[ 0 ] = OpenGLVertex( Vec3( P, 0.0f ), Vec2( 0.0f, 0.0f ), ToU32Color( Color ) );
    Vertex[ 1 ] = OpenGLVertex( Vec3( Q, 0.0f ), Vec2( 0.0f, 0.0f ), ToU32Color( Color ) );
    List->nVertex += 2;
    Assert( List->nVertex < List->MaxVertex );
    
    AddObjectToDrawList( List, TextureID_WhiteTexture );
#endif
    
    AddObjectToRenderPass( Pass, TextureID_WhiteTexture );
    vec2 Offset = GetPerp( GetNormal( Q - P ) ) * 0.5f;
    
    OPENGL_VERTEX * Vertex = Pass->Vertex + Pass->nVertex;
    Vertex[ 0 ] = OpenGLVertex( Vec3( P + Offset, 0.0f ), Vec2( 0.0f, 0.0f ), ToU32Color( Color ) );
    Vertex[ 1 ] = OpenGLVertex( Vec3( P - Offset, 0.0f ), Vec2( 1.0f, 0.0f ), ToU32Color( Color ) );
    Vertex[ 2 ] = OpenGLVertex( Vec3( Q - Offset, 0.0f ), Vec2( 1.0f, 1.0f ), ToU32Color( Color ) );
    Vertex[ 3 ] = OpenGLVertex( Vec3( Q + Offset, 0.0f ), Vec2( 0.0f, 1.0f ), ToU32Color( Color ) );
    Pass->nVertex += 4;
    Assert( Pass->nVertex <= Pass->MaxVertex );
}

internal void
DrawQuad( RENDER_PASS * Pass, TEXTURE_ID TextureID, rect Bound, vec4 Color ) {
    AddObjectToRenderPass( Pass, TextureID );
    
    OPENGL_VERTEX * Vertex = Pass->Vertex + Pass->nVertex;
    Vertex[ 0 ] = OpenGLVertex( Vec3( GetBL( Bound ), 0.0f ), Vec2( 0.0f, 0.0f ), ToU32Color( Color ) );
    Vertex[ 1 ] = OpenGLVertex( Vec3( GetBR( Bound ), 0.0f ), Vec2( 1.0f, 0.0f ), ToU32Color( Color ) );
    Vertex[ 2 ] = OpenGLVertex( Vec3( GetTR( Bound ), 0.0f ), Vec2( 1.0f, 1.0f ), ToU32Color( Color ) );
    Vertex[ 3 ] = OpenGLVertex( Vec3( GetTL( Bound ), 0.0f ), Vec2( 0.0f, 1.0f ), ToU32Color( Color ) );
    Pass->nVertex += 4;
    Assert( Pass->nVertex <= Pass->MaxVertex );
}

internal void
DrawQuad( RENDER_PASS * Pass, TEXTURE_ID TextureID, orect Bound, vec4 Color ) {
    AddObjectToRenderPass( Pass, TextureID );
    
    OPENGL_VERTEX * Vertex = Pass->Vertex + Pass->nVertex;
    Vertex[ 0 ] = OpenGLVertex( Vec3( GetT( Bound, Vec2( 0.0f, 0.0f ) ), 0.0f ), Vec2( 0.0f, 0.0f ), ToU32Color( Color ) );
    Vertex[ 1 ] = OpenGLVertex( Vec3( GetT( Bound, Vec2( 1.0f, 0.0f ) ), 0.0f ), Vec2( 1.0f, 0.0f ), ToU32Color( Color ) );
    Vertex[ 2 ] = OpenGLVertex( Vec3( GetT( Bound, Vec2( 1.0f, 1.0f ) ), 0.0f ), Vec2( 1.0f, 1.0f ), ToU32Color( Color ) );
    Vertex[ 3 ] = OpenGLVertex( Vec3( GetT( Bound, Vec2( 0.0f, 1.0f ) ), 0.0f ), Vec2( 0.0f, 1.0f ), ToU32Color( Color ) );
    Pass->nVertex += 4;
    Assert( Pass->nVertex <= Pass->MaxVertex );
}

internal void
DrawQuadOutline( RENDER_PASS * Pass, rect Bound, vec4 Color ) {
    vec2 P[ 4 ] = {};
    P[ 0 ] = GetT( Bound, Vec2( 0.0f, 0.0f ) );
    P[ 1 ] = GetT( Bound, Vec2( 1.0f, 0.0f ) );
    P[ 2 ] = GetT( Bound, Vec2( 1.0f, 1.0f ) );
    P[ 3 ] = GetT( Bound, Vec2( 0.0f, 1.0f ) );
    
    DrawLine( Pass, P[ 0 ], P[ 1 ], Color );
    DrawLine( Pass, P[ 1 ], P[ 2 ], Color );
    DrawLine( Pass, P[ 2 ], P[ 3 ], Color );
    DrawLine( Pass, P[ 3 ], P[ 0 ], Color );
}

internal void
DrawQuadOutline( RENDER_PASS * Pass, orect Bound, vec4 Color ) {
    vec2 P[ 4 ] = {};
    P[ 0 ] = GetT( Bound, Vec2( 0.0f, 0.0f ) );
    P[ 1 ] = GetT( Bound, Vec2( 1.0f, 0.0f ) );
    P[ 2 ] = GetT( Bound, Vec2( 1.0f, 1.0f ) );
    P[ 3 ] = GetT( Bound, Vec2( 0.0f, 1.0f ) );
    
    DrawLine( Pass, P[ 0 ], P[ 1 ], Color );
    DrawLine( Pass, P[ 1 ], P[ 2 ], Color );
    DrawLine( Pass, P[ 2 ], P[ 3 ], Color );
    DrawLine( Pass, P[ 3 ], P[ 0 ], Color );
}

internal void
DrawRect( RENDER_PASS * Pass, TEXTURE_ID TextureID, rect Bound, vec4 Color ) {
    DrawQuad( Pass, TextureID, Bound, Color );
}

internal void
DrawRect( RENDER_PASS * Pass, rect Bound, vec4 Color ) {
    DrawRect( Pass, TextureID_WhiteTexture, Bound, Color );
}

internal void
DrawORect( RENDER_PASS * Pass, TEXTURE_ID TextureID, orect Bound, vec4 Color ) {
    DrawQuad( Pass, TextureID, Bound, Color );
}

internal void
DrawORect( RENDER_PASS * Pass, orect Bound, vec4 Color ) {
    DrawORect( Pass, TextureID_WhiteTexture, Bound, Color );
}

internal void
DrawPoint( RENDER_PASS * Pass, vec2 Position, vec2 Dim, vec4 Color ) {
    rect Bound = RectCD( Position, Dim );
    DrawRect( Pass, TextureID_WhiteTexture, Bound, Color );
}

internal void
DrawRectOutline( RENDER_PASS * Pass, rect Bound, vec4 Color ) {
    DrawQuadOutline( Pass, Bound, Color );
}

internal void
DrawORectOutline( RENDER_PASS * Pass, orect Bound, vec4 Color ) {
    DrawQuadOutline( Pass, Bound, Color );
}


#if 0
internal void
DrawTriangle( RENDER_PASS * Pass, vec2 Center, vec2 Scale, flo32 Radians, vec4 Color ) {
    mat4 transform = mat4_translate( Center, 0.0f ) * mat4_rotate( Vec3( 0.0f, 0.0f, 1.0f ), Radians ) * mat4_Scale( Scale, 1.0f );
    DrawModel( &Pass->TriObjectList, MeshID_Triangle, TextureID_WhiteTexture, transform, Color );
}

internal void
DrawTriangle( RENDER_PASS * Pass, TEXTURE_ID TextureID, vec2 Center, vec2 Scale, flo32 Radians, vec4 Color ) {
    mat4 transform = mat4_translate( Center, 0.0f ) * mat4_rotate( Vec3( 0.0f, 0.0f, 1.0f ), Radians ) * mat4_Scale( Scale, 1.0f );
    DrawModel( &Pass->TriObjectList, MeshID_Triangle, TextureID, transform, Color );
}

internal void
DrawTriangleFlipUV( RENDER_PASS * Pass, TEXTURE_ID TextureID, vec2 Center, vec2 Scale, flo32 Radians, vec4 Color ) {
    mat4 transform = mat4_translate( Center, 0.0f ) * mat4_rotate( Vec3( 0.0f, 0.0f, 1.0f ), Radians ) * mat4_Scale( Scale, 1.0f );
    DrawModel( &Pass->TriObjectList, MeshID_Triangle_FlipUV, TextureID, transform, Color );
}

internal void
DrawTriangleBC( RENDER_PASS * Pass, vec2 BottomCenter, vec2 Scale, flo32 Radians, vec4 Color ) {
    vec2 Pos = BottomCenter + ToDirection2D( Radians + PI * 0.5f ) * Scale.y;
    DrawTriangle( Pass, Pos, Scale, Radians, Color );
}

internal void
DrawTriangleOutline( RENDER_PASS * Pass, vec2 Center, vec2 Scale, flo32 Radians, vec4 Color ) {
    mat4 transform = mat4_translate( Center, 0.0f ) * mat4_rotate( Vec3( 0.0f, 0.0f, 1.0f ), Radians ) * mat4_Scale( Scale, 1.0f );
    DrawModel( &Pass->LineObjectList, MeshID_TriangleOutline, TextureID_WhiteTexture, transform, Color );
}

internal void
DrawCircle( RENDER_PASS * Pass, vec2 Center, flo32 Radius, vec4 Color ) {
    mat4 transform = mat4_translate( Center, 0.0f ) * mat4_Scale( Radius, Radius, 1.0f );
    DrawModel( &Pass->TriObjectList, MeshID_Circle, TextureID_WhiteTexture, transform, Color );
}

internal void
DrawCircleOutline( RENDER_PASS * Pass, vec2 Position, flo32 Radius, vec4 modColor ) {
    mat4 transform = mat4_translate( Position, 0.0f ) * mat4_Scale( Radius, Radius, 1.0f );
    DrawModel( &Pass->LineObjectList, MeshID_CircleOutline, TextureID_WhiteTexture, transform, modColor );
}

internal void
DrawCircleSegment( RENDER_PASS * Pass, vec2 Position, flo32 Radius, flo32 StartRadians, flo32 RadiansToDraw, boo32 DoNegative, vec4 ModColor ) {
    RadiansToDraw = WrapRadians( RadiansToDraw );
    
    flo32 Epsilon = ( 0.01f / Radius ) + 1.0f;
    
    int32 MaxSegment = 64;
    int32 nSegment   = ( int32 )( ( RadiansToDraw / TAU ) * ( flo32 )MaxSegment );
    
    flo32 RadiansPerSegment     = RadiansToDraw / ( flo32 )nSegment;
    vec2  Scale                 = Vec2( RadiansPerSegment * Radius * Epsilon, Radius ) * 0.5f;
    
    if( DoNegative ) {
        RadiansPerSegment = -RadiansPerSegment;
    }
    flo32 HalfRadiansPerSegment = RadiansPerSegment * 0.5f;
    
    for( int32 iSegment = 0; iSegment < nSegment; iSegment++ ) {
        flo32 Radians = StartRadians + RadiansPerSegment * ( flo32 )iSegment + HalfRadiansPerSegment;
        vec2  Pos     = Position + ToDirection2D( Radians ) * Scale.y;
        
        DrawTriangle( Pass, Pos, Scale, Radians + PI * 0.5f, ModColor );
    }
}

internal void
DrawCircleSegmentOutline( RENDER_PASS * Pass, vec2 Position, flo32 Radius, flo32 StartRadians, flo32 RadiansToDraw, boo32 DoNegative, vec4 ModColor ) {
    RadiansToDraw = WrapRadians( RadiansToDraw );
    
    flo32 Epsilon = ( 0.01f / Radius ) + 1.0f;
    
    int32 MaxSegment = 64;
    int32 nSegment   = ( int32 )( ( RadiansToDraw / TAU ) * ( flo32 )MaxSegment );
    
    flo32 RadiansPerSegment = RadiansToDraw / ( flo32 )nSegment;
    
    if( DoNegative ) {
        RadiansPerSegment = -RadiansPerSegment;
    }
    flo32 HalfRadiansPerSegment = RadiansPerSegment * 0.5f;
    
    for( int32 iSegment = 0; iSegment < nSegment; iSegment++ ) {
        flo32 RadiansA = StartRadians + RadiansPerSegment *   ( flo32 )iSegment;
        flo32 RadiansB = StartRadians + RadiansPerSegment * ( ( flo32 )iSegment + 1.0f );
        
        vec2  PosA     = Position + ToDirection2D( RadiansA ) * Radius;
        vec2  PosB     = Position + ToDirection2D( RadiansB ) * Radius;
        
        DrawLine( Pass, PosA, PosB, ModColor );
    }
}

internal void
DrawCapsuleOutline( RENDER_PASS * Pass, CAPSULE2 Cap, vec4 ModColor ) {
    DrawCircleOutline( Pass, Cap.P, Cap.Radius, ModColor );
    DrawCircleOutline( Pass, Cap.Q, Cap.Radius, ModColor );
}

internal void
DrawEllipse( RENDER_PASS * Pass, vec2 Center, vec2 Radius, flo32 Radians, vec4 Color ) {
    mat4 transform = mat4_translate( Center, 0.0f ) * mat4_rotate( Vec3( 0.0f, 0.0f, 1.0f ), Radians ) * mat4_Scale( Radius.x, Radius.y, 1.0f );
    DrawModel( &Pass->TriObjectList, MeshID_Circle, TextureID_WhiteTexture, transform, Color );
}

internal void
DrawEllipseOutline( RENDER_PASS * Pass, vec2 Center, vec2 Radius, flo32 Radians, vec4 Color ) {
    mat4 transform = mat4_translate( Center, 0.0f ) * mat4_rotate( Vec3( 0.0f, 0.0f, 1.0f ), Radians ) * mat4_Scale( Radius.x, Radius.y, 1.0f );
    DrawModel( &Pass->LineObjectList, MeshID_CircleOutline, TextureID_WhiteTexture, transform, Color );
}

internal void
DrawHexagon( RENDER_PASS * Pass, vec2 Center, flo32 Radius, vec4 Color ) {
    mat4 transform = mat4_translate( Center, 0.0f ) * mat4_Scale( Radius, Radius, 1.0f );
    DrawModel( &Pass->TriObjectList, MeshID_Hexagon, TextureID_WhiteTexture, transform, Color );
}

internal void
DrawOctagon( RENDER_PASS * Pass, vec2 Center, flo32 Radius, vec4 Color ) {
    mat4 transform = mat4_translate( Center, 0.0f ) * mat4_Scale( Radius, Radius, 1.0f );
    DrawModel( &Pass->TriObjectList, MeshID_Octagon, TextureID_WhiteTexture, transform, Color );
}
#endif
