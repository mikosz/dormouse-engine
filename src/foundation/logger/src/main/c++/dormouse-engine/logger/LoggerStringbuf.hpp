#ifndef DORMOUSEENGINE_LOGGER_LOGGERSTRINGBUF_HPP_
#define DORMOUSEENGINE_LOGGER_LOGGERSTRINGBUF_HPP_

#include <string_view>
#include <ostream>
#include <locale>

#include "Logger.hpp"

namespace dormouse_engine::logger {

template <class CharT, class TraitsT = std::char_traits<CharT>>
class LoggerStringbuf : public std::basic_stringbuf<CharT, TraitsT> {
public:

	LoggerStringbuf() = default;

	LoggerStringbuf(LoggerSharedPtr log) :
		log_(std::move(log)),
		buffer_(BUFFER_SIZE, '\0')
	{
		setg(nullptr, nullptr, nullptr);
		setp(buffer_.data(), buffer_.data(), buffer_.data() + buffer_.size());
	}

	~LoggerStringbuf() {
		if (!line_.empty()) {
			log_->log(Level::INFO) << line_;
		}
	}

	LoggerStringbuf(LoggerStringbuf&& other) :
		LoggerStringbuf()
	{
		swap(other);
	}

	LoggerStringbuf& operator=(LoggerStringbuf&& other) {
		swap(LoggerStringbuf(std::move(other)));
	}

	void swap(LoggerStringbuf& other) {
		std::swap(log_, other.log_);
		std::swap(buffer_, other.buffer_);
		std::swap(line_, other.line_);
	}

protected:

	int sync() override {
		const auto str = std::basic_string_view<CharT, TraitsT>(pbase(), pptr() - pbase());

		line_.append(str);
		if (!line_.empty() && line_.back() == CharT('\n')) {
			line_.pop_back();
			log_->log(Level::INFO) << line_;
			line_.clear();
		}

		setp(buffer_.data(), buffer_.data(), buffer_.data() + buffer_.size());

		return 0;
	}

	int_type overflow(int_type c = TraitsT::eof()) override {
		const auto syncResult = sync();

		if (c != TraitsT::eof()) {
			buffer_[0] = static_cast<char>(c);
			setp(buffer_.data(), buffer_.data() + 1, buffer_.data() + buffer_.size());
		}

		return (syncResult == -1) ? TraitsT::eof() : 0;
	}

private:

	static constexpr auto BUFFER_SIZE = 1024u;

	LoggerSharedPtr log_;

	std::vector<CharT> buffer_;

	std::basic_string<CharT, TraitsT> line_;

};

} // namespace dormouse_engine::logger

#endif /* DORMOUSEENGINE_LOGGER_LOGGERSTRINGBUF_HPP_ */
