
#include "FModTools.cpp"

internal void
InitFMod( PLATFORM * Platform, APP_STATE * AppState, MEMORY * TempMemory, uint8 ** Asset_Ptr ) {
    uint8 * Ptr = *Asset_Ptr;
    
    FMOD_STUDIO_SYSTEM           *  FMOD_System           = 0;
    FMOD_RESULT                     FMOD_Result           = FMOD_OK;
    void                         *  FMOD_ExtraDriverData  = 0;
    FMOD_STUDIO_BANK             *  FMOD_Bank             = 0;
    FMOD_STUDIO_BANK             *  FMOD_StringBank       = 0;
    FMOD_STUDIO_BANK             *  FMOD_SoundEffectsBank = 0;
    FMOD_STUDIO_EVENTDESCRIPTION ** FMOD_EventDescPtr     = 0;
    
    FMOD_Result = FMOD_Studio_System_Create( &FMOD_System, FMOD_VERSION );
    Assert( FMOD_Result == FMOD_OK );
    
    FMOD_Studio_System_Initialize( FMOD_System, 32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, FMOD_ExtraDriverData );
    Assert( FMOD_Result == FMOD_OK );
    
    int32   Master_Size = _read ( Ptr, int32 );
    uint8 * Master_Data = _addra( Ptr, uint8, Master_Size );
    FMOD_Result = FMOD_Studio_System_LoadBankMemory( FMOD_System, ( const char * )Master_Data, Master_Size, FMOD_STUDIO_LOAD_MEMORY, FMOD_STUDIO_LOAD_BANK_NORMAL, &FMOD_Bank );
    Assert( FMOD_Result == FMOD_OK );
    
    //FILE_DATA FMOD_File = Platform->ReadFile( TempMemory, 0, "Master.bank", 0 );
    //Assert( FMOD_File.Contents );
    //FMOD_Result = FMOD_Studio_System_LoadBankMemory( FMOD_System, ( const char * )FMOD_File.Contents, FMOD_File.Size, FMOD_STUDIO_LOAD_MEMORY, FMOD_STUDIO_LOAD_BANK_NORMAL, &FMOD_Bank );
    //Assert( FMOD_Result == FMOD_OK );
    
    // NOTE: Load Master Bank from file.
    //FMOD_Result = FMOD_Studio_System_LoadBankFile( FMOD_System, "Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &FMOD_Bank );
    //Assert( FMOD_Result == FMOD_OK );
    
    //FMOD_Result = FMOD_Studio_Bank_LoadSampleData( FMOD_Bank );
    //Assert( FMOD_Result == FMOD_OK );
    
    // NOTE: Load Strings Bank from file.
    //FMOD_Result = FMOD_Studio_System_LoadBankFile( FMOD_System, "Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &FMOD_StringBank );
    //Assert( FMOD_Result == FMOD_OK );
    
    //FMOD_Result = FMOD_Studio_Bank_LoadSampleData( FMOD_StringBank );
    //Assert( FMOD_Result == FMOD_OK );
    
    int32   SoundEffects_Size = _read ( Ptr, int32 );
    uint8 * SoundEffects_Data = _addra( Ptr, uint8, SoundEffects_Size );
    FMOD_Result = FMOD_Studio_System_LoadBankMemory( FMOD_System, ( const char * )SoundEffects_Data, SoundEffects_Size, FMOD_STUDIO_LOAD_MEMORY, FMOD_STUDIO_LOAD_BANK_NORMAL, &FMOD_SoundEffectsBank );
    Assert( FMOD_Result == FMOD_OK );
    
    //FMOD_Result = FMOD_Studio_System_LoadBankFile( FMOD_System, "Sound Effects.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &FMOD_SoundEffectsBank );
    //Assert( FMOD_Result == FMOD_OK );
    
    FMOD_Result = FMOD_Studio_Bank_LoadSampleData( FMOD_SoundEffectsBank );
    Assert( FMOD_Result == FMOD_OK );
    
    {
        AppState->FMOD_System = FMOD_System;
        
        int32 nGuid = _read( Ptr, int32 );
        Assert( nGuid == SoundID_Count );
        
        for( int32 iGuid = 0; iGuid < nGuid; iGuid++ ) {
            AppState->FMOD_Guid[ iGuid ] = _read( Ptr, FMOD_GUID );
        }
        
#if 0        
        AppState->FMOD_Guid[ SoundID_Menu_Confirm ] = FMODGuid( 0xF4ECBB9D, 0x5A5D4AE7, 0xB1BFFBB4, 0x60BA7AF6 );
        AppState->FMOD_Guid[ SoundID_Menu_Back ] = FMODGuid( 0xD8279E17, 0xFA39420F, 0xBDCF4759, 0x67110C02 );
        AppState->FMOD_Guid[ SoundID_NewGame ] = FMODGuid( 0xC4C7F3E1, 0x9D4D44AC, 0xBE91E412, 0x040169F6 );
        AppState->FMOD_Guid[ SoundID_LoseGame ] = FMODGuid( 0xC1919ADE, 0x4BB94980, 0x8665BAEB, 0x5C0584F6 );
        AppState->FMOD_Guid[ SoundID_Health_Down ] = FMODGuid( 0x6A57370F, 0x34174B7C, 0xB825607A, 0x62C620C5 );
        AppState->FMOD_Guid[ SoundID_Health_Up ] = FMODGuid( 0xE6CAD347, 0xCAAE4387, 0x99FD09DA, 0x5E1664B3 );
        AppState->FMOD_Guid[ SoundID_AddPoints ] = FMODGuid( 0x307D9ED9, 0x6D48487F, 0xA02BB0B2, 0xD76CB99D );
        AppState->FMOD_Guid[ SoundID_AddBonusPoints ] = FMODGuid( 0x52A8E74C, 0x3C6A4093, 0x90CDE47A, 0xF35CECB2 );
        AppState->FMOD_Guid[ SoundID_Add2BonusPoints ] = FMODGuid( 0xF013DAD3, 0xFB4B45C0, 0xAFBFB04F, 0x5B2A65B1 );
        AppState->FMOD_Guid[ SoundID_ItemsUp ] = FMODGuid( 0xF1E66B8D, 0x0CC24166, 0x9E36E7FB, 0x252399AB );
        AppState->FMOD_Guid[ SoundID_Item_PickUp ] = FMODGuid( 0xE88BA665, 0x0439432B, 0x96BB6ADA, 0x32A0A9A3 );
        AppState->FMOD_Guid[ SoundID_Item_Drop ] = FMODGuid( 0xC5DABFEB, 0x9DCB4C25, 0x9F541A00, 0x92C4EC5F );
#endif
    }
    
    *Asset_Ptr = Ptr;
    
    if( FMOD_StringBank ) {
        int32    MaxTemp = 128;
        int32      nTemp = 0;
        TEMP_DATA * Temp = _PushArray_Clear( TempMemory, TEMP_DATA, MaxTemp );
        
        {
            int32 nString = 0;
            
            FMOD_Result = FMOD_Studio_Bank_GetStringCount( FMOD_StringBank, &nString );
            Assert( FMOD_Result == FMOD_OK );
            
            char  String[ 256 ] = {};
            int32 Length        = _ArrayCount( String );
            int32 LengthWritten = 0;
            
            for( int32 iString = 0; iString < nString; iString++ ) {
                FMOD_GUID Guid = {};
                
                FMOD_Result = FMOD_Studio_Bank_GetStringInfo( FMOD_StringBank, iString, &Guid, String, Length, &LengthWritten );
                Assert( FMOD_Result == FMOD_OK );
                
                TEMP_DATA T = {};
                T.iString = iString;
                T.iEvent  = -1;
                T.Guid    = Guid;
                strcpy( T.String, String );
                
                Assert( nTemp < MaxTemp );
                Temp[ nTemp++ ] = T;
            }
        }
        
        {
            int32 nEvent        = 0;
            int32 nEventWritten = 0;
            
            FMOD_Result = FMOD_Studio_Bank_GetEventCount( FMOD_SoundEffectsBank, &nEvent );
            Assert( FMOD_Result == FMOD_OK );
            
            FMOD_EventDescPtr = _PushArray_Clear( TempMemory, FMOD_STUDIO_EVENTDESCRIPTION *, nEvent );
            FMOD_Result = FMOD_Studio_Bank_GetEventList( FMOD_SoundEffectsBank, FMOD_EventDescPtr, nEvent, &nEventWritten );
            Assert( FMOD_Result == FMOD_OK );
            
            for( int32 iEventDesc = 0; iEventDesc < nEventWritten; iEventDesc++ ) {
                FMOD_GUID Guid = {};
                int32 Length  = 0;
                
                FMOD_Result = FMOD_Studio_EventDescription_GetID( FMOD_EventDescPtr[ iEventDesc ], &Guid );
                Assert( FMOD_Result == FMOD_OK );
                
                for( int32 iTemp = 0; iTemp < nTemp; iTemp++ ) {
                    TEMP_DATA * T = Temp + iTemp;
                    if( ( T->Guid.Data1 == Guid.Data1 ) && ( T->Guid.Data2 == Guid.Data2 ) && ( T->Guid.Data3 == Guid.Data3 ) ) {
                        boo32 IsValid = true;
                        for( int32 Iter = 0; Iter < 8; Iter++ ) {
                            if( T->Guid.Data4[ Iter ] != Guid.Data4[ Iter ] ) {
                                IsValid = false;
                            }
                        }
                        
                        if( IsValid ) {
                            T->iEvent = iEventDesc;
                        }
                    }
                }
            }
            
            _PopArray( TempMemory, FMOD_STUDIO_EVENTDESCRIPTION *, nEvent );
        }
        
        _PopArray( TempMemory, TEMP_DATA, MaxTemp );
    }
}