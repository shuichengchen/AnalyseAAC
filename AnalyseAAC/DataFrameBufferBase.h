#pragma once
#ifndef __DATA_FRAME_BUFFER_BASE__
#define __DATA_FRAME_BUFFER_BASE__

#include <vector>

class DataFrameBufferBase
{
public:
	DataFrameBufferBase(void);
	virtual ~DataFrameBufferBase(void);

public:
	/**
	 * reset
	 *
	 * 重置数据缓冲区
	 *
	 * @return void
	 */
	virtual void reset();

	/**
	 * addStreamData
	 *
	 * 追加数据流
	 *
	 * @param[in] const char * data      要追加的数据流
	 * @param[in] const int data_len     数据长度
	 *
	 * @return int 已经追加的数据长度。大于零表示追加成功，否则表示追加失败。
	 *
	 */
	virtual int  addStreamData(const char * data, const int data_len);

	/**
	 * getNextFrameLen
	 *
	 * 获取下一个完整的数据帧长度
	 *
	 * @return int 数据帧长度。大于零表示获取成功，否则表示数据帧还未收集完整。
	 *
	 * @note
	 *     不考虑内存访问异常等系统错误的情况下，获取失败可能为以下原因：
	 *     1、已缓存的数据无法判断是否为一帧完整的数据帧；
	 */
	virtual int  getNextFrameLen() const;

	/**
	 * getNextFrame
	 *
	 * 获取一个完整的数据帧
	 *
	 * @param[in,out] char * buffer        保存完整数据帧的缓冲区，由调用者提供
	 * @param[in] const int buffer_size    缓冲区大小
	 *
	 * @return int 获取的数据帧长度。大于零表示获取成功，否则表示获取失败。
	 *
	 * @note
	 *     不考虑内存访问异常等系统错误的情况下，获取失败可能为以下原因：
	 *     1、缓冲区指针_szDataBuffer为NULL或者长度_iBufferSize小于数据帧长度；
	 *     2、已缓存的数据无法判断是否为一帧完整的数据帧；
	 */
	virtual int  getNextFrame(char * buffer, const int buffer_size);

protected:
	/**
	 * checkNextFrame
	 *
	 * 检测下一个完整的数据帧
	 *
	 * @param[in] const int _iOffset        缓冲区中的偏移，从该字节开始检测
	 */
	virtual void checkNextFrame(const int offset = 0);

	/**
	 * resetState
	 *
	 * 重置状态，但不重置缓冲区
	 */
	virtual void resetState();

protected:
	int                 m_data_len;         ///< 已缓存的数据长度
	std::vector<char>   m_data_buffer;    ///< NALU数据缓冲区

	int                 m_next_frame_offset; ///< 下一个完整数据帧的起始位置
	int                 m_next_frame_len;    ///< 下一个完整数据帧的长度
	bool                m_is_next_frame_complete;///< 下一个完整数据帧是否完整
};

#endif//__DATA_FRAME_BUFFER_BASE__


