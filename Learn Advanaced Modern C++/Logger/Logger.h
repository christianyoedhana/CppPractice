#pragma once
#include <queue>
#include <future>
#include <mutex>
#include <condition_variable>
#include <deque>
/*
* Logger implements lock-free concurrency text logging.
* User instantiate a Logger with a file name to create a log file.
* Log file is either created as a brand new, or appended to an existing log file
* User call function "log" to dump a string to the log file
* User can call "log" from multiple transaction units safely.
* Logger strives to create an illusion of lock-free transaction to every log calls for the caller
*/

//Mixin pattern is not appropriate because I cannot allow the user to do this:
//Logger aLoggerInstance("somefile.log");
//TextLoggerImplType * impl = &aLoggerInstance;
//Allowing this to happen negates the multi-transaction safety built in Logger.

/*
* Unfortunately for a text file logger, this is not the appropriate way to achieve lock-free logging.
* At least I don't think so.
* I think for each producer written there has to be a consumer waiting for that specific producer.
* The only access we want to control is the access to the ofstream's buffer.
* So maybe using thread_local storage to store the string to log is enough, then write to the file buffer
* But the access to the WorkToDo container is still synchronized with a lock.
* If ofstream pointer is declared atomic, what would that buy me?
*/

/*
For a logging function that deals with a storage media, chances are te storage media has a buffer. If your policy is to flush after every log entry,
then this scheme might work. But now someone has to clean up the producer's future queue, which will slow down the log because the 'cleaner'
has to lock the producer queue while sweeping away futures with return value.
AND YOU CAN STILL LOSE YOUR LOG.
*/
template<typename TextLoggerImpl>
class Logger
{
public:
	Logger() = delete;
	explicit Logger(const typename TextLoggerImpl::StringType& fileName);
	void log(const typename TextLoggerImpl::StringType& entry);
	~Logger();
	//No copy. Futures cannot be copied.
	//User default generated move
	Logger(Logger&&) = default;
	Logger& operator=(Logger&&) = default;
	Logger(Logger&) = default;
	Logger& operator=(Logger&) = default;

private:
	TextLoggerImpl m_fileLogger;
	//What is the "Done" sentinel now?
	//log will take m_producerLock, 
	//So how do I manage this future container? How do I know which future has a value before blocking to get the value?
	std::deque<std::future<void>> m_producer;
	std::queue<typename TextLoggerImpl::StringType> m_logEntry;
	std::future<void> m_consumer;
	//I don't think I need an atomic operation for now. Mutex lock/unlock pairs should be enough
	std::mutex m_producerLock;
	std::mutex m_logEntryLock;
	//Is this appropriate for multiple producer transaction?
	std::condition_variable m_hasLogEntry;
	bool m_done;	//m_done is set only by the destructor, so no need to be atomic

	void logEntryConsumer();
};