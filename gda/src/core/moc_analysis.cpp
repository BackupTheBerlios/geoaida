/****************************************************************************
** Analysis meta object code from reading C++ file 'analysis.h'
**
** Created: Mon Oct 16 15:49:47 2006
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "analysis.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Analysis::className() const
{
    return "Analysis";
}

QMetaObject *Analysis::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Analysis( "Analysis", &Analysis::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Analysis::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Analysis", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Analysis::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Analysis", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Analysis::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"slotstart", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "slotstart()", &slot_0, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_ptr, "INode", QUParameter::In },
	{ 0, &static_QUType_ptr, "GeoImage", QUParameter::In }
    };
    static const QUMethod signal_0 = {"sigReady", 2, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_ptr, "INode", QUParameter::In }
    };
    static const QUMethod signal_1 = {"sigNodeChange", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_ptr, "INode", QUParameter::In },
	{ 0, &static_QUType_ptr, "GeoImage", QUParameter::In }
    };
    static const QUMethod signal_2 = {"sigMapView", 2, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"message", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"sigInteractive", 1, param_signal_4 };
    static const QMetaData signal_tbl[] = {
	{ "sigReady(INode*,GeoImage*)", &signal_0, QMetaData::Public },
	{ "sigNodeChange(INode*)", &signal_1, QMetaData::Public },
	{ "sigMapView(INode*,GeoImage*)", &signal_2, QMetaData::Public },
	{ "message(QString)", &signal_3, QMetaData::Public },
	{ "sigInteractive(bool)", &signal_4, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"Analysis", parentObject,
	slot_tbl, 1,
	signal_tbl, 5,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Analysis.setMetaObject( metaObj );
    return metaObj;
}

void* Analysis::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Analysis" ) )
	return this;
    return QObject::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL sigReady
void Analysis::sigReady( INode* t0, GeoImage* t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,t0);
    static_QUType_ptr.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL sigNodeChange
void Analysis::sigNodeChange( INode* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

// SIGNAL sigMapView
void Analysis::sigMapView( INode* t0, GeoImage* t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 2 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,t0);
    static_QUType_ptr.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL message
void Analysis::message( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL sigInteractive
void Analysis::sigInteractive( bool t0 )
{
    activate_signal_bool( staticMetaObject()->signalOffset() + 4, t0 );
}

bool Analysis::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotstart(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Analysis::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: sigReady((INode*)static_QUType_ptr.get(_o+1),(GeoImage*)static_QUType_ptr.get(_o+2)); break;
    case 1: sigNodeChange((INode*)static_QUType_ptr.get(_o+1)); break;
    case 2: sigMapView((INode*)static_QUType_ptr.get(_o+1),(GeoImage*)static_QUType_ptr.get(_o+2)); break;
    case 3: message((QString)static_QUType_QString.get(_o+1)); break;
    case 4: sigInteractive((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool Analysis::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool Analysis::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
