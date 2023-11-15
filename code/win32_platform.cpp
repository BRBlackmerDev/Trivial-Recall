
internal FILE_DATA
Win32_ReadFile( MEMORY * Memory, char * SaveDir, char * FileName, char * extension ) {
	FILE_DATA Result = {};
	
	char FilePath[ 2048 ] = {};
    GenFilePath( FilePath, SaveDir, FileName, extension );
	
	HANDLE File = CreateFile( FilePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );
	if( File != INVALID_HANDLE_VALUE ) {
		LARGE_INTEGER _FileSize = {};
		if( GetFileSizeEx( File, &_FileSize ) ) {
            uint32 FileSize = ( uint32 )_FileSize.QuadPart;
            uint8 * dest = ( uint8 * )_PushSize( Memory, FileSize );
			
			DWORD bytesRead = {};
            BOOL  Status    = ReadFile( File, dest, FileSize, &bytesRead, 0 );
            if( ( Status ) && ( FileSize == bytesRead ) ) {
                Result.Contents = dest;
                Result.Size     = FileSize;
			} else {
				_PopSize( Memory, Result.Size );
                
                char Debug[ 128 ] {};
                sprintf( Debug, "ERROR! File: %s : unable to read contents!\n", FilePath );
                OutputDebugString( Debug );
			}
			CloseHandle( File );
		} else {
            char Debug[ 128 ] = {};
            sprintf( Debug, "ERROR! File: %s : does not contain any data!\n", FilePath );
            OutputDebugString( Debug );
		}
	} else {
        char Debug[ 128 ] = {};
        sprintf( Debug, "ERROR! File: %s : unable to open!\n", FilePath );
        OutputDebugString( Debug );
	}
	
	return Result;
}

internal boo32
Win32_WriteFile( char * SaveDir, char * FileName, char * extension, void * data, uint32 size ) {
	boo32 Result = false;
	
	char FilePath[ 2048 ] = {};
    GenFilePath( FilePath, SaveDir, FileName, extension );
    
	HANDLE FileHandle = CreateFile( FilePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0 );
	if( FileHandle != INVALID_HANDLE_VALUE ) {
		DWORD bytesWritten;
		if( WriteFile( FileHandle, data, size, &bytesWritten, 0 ) ) {
			Result = ( bytesWritten == size );
		}
        
		CloseHandle( FileHandle );
	}
    
	return Result;
}

internal boo32
Win32_DoesFileExist( char * SaveDir, char * FileName, char * extension ) {
	char FilePath[ 2048 ] = {};
    GenFilePath( FilePath, SaveDir, FileName, extension );
    
    boo32 Result = PathFileExists( FilePath );
    return Result;
}

internal boo32
Win32_DeleteFile( char * SaveDir, char * FileName, char * extension ) {
	char FilePath[ 2048 ] = {};
    GenFilePath( FilePath, SaveDir, FileName, extension );
    
    boo32 Result = DeleteFile( FilePath );
    return Result;
}

uint32 Frame_Counter = 0;
internal void
Win32_outputMessage( char * header, char * message ) {
    char str[ 64 ] = {};
    sprintf( str, "%10u : %8s : %s\n", Frame_Counter, header, message );
    OutputDebugString( str );
}

internal void
Win32_logMessage( uint32 message, boo32 fromWndProc ) {
    char * header0 = "WndProc";
    char * header1 = "my";
    
    char * header = ( fromWndProc ? header0 : header1 );
    
    switch( message ) {
        case WM_ACTIVATE: {
            // Sent to both the window being activated and the window being deactivated. If the windows use the same input queue, the message is sent synchronously, first to the window procedure of the Top-level window being deactivated, then to the window procedure of the Top-level window being activated. If the windows use different input queues, the message is sent asynchronously, so the window is activated immediately.
            Win32_outputMessage( header, "WM_ACTIVATE" );
        } break;
        
        case WM_ACTIVATEAPP: {
            // Sent when the active window changes. Sent to both the activated window and the deactivated window.
            Win32_outputMessage( header, "WM_ACTIVATEAPP" );
        } break;
        
        case WM_CAPTURECHANGED: {
            // Sent to the window that is losing the Mouse capture.
            Win32_outputMessage( header, "WM_CAPTURECHANGED" );
        } break;
        
        case WM_CLOSE: {
            // Sent as a signal that a window or an application should terminate.
            Win32_outputMessage( header, "WM_CLOSE" );
        } break;
        
        case WM_DESTROY: {
            // Sent when a window is being destroyed. It is sent to the window procedure of the window being destroyed after the window is removed from the screen.
            Win32_outputMessage( header, "WM_DESTROY" );
        } break;
        
        case WM_GETMINMAXINFO: {
            // Sent to a window when its size or Position is about to change.
            Win32_outputMessage( header, "WM_GETMINMAXINFO" );
        } break;
        
        case WM_GETICON: {
            // Sent to a window to retrieve the handle to the large or small icon associated with the window.
            Win32_outputMessage( header, "WM_GETICON" );
        } break;
        
        case WM_IME_SETCONTEXT: {
            // Sent to an application when a window is activated. A window receives this message through its WindowProc function.
            Win32_outputMessage( header, "WM_IME_SETCONTEXT" );
        } break;
        
        case WM_IME_NOTIFY: {
            // Sent to an application to notify it of changes to the IME window. A window receives this message through its WindowProc function.
            Win32_outputMessage( header, "WM_IME_NOTIFY" );
        } break;
        
        case WM_KILLFOCUS: {
            // Sent to a window immediately before it loses the Keyboard focus.
            Win32_outputMessage( header, "WM_KILLFOCUS" );
        } break;
        
        case WM_SETFOCUS: {
            // Sent to a window after it has gained the Keyboard focus.
            Win32_outputMessage( header, "WM_KILLFOCUS" );
        } break;
        
        case WM_MOUSEMOVE: {
            // Posted to a window when the cursor moves. If the Mouse is not captured, the message is posted to the window that contains the cursor. Otherwise, the message is posted to the window that has captured the Mouse.
            // Win32_outputMessage( header, "WM_MOUSEMOVE" );
        } break;
        
        case WM_NCACTIVATE: {
            // Sent to a window when its nonclient area needs to be changed to indicate an active or inactive State.
            Win32_outputMessage( header, "WM_NCACTIVATE" );
        } break;
        
        case WM_NCCREATE: {
            // Sent when a window is first created. Sent prior to the WM_CREATE message.
            Win32_outputMessage( header, "WM_NCCREATE" );
        } break;
        
        case WM_NCCALCSIZE: {
            // Sent when the size and Position of a window's client area must be calculated.
            Win32_outputMessage( header, "WM_NCCALCSIZE" );
        } break;
        
        case WM_NCDESTROY: {
            // Notifies a window that its nonclient area is being destroyed. The DestroyWindow function sends the WM_NCDESTROY message to the window following the WM_DESTROY message.WM_DESTROY is used to free the allocated Memory object associated with the window. The WM_NCDESTROY message is sent after the child windows have been destroyed. In contrast, WM_DESTROY is sent before the child windows are destroyed. A window receives this message through its WindowProc function.
            Win32_outputMessage( header, "WM_NCDESTROY" );
        } break;
        
        case WM_NCHITTEST: {
            // Sent to a window to determine what part of the window corresponds to a particular screen coordinate. Sent, for example, in response to Mouse cursor movement or when a Mouse button is pressed/released.
            // Win32_outputMessage( header, "WM_NCHITTEST" );
        } break;
        
        case WM_NCLBUTTONDOWN: {
            // Sent to a window when the user presses the Left Mouse button while the cursor is within the nonclient area of the window.
            Win32_outputMessage( header, "WM_NCLBUTTONDOWN" );
        } break;
        
        case WM_NCMOUSEMOVE: {
            // Posted to a window when the cursor is moved within the nonclient area of the window. This message is posted to the window that contains the cursor. If a window has captured the Mouse, this message is not posted.
            // Win32_outputMessage( header, "WM_NCMOUSEMOVE" );
        } break;
        
        case WM_PAINT: {
            // Sent when the system or another application makes a request to paint a portion of the application's window.
            Win32_outputMessage( header, "WM_PAINT" );
        } break;
        
        case WM_QUIT: {
            // Indicates a request to terminate an application, and is generated when the application calls the PostQuitMessage function.
            Win32_outputMessage( header, "WM_QUIT" );
        } break;
        
        case WM_SETCURSOR: {
            // Sent if the Mouse cursor moves, but the input is not captured by the window.
            //Win32_outputMessage( header, "WM_SETCURSOR" );
        } break;
        
        case WM_SYSCOMMAND: {
            // A window receives this message when the user chooses a command from the Window menu (formerly known as the system or control menu) or when the user chooses the maximize button, minimize button, restore button, or close button.
            Win32_outputMessage( header, "WM_SYSCOMMAND" );
        } break;
		
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP: {
            Win32_outputMessage( header, "Windows Key Message" );
            // ERROR! Keyboard message was Passed directly to WindowProc! All Keyboard messages should be handled directly by the Win32_processPendingMessages()!
        } break;
        
        case WM_TIMER: {
            // Posted to the installing thread's message queue when a Timer expires. The message is posted by the GetMessage or PeekMessage function.
            // Win32_outputMessage( header, "WM_TIMER" );
        } break;
        
        case WM_WINDOWPOSCHANGED: {
            // Sent to a window whose size, Position, or place in the Z order has changed as a Result of a call to the SetWindowPos function or another window-management function.
            Win32_outputMessage( header, "WM_WINDOWPOSCHANGED" );
        } break;
        
        case WM_WINDOWPOSCHANGING: {
            // Sent to a window whose size, Position, or place in the Z order is about to change as a Result of a call to the SetWindowPos function or another window-management function.
            Win32_outputMessage( header, "WM_WINDOWPOSCHANGING" );
        } break;
        
        default: {
            char str[ 8 ] = {};
            sprintf( str, "0x%04X", ( uint32 )message );
            Win32_outputMessage( header, str );
        } break;
    }
}

global_variable boo32 global_restoreFocus = false;
global_variable WINDOWPLACEMENT GlobalWindowPosition = { sizeof( GlobalWindowPosition ) };

LRESULT CALLBACK
Win32_WindowProc( HWND window, uint32 message, WPARAM wParam, LPARAM lParam ) {
    //Win32_logMessage( message, true );
	LRESULT Result = 0;
    switch( message ) {
        case WM_ACTIVATEAPP: {
            global_restoreFocus = true;
        } break;
        
        case WM_DESTROY: {
            PostQuitMessage( 0 );
        } break;
        
        default: {
            Result = DefWindowProc( window, message, wParam, lParam );
        } break;
    }
    return Result;
}

internal void
Win32_processPendingMessages( PLATFORM * Platform, HWND windowHandle ) {
    KEYBOARD_STATE * Keyboard = &Platform->Keyboard;
    Keyboard->nEvents = 0;
    
    MOUSE_STATE * Mouse = &Platform->Mouse;
    MOUSE_endOfFrame( Mouse );
    
	MSG message = {};
    while( PeekMessage( &message, 0, 0, 0, PM_REMOVE ) ) {
        //Win32_logMessage( message.message, false );
        
        switch( message.message ) {
            case WM_QUIT: {
                Platform->IsRunning = false;
            } break;
            
			case WM_MOUSEWHEEL: {
				int16 wParam_hi = ( ( message.wParam >> 16 ) & 0xFFFF );
				int32 WheelClicks = wParam_hi / 120;
				Mouse->WheelClicks = WheelClicks;
			} break;
			
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP: {
				addButtonEvent( Mouse, MouseButton_Left, ( message.wParam & MK_LBUTTON ), Keyboard->flags );
			} break;
			
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP: {
				addButtonEvent( Mouse, MouseButton_Middle, ( message.wParam & MK_MBUTTON ), Keyboard->flags );
			} break;
			
			case WM_RBUTTONUP:
			case WM_RBUTTONDOWN: {
				addButtonEvent( Mouse, MouseButton_Right, ( message.wParam & MK_RBUTTON ), Keyboard->flags );
			} break;
			
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYDOWN:
			case WM_KEYUP: {
                boo32 wasDown = ( ( message.lParam & ( 1 << 30 ) ) != 0 );
				boo32 IsDown  = ( ( message.lParam & ( 1 << 31 ) ) == 0 );
				if( IsDown != wasDown ) {
                    switch( message.wParam ) {
						case VK_BACK: { addKeyEvent( Keyboard, KeyCode_Backspace, IsDown, Keyboard->flags ); } break;
						case VK_TAB: 		{ addKeyEvent( Keyboard, KeyCode_tab, IsDown, Keyboard->flags ); } break;
						case VK_RETURN:	{ addKeyEvent( Keyboard, KeyCode_Enter, IsDown, Keyboard->flags ); } break;
						case VK_SHIFT: 	{
                            if( IsDown ) {
                                Keyboard->flags |=  KEYBOARD_FLAGS__SHIFT;
                            } else {
                                Keyboard->flags &= ~KEYBOARD_FLAGS__SHIFT;
                            }
							addKeyEvent( Keyboard, KeyCode_shift, IsDown, Keyboard->flags );
						} break;
						case VK_CONTROL: {
                            if( IsDown ) {
                                Keyboard->flags |=  KEYBOARD_FLAGS__CONTROL;
                            } else {
                                Keyboard->flags &= ~KEYBOARD_FLAGS__CONTROL;
                            }
							addKeyEvent( Keyboard, KeyCode_control, IsDown, Keyboard->flags );
						} break;
						case VK_MENU: 		{
                            if( IsDown ) {
                                Keyboard->flags |=  KEYBOARD_FLAGS__ALT;
                            } else {
                                Keyboard->flags &= ~KEYBOARD_FLAGS__ALT;
                            }
                            addKeyEvent( Keyboard, KeyCode_alt, IsDown, Keyboard->flags );
                        } break;
						case VK_CAPITAL: 	{ addKeyEvent( Keyboard, KeyCode_capsLock, 	IsDown, Keyboard->flags ); } break;
						case VK_ESCAPE: 	{ addKeyEvent( Keyboard, KeyCode_Escape, 		IsDown, Keyboard->flags ); } break;
						case VK_SPACE: 	{ addKeyEvent( Keyboard, KeyCode_space, 		IsDown, Keyboard->flags ); } break;
						case VK_PRIOR: 	{ addKeyEvent( Keyboard, KeyCode_pageUp, 		IsDown, Keyboard->flags ); } break;
						case VK_NEXT: 		{ addKeyEvent( Keyboard, KeyCode_pageDown, 	IsDown, Keyboard->flags ); } break;
						case VK_END: 		{ addKeyEvent( Keyboard, KeyCode_end, 			IsDown, Keyboard->flags ); } break;
						case VK_HOME: 		{ addKeyEvent( Keyboard, KeyCode_home, 		IsDown, Keyboard->flags ); } break;
						case VK_LEFT: 		{ addKeyEvent( Keyboard, KeyCode_Left, 		IsDown, Keyboard->flags ); } break;
						case VK_RIGHT: 	{ addKeyEvent( Keyboard, KeyCode_Right, 		IsDown, Keyboard->flags ); } break;
						case VK_UP: 		{ addKeyEvent( Keyboard, KeyCode_Up, 			IsDown, Keyboard->flags ); } break;
						case VK_DOWN: 		{ addKeyEvent( Keyboard, KeyCode_Down,       IsDown, Keyboard->flags ); } break;
						case VK_INSERT: 	{ addKeyEvent( Keyboard, KeyCode_insert, 		IsDown, Keyboard->flags ); } break;
						case VK_DELETE: 	{ addKeyEvent( Keyboard, KeyCode_delete, 		IsDown, Keyboard->flags ); } break;
						case 0x30: { addKeyEvent( Keyboard, KeyCode_0, IsDown, Keyboard->flags ); } break;
						case 0x31: { addKeyEvent( Keyboard, KeyCode_1, IsDown, Keyboard->flags ); } break;
						case 0x32: { addKeyEvent( Keyboard, KeyCode_2, IsDown, Keyboard->flags ); } break;
						case 0x33: { addKeyEvent( Keyboard, KeyCode_3, IsDown, Keyboard->flags ); } break;
						case 0x34: { addKeyEvent( Keyboard, KeyCode_4, IsDown, Keyboard->flags ); } break;
						case 0x35: { addKeyEvent( Keyboard, KeyCode_5, IsDown, Keyboard->flags ); } break;
						case 0x36: { addKeyEvent( Keyboard, KeyCode_6, IsDown, Keyboard->flags ); } break;
						case 0x37: { addKeyEvent( Keyboard, KeyCode_7, IsDown, Keyboard->flags ); } break;
						case 0x38: { addKeyEvent( Keyboard, KeyCode_8, IsDown, Keyboard->flags ); } break;
						case 0x39: { addKeyEvent( Keyboard, KeyCode_9, IsDown, Keyboard->flags ); } break;
						case 0x41: { addKeyEvent( Keyboard, KeyCode_a, IsDown, Keyboard->flags ); } break;
						case 0x42: { addKeyEvent( Keyboard, KeyCode_b, IsDown, Keyboard->flags ); } break;
						case 0x43: { addKeyEvent( Keyboard, KeyCode_c, IsDown, Keyboard->flags ); } break;
						case 0x44: { addKeyEvent( Keyboard, KeyCode_d, IsDown, Keyboard->flags ); } break;
						case 0x45: { addKeyEvent( Keyboard, KeyCode_e, IsDown, Keyboard->flags ); } break;
						case 0x46: { addKeyEvent( Keyboard, KeyCode_f, IsDown, Keyboard->flags ); } break;
						case 0x47: { addKeyEvent( Keyboard, KeyCode_g, IsDown, Keyboard->flags ); } break;
						case 0x48: { addKeyEvent( Keyboard, KeyCode_h, IsDown, Keyboard->flags ); } break;
						case 0x49: { addKeyEvent( Keyboard, KeyCode_i, IsDown, Keyboard->flags ); } break;
						case 0x4A: { addKeyEvent( Keyboard, KeyCode_j, IsDown, Keyboard->flags ); } break;
						case 0x4B: { addKeyEvent( Keyboard, KeyCode_k, IsDown, Keyboard->flags ); } break;
						case 0x4C: { addKeyEvent( Keyboard, KeyCode_l, IsDown, Keyboard->flags ); } break;
						case 0x4D: { addKeyEvent( Keyboard, KeyCode_m, IsDown, Keyboard->flags ); } break;
						case 0x4E: { addKeyEvent( Keyboard, KeyCode_n, IsDown, Keyboard->flags ); } break;
						case 0x4F: { addKeyEvent( Keyboard, KeyCode_o, IsDown, Keyboard->flags ); } break;
						case 0x50: { addKeyEvent( Keyboard, KeyCode_p, IsDown, Keyboard->flags ); } break;
						case 0x51: { addKeyEvent( Keyboard, KeyCode_q, IsDown, Keyboard->flags ); } break;
						case 0x52: { addKeyEvent( Keyboard, KeyCode_r, IsDown, Keyboard->flags ); } break;
						case 0x53: { addKeyEvent( Keyboard, KeyCode_s, IsDown, Keyboard->flags ); } break;
						case 0x54: { addKeyEvent( Keyboard, KeyCode_t, IsDown, Keyboard->flags ); } break;
						case 0x55: { addKeyEvent( Keyboard, KeyCode_u, IsDown, Keyboard->flags ); } break;
						case 0x56: { addKeyEvent( Keyboard, KeyCode_v, IsDown, Keyboard->flags ); } break;
						case 0x57: { addKeyEvent( Keyboard, KeyCode_w, IsDown, Keyboard->flags ); } break;
						case 0x58: { addKeyEvent( Keyboard, KeyCode_x, IsDown, Keyboard->flags ); } break;
						case 0x59: { addKeyEvent( Keyboard, KeyCode_y, IsDown, Keyboard->flags ); } break;
						case 0x5A: { addKeyEvent( Keyboard, KeyCode_z, IsDown, Keyboard->flags ); } break;
						case VK_OEM_1: { addKeyEvent( Keyboard, KeyCode_semicolon, IsDown, Keyboard->flags ); } break;
						case VK_OEM_PLUS: { addKeyEvent( Keyboard, KeyCode_equal, IsDown, Keyboard->flags ); } break;
						case VK_OEM_COMMA: { addKeyEvent( Keyboard, KeyCode_comma, IsDown, Keyboard->flags ); } break;
						case VK_OEM_MINUS: { addKeyEvent( Keyboard, KeyCode_dash, IsDown, Keyboard->flags ); } break;
						case VK_OEM_PERIOD: { addKeyEvent( Keyboard, KeyCode_period, IsDown, Keyboard->flags ); } break;
						case VK_OEM_2: { addKeyEvent( Keyboard, KeyCode_forwardSlash, IsDown, Keyboard->flags ); } break;
						case VK_OEM_3: { addKeyEvent( Keyboard, KeyCode_tilde, IsDown, Keyboard->flags ); } break;
						case VK_OEM_4: { addKeyEvent( Keyboard, KeyCode_openBracket, IsDown, Keyboard->flags ); } break;
						case VK_OEM_5: { addKeyEvent( Keyboard, KeyCode_BackSlash, IsDown, Keyboard->flags ); } break;
						case VK_OEM_6: { addKeyEvent( Keyboard, KeyCode_closeBracket, IsDown, Keyboard->flags ); } break;
						case VK_OEM_7: { addKeyEvent( Keyboard, KeyCode_quote, IsDown, Keyboard->flags ); } break;
						case VK_NUMPAD0: { addKeyEvent( Keyboard, KeyCode_num0, IsDown, Keyboard->flags ); } break;
						case VK_NUMPAD1: { addKeyEvent( Keyboard, KeyCode_num1, IsDown, Keyboard->flags ); } break;
						case VK_NUMPAD2: { addKeyEvent( Keyboard, KeyCode_num2, IsDown, Keyboard->flags ); } break;
						case VK_NUMPAD3: { addKeyEvent( Keyboard, KeyCode_num3, IsDown, Keyboard->flags ); } break;
						case VK_NUMPAD4: {addKeyEvent( Keyboard, KeyCode_num4, IsDown, Keyboard->flags ); } break;
						case VK_NUMPAD5: { addKeyEvent( Keyboard, KeyCode_num5, IsDown, Keyboard->flags ); } break;
						case VK_NUMPAD6: { addKeyEvent( Keyboard, KeyCode_num6, IsDown, Keyboard->flags ); } break;
						case VK_NUMPAD7: { addKeyEvent( Keyboard, KeyCode_num7, IsDown, Keyboard->flags ); } break;
						case VK_NUMPAD8: { addKeyEvent( Keyboard, KeyCode_num8, IsDown, Keyboard->flags ); } break;
						case VK_NUMPAD9: { addKeyEvent( Keyboard, KeyCode_num9, IsDown, Keyboard->flags ); } break;
						case VK_MULTIPLY: { addKeyEvent( Keyboard, KeyCode_numMul, IsDown, Keyboard->flags ); } break;
						case VK_ADD: { addKeyEvent( Keyboard, KeyCode_numAdd, IsDown, Keyboard->flags ); } break;
						case VK_DECIMAL: { addKeyEvent( Keyboard, KeyCode_numPeriod, IsDown, Keyboard->flags ); } break;
						case VK_SUBTRACT: { addKeyEvent( Keyboard, KeyCode_numSub, IsDown, Keyboard->flags ); } break;
						case VK_DIVIDE: { addKeyEvent( Keyboard, KeyCode_numDiv, IsDown, Keyboard->flags ); } break;
						case VK_F1:      { addKeyEvent( Keyboard, KeyCode_F1,   IsDown, Keyboard->flags ); } break;
						case VK_F2:      { addKeyEvent( Keyboard, KeyCode_F2,   IsDown, Keyboard->flags ); } break;
						case VK_F3:      { addKeyEvent( Keyboard, KeyCode_F3,   IsDown, Keyboard->flags ); } break;
						case VK_F4:      { addKeyEvent( Keyboard, KeyCode_F4,   IsDown, Keyboard->flags ); } break;
						case VK_F5:      { addKeyEvent( Keyboard, KeyCode_F5,   IsDown, Keyboard->flags ); } break;
						case VK_F6:      { addKeyEvent( Keyboard, KeyCode_F6,   IsDown, Keyboard->flags ); } break;
						case VK_F7:      { addKeyEvent( Keyboard, KeyCode_F7,   IsDown, Keyboard->flags ); } break;
						case VK_F8:      { addKeyEvent( Keyboard, KeyCode_F8,   IsDown, Keyboard->flags ); } break;
						case VK_F9:      { addKeyEvent( Keyboard, KeyCode_F9,   IsDown, Keyboard->flags ); } break;
						case VK_F10:     { addKeyEvent( Keyboard, KeyCode_F10,  IsDown, Keyboard->flags ); } break;
						case VK_F11:     { addKeyEvent( Keyboard, KeyCode_F11,  IsDown, Keyboard->flags ); } break;
						case VK_F12:     { addKeyEvent( Keyboard, KeyCode_F12,  IsDown, Keyboard->flags ); } break;
						default: {
							char String[ 128 ] = {};
							sprintf( String, "Key message received, but not processed: %lu %s\n", (uint32)message.wParam, ( IsDown ? "PRESSED" : "RELEASED" ) );
							OutputDebugString( String );
						} break;
					};
				}
			} break;
            
            default: {
                TranslateMessage( &message );
                DispatchMessage ( &message );
            } break;
        }
	}
}

internal int64
Win32_GetPerfCount() {
	LARGE_INTEGER PerfCounter = {};
	QueryPerformanceCounter( &PerfCounter );
	
	int64 Result = *( ( int64 * )&PerfCounter );
	return Result;
}


internal flo32
Win32_GetMSElapsed( int64 StartCounter, int64 EndCounter, int64 PerfFrequency ) {
	LARGE_INTEGER CounterA  = *( ( LARGE_INTEGER * )&StartCounter  );
	LARGE_INTEGER CounterB  = *( ( LARGE_INTEGER * )&EndCounter    );
	LARGE_INTEGER Frequency = *( ( LARGE_INTEGER * )&PerfFrequency );
	flo32 Result = 1000.0f * ( ( flo32 )( CounterB.QuadPart - CounterA.QuadPart ) / ( flo32 )( Frequency.QuadPart ) );
	return Result;
}

internal void
ToggleFullscreen( HWND Window ) {
    // NOTE(casey): This follows Raymond Chen's prescription
    // for fullscreen toggling, see:
    // http://blogs.msdn.com/b/oldnewthing/archive/2010/04/12/9994016.aspx
    
    DWORD Style = GetWindowLong( Window, GWL_STYLE );
    if( Style & WS_POPUPWINDOW ) {
        MONITORINFO MonitorInfo = { sizeof( MonitorInfo ) };
        if( GetWindowPlacement( Window, &GlobalWindowPosition ) &&
           GetMonitorInfo( MonitorFromWindow( Window, MONITOR_DEFAULTTOPRIMARY ), &MonitorInfo ) ) {
            SetWindowLong( Window, GWL_STYLE, Style & ~WS_POPUPWINDOW );
            SetWindowPos( Window, HWND_TOP, MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top, MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED );
        }
    } else {
        SetWindowLong( Window, GWL_STYLE, Style | WS_POPUPWINDOW | WS_MAXIMIZE );
        SetWindowPlacement( Window, &GlobalWindowPosition );
        SetWindowPos( Window, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED );
    }
}

int32 CALLBACK
WinMain( HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int32 windowShowCode ) {
    srand( ( uint32 )time( 0 ) );
    
	boo32 SleepFreqIsHiRes = ( timeBeginPeriod( 1 ) == TIMERR_NOERROR );
    Assert( SleepFreqIsHiRes );
    
    LARGE_INTEGER PerfFrequency;
    QueryPerformanceFrequency( &PerfFrequency );
    
    uint64 Memory_size = 0;
    Memory_size += sizeof( APP_STATE );
    Memory_size += sizeof( PLATFORM  );
    Memory_size += APP_PermMemorySize;
    Memory_size += APP_TempMemorySize;
    
    MEMORY PlatformMemory = {};
    PlatformMemory.Size = Memory_size;
    PlatformMemory.Base = VirtualAlloc( 0, Memory_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
    
    APP_STATE * AppState = _PushType( &PlatformMemory, APP_STATE );
    PLATFORM  * Platform = _PushType( &PlatformMemory, PLATFORM  );
    Platform->TargetSec  = 1.0f / APP_TargetFPS;
    
    Platform->PermMemory = SubArena( &PlatformMemory, APP_PermMemorySize, false );
    Platform->TempMemory = SubArena( &PlatformMemory, APP_TempMemorySize, false );
    Platform->ReadFile      = Win32_ReadFile;
    Platform->WriteFile     = Win32_WriteFile;
    Platform->DoesFileExist = Win32_DoesFileExist;
    Platform->DeleteFile    = Win32_DeleteFile;
    
    int32 Screen_Width       = GetSystemMetrics( SM_CXSCREEN );
    int32 Screen_Height      = GetSystemMetrics( SM_CYSCREEN );
    flo32 Screen_FPS         = 60.0f;
    flo32 Screen_AspectRatio = ( flo32 )Screen_Width / ( flo32 )Screen_Height;
    
    DISPLAY_SETTINGS DisplaySettings = {};
    DisplaySettings.Width             = Screen_Width;
    DisplaySettings.Height            = Screen_Height;
    DisplaySettings.RefreshRate_Numer = 1;
    DisplaySettings.RefreshRate_Denom = 60;
    
    WNDCLASS WindowClass = {};
    WindowClass.lpfnWndProc   = Win32_WindowProc;
    WindowClass.hInstance     = instance;
    WindowClass.lpszClassName = "WindowClass";
    WindowClass.hCursor       = LoadCursor( 0, IDC_ARROW );
    WindowClass.hbrBackground = CreateSolidBrush( RGB( 0, 0, 0 ) );
    WindowClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    
    if( RegisterClass( &WindowClass ) ) {
        uint32 Window_Flags = WS_POPUPWINDOW | WS_MAXIMIZE;
        // NOTE: These flags are the only combination I can find that will work correctly with OpenGL to:
        //  1) Switch to debugger on breakpoint. Otherwise, the window just freezes and you have to use Ctrl + Alt + Del;
        //  2) Screen Capture using PrtSc. Otherwise, if the OpenGL window is fullscreen, it will only capture the desktop windows.
        //  3) Fill the screen without a window border.
        //  4) Alt + Tab.
        // Now, my understanding is that this might still be using the Windows Desktop Compositor, which can be bypassed by using fullscreen. Toggling out of fullscreen adds a border line around the window. I didn't notice any performance change between fullscreen and windowed modes, so I'm just leaving these flags for the window. As far as I know, there are no other side-effects that I don't know about.
        
        HWND Window = CreateWindowEx( 0, "WindowClass", "Caption", Window_Flags, 0, 0, DisplaySettings.Width, DisplaySettings.Height, 0, 0, instance, 0 );
        
        if( Window ) {
            RENDERER     * Renderer    = &Platform->Renderer;
            MEMORY       * PermMemory  = &Platform->PermMemory;
            MEMORY       * TempMemory  = &Platform->TempMemory;
            //AUDIO_SYSTEM * AudioSystem = &Platform->AudioSystem;
            ShowWindow( Window, SW_SHOW );
            
            Platform->Renderer  = InitOpenGL( Window, PermMemory, DisplaySettings );
            Platform->IsRunning = true;
            
            Platform->Renderer.Texture_IsLoaded[ TextureID_Framebuffer_Layer ] = true;
            Platform->Renderer.Texture_Handle  [ TextureID_Framebuffer_Layer ] = Platform->Renderer.Framebuffer[ FramebufferID_Layer ].TextureID;
            
            if( Renderer->HasVSync ) {
                DEVMODE DevMode = {};
                DevMode.dmSize  = sizeof( DEVMODE );
                if( EnumDisplaySettings( 0, ENUM_CURRENT_SETTINGS, &DevMode ) ) {
                    uint32 TargetFPS  = ( uint32 )APP_TargetFPS;
                    uint32 DisplayFPS = ( uint32 )DevMode.dmDisplayFrequency;
                    
                    Platform->FramesPerSec = ( int32 )DisplayFPS;
                    
                    if( DisplayFPS > 1 )  {
                        if( DisplayFPS != TargetFPS ) {
                            Platform->TargetSec    = 1.0f / ( flo32 )DisplayFPS;
                            // set framerate and dt for the game
                            // adjust friction
                        }
                    } else {
                        // TODO: NOTE: Research and implement what to do if EnumDisplaySettings() returns a  0 or 1  for  dmDisplayFrequency
                        InvalidCodePath;
                    }
                } else {
                    InvalidCodePath;
                    // TODO: Error logging.
                    // NOTE: Probably catastrophic failure. I can't imagine why EnumDisplaySettings() would fail on the call to the current settings.
                }
                
#if 0
                // NOTE: This will iterate over the available display modes, like when using DirectX.
                int32  nDevMode = 0;
                int32  iModeNum = ( int32 )ENUM_CURRENT_SETTINGS;
                DEVMODE DevMode = {};
                DevMode.dmSize  = sizeof( DEVMODE );
                
                while( EnumDisplaySettings( 0, iModeNum, &DevMode ) ) {
                    nDevMode += 1;
                    iModeNum += 1;
                    
                    int32 BreakHere = 10;
                }
                
                // NOTE: This will set the display mode to the settings in DevMode. There are some additional flags that could be useful in some circumstances. See documentation.
                //uint32 Flags  = 0;
                //int32  Result = ChangeDisplaySettings( &DevMode, 0 );
#endif
            }
            
            int64 FontPerf_Start = Win32_GetPerfCount();
            
            flo32 Dev_Width       = DEV_WIDTH;
            flo32 Dev_Height      = DEV_HEIGHT;
            flo32 Dev_AspectRatio = Dev_Width / Dev_Height;
            
            flo32 Screen_xScale = 1.0f;
            flo32 Screen_yScale = 1.0f;
            
            if( Screen_AspectRatio < Dev_AspectRatio ) {
                Screen_yScale = Dev_AspectRatio / Screen_AspectRatio;
            } else {
                Screen_xScale = Dev_AspectRatio / Screen_AspectRatio;
            }
            
            // NOTE: FontScale can only be used after text is being drawn to a render buffer that matches the screen dimensions, not the dev dimensions.
            flo32 FontScale = ( flo32 )Screen_Height / Dev_Height;
            flo32 bFontSize = 16.0f;
            
            AppState->FontList[ FontID_1_00 ] = LoadFont( FONT_SAVE_DIRECTORY, "SundayGrapes", bFontSize * 1.00f );
            AppState->FontList[ FontID_1_50 ] = LoadFont( FONT_SAVE_DIRECTORY, "SundayGrapes", bFontSize * 1.50f );
            AppState->FontList[ FontID_2_00 ] = LoadFont( FONT_SAVE_DIRECTORY, "SundayGrapes", bFontSize * 2.00f );
            AppState->FontList[ FontID_2_25 ] = LoadFont( FONT_SAVE_DIRECTORY, "SundayGrapes", bFontSize * 2.25f );
            AppState->FontList[ FontID_2_50 ] = LoadFont( FONT_SAVE_DIRECTORY, "SundayGrapes", bFontSize * 2.50f );
            AppState->FontList[ FontID_3_00 ] = LoadFont( FONT_SAVE_DIRECTORY, "SundayGrapes", bFontSize * 3.00f );
            AppState->FontList[ FontID_4_00 ] = LoadFont( FONT_SAVE_DIRECTORY, "SundayGrapes", bFontSize * 4.00f );
            AppState->FontList[ FontID_6_00 ] = LoadFont( FONT_SAVE_DIRECTORY, "SundayGrapes", bFontSize * 6.00f );
            AppState->FontList[ FontID_7_00 ] = LoadFont( FONT_SAVE_DIRECTORY, "SundayGrapes", bFontSize * 7.00f );
            
            for( int32 iFont = 0; iFont < FontID_Count; iFont++ ) {
                FONT * Font = AppState->FontList + iFont;
                Font->TextureID = TextureID_Font + 1 + iFont;
                UploadTexture( Renderer, ( TEXTURE_ID )Font->TextureID, Font->Texture_Width, Font->Texture_Height, Font->Texture_Data );
            }
            
#if ( DEBUG_BUILD )
            FONT Font  = LoadFont( FONT_SAVE_DIRECTORY, "LiberationMono", bFontSize );
            Font.TextureID = TextureID_Font;
            UploadTexture( Renderer, TextureID_Font, Font.Texture_Width, Font.Texture_Height, Font.Texture_Data );
            AppState->Font        = &Font;
#endif
            
            int64 FontPerf_End = Win32_GetPerfCount();
            flo32 FontPerf_MSElapsed = Win32_GetMSElapsed( FontPerf_Start, FontPerf_End, PerfFrequency.QuadPart );
            CONSOLE_VALUE( flo32, FontPerf_MSElapsed );
            
            InitPass( Renderer, &Renderer->Pass_Game,  PermMemory, 4096 * 16 );
            InitPass( Renderer, &Renderer->Pass_Layer, PermMemory, 2 );
            InitPass( Renderer, &Renderer->Pass_Debug, PermMemory, 4096 );
            
            int64 TexturePerf_Start = Win32_GetPerfCount();
            
            {
                FILE_DATA File = Win32_ReadFile( TempMemory, 0, "Asset", "pak" );
                if( File.Contents ) {
                    uint8 * Ptr = ( uint8 * )File.Contents;
                    
                    { // TEXTURES
                        int32 nTexture = _read( Ptr, int32 );
                        for( int32 iTexture = 0; iTexture < nTexture; iTexture++ ) {
                            int32 Width  = _read( Ptr, int32 );
                            int32 Height = _read( Ptr, int32 );
                            
                            if( ( Width > 0 ) && ( Height > 0 ) ) {
                                uint32 * Texture_Data = _addra( Ptr, uint32, Width * Height );
                                UploadTexture( Renderer, ( TEXTURE_ID )iTexture, Width, Height, Texture_Data );
                            }
                        }
                    }
                    
                    { // MESHES
                        int32 nMesh = _read( Ptr, int32 );
                        for( int32 iMesh = 0; iMesh < nMesh; iMesh++ ) {
                            MODEL_ID MeshID = _read( Ptr, MODEL_ID );
                            
                            int32 nVertex = _read( Ptr, int32 );
                            int32 nIndex  = _read( Ptr, int32 );
                            
                            VERTEX1 * Vertex = _addra( Ptr, VERTEX1, nVertex );
                            int32   * Index  = _addra( Ptr, int32,   nIndex  );
                            
                            //Renderer->ModelData[ MeshID ] = UploadModel( &Renderer->ModelBuffer, Vertex, nVertex, ( uint32 * )Index, nIndex );
                        }
                    }
                    
                    { // AUDIO
#if USE_FMOD
                        InitFMod( Platform, AppState, TempMemory, &Ptr );
#else
                        int32 nAudio = _read( Ptr, int32 );
#if 0
                        for( int32 iAudio = 0; iAudio < nAudio; iAudio++ ) {
                            AUDIO_ID AudioID = _read( Ptr, AUDIO_ID );
                            
                            int32 nSamples   = _read( Ptr, int32 );
                            int32 SampleSize = AUDIO__BYTES_PER_SAMPLE * AUDIO__CHANNEL_COUNT;
                            
                            int32 Size = nSamples * SampleSize;
                            
                            uint8 * Dest = ( uint8 * )_PushSize( PermMemory, Size );
                            memcpy( Dest, Ptr, nSamples * SampleSize );
                            
                            Ptr += ( nSamples * SampleSize );
                            
                            AUDIO_DATA Audio = {};
                            Audio.Audio    = Dest;
                            Audio.nSamples = nSamples;
                            AppState->AudioList[ AudioID ] = Audio;
                        }
#endif
#endif
                    }
                    
                    _PopSize( TempMemory, File.Size );
                }
            }
            
            int64 TexturePerf_End = Win32_GetPerfCount();
            flo32 TexturePerf_MSElapsed = Win32_GetMSElapsed( TexturePerf_Start, TexturePerf_End, PerfFrequency.QuadPart );
            CONSOLE_VALUE( flo32, TexturePerf_MSElapsed );
            
            //WIN32_AUDIO Win32_Audio = InitAudio( window, AudioSystem, PermMemory );
            
            //vec2 App_Dim     = Vec2( Dev_Width, Dev_Height );
            //vec2 App_HalfDim = App_Dim * 0.5f;
            //rect App_Bound   = RectBLD( Vec2( 0.0f, 0.0f ), App_Dim );
            
            //AppState->App_Dim     = App_Dim;
            //AppState->App_HalfDim = App_HalfDim;
            //AppState->App_Bound   = App_Bound;
            
#if DEBUG_BUILD
            GlobalVar_DebugSystem.Pass          = &Renderer->Pass_Debug;
            GlobalVar_DebugSystem.Font          = &Font;
            GlobalVar_DebugSystem.AdvanceHeight = Font.AdvanceHeight;
            GlobalVar_DebugSystem.ShowDebug     = true;
            Frame_Counter++;
#endif
            
            MOUSE_STATE      * Mouse      = &Platform->Mouse;
            KEYBOARD_STATE   * Keyboard   = &Platform->Keyboard;
            
            ShowCursor( true );
            
            int64  PerfCounter_Wrap  = Win32_GetPerfCount();
            uint64 CycleCounter_Wrap = __rdtsc();
            while( Platform->IsRunning ) {
                //if( WasPressed( Keyboard, KeyCode_f, KEYBOARD_FLAGS__CONTROL ) ) {
                //ToggleFullscreen( Window );
                //}
                
                int64  PerfCounter_Start  = Win32_GetPerfCount();
                uint64 CycleCounter_Start = __rdtsc();
                
                flo32 ThisMSElapsed = Win32_GetMSElapsed( PerfCounter_Wrap, PerfCounter_Start, PerfFrequency.QuadPart );
                //CONSOLE_VALUE( flo32, ThisMSElapsed );
                PerfCounter_Wrap = PerfCounter_Start;
                
                uint64 CycleCounter_Prev = CycleCounter_Wrap;
                CycleCounter_Wrap = __rdtsc();
                uint64 CycleCounter_Temp = CycleCounter_Wrap - CycleCounter_Prev;
                //CONSOLE_VALUE( uint64, CycleCounter_Temp );
                
#if DEBUG_BUILD
                // NOTE : TODO : I'm using UI_Bound for now. This isn't great because UI_Bound isn't initialized until the game is initialized. The platform isn't really supposed to have any knowledge of the game/ui bound. This should probably use the screen bound, but I don't know if the scissor rect would cause issues. This dev machine is the only one I use, so it should be fine. I'll fix it later if it becomes an issue.
                GlobalVar_DebugSystem.AtPos = GetBL( AppState->UI_Bound ) + Vec2( 10.0f, 14.0f );
#endif
                
                Win32_processPendingMessages( Platform, Window );
                if( global_restoreFocus ) {
                    boo32 ResetPos = Mouse->ResetPos;
                    vec2  BasePos  = Mouse->BasePos;
                    
                    global_restoreFocus = false;
                    memset( &Platform->Mouse,    0, sizeof( MOUSE_STATE    ) );
                    memset( &Platform->Keyboard, 0, sizeof( KEYBOARD_STATE ) );
                    
                    Mouse->ResetPos = ResetPos;
                    Mouse->BasePos  = BasePos;
                }
                
                if( WasPressed( Keyboard, KeyCode_F4, ( KEYBOARD_FLAGS__ALT ) ) ) {
                    PostQuitMessage( 0 );
                }
                
                {
                    POINT Mouse_Position = {};
                    GetCursorPos( &Mouse_Position );
                    flo32 MouseX = ( flo32 )Mouse_Position.x;
                    flo32 MouseY = ( flo32 )( DisplaySettings.Height - 1 - Mouse_Position.y );
                    
                    vec2  Screen_Center = Vec2( ( flo32 )Screen_Width, ( flo32 )Screen_Height ) * 0.5f;
                    vec2  Dev_Center    = Vec2( ( flo32 )Dev_Width,    ( flo32 )Dev_Height    ) * 0.5f;
                    
                    vec2  V = Vec2( MouseX, MouseY ) - Screen_Center;
                    V.x *= ( ( flo32 )Dev_Width  / ( flo32 )Screen_Width  ) * Screen_xScale;
                    V.y *= ( ( flo32 )Dev_Height / ( flo32 )Screen_Height ) * Screen_yScale;
                    
                    Mouse->PrevPosition = Mouse->Position;
                    Mouse->Position     = Dev_Center + V;
                    Mouse->dPos         = Mouse->Position - Mouse->PrevPosition;
                    
                    //DISPLAY_VALUE( vec2, Mouse->dPos );
                }
                
                APP_UpdateAndRender( AppState, Platform );
                //UpdateAudio( &Win32_Audio, AppState, AudioSystem );
                
                DISPLAY_VALUE( boo32, Renderer->HasVSync );
                DISPLAY_VALUE( int32, Platform->FramesPerSec );
                
                //AppState->UI_Bound = Rect( 0.0f, 0.0f, ( flo32 )Screen_Width, ( flo32 )Screen_Height );
                //DrawString( &Renderer->Pass_Game, AppState->FontList + FontID_2_00, "AB", Vec2( 400.0f, 400.0f ), Vec2( 1.0f, 1.0f ), COLOR_WHITE );
                //DrawString( &Renderer->Pass_Game, AppState->FontList + FontID_3_00, "CD", Vec2( 800.0f, 400.0f ), Vec2( 1.0f, 1.0f ), COLOR_WHITE );
                
#if USE_FMOD
                {
                    FMOD_RESULT FMOD_Result = FMOD_Studio_System_Update( AppState->FMOD_System );
                    Assert( FMOD_Result == FMOD_OK );
                }
#endif
                
                if( Mouse->DoHideCursor ) {
                    Mouse->DoHideCursor = false;
                    
                    // NOTE: Leaving this commented code here so I don't have to look it up again on msdn.
                    //CURSORINFO CursorInfo = {};
                    //CursorInfo.cbSize = sizeof( CURSORINFO );
                    //GetCursorInfo( &CursorInfo );
                    
                    int32 DisplayCount = 0;
                    do {
                        DisplayCount = ( int32 )ShowCursor( false );
                    } while( DisplayCount > -1 );
                    
                    //CURSORINFO CursorInfo = {};
                    //CursorInfo.cbSize = sizeof( CURSORINFO );
                    //GetCursorInfo( &CursorInfo );
                    
                    //uint32 BreakHere = 10;
                }
                if( Mouse->DoShowCursor ) {
                    Mouse->DoShowCursor = false;
                    
                    int32 DisplayCount = 0;
                    do {
                        DisplayCount = ( int32 )ShowCursor( true );
                    } while( DisplayCount < 0 );
                }
                
                //DISPLAY_VALUE( boo32, Mouse->ResetPos );
                //DISPLAY_VALUE( vec2,  Mouse->BasePos  );
                
                if( Mouse->ResetPos ) {
                    vec2  Screen_Center = Vec2( ( flo32 )Screen_Width, ( flo32 )Screen_Height ) * 0.5f;
                    vec2  Dev_Center    = Vec2( ( flo32 )Dev_Width,    ( flo32 )Dev_Height    ) * 0.5f;
                    
                    vec2 V = Dev_Center - Mouse->BasePos;
                    V.x *= ( ( flo32 )Screen_Width  / ( flo32 )Dev_Width  ) / Screen_xScale;
                    V.y *= ( ( flo32 )Screen_Height / ( flo32 )Dev_Height ) / Screen_yScale;
                    
                    flo32 MouseX = Screen_Center.x + V.x;
                    flo32 MouseY = Screen_Center.y + V.y;
                    
                    int32 X = ( int32 )MouseX;
                    int32 Y = DisplaySettings.Height - 1 - ( int32 )MouseY;
                    SetCursorPos( X, Y );
                    
                    Mouse->Position = Mouse->BasePos;
                }
                
#if DEBUG_BUILD
                if( WasPressed( Keyboard, KeyCode_d, KEYBOARD_FLAGS__CONTROL ) ) {
                    GlobalVar_DebugSystem.ShowDebug = !GlobalVar_DebugSystem.ShowDebug;
                }
                if( GlobalVar_DebugSystem.ErrorOccurred ) {
                    DrawString( GlobalVar_DebugSystem.Pass, GlobalVar_DebugSystem.Font, "ERROR OCCURRED!! Check console for details!", GlobalVar_DebugSystem.AtPos, Vec2( 1.0f, 1.0f ), COLOR_RED );
                    GlobalVar_DebugSystem.AtPos.y += GlobalVar_DebugSystem.AdvanceHeight;
                }
#endif
                
                uint64 CycleCounter_App = __rdtsc();
                
                mat4 Screen_Transform = mat4_Scale( Screen_xScale, 1.0f / Screen_yScale, 1.0f );
                mat4 UI_Transform     = mat4_orthographic( AppState->UI_Bound );
                Renderer->Pass_Game.Camera_Transform  = Screen_Transform * UI_Transform;
                Renderer->Pass_Layer.Camera_Transform = Screen_Transform * UI_Transform;
                Renderer->Pass_Debug.Camera_Transform = Screen_Transform * UI_Transform;
                
#if 0
                {
                    int32 TotalObject = 0;
                    TotalObject += Renderer->Pass_Game.nObject;
                    TotalObject += Renderer->Pass_Debug.nObject;
                    DISPLAY_VALUE( int32, TotalObject );
                    DISPLAY_VALUE( int32, Renderer->Pass_Game.nObject );
                    DISPLAY_VALUE( int32, Renderer->Pass_Game.nVertex );
                    DISPLAY_VALUE( int32, Renderer->Pass_Debug.nObject );
                    DISPLAY_VALUE( int32, Renderer->Pass_Debug.nVertex );
                }
#endif
                
                RenderGame( DisplaySettings, Renderer );
                
#if DEBUG_BUILD
                int64  PerfCounter_End  = Win32_GetPerfCount();
                uint64 CycleCounter_End = __rdtsc();
                
                char   PerfString[ 128 ] = {};
                uint64 CycleElapsed_App  = CycleCounter_App - CycleCounter_Start;
                { // PROFILE MAIN LOOP
                    flo32  MSElapsed = Win32_GetMSElapsed( PerfCounter_Start, PerfCounter_End, PerfFrequency.QuadPart );
                    uint64 CycleElapsed = CycleCounter_End - CycleCounter_Start;
                    
                    sprintf( PerfString, "FRAME PERF: %6.02fms, %10lluMC", MSElapsed, CycleElapsed );
                    DISPLAY_STRING( PerfString );
                    
                    flo32 Percent = ( flo32 )( CycleElapsed_App ) / ( flo32 )( CycleElapsed ) * 100.0f;
                    sprintf( PerfString, "FRAME PERF: APP: %10llu, %.02f%%, DRAW: %.02f%%", CycleElapsed_App, Percent, 100.0f - Percent );
                    DISPLAY_STRING( PerfString );
                }
                
                { // OTHER PROFILING
                    PROFILE_STATE * State = &AppState->Profile;
                    for( uint32 iProfileBlock = 0; iProfileBlock < State->nProfileBlock; iProfileBlock++ ) {
                        PROFILE_BLOCK Profile = State->ProfileBlock[ iProfileBlock ];
                        
                        uint64 CycleElapsed = Profile.CycleCounter_End - Profile.CycleCounter_Start;
                        flo32  Percent = ( flo32 )CycleElapsed / ( flo32 )CycleElapsed_App * 100.0f;
                        
                        sprintf( PerfString, "%s: %10llu, %.02f%%", Profile.Label, CycleElapsed, Percent );
                        DISPLAY_STRING( PerfString );
                    }
                    State->nProfileBlock = 0;
                }
#endif
                
                {
                    HDC DeviceContext = GetDC( Window );
                    SwapBuffers( DeviceContext );
                    ReleaseDC( Window, DeviceContext );
                }
            }
        }
    }
    
    return 0;
}