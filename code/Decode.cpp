
internal boo32
IsValidChar( char C ) {
    boo32 Result = ( C >= 'A' ) && ( C <= 'Z' );
    return Result;
}


internal void
DecodeMessage( RENDER_PASS * Pass, APP_STATE * AppState, KEYBOARD_STATE * Keyboard, MOUSE_STATE * Mouse, MEMORY * TempMemory ) {
    DrawRect( Pass, AppState->UI_Bound, COLOR_BLACK );
    
    char Str[ 2048 ] = {};
    //strcpy( Str, "AJLPNYRJZJFLZYASGSKQGSMME" );
    //strcat( Str, "JKEJPFSVLPJLKKEJELNNSPZKY" );
    //strcat( Str, "NNYGNSGASGYZJGAKEYZYGASVW" );
    //strcat( Str, "NJKSWSKECNLUJZKEJZEYCYZWS" );
    //strcat( Str, "VOEKLGAHYKKJAZEJNYJZLKLGU" );
    //strcat( Str, "ESPPJLAFHSPZJLFSVGJRJPYTL" );
    //strcat( Str, "OYGJALZMJJKJPZUESSGJPLUEY" );
    //strcat( Str, "NATYOEKZLYNEJPKMSEVGAPJAK" );
    //strcat( Str, "SGZGLTJEYZCLGYSNL" );
    
    //strcpy( Str, "FX IWBBJX PB NB PB PWX GBBD. VSP FWO, JBGX JRO, PWX GBBD? FWO IWBBJX PWUJ RJ BSA NBRK? RDL PWXO GRO FXKK RJM FWO IKUGV PWX WUNWXJP GBSDPRUD? FWO, 35 OXRAJ RNB, EKO PWX RPKRDPUI? FWO LBXJ AUIX CKRO PXQRJ? FX IWBBJX PB NB PB PWX GBBD UD PWUJ LXIRLX RDL LB PWX BPWXA PWUDNJ, DBP VXIRSJX PWXO RAX XRJO, VSP VXIRSJX PWXO RAX WRAL, VXIRSJX PWRP NBRK FUKK JXATX PB BANRDUZX RDL GXRJSAX PWX VXJP BE BSA XDXANUXJ RDL JMUKKJ, VXIRSJX PWRP IWRKKXDNX UJ BDX PWRP FX RAX FUKKUDN PB RIIXCP, BDX FX RAX SDFUKKUDN PB CBJPCBDX, RDL BDX FWUIW FX UDPXDL PB FUD, RDL PWX BPWXAJ, PBB." );
    
    //strcpy( Str, "ZRTFT IH PQFTHZ IQ ZRT XBGBOZIO HTQBZT. HTWTFBG ZRLPHBQV HLGBF HYHZTSH RBWT VTOGBFTV ZRTIF IQZTQZILQH ZL GTBWT ZRT FTEPKGIO. ZRIH HTEBFBZIHZ SLWTSTQZ, PQVTF ZRT GTBVTFHRIE LD ZRT SYHZTFILPH OLPQZ VLLAP, RBH SBVT IZ VIDDIOPGZ DLF ZRT GISIZTV QPSKTF LD CTVI AQIXRZH ZL SBIQZBIQ ETBOT BQV LFVTF IQ ZRT XBGBJY. HTQBZLF BSIVBGB, ZRT DLFSTF NPTTQ LD QBKLL, IH FTZPFQIQX ZL ZRT XBGBOZIO HTQBZT ZL WLZT LQ ZRT OFIZIOBG IHHPT LD OFTBZIQX BQ BFSY LD ZRT FTEPKGIO ZL BHHIHZ ZRT LWTFMRTGSTV CTVI…" );
    
    //strcpy( Str, "IZRN P SPNA QWBRXS PN DPQRB ES DHEJVXR, QEDZRH QUHW GEQRB DE QR BLRUOPNY IEHAB ES IPBAEQ, XRD PD VR UNA PN QW ZEJH ES AUHONRBB BZR PB BDUNAPNY HPYZD PN SHEND ES QR BLRUOPNY IEHAB ES IPBAEQ, XRD PD VR XRD PD VR, XRD PD VR, XRD PD VR, XRD PD VR IZPBLRH IEHAB ES IPBAEQ, XRD PD VR UNA IZRN DZR VHEORN-ZRUHDRA LRELXR XPCPNY PN DZR IEHXA UYHRR DZRHR IPXX VR UN UNBIRH, XRD PD VR SEH DZEJYZ DZRW QUW VR LUHDRA, DZRHR PB BDPXX U GZUNGR DZUD DZRW IPXX BRR DZRHR IPXX VR UN UNBIRH, XRD PD VR XRD PD VR, XRD PD VR, XRD PD VR, XRD PD VR WRUZ, DZRHR IPXX VR UN UNBIRH, XRD PD VR XRD PD VR, XRD PD VR, XRD PD VR, XRD PD VR IZPBLRH IEHAB ES IPBAEQ, XRD PD VR XRD PD VR, XRD PD VR, XRD PD VR, WRUZ, XRD PD VR IZPBLRH IEHAB ES IPBAEQ, XRD PD VR UNA IZRN DZR NPYZD PB GXEJAW DZRHR PB BDPXX U XPYZD DZUD BZPNRB EN QR BZPNR JNDPX DEQEHHEI, XRD PD VR P IUOR JL DE DZR BEJNA ES QJBPG, QEDZRH QUHW GEQRB DE QR BLRUOPNY IEHAB ES IPBAEQ, XRD PD VR XRD PD VR, XRD PD VR, XRD PD VR, WRUZ, XRD PD VR DZRHR IPXX VR UN UNBIRH, XRD PD VR XRD PD VR, XRD PD VR, XRD PD VR, WRUZ, XRD PD VR DZRHR IPXX VR UN UNBIRH, XRD PD VR XRD PD VR, XRD PD VR, XRD PD VR, WRUZ, XRD PD VR IZPBLRH IEHAB ES IPBAEQ, XRD PD VR" );
    
    //strcpy( Str, "Q WNN JFNNW XP TFNNS, FNR FXWNW JXX Q WNN JYNA GHXXA PXF AN DSR KXB DSR Q JYQSE JX AKWNHP OYDJ D OXSRNFPBH OXFHR Q WNN WEQNW XP GHBN DSR LHXBRW XP OYQJN JYN GFQTYJ GHNWWNR RDK, JYN RDFE WDLFNR SQTYJ DSR Q JYQSE JX AKWNHP OYDJ D OXSRNFPBH OXFHR JYN LXHXFW XP JYN FDQSGXO WX ZFNJJK QS JYN WEK DFN DHWX XS JYN PDLNW XP ZNXZHN TXQST GK Q WNN PFQNSRW WYDEQST YDSRW WDKQST YXO RX KXB RX JYNK'FN FNDHHK WDKQST Q HXUN KXB Q YNDF GDGQNW LFKQST, Q ODJLY JYNA TFXO JYNK'HH HNDFS ABLY AXFN JYDS Q'HH SNUNF ESXO DSR Q JYQSE JX AKWNHP OYDJ D OXSRNFPBH OXFHR KNW Q JYQSE JX AKWNHP OYDJ D OXSRNFPBH OXFHR" );
    
    //strcpy( Str, "DJ DK C QLXDWI WF SDGDU PCX. XLRLU KQCSLKBDQK, KJXDHDET FXWZ C BDIILE RCKL, BCGL PWE JBLDX FDXKJ GDSJWXO CTCDEKJ JBL LGDU TCUCSJDS LZQDXL. IYXDET JBL RCJJUL, XLRLU KQDLK ZCECTLI JW KJLCU KLSXLJ QUCEK JW JBL LZQDXL'K YUJDZCJL PLCQWE, JBL ILCJB KJCX, CE CXZWXLI KQCSL KJCJDWE PDJB LEWYTB QWPLX JW ILKJXWO CE LEJDXL QUCELJ. QYXKYLI RO JBL LZQDXL'K KDEDKJLX CTLEJK, QXDESLKK ULDC XCSLK BWZL CRWCXI BLX KJCXKBDQ, SYKJWIDCE WF JBL KJWULE QUCEK JBCJ SCE KCGL BLX QLWQUL CEI XLKJWXL FXLLIWZ JW JBL TCUCVO…" );
    
    strcpy( Str, "BLK OMUKVY GZBBGK VKU VZUZYH LMMU, VKRVKU MY EZYHZYH PMOOKVPZRGE, LRE YM MSIKPBZMY BM SKZYH KRBKY SF BLK WMGJ." );
    
    strcpy( Str, "LFUTZ XP RXPETIKZKR WT UK WSK GZFYR ETYCMKZTZ, KYZXESXYG FYR UMXLRXYG MQ YFWXTYP HTZK PMZKLO WSFY WSK QZTMRKPW UFWWLKP." );
    
    DrawString( Pass, AppState->Font, Str, GetTL( AppState->UI_Bound ) + Vec2( 20.0f, -20.0f ), TextAlign_TopLeft, TEXT_COLOR );
    
    int32   Frequency[ 26 ] = {};
    
    int32 nChar = ( int32 )strlen( Str );
    
    DISPLAY_VALUE( int32, nChar );
    
    for( int32 iChar = 0; iChar < nChar; iChar++ ) {
        if( IsValidChar( Str[ iChar ] ) ) {
            Frequency[ Str[ iChar ] - 'A' ] += 1;
        }
    }
    
    if( !AppState->PairFrequency ) {
        int32 MaxPair = 26 * 26;
        AppState->PairFrequency = _PushArray( TempMemory, INT32_PAIR, MaxPair );
        
        for( int32 iPair = 0; iPair < MaxPair; iPair++ ) {
            AppState->PairFrequency[ iPair ].x = iPair;
            AppState->PairFrequency[ iPair ].y = 0;
        }
        
        for( int32 iChar = 0; iChar < ( nChar - 1 ); iChar++ ) {
            if( IsValidChar( Str[ iChar ] ) && IsValidChar( Str[ iChar + 1 ] ) ) {
                int32 Index0 = Str[ iChar     ] - 'A';
                int32 Index1 = Str[ iChar + 1 ] - 'A';
                int32 Index  = Index0 * 26 + Index1;
                AppState->PairFrequency[ Index ].y += 1;
            }
        }
        
        {
            boo32 DoSort = true;
            
            INT32_PAIR * Value = AppState->PairFrequency;
            int32       nValue = MaxPair;
            
            while( DoSort ) {
                DoSort = false;
                for( int32 iValue = 0; iValue < ( nValue - 1 ); iValue++ ) {
                    if( Value[ iValue ].y < Value[ iValue + 1 ].y ) {
                        _swap( INT32_PAIR, Value[ iValue ], Value[ iValue + 1 ] );
                        DoSort = true;
                    }
                }
            }
        }
        
        for( int32 iChar = 0; iChar < 26; iChar++ ) {
            AppState->Replace[ iChar ] = 'A' + ( char )iChar;
        }
    }
    
    if( !AppState->TripFrequency ) {
        int32 MaxTrip = 26 * 26 * 26;
        AppState->TripFrequency = _PushArray( TempMemory, INT32_PAIR, MaxTrip );
        
        for( int32 iTrip = 0; iTrip < MaxTrip; iTrip++ ) {
            AppState->TripFrequency[ iTrip ].x = iTrip;
            AppState->TripFrequency[ iTrip ].y = 0;
        }
        
        for( int32 iChar = 0; iChar < ( nChar - 1 ); iChar++ ) {
            if( IsValidChar( Str[ iChar ] ) && IsValidChar( Str[ iChar + 1 ] ) && IsValidChar( Str[ iChar + 2 ] ) ) {
                int32 Index0 = Str[ iChar     ] - 'A';
                int32 Index1 = Str[ iChar + 1 ] - 'A';
                int32 Index2 = Str[ iChar + 2 ] - 'A';
                int32 Index  = Index0 * 26 * 26 + Index1 * 26 + Index2;
                AppState->TripFrequency[ Index ].y += 1;
            }
        }
        
        {
            boo32 DoSort = true;
            
            INT32_PAIR * Value = AppState->TripFrequency;
            int32       nValue = MaxTrip;
            
            while( DoSort ) {
                DoSort = false;
                for( int32 iValue = 0; iValue < ( nValue - 1 ); iValue++ ) {
                    if( Value[ iValue ].y < Value[ iValue + 1 ].y ) {
                        _swap( INT32_PAIR, Value[ iValue ], Value[ iValue + 1 ] );
                        DoSort = true;
                    }
                }
            }
        }
    }
    
    int32 iHover          = -1;
    char   Hover_Str[ 8 ] = {};
    
    {
        vec2 Cell = Vec2( 80.0f, 14.0f );
        vec2 bPos = GetTL( AppState->UI_Bound ) + Vec2( 20.0f, -40.0f );
        
        {
            vec2 AtPos = bPos;
            char String[ 8 ] = {};
            for( int32 iChar = 0; iChar < 26; iChar++ ) {
                rect Bound = RectTLD( AtPos, Cell );
                if( ( iHover <= -1 ) && ( IsInRect( Mouse->Position, Bound ) ) ) {
                    iHover = iChar;
                    DrawRect( Pass, Bound, COLOR_GRAY( 0.1f ) );
                    
                    sprintf( Hover_Str, "%c", 'A' + iChar );
                }
                
                sprintf( String, "%c = %d", iChar + 'A', Frequency[ iChar ] );
                DrawString( Pass, AppState->Font, String, AtPos, TextAlign_TopLeft, TEXT_COLOR );
                
                AtPos.y -= Cell.y;
            }
            
            bPos.x += ( Cell.x * 1.5f );
        }
        
        {
            char String[ 16 ] = {};
            vec2 AtPos = bPos;
            
            boo32 DoPrint = true;
            int32 iRow    = 0;
            for( int32 iPair = 0; DoPrint; iPair++ ) {
                INT32_PAIR Pair = AppState->PairFrequency[ iPair ];
                if( Pair.y > 1 ) {
                    char C0 = 'A' + ( char )( Pair.x / 26 );
                    char C1 = 'A' + ( char )( Pair.x % 26 );
                    
                    rect Bound = RectTLD( AtPos, Cell );
                    if( ( iHover <= -1 ) && ( IsInRect( Mouse->Position, Bound ) ) ) {
                        iHover = iPair;
                        DrawRect( Pass, Bound, COLOR_GRAY( 0.1f ) );
                        
                        sprintf( Hover_Str, "%c%c", C0, C1 );
                    }
                    
                    sprintf( String, "%c%c = %d", C0, C1, Pair.y );
                    DrawString( Pass, AppState->Font, String, AtPos, TextAlign_TopLeft, TEXT_COLOR );
                    
                    AtPos.y -= Cell.y;
                    
                    iRow += 1;
                    if( iRow > 26 ) {
                        iRow    = 0;
                        bPos.x += Cell.x;
                        AtPos   = bPos;
                    }
                } else {
                    DoPrint = false;
                }
            }
            
            bPos.x += ( Cell.x );
        }
        
        {
            char String[ 16 ] = {};
            vec2 AtPos = bPos;
            
            boo32 DoPrint = true;
            int32 iRow    = 0;
            for( int32 iTrip = 0; DoPrint; iTrip++ ) {
                INT32_PAIR Trip = AppState->TripFrequency[ iTrip ];
                if( Trip.y > 1 ) {
                    char C0 = 'A' + ( char )( Trip.x / ( 26 * 26 ) );
                    char C1 = 'A' + ( char )( ( Trip.x / 26 ) % 26 );
                    char C2 = 'A' + ( char )( Trip.x % 26 );
                    
                    rect Bound = RectTLD( AtPos, Cell );
                    if( ( iHover <= -1 ) && ( IsInRect( Mouse->Position, Bound ) ) ) {
                        iHover = iTrip;
                        DrawRect( Pass, Bound, COLOR_GRAY( 0.1f ) );
                        
                        sprintf( Hover_Str, "%c%c%c", C0, C1, C2 );
                    }
                    
                    sprintf( String, "%c%c%c = %d", C0, C1, C2, Trip.y );
                    DrawString( Pass, AppState->Font, String, AtPos, TextAlign_TopLeft, TEXT_COLOR );
                    
                    AtPos.y -= Cell.y;
                    
                    iRow += 1;
                    if( iRow > 26 ) {
                        iRow    = 0;
                        bPos.x += Cell.x;
                        AtPos   = bPos;
                    }
                } else {
                    DoPrint = false;
                }
            }
            
            bPos.x += ( Cell.x * 1.5f );
        }
        
        {
            char String[ 16 ] = {};
            vec2 AtPos = bPos;
            
            boo32 DoPrint = true;
            int32 iRow    = 0;
            for( int32 iPair = 0; DoPrint; iPair++ ) {
                INT32_PAIR Pair = AppState->PairFrequency[ iPair ];
                if( Pair.y > 1 ) {
                    char C0 = 'A' + ( char )( Pair.x / 26 );
                    char C1 = 'A' + ( char )( Pair.x % 26 );
                    
                    rect Bound = RectTLD( AtPos, Cell );
                    if( ( iHover <= -1 ) && ( IsInRect( Mouse->Position, Bound ) ) ) {
                        iHover = iPair;
                        DrawRect( Pass, Bound, COLOR_GRAY( 0.1f ) );
                        
                        sprintf( Hover_Str, "%c%c", C0, C1 );
                    }
                    
                    C0 = AppState->Replace[ C0 - 'A' ];
                    C1 = AppState->Replace[ C1 - 'A' ];
                    
                    sprintf( String, "%c%c = %d", C0, C1, Pair.y );
                    DrawString( Pass, AppState->Font, String, AtPos, TextAlign_TopLeft, TEXT_COLOR );
                    
                    AtPos.y -= Cell.y;
                    
                    iRow += 1;
                    if( iRow > 26 ) {
                        iRow    = 0;
                        bPos.x += Cell.x;
                        AtPos   = bPos;
                    }
                } else {
                    DoPrint = false;
                }
            }
            
            bPos.x += ( Cell.x );
        }
        
        {
            char String[ 16 ] = {};
            vec2 AtPos = bPos;
            
            boo32 DoPrint = true;
            int32 iRow    = 0;
            for( int32 iTrip = 0; DoPrint; iTrip++ ) {
                INT32_PAIR Trip = AppState->TripFrequency[ iTrip ];
                if( Trip.y > 1 ) {
                    char C0 = 'A' + ( char )( Trip.x / ( 26 * 26 ) );
                    char C1 = 'A' + ( char )( ( Trip.x / 26 ) % 26 );
                    char C2 = 'A' + ( char )( Trip.x % 26 );
                    
                    rect Bound = RectTLD( AtPos, Cell );
                    if( ( iHover <= -1 ) && ( IsInRect( Mouse->Position, Bound ) ) ) {
                        iHover = iTrip;
                        DrawRect( Pass, Bound, COLOR_GRAY( 0.1f ) );
                        
                        sprintf( Hover_Str, "%c%c%c", C0, C1, C2 );
                    }
                    
                    C0 = AppState->Replace[ C0 - 'A' ];
                    C1 = AppState->Replace[ C1 - 'A' ];
                    C2 = AppState->Replace[ C2 - 'A' ];
                    
                    sprintf( String, "%c%c%c = %d", C0, C1, C2, Trip.y );
                    DrawString( Pass, AppState->Font, String, AtPos, TextAlign_TopLeft, TEXT_COLOR );
                    
                    AtPos.y -= Cell.y;
                    
                    iRow += 1;
                    if( iRow > 26 ) {
                        iRow    = 0;
                        bPos.x += Cell.x;
                        AtPos   = bPos;
                    }
                } else {
                    DoPrint = false;
                }
            }
            
            bPos.x += ( Cell.x * 1.5f );
        }
        
#if 0
        {
            char String[ 16 ] = {};
            
            int32 Length = 4;
            for( int32 Char0 = 0; Char0 < ( nChar - Length ); Char0++ ) {
                for( int32 Char1 = Char0 + Length; Char1 < ( nChar - Length ); Char1++ ) {
                    boo32 DoMatch = true;
                    for( int32 Iter = 0; ( DoMatch ) && ( Iter < Length ); Iter++ ) {
                        if( Str[ Char0 + Iter ] != Str[ Char1 + Iter ] ) {
                            DoMatch = false;
                        }
                    }
                    
                    if( DoMatch ) {
                        for( int32 Iter = 0; Iter < Length; Iter++ ) {
                            String[ Iter ] = Str[ Char0 + Iter ];
                        }
                        DISPLAY_STRING( String );
                    }
                }
            }
        }
#endif
    }
    
    {
        flo32 Between = 8.0f;
        flo32 Width   = ( GetWidth( AppState->UI_Bound ) - ( 27.0f * Between ) ) / 26.0f;
        vec2  Dim     = Vec2( Width, Width );
        
        vec2  AtPos = GetTL( AppState->UI_Bound ) + Vec2( Between, -480.0f );
        
        
        char String[ 8 ] = {};
        
        for( int32 iChar = 0; iChar < 26; iChar++ ) {
            vec4 Outline_Color    = COLOR_GRAY( 0.25f );
            vec4 Background_Color = COLOR_GRAY( 0.1f );
            
            rect Bound = RectTLD( AtPos, Dim );
            if( IsInRect( Mouse->Position, Bound ) ) {
                Outline_Color = COLOR_GRAY( 0.75f );
                
                for( int32 iKey = 0; iKey < 26; iKey++ ) {
                    KEY_STATE_CODE KeyCode = ( KEY_STATE_CODE )( KeyCode_a + iKey );
                    if( WasPressed( Keyboard, KeyCode ) ) {
                        char C = 'a' + ( char )iKey;
                        
                        AppState->Replace[ iChar ] = C;
                    }
                }
                
                if( WasPressed( Mouse, MouseButton_Right ) ) {
                    AppState->Replace[ iChar ] = ( char )( 'A' + iChar );
                }
            }
            DrawRect( Pass, Bound, Outline_Color );
            DrawRect( Pass, AddRadius( Bound, -2.0f ), Background_Color );
            
            sprintf( String, "%c = %c", 'A' + iChar, AppState->Replace[ iChar ] );
            DrawString( Pass, AppState->Font, String, GetCenter( Bound ), TextAlign_Center, TEXT_COLOR );
            
            AtPos.x += ( Width + Between );
        }
    }
    
    {
        int32 CharPerLine = 220;
        int32 nLine       = ceilToInt32( ( flo32 )nChar / ( flo32 )CharPerLine );
        for( int32 iLine = 0; iLine < nLine; iLine++ ) {
            int32 bChar = CharPerLine * iLine;
            vec2  Dim   = Vec2( AppState->Font->AdvanceWidth_Space, 14.0f );
            vec2 bPos   = GetTL( AppState->UI_Bound ) + Vec2( 20.0f - Dim.x * ( flo32 )bChar, -600.0f - ( Dim.y * ( flo32 )iLine ) );
            vec2 AtPos  = bPos;
            
            if( iHover >= 0 ) {
                int32 Hover_nChar = ( int32 )strlen( Hover_Str );
                
                if( Hover_nChar == 1 ) {
                    for( int32 iChar = 0; iChar < nChar; iChar++ ) {
                        if( Str[ iChar ] == Hover_Str[ 0 ] ) {
                            rect Bound = RectTLD( AtPos, Dim );
                            DrawRect( Pass, Bound, COLOR_GRAY( 0.25f ) );
                        }
                        
                        AtPos.x += Dim.x;
                        AtPos.x = ( flo32 )RoundToInt32( AtPos.x );
                    }
                } else if( Hover_nChar == 2 ) {
                    for( int32 iChar = 0; iChar < ( nChar - 1 ); iChar++ ) {
                        if( ( Str[ iChar ] == Hover_Str[ 0 ] ) && ( Str[ iChar + 1 ] == Hover_Str[ 1 ] ) ) {
                            rect Bound = RectTLD( AtPos, Dim * Vec2( 2.0f, 1.0f ) );
                            DrawRect( Pass, Bound, COLOR_GRAY( 0.25f ) );
                        }
                        
                        AtPos.x += Dim.x;
                        AtPos.x = ( flo32 )RoundToInt32( AtPos.x );
                    }
                } else if( Hover_nChar == 3 ) {
                    for( int32 iChar = 0; iChar < ( nChar - 2 ); iChar++ ) {
                        if( ( Str[ iChar ] == Hover_Str[ 0 ] ) && ( Str[ iChar + 1 ] == Hover_Str[ 1 ] ) && ( Str[ iChar + 2 ] == Hover_Str[ 2 ] ) ) {
                            rect Bound = RectTLD( AtPos, Dim * Vec2( 3.0f, 1.0f ) );
                            DrawRect( Pass, Bound, COLOR_GRAY( 0.25f ) );
                        }
                        
                        AtPos.x += Dim.x;
                        AtPos.x = ( flo32 )RoundToInt32( AtPos.x );
                    }
                }
            }
        }
        
        for( int32 iChar = 0; iChar < nChar; iChar++ ) {
            for( int32 iReplace = 0; iReplace < 26; iReplace++ ) {
                if( Str[ iChar ] == ( char )( iReplace + 'A' ) ) {
                    Str[ iChar ] = AppState->Replace[ iReplace ];
                }
            }
        }
        
        for( int32 iLine = 0; iLine < nLine; iLine++ ) {
            int32 bChar = CharPerLine * iLine;
            vec2  Dim   = Vec2( AppState->Font->AdvanceWidth_Space, 14.0f );
            vec2 bPos   = GetTL( AppState->UI_Bound ) + Vec2( 20.0f - Dim.x * ( flo32 )bChar, -600.0f - ( Dim.y * ( flo32 )iLine ) );
            
            DrawString( Pass, AppState->Font, Str, bPos, TextAlign_TopLeft, TEXT_COLOR );
        }
    }
    
    DISPLAY_STRING( "E T A O N I S R H L D C U P F M W Y B G V K Q X J Z" );
}