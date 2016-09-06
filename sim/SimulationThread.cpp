#include "SimulationThread.hpp"

SimulationThread::SimulationThread(QObject* parent) :
  QThread(parent),
  request_(IDLE)
{
  start(NormalPriority);
}

SimulationThread::~SimulationThread()
{
  // Tell the thread to terminate.
  mutex_.lock();
  request_ = TERMINATE;
  condition_.wakeOne();
  mutex_.unlock();
  // Wait until the thread is done.
  wait();
}

void SimulationThread::start_simulation()
{
  QMutexLocker locker(&mutex_);
  request_ |= RUN;
  condition_.wakeOne();
}

void SimulationThread::stop_simulation()
{
  QMutexLocker locker(&mutex_);
  request_ &= ~RUN;
}

void SimulationThread::step_simulation()
{
  QMutexLocker locker(&mutex_);
  request_ |= STEP;
  condition_.wakeOne();
}

void SimulationThread::reset_simulation()
{
  QMutexLocker locker(&mutex_);
  request_ |= RESET;
  condition_.wakeOne();
}

void SimulationThread::load_environment(const QString& path)
{
  QMutexLocker locker(&mutex_);
  request_ |= LOAD;
  load_path_= path;
  condition_.wakeOne();
}

void SimulationThread::run()
{
  forever {
    mutex_.lock();
    while (request_ == IDLE) {
      condition_.wait(&mutex_);
    }
    unsigned int request = request_;
    QString path = load_path_;
    request_ &= ~(RESET | LOAD | STEP);
    mutex_.unlock();
    if (request & TERMINATE) {
      return;
    }
    if (request & LOAD) {
      SimulatorLoadBundle bundle;
      simulator_.load_environment(path.toStdString(), bundle);
      emit post_load(bundle);
    } else if (request & RESET) {
      simulator_.reset();
    }
    if (request & (RUN | STEP)) {
      SimulatorCycleBundle bundle;
      simulator_.cycle(bundle);
      emit post_cycle(bundle);
      msleep(50);
    }
  }
}
