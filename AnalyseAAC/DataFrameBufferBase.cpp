#include "pch.h"
#include "DataFrameBufferBase.h"

#include <assert.h>

DataFrameBufferBase::DataFrameBufferBase(void)
	: m_data_len(0)
	, m_data_buffer(0)
	, m_next_frame_offset(-1)
	, m_next_frame_len(0)
	, m_is_next_frame_complete(false)
{
}

DataFrameBufferBase::~DataFrameBufferBase(void)
{
}

void DataFrameBufferBase::reset()
{
	m_data_len = 0;
	m_next_frame_offset = -1;
	m_next_frame_len = 0;
	m_is_next_frame_complete = false;
}

int DataFrameBufferBase::addStreamData(const char * data, const int data_Len)
{
	try
	{
		if (m_data_len + data_Len > (int)m_data_buffer.size())
		{
			m_data_buffer.resize((size_t)m_data_len + (size_t)data_Len + 1, 0x00);
		}

		::memcpy_s((char*)&m_data_buffer[m_data_len],
			m_data_buffer.size() - m_data_len, data, data_Len);

		const int offset = m_data_len;
		m_data_len += data_Len;

		checkNextFrame(offset);
	}
	catch (...)
	{
		return 0;
	}

	return data_Len;
}

int DataFrameBufferBase::getNextFrameLen() const
{
	return m_is_next_frame_complete ? m_next_frame_len : 0;
}

int DataFrameBufferBase::getNextFrame(char * buffer, const int buffer_size)
{
	assert(buffer && buffer_size);
	if (!buffer || !buffer_size)
		return 0;

	if (!m_is_next_frame_complete || 0 >= m_next_frame_len)
		return 0;

	if (m_next_frame_len > buffer_size)
		return 0;

	assert(m_data_len >= m_next_frame_len);
	::memcpy_s(buffer, buffer_size, (char*)&m_data_buffer[m_next_frame_offset], m_next_frame_len);

	::memcpy_s((char*)& m_data_buffer[0], m_data_buffer.size(),
		(char*)& m_data_buffer[(size_t)m_next_frame_offset + (size_t)m_next_frame_len],
		(size_t)m_data_len - (size_t)m_next_frame_offset - (size_t)m_next_frame_len);
	m_data_len -= m_next_frame_offset + m_next_frame_len;

	const int frame_len = m_next_frame_len;

	resetState();
	checkNextFrame();

	return frame_len;
}

void DataFrameBufferBase::checkNextFrame(const int offset /*= 0*/)
{
	if (offset >= m_data_len)
	{
		return;
	}

	if (m_is_next_frame_complete)
	{
		return;
	}

	m_next_frame_offset = offset;
	m_next_frame_len = m_data_len - m_next_frame_offset;
	m_is_next_frame_complete = true;
}

void DataFrameBufferBase::resetState()
{
	m_next_frame_offset = -1;
	m_next_frame_len = 0;
	m_is_next_frame_complete = false;
}
