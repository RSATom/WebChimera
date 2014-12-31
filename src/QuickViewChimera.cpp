#include "QuickViewChimera.h"

#include <QQmlContext>
#include <QQmlComponent>

void QuickViewChimera::setQml()
{
    if( !m_quickViewPtr )
        return;

    if( m_quickViewPtr ) {
        QQmlContext* context = m_quickViewPtr->rootContext();
        context->setContextObject( this );
        context->setContextProperty( QStringLiteral( "plugin" ), this );
    }

    const std::string& qml = get_options().get_qml();
    QList<QQmlError> errors;
    if( qml.empty() ) {
        m_quickViewPtr->setSource( getQmlSource() );
        if( QQuickView::Error == m_quickViewPtr->status() )
            errors = m_quickViewPtr->errors();
    } else {
        QUrl qmlUrl = QStringLiteral( "qml" );
        QScopedPointer<QQmlComponent> component( new QQmlComponent( m_quickViewPtr->engine(), m_quickViewPtr.data() ) );

        component->setData( QByteArray( qml.data(), qml.size() ), qmlUrl );
        QObject* rootObject = component->create();
        if( QQmlComponent::Error == component->status() )
            errors = component->errors();

        if( rootObject ) {
            cleanQuickView();
            m_quickViewPtr->setContent( qmlUrl, component.take(), rootObject );
        }
    }

    if( !errors.empty() ) {
        QString errStr;
        for( int i = 0; i < errors.count(); ++i )
            errStr += errors[i].toString();
        m_qmlError = errStr.toStdString();
    } else
        m_qmlError.clear();
}

void QuickViewChimera::cleanQuickView()
{
    if( !m_quickViewPtr )
        return;

    //little hack to cleanup QQuickView content on Qt 5.2.
    //FIXME! check for compatibility with future Qt versions.
    m_quickViewPtr->setSource( QUrl() );
}

void QuickViewChimera::on_option_change( vlc_player_option_e o )
{
    QmlChimera::on_option_change( o );

    if( po_qml == o ) {
        setQml();
    }
}
