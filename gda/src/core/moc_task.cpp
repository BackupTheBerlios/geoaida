/****************************************************************************
** Task meta object code from reading C++ file 'task.h'
**
** Created: Mon Oct 16 15:49:48 2006
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "task.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Task::className() const
{
    return "Task";
}

QMetaObject *Task::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Task( "Task", &Task::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Task::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Task", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Task::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Task", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Task::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"check", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "check()", &slot_0, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_ptr, "ProcessEntry", QUParameter::In }
    };
    static const QUMethod signal_0 = {"newProcess", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ "load", &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod signal_1 = {"stateChanged", 5, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod signal_2 = {"processFinished", 1, param_signal_2 };
    static const QMetaData signal_tbl[] = {
	{ "newProcess(ProcessEntry*)", &signal_0, QMetaData::Protected },
	{ "stateChanged(QListViewItem*,int,QString,QString,float)", &signal_1, QMetaData::Protected },
	{ "processFinished(QListViewItem*)", &signal_2, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"Task", parentObject,
	slot_tbl, 1,
	signal_tbl, 3,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Task.setMetaObject( metaObj );
    return metaObj;
}

void* Task::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Task" ) )
	return this;
    return QObject::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL newProcess
void Task::newProcess( ProcessEntry* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

// SIGNAL stateChanged
void Task::stateChanged( QListViewItem* t0, int t1, QString t2, QString t3, float t4 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[6];
    static_QUType_ptr.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    static_QUType_QString.set(o+3,t2);
    static_QUType_QString.set(o+4,t3);
    static_QUType_ptr.set(o+5,&t4);
    activate_signal( clist, o );
}

// SIGNAL processFinished
void Task::processFinished( QListViewItem* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 2 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

bool Task::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: check(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Task::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: newProcess((ProcessEntry*)static_QUType_ptr.get(_o+1)); break;
    case 1: stateChanged((QListViewItem*)static_QUType_ptr.get(_o+1),(int)static_QUType_int.get(_o+2),(QString)static_QUType_QString.get(_o+3),(QString)static_QUType_QString.get(_o+4),(float)(*((float*)static_QUType_ptr.get(_o+5)))); break;
    case 2: processFinished((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool Task::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool Task::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
