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
