/****************************************************************************
** MainGui meta object code from reading C++ file 'maingui.h'
**
** Created: Mon Oct 16 15:50:15 2006
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "maingui.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MainGui::className() const
{
    return "MainGui";
}

QMetaObject *MainGui::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MainGui( "MainGui", &MainGui::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MainGui::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MainGui", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MainGui::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MainGui", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MainGui::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"fileLoad", 0, 0 };
    static const QUMethod slot_1 = {"netLoad", 0, 0 };
    static const QUMethod slot_2 = {"netSave", 0, 0 };
    static const QUMethod slot_3 = {"prjLoad", 0, 0 };
    static const QUMethod slot_4 = {"resultSave", 0, 0 };
    static const QUMethod slot_5 = {"resultLoad", 0, 0 };
    static const QUMethod slot_6 = {"usage", 0, 0 };
    static const QUMethod slot_7 = {"start", 0, 0 };
    static const QUMethod slot_8 = {"analyze", 0, 0 };
    static const QUMethod slot_9 = {"showResult", 0, 0 };
    static const QUParameter param_slot_10[] = {
	{ 0, &static_QUType_ptr, "INode", QUParameter::In },
	{ 0, &static_QUType_ptr, "GeoImage", QUParameter::In }
    };
    static const QUMethod slot_10 = {"slotAnalysisReady", 2, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ 0, &static_QUType_ptr, "INode", QUParameter::In }
    };
    static const QUMethod slot_11 = {"slotNodeChange", 1, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ "m", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"opMode", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ 0, &static_QUType_int, 0, QUParameter::Out }
    };
    static const QUMethod slot_13 = {"step", 1, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ 0, &static_QUType_int, 0, QUParameter::Out }
    };
    static const QUMethod slot_14 = {"stop", 1, param_slot_14 };
    static const QUMethod slot_15 = {"proceed", 0, 0 };
    static const QUParameter param_slot_16[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_16 = {"receiveMessage", 1, param_slot_16 };
    static const QUMethod slot_17 = {"cleanUp", 0, 0 };
    static const QUParameter param_slot_18[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_18 = {"slotInteractive", 1, param_slot_18 };
    static const QUMethod slot_19 = {"test", 0, 0 };
    static const QUMethod slot_20 = {"mapSave", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "fileLoad()", &slot_0, QMetaData::Public },
	{ "netLoad()", &slot_1, QMetaData::Public },
	{ "netSave()", &slot_2, QMetaData::Public },
	{ "prjLoad()", &slot_3, QMetaData::Public },
	{ "resultSave()", &slot_4, QMetaData::Public },
	{ "resultLoad()", &slot_5, QMetaData::Public },
	{ "usage()", &slot_6, QMetaData::Public },
	{ "start()", &slot_7, QMetaData::Public },
	{ "analyze()", &slot_8, QMetaData::Public },
	{ "showResult()", &slot_9, QMetaData::Public },
	{ "slotAnalysisReady(INode*,GeoImage*)", &slot_10, QMetaData::Public },
	{ "slotNodeChange(INode*)", &slot_11, QMetaData::Public },
	{ "opMode(int)", &slot_12, QMetaData::Public },
	{ "step()", &slot_13, QMetaData::Public },
	{ "stop()", &slot_14, QMetaData::Public },
	{ "proceed()", &slot_15, QMetaData::Public },
	{ "receiveMessage(QString)", &slot_16, QMetaData::Public },
	{ "cleanUp()", &slot_17, QMetaData::Public },
	{ "slotInteractive(bool)", &slot_18, QMetaData::Public },
	{ "test()", &slot_19, QMetaData::Public },
	{ "mapSave()", &slot_20, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"readyToAna", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"interactive", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"message", 2, param_signal_2 };
    static const QMetaData signal_tbl[] = {
	{ "readyToAna(bool)", &signal_0, QMetaData::Protected },
	{ "interactive(bool)", &signal_1, QMetaData::Protected },
	{ "message(QString,int)", &signal_2, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MainGui", parentObject,
	slot_tbl, 21,
	signal_tbl, 3,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MainGui.setMetaObject( metaObj );
    return metaObj;
}

void* MainGui::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MainGui" ) )
	return this;
    return QObject::qt_cast( clname );
}

// SIGNAL readyToAna
void MainGui::readyToAna( bool t0 )
{
    activate_signal_bool( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL interactive
void MainGui::interactive( bool t0 )
{
    activate_signal_bool( staticMetaObject()->signalOffset() + 1, t0 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL message
void MainGui::message( QString t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 2 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_QString.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

bool MainGui::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: fileLoad(); break;
    case 1: netLoad(); break;
    case 2: netSave(); break;
    case 3: prjLoad(); break;
    case 4: resultSave(); break;
    case 5: resultLoad(); break;
    case 6: usage(); break;
    case 7: start(); break;
    case 8: analyze(); break;
    case 9: showResult(); break;
    case 10: slotAnalysisReady((INode*)static_QUType_ptr.get(_o+1),(GeoImage*)static_QUType_ptr.get(_o+2)); break;
    case 11: slotNodeChange((INode*)static_QUType_ptr.get(_o+1)); break;
    case 12: opMode((int)static_QUType_int.get(_o+1)); break;
    case 13: static_QUType_int.set(_o,step()); break;
    case 14: static_QUType_int.set(_o,stop()); break;
    case 15: proceed(); break;
    case 16: receiveMessage((QString)static_QUType_QString.get(_o+1)); break;
    case 17: cleanUp(); break;
    case 18: slotInteractive((bool)static_QUType_bool.get(_o+1)); break;
    case 19: test(); break;
    case 20: mapSave(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MainGui::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: readyToAna((bool)static_QUType_bool.get(_o+1)); break;
    case 1: interactive((bool)static_QUType_bool.get(_o+1)); break;
    case 2: message((QString)static_QUType_QString.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MainGui::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool MainGui::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
