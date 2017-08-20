#ifndef DORMOUSEENGINE_LOGGER_LOGGER_HPP_
#define DORMOUSEENGINE_LOGGER_LOGGER_HPP_

#include <sstream>
#include <cassert>
#include <vector>
#include <functional>

#include <boost/call_traits.hpp>
#include <boost/noncopyable.hpp>

#include "dormouse-engine/essentials/smart-pointer-definitions.hpp"

#include "appender/Appender.hpp"
#include "Level.hpp"
#include "Context.hpp"
#include "print.hpp"

namespace dormouse_engine::logger {

class Logger {
private:

	class Stream {
	public:

		template <class T>
		Stream& operator<< (const T& v) {
			print(oss_, v);
			return *this;
		}

		void flush() {
			assert(context_);

			std::for_each(
					appenders_.begin(),
					appenders_.end(),
					std::bind(&appender::Appender::append, std::placeholders::_1, level_, *context_, oss_.str())
			);

			oss_.rdbuf()->str("");
		}

		void reset(Level level, const Context& context) {
			level_ = level;
			context_ = &context;
		}

	private:

		friend class Logger;

		typedef std::vector<appender::AppenderSharedPtr> Appenders;

		Stream() :
			level_(Level::INFO),
			context_(0) {
		}

		std::ostringstream oss_;

		Level level_;

		const Context* context_;

		Appenders appenders_;

	};

public:

	class StreamRef : boost::noncopyable {
	public:

		StreamRef() :
			stream_(0)
		{
		}

		StreamRef(Stream& stream, Level level, const Context& context) :
			stream_(&stream)
		{
			stream_->reset(level, context);
		}

		StreamRef(StreamRef&& other) :
			stream_(other.stream_)
		{
			other.stream_ = 0;
		}

		~StreamRef() {
			if (stream_) {
				stream_->flush();
			}
		}

		template <class T>
		StreamRef& operator<< (const T& v) {
			if (stream_) {
				*stream_ << v;
			}
			return *this;
		}

	private:

		Stream* stream_;

	};

	Logger(Level level) :
		level_(level)
	{
	}

	StreamRef log(Level level, const Context& context = Context::empty()) {
		if (level_ > level) {
			return StreamRef();
		} else {
			return StreamRef(stream_, level, context);
		}
	}

	StreamRef trace(const Context& context = Context::empty()) {
		return log(Level::TRACE, context);
	}

	StreamRef debug(const Context& context = Context::empty()) {
		return log(Level::DEBUG, context);
	}

	StreamRef info(const Context& context = Context::empty()) {
		return log(Level::INFO, context);
	}

	StreamRef warning(const Context& context = Context::empty()) {
		return log(Level::WARNING, context);
	}

	StreamRef error(const Context& context = Context::empty()) {
		return log(Level::ERROR, context);
	}

	StreamRef critical(const Context& context = Context::empty()) {
		return log(Level::CRITICAL, context);
	}

	void addAppender(appender::AppenderSharedPtr appender) {
		stream_.appenders_.push_back(appender);
	}

	Level getLevel() const {
		return level_;
	}

	void setLevel(Level level) {
		level_ = level;
	}

private:

	Level level_;

	Stream stream_;

};


DE_SMART_POINTER_DEFINITONS(Logger);

} // namespace dormouse_engine::logger

#endif /* DORMOUSEENGINE_LOGGER_LOGGER_HPP_ */
