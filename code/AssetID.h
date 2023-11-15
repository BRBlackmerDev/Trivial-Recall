
#define TEXTURE_ID_MAX_COUNT  ( 1024 )
enum TEXTURE_ID {
    TextureID_DefaultTexture,
    TextureID_WhiteTexture,
    
    TextureID_Circle,
    TextureID_Arrow,
    TextureID_ArrowWithShadow,
    TextureID_Arrow_Button,
    TextureID_Spotlight,
    TextureID_Glow,
    TextureID_Ring,
    TextureID_PauseMenuButton,
    TextureID_DashedBoxOutline,
    TextureID_SolidBoxOutline,
    TextureID_ToStore_Tray,
    TextureID_ToStore_Arrow,
    TextureID_Lock,
    TextureID_Checkmark,
    TextureID_Cell,
    
    TextureID_CardboardBox_Empty,
    TextureID_CardboardBox_Full,
    TextureID_CardboardBox_Hover,
    
    TextureID_Icon_LockerUp,
    TextureID_Icon_LockerDown,
    
    TextureID_Shape_Circle,
    TextureID_Shape_Cross,
    TextureID_Shape_Triangle,
    TextureID_Shape_Square,
    TextureID_Shape_Star,
    
    TextureID_InsideTunnel,
    TextureID_Shadow,
    //TextureID_DisplayBorder,
    TextureID_Icon_Health,
    //TextureID_Background_Light,
    TextureID_Fence,
    //TextureID_StorageArea,
    //TextureID_ToFind_Panel,
    //TextureID_Background_Dust,
    TextureID_Background,
    TextureID_StorageArea_Shadow,
    TextureID_QuestionArea_Shadow,
    
    TextureID_Item,
    TextureID_Item_End = TextureID_Item + ITEM_MAX_COUNT,
    
    TextureID_Font,
    TextureID_Font_1_00,
    TextureID_Font_1_50,
    TextureID_Font_2_00,
    TextureID_Font_2_25,
    TextureID_Font_2_50,
    TextureID_Font_3_00,
    TextureID_Font_4_00,
    TextureID_Font_6_00,
    TextureID_Font_7_00,
    
    TextureID_Framebuffer_Layer,
    
    TextureID_Count,
};

enum FONT_ID {
    FontID_1_00,
    FontID_1_50,
    FontID_2_00,
    FontID_2_25,
    FontID_2_50,
    FontID_3_00,
    FontID_4_00,
    FontID_6_00,
    FontID_7_00,
    
    FontID_Count,
};

enum MODEL_ID {
    MeshID_Line,
    MeshID_Rect,
    MeshID_RectOutline,
    MeshID_Circle,
    MeshID_CircleOutline,
    MeshID_Triangle,
    MeshID_Triangle_FlipUV,
    MeshID_TriangleOutline,
    
    MeshID_Rect_BRQuad,
    
    MeshID_Count,
};

enum AUDIO_ID {
    AudioID_Null,
    AudioID_Music_End,
    
    AudioID_Count,
};

#include "SoundID.h"

