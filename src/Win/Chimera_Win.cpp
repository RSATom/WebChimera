#include "Chimera_Win.h"

#include <boost/algorithm/string/replace.hpp>

#include <QtCore/qglobal.h>
#include <QtPlugin>
#include <QQmlContext.h>
#include <QtQml/QQml.h>
#include <QtEndian>

#include "QmlVlc/QmlVlcSurfacePlayerProxy.h"

#ifdef QT_STATIC
    Q_IMPORT_PLUGIN( QWindowsIntegrationPlugin );
    Q_IMPORT_PLUGIN( QtQuick2Plugin );
    Q_IMPORT_PLUGIN( QtQuickLayoutsPlugin );
#endif

static std::string qtConf_resource_data;

static const unsigned char qtConf_resource_name[] = {
  // qt
  0x0,0x2,
  0x0,0x0,0x7,0x84,
  0x0,0x71,
  0x0,0x74,
    // etc
  0x0,0x3,
  0x0,0x0,0x6c,0xa3,
  0x0,0x65,
  0x0,0x74,0x0,0x63,
    // qt.conf
  0x0,0x7,
  0x8,0x74,0xa6,0xa6,
  0x0,0x71,
  0x0,0x74,0x0,0x2e,0x0,0x63,0x0,0x6f,0x0,0x6e,0x0,0x66,
};

static const unsigned char qtConf_resource_struct[] = {
  // :
  0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x1,
  // :/qt
  0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x2,
  // :/qt/etc
  0x0,0x0,0x0,0xa,0x0,0x2,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x3,
  // :/qt/etc/qt.conf
  0x0,0x0,0x0,0x16,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,
};

QT_BEGIN_NAMESPACE
extern Q_CORE_EXPORT
bool qRegisterResourceData( int, const unsigned char*,
                            const unsigned char* ,
                            const unsigned char* );
extern Q_CORE_EXPORT
bool qUnregisterResourceData( int, const unsigned char*,
                              const unsigned char*,
                              const unsigned char* );
QT_END_NAMESPACE

extern std::string g_dllPath;

////////////////////////////////////////////////////////////////////////////////
//Chimera_Win class
////////////////////////////////////////////////////////////////////////////////
void Chimera_Win::StaticInitialize()
{
    if( !qApp ) {
        std::string qtPrefix = g_dllPath + "/../";
        boost::algorithm::replace_all( qtPrefix, "\\", "/" );

        qtConf_resource_data = "4321[Paths]\n";
        qtConf_resource_data += "Prefix = " + qtPrefix + "\n";
        uint32_t qtConfSize = qtConf_resource_data.size() - sizeof( qtConfSize );
        uint32_t qtConfSwappedSize = qToBigEndian( qtConfSize );
        memcpy( &qtConf_resource_data[0], &qtConfSwappedSize, sizeof( qtConfSwappedSize ) );

        qRegisterResourceData( 0x01, qtConf_resource_struct,
                               qtConf_resource_name,
                               (const unsigned char*)qtConf_resource_data.data() );
    }

    Chimera::StaticInitialize();
}

void Chimera_Win::StaticDeinitialize()
{
    Chimera::StaticDeinitialize();

    qUnregisterResourceData( 0x01, qtConf_resource_struct,
                             qtConf_resource_name,
                             (const unsigned char*)qtConf_resource_data.data() );
}

Chimera_Win::Chimera_Win()
{
#ifdef QT_STATIC
    qmlProtectModule( "QtQuick", 2 );
    qmlProtectModule( "QtQuick.Layouts", 1 );
#endif
}

Chimera_Win::~Chimera_Win()
{
}

bool Chimera_Win::onWindowAttached( FB::AttachedEvent *evt, FB::PluginWindowWin* w )
{
    vlc_open();

    m_quickViewPtr.reset( new QQuickView );
    m_quickViewPtr->setResizeMode( QQuickView::SizeRootObjectToView );
    m_quickViewPtr->setProperty( "_q_embedded_native_parent_handle", WId( w->getHWND() ) );
    m_quickViewPtr->setFlags( m_quickViewPtr->flags() | Qt::FramelessWindowHint );

    QQmlContext* context = m_quickViewPtr->rootContext();
    m_qmlVlcPlayer = new QmlVlcSurfacePlayerProxy( (vlc::player*)this, m_quickViewPtr.data() );
    m_qmlVlcPlayer->classBegin();
    context->setContextProperty( "vlcPlayer", QVariant::fromValue( m_qmlVlcPlayer ) );

    process_startup_options();

    setQml();

    MoveWindow( (HWND)m_quickViewPtr->winId(), 0, 0, w->getWindowWidth(), w->getWindowHeight(), FALSE );
    m_quickViewPtr->show();

    return false;
}

bool Chimera_Win::onWindowResized( FB::ResizedEvent *evt, FB::PluginWindowWin* w )
{
    if( m_quickViewPtr ) {
        MoveWindow( (HWND)m_quickViewPtr->winId(), 0, 0, w->getWindowWidth(), w->getWindowHeight(), TRUE );
    }

    return false;
}

bool Chimera_Win::is_fullscreen()
{
    return false;
}

void Chimera_Win::set_fullscreen( bool fs )
{
}

void Chimera_Win::toggle_fullscreen()
{
}
