#pragma once

#ifdef FB_WIN
#define WIN32_LEAN_AND_MEAN
#define NOCOMM
#define NOMINMAX
#include <windows.h>
#endif

#include <string>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"

#include "PluginWindow.h"

#ifdef FB_WIN
#include "PluginWindowWin.h"
#endif

#include "PluginEvents/MouseEvents.h"
#include "PluginEvents/AttachedEvent.h"
#include "PluginEvents/DrawingEvents.h"

#include "PluginCore.h"

#include <vlc/vlc.h>

#include <QObject>
#include <QScopedPointer>
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlParserStatus>
#include <QQmlContext>
