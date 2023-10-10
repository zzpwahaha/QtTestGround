#include <qapplication.h>
#include "Worker.h"



int main(int argc, char* argv[]) 
{
    qDebug() << "Main thread is: " << QThread::currentThread();
    WQThread* thread = new WQThread;
    Worker* worker = new Worker();
    worker->moveToThread(thread);
    //QObject::connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    QObject::connect(thread, &QThread::started, worker, &Worker::process);
    //QObject::connect(worker, &Worker::finished, thread, &QThread::quit);
    QObject::connect(worker, &Worker::finished, thread, &WQThread::wquit);

    
    //QObject::connect(worker, &Worker::finished, worker, &QObject::deleteLater);
    QObject::connect(thread, &QThread::finished, worker, &QObject::deleteLater);

    //QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    QObject::connect(worker, &QObject::destroyed, thread, &QObject::deleteLater);

    thread->start();


    QApplication app(argc, argv);
    app.exec();
}