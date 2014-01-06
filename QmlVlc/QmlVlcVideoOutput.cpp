#include "QmlVlcVideoOutput.h"

QmlVlcVideoOutput::QmlVlcVideoOutput( vlc::player* player, QObject *parent /*= 0*/)
    : QObject( parent ), m_player( player ), m_videoSurface( 0 )
{
    assert( player && m_player->is_open() );
    vlc::basic_vmem_wrapper::open( &( m_player->basic_player() ) );
}

unsigned QmlVlcVideoOutput::video_format_cb( char *chroma,
                                             unsigned *width, unsigned *height,
                                             unsigned *pitches, unsigned *lines )
{
    QMutexLocker locker( &m_frameGuard );

    memcpy( chroma, vlc::DEF_CHROMA, sizeof( vlc::DEF_CHROMA ) - 1 );
    (*pitches) = (*width) * vlc::DEF_PIXEL_BYTES;
    (*lines)   = (*height);

    //+1 for vlc 2.0.3/2.1 bug workaround.
    //They writes after buffer end boundary by some reason unknown to me...
    m_videoFrame =
        QVideoFrame( (*pitches) * ( (*lines) + 1 ),
                     QSize( *width, *height ), (*pitches),
                     QVideoFrame::Format_RGB32 );

    m_surfaceFormat = QVideoSurfaceFormat( QSize( *width, *height ), m_videoFrame.pixelFormat() );
    QMetaObject::invokeMethod( this, "initVideoSurface" );

    return 1;
}

void QmlVlcVideoOutput::video_cleanup_cb()
{
    QMutexLocker locker( &m_frameGuard );

    Q_ASSERT( !m_videoFrame.isMapped() );

    QMetaObject::invokeMethod( this, "cleanupVideoSurface" );
}

void* QmlVlcVideoOutput::video_lock_cb( void **planes )
{
    m_frameGuard.lock();

    if( m_videoFrame.map( QAbstractVideoBuffer::WriteOnly ) )
        (*planes) = m_videoFrame.bits();
    else
        (*planes) = 0;

    return 0;
}

void QmlVlcVideoOutput::video_unlock_cb( void* /*picture*/, void *const * /*planes*/ )
{
    if( m_videoFrame.isMapped() )
        m_videoFrame.unmap();

    m_frameGuard.unlock();
}

void QmlVlcVideoOutput::video_display_cb( void* /*picture*/ )
{
    QMetaObject::invokeMethod( this, "updateFrame" );
}

void QmlVlcVideoOutput::setVideoSurface( QAbstractVideoSurface* s )
{
    if( m_videoSurface == s )
        return;

    cleanupVideoSurface();

    m_videoSurface = s;

    initVideoSurface();
}

void QmlVlcVideoOutput::initVideoSurface()
{
    if( !m_videoSurface )
        return;

    cleanupVideoSurface();

    QMutexLocker locker( &m_frameGuard );
    if( m_surfaceFormat.isValid() )
        m_videoSurface->start( m_surfaceFormat );
}

void QmlVlcVideoOutput::updateFrame()
{
    QMutexLocker locker( &m_frameGuard );

    if( m_videoSurface ) {
        m_videoSurface->present( m_videoFrame );
    }
}

void QmlVlcVideoOutput::cleanupVideoSurface()
{
    if( m_videoSurface && m_videoSurface->isActive() )
        m_videoSurface->stop();
}
