#pragma once
#include <qthread.h>
#include <qstring.h>
#include <qdebug.h>
#include <qobject.h>

class WQThread : public QThread {
    Q_OBJECT
public:
    ~WQThread() {
        qDebug() << "QThread::currentThread()->isInterruptionRequested()" << QThread::currentThread()->isInterruptionRequested();
        qDebug() << "WQThread done at thread: " << QThread::currentThread();
    }
public slots:
    void wquit() {
        qDebug() << "QThread::currentThread()->isInterruptionRequested()" << QThread::currentThread()->isInterruptionRequested();
        qDebug()<<"Thread is quiting: "<< QThread::currentThread();
        QThread::quit();
        qDebug() << "QThread::currentThread()->isInterruptionRequested()" << QThread::currentThread()->isInterruptionRequested();

    }

};


class Worker : public QObject {
    Q_OBJECT

public:
    Worker() {};
    ~Worker() {
        qDebug() << "QThread::currentThread()->isInterruptionRequested()" << QThread::currentThread()->isInterruptionRequested();
        qDebug() << "Worker done at thread: "<<QThread::currentThread();
    };

signals:
    void finished();
    //void error(QString err);

public slots:
    void process() {
        // allocate resources using new here
        qDebug("Hello World!");
        emit finished();
    };


private:
    // add your variables here
};
