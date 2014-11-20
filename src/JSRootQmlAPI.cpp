#include "JSRootQmlAPI.h"

std::string JSRootQmlAPI::get_qmlError()
{
    ChimeraPtr plg = getPlugin();

    return plg->getQmlError();
}

std::string JSRootQmlAPI::get_qml()
{
    ChimeraPtr plg = getPlugin();
    const vlc_player_options& o = plg->get_options();

    return o.get_qml();
}

void JSRootQmlAPI::set_qml( const std::string& qml )
{
    ChimeraPtr plg = getPlugin();
    vlc_player_options& o = plg->get_options();

    o.set_qml( qml );
}

void JSRootQmlAPI::emitJsMessage( const std::string& message )
{
    QmlChimeraPtr plg = boost::static_pointer_cast<QmlChimera>( getPlugin() );

    plg->emitJsMessage( message );
}
