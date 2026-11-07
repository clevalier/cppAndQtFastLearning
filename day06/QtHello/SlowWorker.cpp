
#include "SlowWorker.h"
#include <QThread>


void SlowWorker::doWork()
{
    double temperature = 45.8;

    for (int i = 0; i < 3; ++i)
    {
        if (QThread::currentThread()
                ->isInterruptionRequested())
            break;

        QThread::msleep(1000);

        if (QThread::currentThread()
                ->isInterruptionRequested())
            break;

        temperature += 0.1;

        emit readingReady(temperature);
    }

    emit finished();
}
