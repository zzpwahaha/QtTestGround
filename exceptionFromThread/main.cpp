#include <qapplication.h>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <exception>

// https://stackoverflow.com/questions/24270249/qt-handling-exceptions-thrown-from-handlers-of-background-threads

class NormalThrow {
public:
    void doThrow()
    {
        qDebug() << "From NormalThrow: I am execution on thread id" << QThread::currentThreadId() << " and I am throwing right now";

        //This exception is not catched, and definitly crash the process
        throw std::exception("KBOOOM NormalThrow");
    }
};


class ThrowingObject : public QObject
{
public:
    NormalThrow* thrower;
    void doThrowInNotify()
    {
        qDebug() << "I am execution on thread id" << QThread::currentThreadId() << " and I am throwing";
        throw std::exception("KBOOOM");
    }

    void scheduleThrow()
    {
        QTimer* singleShot = new QTimer(this);
        singleShot->setSingleShot(true);

        connect(singleShot, &QTimer::timeout, this, &ThrowingObject::doThrow);

        singleShot->start();

        qDebug() << "I am execution on thread id" << QThread::currentThreadId() << " and I will throw in run";
    }

    void doThrow()
    {
        try {
            thrower->doThrow();
        }
        catch (std::exception e) {
            qDebug() << "Can I catch it? -- std::exception in thread : " << QThread::currentThreadId();
            qDebug() << e.what();
        }

        qDebug() << "I am execution on thread id" << QThread::currentThreadId() << " and I am throwing right now";

        //This exception is not catched, and definitly crash the process
        throw std::exception("KBOOOM");



    }

    void doThrowOutsideNotify()
    {
        //wait 5s for demo purpose, this slot is called by Object2, after Object1 throw in thread1 event loop
        QThread::sleep(5);

        try {
            thrower->doThrow();
        }
        catch (std::exception e) {
            qDebug() << "Can I catch it? -- std::exception in thread : " << QThread::currentThreadId();
            qDebug() << e.what();
        }

        qDebug() << "I am execution on thread id" << QThread::currentThreadId() << " and I am throwing right now";

        //This exception is not catched, and definitly crash the process
        throw std::exception("FATAL KBOOOM");
    }

};

class ApplicationWithExceptionCatchedInNotify : public QApplication
{
public:
    ApplicationWithExceptionCatchedInNotify(int argc, char* argv[]) :
        QApplication(argc, argv)
    {}

    bool notify(QObject* receiver, QEvent* e) override
    {
        try {
            return QApplication::notify(receiver, e);
        }
        catch (std::runtime_error e)
        {
            qDebug() << "std::runtime_error in thread : " << QThread::currentThreadId();
            qDebug() << e.what();
        }
        catch (std::exception e)
        {
            qDebug() << "std::exception in thread : " << QThread::currentThreadId();
            qDebug() << e.what();
        }
        catch (...)
        {
            qDebug() << "exception thread : " << QThread::currentThreadId();
        }

        qDebug() << "catch in notify ";
        return false;
    }

};



int main(int argc, char* argv[])
{
    ApplicationWithExceptionCatchedInNotify app(argc, argv);

    qDebug() << "Main QThread id" << QThread::currentThreadId();

    NormalThrow thrower1;
    NormalThrow thrower2;
    //Object o1 will throw in its event loop (in notify)
    QThread thread1;
    ThrowingObject o1;
    o1.thrower = &thrower1;
    o1.moveToThread(&thread1);
    QObject::connect(&thread1, &QThread::started, &o1, &ThrowingObject::scheduleThrow);

    thread1.start();

    //Object o2 will throw before the event loop is installed
    QThread thread2;
    ThrowingObject o2;
    o2.thrower = &thrower2;
    o2.moveToThread(&thread2);
    //Connect to started signal.
    QObject::connect(&thread2, &QThread::started, &o2, &ThrowingObject::doThrowOutsideNotify);
    thread2.start();

    app.exec();
}