
struct SPRING_COEF {
    // Pos0 = A * Pos + B * Vel + Dest;
    // Vel0 = C * Pos + D * Vel;
    flo32 A;
    flo32 B;
    flo32 C;
    flo32 D;
};

// TODO: This code needs to be redone. According to  GDC 2018 - Weaving 13 Prototypes into 1 Game: Lessons from Edith Finch, the equations for a  Critically Damped Spring  have two values: Stiffness and Damping. This version only uses one variable.  I found out about Damped Springs from  GDC 2012 - The Cameras of Uncharted 3, but I don't remember where I got these equations. I believe that these equations are critically damped, meaning that there is no spring oscillation/wobble around the destination value.

internal SPRING_COEF
SpringCoef( flo32 Stiffness, flo32 dT ) {
    flo32 E = expf( -Stiffness * dT );
    flo32 F = E * dT;
    flo32 G = Stiffness * F;
    
    SPRING_COEF Result = {};
    Result.A = G + E;
    Result.B = F;
    Result.C = -Stiffness * G;
    Result.D = -G + E;
    return Result;
}

internal void
UpdateSpring( flo32 * Position, flo32 * Velocity, flo32 Destination, flo32 Stiffness, flo32 dT ) {
    SPRING_COEF Spring = SpringCoef( Stiffness, dT );
    
    flo32 Dist = *Position - Destination;
    flo32 Vel  = *Velocity;
    
    *Position = Spring.A * Dist + Spring.B * Vel + Destination;
    *Velocity = Spring.C * Dist + Spring.D * Vel;
}

internal void
UpdateSpring( vec2 * Position, vec2 * Velocity, vec2 Destination, flo32 Stiffness, flo32 dT ) {
    SPRING_COEF Spring = SpringCoef( Stiffness, dT );
    
    vec2 Dist = *Position - Destination;
    vec2 Vel  = *Velocity;
    
    *Position = Spring.A * Dist + Spring.B * Vel + Destination;
    *Velocity = Spring.C * Dist + Spring.D * Vel;
}

internal void
UpdateSpring( vec3 * Position, vec3 * Velocity, vec3 Destination, flo32 Stiffness, flo32 dT ) {
    SPRING_COEF Spring = SpringCoef( Stiffness, dT );
    
    vec3 Dist = *Position - Destination;
    vec3 Vel  = *Velocity;
    
    *Position = Spring.A * Dist + Spring.B * Vel + Destination;
    *Velocity = Spring.C * Dist + Spring.D * Vel;
}