#include "pch.h"
#include "AACFrameBuffer.h"

enum
{
	STATE_FIXED_HEADER_SYNCWORD1,
	STATE_FIXED_HEADER_SYNCWORD2,
	STATE_FIXED_HEADER_ID,
	STATE_FIXED_HEADER_LAYER,
	STATE_FIXED_HEADER_PROTECTION_ABSENT,
	STATE_FIXED_HEADER_PROFILE,
	STATE_FIXED_HEADER_SAMPLING_FREQUENCY_INDEX,
	STATE_FIXED_HEADER_PRIVATE_BIT,
	STATE_FIXED_HEADER_CHANNEL_CONFIGURATION,
	STATE_FIXED_HEADER_ORIGINAL_COPY,
	STATE_FIXED_HEADER_HOME,

	STATE_VARIABLE_HEADER_COPYRIGHT_IDENTIFICATION_BIT,
	STATE_VARIABLE_HEADER_COPYRIGHT_IDENTIFICATION_START,
	STATE_VARIABLE_HEADER_ACC_FRAME_LENGTH1,
	STATE_VARIABLE_HEADER_ACC_FRAME_LENGTH2,
	STATE_VARIABLE_HEADER_ADTS_BUFFER_FULLNESS,
	STATE_VARIABLE_HEADER_NUMBER_OF_RAW_DATA_BLOCKS_IN_FRAME,

	STATE_CRC1,
	STATE_CRC2,

	STATE_RAW_DATA,
};

#define RESET_PARSER_STATE() \
{ \
    m_evaluation_state = STATE_FIXED_HEADER_SYNCWORD1; \
    m_expected_raw_data_len = 0; \
    m_parsed_raw_data_len = 0; \
    m_is_fixed_header_protection_absent = false; \
    }

AACFrameBuffer::AACFrameBuffer(void)
	: m_evaluation_state(STATE_FIXED_HEADER_SYNCWORD1)
	, m_expected_raw_data_len(0)
	, m_parsed_raw_data_len(0)
	, m_is_fixed_header_protection_absent(false)
{
}

AACFrameBuffer::~AACFrameBuffer(void)
{
}

void AACFrameBuffer::checkNextFrame(const int offset /*= 0*/)
{
	if (offset >= m_data_len)
	{
		return;
	}

	if (m_is_next_frame_complete)
	{
		return;
	}

	for (int pos = offset; pos < m_data_len; ++pos)
	{
		const unsigned char c = (unsigned char)m_data_buffer[pos];
		switch (m_evaluation_state)
		{
		case STATE_FIXED_HEADER_SYNCWORD1:
		{
			if (0xFF != c)
			{
				break;
			}

			m_next_frame_offset = pos;
			m_evaluation_state = STATE_FIXED_HEADER_SYNCWORD2;
		}
		break;
		case STATE_FIXED_HEADER_SYNCWORD2:
		{
			if (0xF0 != (c & 0xF0))
			{
				RESET_PARSER_STATE();
				break;
			}

			if (0x00 != (c & 0x06)) // layer
			{
				RESET_PARSER_STATE();
				break;
			}

			m_is_fixed_header_protection_absent = c & 0x01;
			m_evaluation_state = STATE_FIXED_HEADER_PROFILE;
		}
		break;
		case STATE_FIXED_HEADER_PROFILE:
		{
			m_evaluation_state = STATE_FIXED_HEADER_CHANNEL_CONFIGURATION;
		}
		break;
		case STATE_FIXED_HEADER_CHANNEL_CONFIGURATION:
		{
			m_expected_raw_data_len |= (c & 0x03) << 11;
			m_evaluation_state = STATE_VARIABLE_HEADER_ACC_FRAME_LENGTH1;
		}
		break;
		case STATE_VARIABLE_HEADER_ACC_FRAME_LENGTH1:
		{
			m_expected_raw_data_len |= (c & 0xFF) << 3;
			m_evaluation_state = STATE_VARIABLE_HEADER_ACC_FRAME_LENGTH2;
		}
		break;
		case STATE_VARIABLE_HEADER_ACC_FRAME_LENGTH2:
		{
			m_expected_raw_data_len |= ((c & 0xE0) >> 5) & 0x07;

			m_expected_raw_data_len -= 7; // fixed header + variable header
			if (!m_is_fixed_header_protection_absent)
				m_expected_raw_data_len -= 2; // CRC

			m_evaluation_state = STATE_VARIABLE_HEADER_ADTS_BUFFER_FULLNESS;
		}
		break;
		case STATE_VARIABLE_HEADER_ADTS_BUFFER_FULLNESS:
		{
			if (!m_is_fixed_header_protection_absent)
				m_evaluation_state = STATE_CRC1;
			else
				m_evaluation_state = STATE_RAW_DATA;
		}
		break;
		case STATE_CRC1:
		{
			m_evaluation_state = STATE_CRC2;
		}
		break;
		case STATE_CRC2:
		{
			m_evaluation_state = STATE_RAW_DATA;
		}
		break;
		case STATE_RAW_DATA:
		{
			m_parsed_raw_data_len++;

			if (m_expected_raw_data_len <= m_parsed_raw_data_len)
			{
				m_next_frame_len = pos - m_next_frame_offset + 1;
				m_is_next_frame_complete = true;

				return;
			}
		}
		break;
		}
	}
}

void AACFrameBuffer::resetState()
{
	DataFrameBufferBase::resetState();

	m_evaluation_state = STATE_FIXED_HEADER_SYNCWORD1;
	m_expected_raw_data_len = 0;
	m_parsed_raw_data_len = 0;
	m_is_fixed_header_protection_absent = false;
}

