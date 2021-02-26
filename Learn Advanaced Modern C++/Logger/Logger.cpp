#include "Logger.h"
#include <exception>

using namespace std;	//I still hate retyping std::, and adding "using std::blah" can get out of control pretty fast which outweights the risks of "using namespace std"

template <typename TextLoggerImpl>
Logger<TextLoggerImpl>::Logger(const typename TextLoggerImpl::StringType& filename) : m_fileLogger{filename}
{
	//launch consumer thread
	m_consumer = async([this]() { this->logEntryConsumer(); });
}

template <typename TextLoggerImpl>
void Logger<TextLoggerImpl>::log(const typename TextLoggerImpl::StringType& entry)
{
	lock_guard<mutex> producerGuard(m_producerLock);
	try {
		//Somebody has to manage the m_producer future deque, otherwise log entries will swamp the memory
		//This should be a fast thread spawning to create the illusion that log is lock-free, which is not really
		m_producer.push_back(async([&entry, this]() {
			thread_local typename TextLoggerImpl::StringType toPush{ entry };
			try {
				lock_guard<mutex> entryGuard(m_this->m_logEntryLock);
				this->m_logEntry.push(toPush);
			}
			catch (...)
			{
				//set exception. Find out how to do this
			}
			}));
	}
	catch (...) {
		//Lock-free logging attempt has failed. Most likely m_producer container is full, so log directly
		//Do I need to indicate that this is not lock-free?
		lock_guard<mutex> entryGuard(m_this->m_logEntryLock);
		m_fileLogger.log(entry);
	}
}

template <typename TextLoggerImpl>
Logger<TextLoggerImpl>::~Logger() {
	m_done = true;	//Triggers the consumer lock to flush the producer queue
	//Must serialize here, otherwise destruction becomes order-dependent, which is undefined in this type
	try {
		m_consumer.wait();
	}
	catch (...)
	{
	}
}

template <typename TextLoggerImpl>
void Logger<TextLoggerImpl>::logEntryConsumer() {
	try {
		std::unique_lock<mutex> logEntryLock(m_logEntryLock);
		while (!m_done) {
			while (m_logEntry.empty()) {
				m_hasLogEntry.wait(logEntryLock);
			}
			//logEntryLock is in a locked state once condition_variable gets the signal
			//Serialize access to the log file
			m_fileLogger.log(m_logEntry.front());
			m_logEntry.pop();
			//Pretty sure I have to do this
			//logEntryLock.unlock();
		}
		//Destructor sets m_done, now I have to flush the producer queue.
		//Every other work request after this lock will be ignored
		std::lock_guard producerLock(m_producerLock);
		//Uh what do we do with the future queue? Or should it be a vector? or a deque
		//For sure perform a serialized producer flush here. A serialied producer flush is:
		//while (!m_producer.empty())
		//m_producer.front.future.wait; //result is ready, thread is done. Chances of jamming, while not 0, is slim
		//m_producer.remove_front
		//All producer transactions have been fulfilled. No more contention to m_logEntry at this point
		while (!m_logEntry.empty())
		{
			TextLoggerImpl::log(m_logEntry.front());
			m_logEntry.pop();
		}
	}
	catch (...)
	{
		//promise.set_exception
	}
	//Consumer thread is now done.
	//promise.set(void);
}