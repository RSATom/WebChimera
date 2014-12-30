#pragma once

#include <QScopedPointer>
#include <QQuickView>

#include "QmlChimera.h"

class QuickViewChimera
    : public QmlChimera
{
protected:
    void on_option_change( vlc_player_option_e o ) override;
    void setQml();

private:
    void cleanQuickView();

protected:
    QScopedPointer<QQuickView> m_quickViewPtr;
};
