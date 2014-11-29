#pragma once

#ifndef THROTTLE_H
#define THROTTLE_H

// Throttle class 
// by Vitaly Yakovlev, November 28, 2014
// throttles execution of some method to maximum M calls in N seconds
// http://blog.coolsoftware.ru/

#include <time.h>

class throttle
{
private:
	class throttle_cbuf //ring buffer
	{
	public:
		explicit throttle_cbuf(const size_t size, const clock_t interval) : 
			m_size(size), m_interval (interval), m_head(0), m_tail(0), m_empty(true) 
		{
			m_buffer = size > 0 ? new clock_t[size] : 0;
		}
		
		~throttle_cbuf() 
		{
			if (m_buffer) delete [] m_buffer;
		}
		
		bool push(clock_t clock)
		{
			pop(clock);
			size_t new_tail = m_tail+1;
			if (new_tail >= m_size) new_tail = 0;
			if (new_tail == m_head && !m_empty) return false;
			m_buffer[new_tail] = clock;
			m_tail = new_tail;
			m_empty = false;
			return true;
		}

	private:
		void pop(clock_t clock)
		{
			while (m_head != m_tail || !m_empty)
			{
				if (clock-m_buffer[m_head] < m_interval) break;
				if (++m_head >= m_size) m_head = 0;
				if (m_head == m_tail) m_empty = true;
			}
		}

	private:
		clock_t *m_buffer;
		const size_t m_size;
		const clock_t m_interval;
		size_t m_head;
		size_t m_tail;
		bool m_empty;
	};

public:
	explicit throttle(const size_t count, const clock_t interval = CLOCKS_PER_SEC) : 
		m_count(count), m_interval(interval), m_cbuf(count, interval) { }
	~throttle() { }

	bool acquire()
	{
		if (!m_count) return false;
		return m_cbuf.push(clock());
	}

private:
	const size_t m_count;
	const clock_t m_interval;
	throttle_cbuf m_cbuf;
};

#endif //THROTTLE_H