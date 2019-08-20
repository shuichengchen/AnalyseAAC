#ifndef __AAC_FRAME_BUFFER_H__
#define __AAC_FRAME_BUFFER_H__

#include "DataFrameBufferBase.h"

class AACFrameBuffer : public DataFrameBufferBase
{
public:
	AACFrameBuffer(void);
	virtual ~AACFrameBuffer(void);

protected:
	virtual void checkNextFrame(const int offset = 0);
	virtual void resetState();

private:
	int                 m_evaluation_state;
	int                 m_expected_raw_data_len;
	int                 m_parsed_raw_data_len;
	bool                m_is_fixed_header_protection_absent;
};

#endif//__AAC_FRAME_BUFFER_H__

