#pragma once

#include "ChimeraAPI.h"

#include "QmlChimera.h"

////////////////////////////////////////////////////////////////////////////
/// JSRootQmlAPI
////////////////////////////////////////////////////////////////////////////
FB_FORWARD_PTR( JSRootQmlAPI)
class JSRootQmlAPI : public JSRootAPI
{
public:
    JSRootQmlAPI( const QmlChimeraPtr& plugin, const FB::BrowserHostPtr& host )
        : JSRootAPI( boost::static_pointer_cast<Chimera>( plugin ), host )
    {
        registerProperty( "qmlError",
                          make_property( this,
                                         &JSRootQmlAPI::get_qmlError ) );

        registerProperty( "qml",
                          make_property( this, &JSRootQmlAPI::get_qml,
                                               &JSRootQmlAPI::set_qml ) );

        registerMethod( "emitJsMessage", make_method( this, &JSRootQmlAPI::emitJsMessage ) );
    }

    virtual ~JSRootQmlAPI() {};

    std::string get_qmlError();

    std::string get_qml();
    void set_qml( const std::string& qml );

    FB_JSAPI_EVENT( QmlMessage, 1, ( std::string ) );
    void emitJsMessage( const std::string& message );
};
