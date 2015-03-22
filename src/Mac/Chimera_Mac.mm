/*****************************************************************************
 * Copyright © 2014-2015 Sergey Radionov <rsatom_gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include "Chimera_Mac.h"

#include <dlfcn.h>

#include <QtDebug>
#include <QtPlugin>
#include <QCoreApplication>
#include <QQmlContext>

#include <QuickLayer/QuickLayer.h>
#include <QuickLayer/FboQuickView.h>
#include <QuickLayer/FboQuickWrapperWindow.h>

#include "QtConf.h"

std::string getPluginPath()
{
    ::Dl_info dlinfo;
    if( ::dladdr( (void*)::NP_Initialize, &dlinfo ) != 0 ) {
        return dlinfo.dli_fname;
    } else {
        return std::string();
    }
}

////////////////////////////////////////////////////////////////////////////////
//Chimera_Mac static members
////////////////////////////////////////////////////////////////////////////////
void Chimera_Mac::StaticInitialize()
{
    InitQtConf( getPluginPath() + "/../" );

    QmlChimera::StaticInitialize();
}

void Chimera_Mac::StaticDeinitialize()
{
}

////////////////////////////////////////////////////////////////////////////////
//Chimera_Mac::Private struct
////////////////////////////////////////////////////////////////////////////////
struct Chimera_Mac::Private
{
    Private();

    QScopedPointer<FboQuickView> quickViewPtr;
    QuickLayer* quickLayer;

    QPointer<FboQuickWrapperWindow> fullscreenWindow;
};

Chimera_Mac::Private::Private()
    : quickLayer( nullptr ), fullscreenWindow( nullptr )
{

}

////////////////////////////////////////////////////////////////////////////////
//Chimera_Mac class
////////////////////////////////////////////////////////////////////////////////
Chimera_Mac::Chimera_Mac()
    : m_p( new Private )
{
}

Chimera_Mac::~Chimera_Mac()
{
    cleanup();
}

void Chimera_Mac::cleanup()
{
    if( m_p->fullscreenWindow ) {
        delete m_p->fullscreenWindow;
        m_p->fullscreenWindow = nullptr;
    }

    if( m_p->quickLayer ) {
        [m_p->quickLayer release];
        m_p->quickLayer = nullptr;
    }

    m_p->quickViewPtr.reset();
}

bool Chimera_Mac::onWindowAttached( FB::AttachedEvent*, FB::PluginWindowMacCA* w )
{
    vlcOpen();

    typedef FB::PluginWindowMac PW;
    if( PW::DrawingModelCoreAnimation == w->getDrawingModel() ||
        PW::DrawingModelInvalidatingCoreAnimation == w->getDrawingModel() )
    {
        m_p->quickViewPtr.reset( new FboQuickView() );
        connect( m_p->quickViewPtr.data(), SIGNAL( statusChanged( Status ) ),
                 this, SLOT( quickViewStatusChanged() ) );

        m_p->quickLayer =
            [[QuickLayer alloc] initWithFboQuickWindow: m_p->quickViewPtr.data()];
        QuickLayer* layer = m_p->quickLayer;

        layer.asynchronous =
            ( PW::DrawingModelInvalidatingCoreAnimation == w->getDrawingModel() ) ?
                NO : YES;
        layer.autoresizingMask = kCALayerWidthSizable | kCALayerHeightSizable;
        layer.needsDisplayOnBoundsChange = YES;

        if( PW::DrawingModelInvalidatingCoreAnimation == w->getDrawingModel() )
            w->StartAutoInvalidate( 1. / 30. );
        [(CALayer*) w->getDrawingPrimitive() addSublayer: layer];
    } else {
        assert( false );
    }

    m_qmlVlcPlayer =
        new QmlVlcSurfacePlayerProxy( static_cast<vlc::player*>( this ),
                                      m_p->quickViewPtr.data() );
    m_qmlVlcPlayer->classBegin();

    applyPlayerOptions();
    setQml();

    return true;
}

bool Chimera_Mac::onWindowDetached( FB::DetachedEvent*, FB::PluginWindowMacCA* )
{
    cleanup();

    return true;
}

void Chimera_Mac::setQml()
{
    if( !m_p->quickViewPtr )
        return;

    if( m_p->quickViewPtr ) {
        QQmlContext* context = m_p->quickViewPtr->rootContext();
        context->setContextObject( this );
        context->setContextProperty( QStringLiteral( "plugin" ), this );
    }

    const std::string& qml = get_options().get_qml();
    if( qml.empty() ) {
        m_p->quickViewPtr->setSource( getQmlSource() );
    } else {
        QUrl qmlUrl = QStringLiteral( "qml" );
        m_p->quickViewPtr->setQml( QString::fromStdString( qml ), qmlUrl );
    }
}

void Chimera_Mac::quickViewStatusChanged()
{
    QList<QQmlError> errors = m_p->quickViewPtr->errors();

    if( !errors.empty() ) {
        QString errStr;
        for( int i = 0; i < errors.count(); ++i ) {
            if( !errStr.isEmpty() ) {
                 errStr += QStringLiteral( "; " );
            }
            errStr += errors[i].toString();
        }
        m_qmlError = errStr.toStdString();
    } else
        m_qmlError.clear();
}

void Chimera_Mac::on_option_change( vlc_player_option_e o )
{
    QmlChimera::on_option_change( o );

    if( po_qml == o ) {
        setQml();
    }
}

bool Chimera_Mac::onWindowResized( FB::ResizedEvent* e, FB::PluginWindowMacCA* w )
{
    if( m_p->quickViewPtr && !isFullscreen() )
         m_p->quickViewPtr->resize( w->getWindowWidth(), w->getWindowHeight() );

    return false;
}

Qt::MouseButton FbToQtMouseButton( const FB::MouseButtonEvent& e )
{
    switch( e.m_Btn )
    {
        case FB::MouseButtonEvent::MouseButton_Left:
            return Qt::LeftButton;
        case FB::MouseButtonEvent::MouseButton_Right:
            return Qt::RightButton;
        case FB::MouseButtonEvent::MouseButton_Middle:
            return Qt::MiddleButton;
        case FB::MouseButtonEvent::MouseButton_None:
            return Qt::NoButton;
    }
}

bool Chimera_Mac::onMouseDown( FB::MouseDownEvent* e, FB::PluginWindowMacCA* )
{
    if( isFullscreen() )
        return false;

    Qt::MouseButton button = FbToQtMouseButton( *e );
    QPointF mousePoint( e->m_x, e->m_y );
    QMouseEvent mouseEvent( QEvent::MouseButtonPress, mousePoint, mousePoint,
                            button, button, Qt::NoModifier );
    QCoreApplication::sendEvent( m_p->quickViewPtr.data(), &mouseEvent );

    return true;
}

bool Chimera_Mac::onMouseUp( FB::MouseUpEvent* e, FB::PluginWindowMacCA* )
{
    if( isFullscreen() )
        return false;

    Qt::MouseButton button = FbToQtMouseButton( *e );
    QPointF mousePoint( e->m_x, e->m_y );
    QMouseEvent mouseEvent( QEvent::MouseButtonRelease, mousePoint, mousePoint,
                            button, Qt::NoButton, Qt::NoModifier );
    QCoreApplication::sendEvent( m_p->quickViewPtr.data(), &mouseEvent );

    return true;
}

bool Chimera_Mac::onMouseDblClick( FB::MouseDoubleClickEvent* e, FB::PluginWindowMacCA* )
{
    if( isFullscreen() )
        return false;

    Qt::MouseButton button = FbToQtMouseButton( *e );
    QPointF mousePoint( e->m_x, e->m_y );
    QMouseEvent mouseEvent( QEvent::MouseButtonDblClick, mousePoint, mousePoint,
                            button, Qt::NoButton, Qt::NoModifier );
    QCoreApplication::sendEvent( m_p->quickViewPtr.data(), &mouseEvent );

    return true;
}

bool Chimera_Mac::onMouseEnter( FB::MouseEnteredEvent* e, FB::PluginWindowMacCA* )
{
    if( isFullscreen() )
        return false;

    QPointF mousePoint( e->m_x, e->m_y );
    QEnterEvent mouseEvent( mousePoint, mousePoint, mousePoint );
    QCoreApplication::sendEvent( m_p->quickViewPtr.data(), &mouseEvent );

    return true;
}

bool Chimera_Mac::onMouseLeave( FB::MouseExitedEvent* e, FB::PluginWindowMacCA* )
{
    if( isFullscreen() )
        return false;

    QPointF mousePoint( e->m_x, e->m_y );
    QMouseEvent mouseEvent( QEvent::Leave, mousePoint, mousePoint,
                            Qt::NoButton, Qt::NoButton, Qt::NoModifier );
    QCoreApplication::sendEvent( m_p->quickViewPtr.data(), &mouseEvent );

    return true;
}

bool Chimera_Mac::onMouseMove( FB::MouseMoveEvent* e, FB::PluginWindowMacCA* )
{
    if( isFullscreen() )
        return false;

    QPointF mousePoint( e->m_x, e->m_y );
    QMouseEvent mouseEvent( QEvent::MouseMove, mousePoint, mousePoint,
                            Qt::NoButton, Qt::NoButton, Qt::NoModifier );
    QCoreApplication::sendEvent( m_p->quickViewPtr.data(), &mouseEvent );

    return true;
}

Qt::Key FbToQtKey( const FB::FBKeyCode k )
{
    switch( k ) {
        //case FBKEY_ADD:
        case FB::FBKEY_ALT: return Qt::Key_Alt;
        case FB::FBKEY_CANCEL: return Qt::Key_Cancel;
        case FB::FBKEY_CAPSLOCK: return Qt::Key_CapsLock;
        case FB::FBKEY_CLEAR: return Qt::Key_Clear;
        case FB::FBKEY_CONTROL: return Qt::Key_Control;
        //case FB::FBKEY_DECIMAL:
        case FB::FBKEY_DELETE: return Qt::Key_Delete;
        //case FB::FBKEY_DIVIDE:
        case FB::FBKEY_ESCAPE: return Qt::Key_Escape;
        case FB::FBKEY_EXECUTE: return Qt::Key_Execute;
        case FB::FBKEY_F1: return Qt::Key_F1;
        case FB::FBKEY_F10: return Qt::Key_F10;
        case FB::FBKEY_F11: return Qt::Key_F11;
        case FB::FBKEY_F12: return Qt::Key_F12;
        case FB::FBKEY_F13: return Qt::Key_F13;
        case FB::FBKEY_F14: return Qt::Key_F14;
        case FB::FBKEY_F15: return Qt::Key_F15;
        case FB::FBKEY_F16: return Qt::Key_F16;
        case FB::FBKEY_F17: return Qt::Key_F17;
        case FB::FBKEY_F18: return Qt::Key_F18;
        case FB::FBKEY_F19: return Qt::Key_F19;
        case FB::FBKEY_F2: return Qt::Key_F2;
        case FB::FBKEY_F20: return Qt::Key_F20;
        case FB::FBKEY_F21: return Qt::Key_F21;
        case FB::FBKEY_F22: return Qt::Key_F22;
        case FB::FBKEY_F3: return Qt::Key_F3;
        case FB::FBKEY_F4: return Qt::Key_F4;
        case FB::FBKEY_F5: return Qt::Key_F5;
        case FB::FBKEY_F6: return Qt::Key_F6;
        case FB::FBKEY_F7: return Qt::Key_F7;
        case FB::FBKEY_F8: return Qt::Key_F8;
        case FB::FBKEY_F9: return Qt::Key_F9;
        case FB::FBKEY_HELP: return Qt::Key_Help;
        case FB::FBKEY_INSERT: return Qt::Key_Insert;
        //case FB::FBKEY_LBUTTON:
        //case FB::FBKEY_MBUTTON:
        case FB::FBKEY_MULTIPLY: return Qt::Key_multiply;
        case FB::FBKEY_NUMLOCK: return Qt::Key_NumLock;
        //case FB::FBKEY_NUMPAD0:
        //case FB::FBKEY_NUMPAD1:
        //case FB::FBKEY_NUMPAD2:
        //case FB::FBKEY_NUMPAD3:
        //case FB::FBKEY_NUMPAD4:
        //case FB::FBKEY_NUMPAD5:
        //case FB::FBKEY_NUMPAD6:
        //case FB::FBKEY_NUMPAD7:
        //case FB::FBKEY_NUMPAD8:
        //case FB::FBKEY_NUMPAD9:
        case FB::FBKEY_PAGEDOWN: return Qt::Key_PageDown;
        case FB::FBKEY_PAGEUP: return Qt::Key_PageUp;
        //case FB::FBKEY_RBUTTON:
        case FB::FBKEY_RETURN: return Qt::Key_Return;
        //case FB::FBKEY_SCROLL:
        case FB::FBKEY_SELECT: return Qt::Key_Select;
        //case FB::FBKEY_SEPARATOR:
        case FB::FBKEY_SHIFT: return Qt::Key_Shift;
        //case FB::FBKEY_PRINTSCREEN:
        case FB::FBKEY_SPACE: return Qt::Key_Space;
        //case FB::FBKEY_SUBTRACT:
        case FB::FBKEY_TAB: return Qt::Key_Tab;
        case FB::FBKEY_UP: return Qt::Key_Up;
        case FB::FBKEY_DOWN: return Qt::Key_Down;
        case FB::FBKEY_LEFT: return Qt::Key_Left;
        case FB::FBKEY_RIGHT: return Qt::Key_Right;
        //case FB::FBKEY_WINDOWS_LEFT:
        case FB::FBKEY_WINDOWS_MENU: return Qt::Key_Menu;
        //case FB::FBKEY_WINDOWS_RIGHT:
        case FB::FBKEY_BACKSPACE: return Qt::Key_Backspace;
        case FB::FBKEY_END: return Qt::Key_End;
        case FB::FBKEY_HOME: return Qt::Key_Home;
        case FB::FBKEY_PAUSE: return Qt::Key_Pause;
        case FB::FBKEY_PRINT: return Qt::Key_Print;
        //case FB::FBKEY_COMMAND:
        case FB::FBKEY_OPTION: return Qt::Key_Option;
        //case FB::FBKEY_RSHIFT:
        //case FB::FBKEY_ROPTION:
        //case FB::FBKEY_RCONTROL:
        //case FB::FBKEY_FUNCTION:
        //case FB::FBKEY_FORWARDDELETE:
        //case FB::FBKEY_LEFTARROW:
        //case FB::FBKEY_RIGHTARROW:
        //case FB::FBKEY_DOWNARROW:
        //case FB::FBKEY_UPARROW:
        //case FB::FBKEY_UNKNOWN:
        default: return Qt::Key_unknown;
    }
}

bool Chimera_Mac::onKeyDown( FB::KeyDownEvent* e, FB::PluginWindowMacCA* )
{
    if( isFullscreen() )
        return false;

    Qt::Key qtKey = FbToQtKey( e->m_key_code );
    if( Qt::Key_unknown != qtKey ) {
        QKeyEvent keyEvent( QEvent::KeyPress, qtKey, Qt::NoModifier );
        QCoreApplication::sendEvent( m_p->quickViewPtr.data(), &keyEvent );

        return true;
    }

    return false;
}

bool Chimera_Mac::onKeyUp( FB::KeyUpEvent* e, FB::PluginWindowMacCA* )
{
    if( isFullscreen() )
        return false;

    Qt::Key qtKey = FbToQtKey( e->m_key_code );
    if( Qt::Key_unknown != qtKey ) {
        QKeyEvent keyEvent( QEvent::KeyRelease, qtKey, Qt::NoModifier );
        QCoreApplication::sendEvent( m_p->quickViewPtr.data(), &keyEvent );

        return true;
    }

    return false;
}

bool Chimera_Mac::isFullscreen()
{
    return ( nullptr != m_p->fullscreenWindow );
}

void Chimera_Mac::setFullscreen( bool fs )
{
    if( fs && !isFullscreen() ) {
        m_p->fullscreenWindow =
            new FboQuickWrapperWindow( m_p->quickViewPtr.data() );

        QWindow* fsw = m_p->fullscreenWindow;
        fsw->setFlags( fsw->flags() | Qt::CustomizeWindowHint );
        const FB::Rect r = GetWindow()->getWindowPosition();
        fsw->setGeometry( r.left, r.top, r.right - r.left, r.bottom - r.top );
        fsw->showFullScreen();

        [m_p->quickLayer setHidden: YES];

        Q_EMIT fullscreenChanged( true );
    } else if( !fs && isFullscreen() ) {
        m_p->fullscreenWindow->showNormal();
        delete m_p->fullscreenWindow;
        m_p->fullscreenWindow = nullptr;

        m_p->quickViewPtr->resize( GetWindow()->getWindowWidth(),
                                   GetWindow()->getWindowHeight() );
        [m_p->quickLayer setHidden: NO];

        Q_EMIT fullscreenChanged( false );
    }
}
