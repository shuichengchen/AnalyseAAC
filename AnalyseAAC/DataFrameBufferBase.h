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
	 * �������ݻ�����
	 *
	 * @return void
	 */
	virtual void reset();

	/**
	 * addStreamData
	 *
	 * ׷��������
	 *
	 * @param[in] const char * data      Ҫ׷�ӵ�������
	 * @param[in] const int data_len     ���ݳ���
	 *
	 * @return int �Ѿ�׷�ӵ����ݳ��ȡ��������ʾ׷�ӳɹ��������ʾ׷��ʧ�ܡ�
	 *
	 */
	virtual int  addStreamData(const char * data, const int data_len);

	/**
	 * getNextFrameLen
	 *
	 * ��ȡ��һ������������֡����
	 *
	 * @return int ����֡���ȡ��������ʾ��ȡ�ɹ��������ʾ����֡��δ�ռ�������
	 *
	 * @note
	 *     �������ڴ�����쳣��ϵͳ���������£���ȡʧ�ܿ���Ϊ����ԭ��
	 *     1���ѻ���������޷��ж��Ƿ�Ϊһ֡����������֡��
	 */
	virtual int  getNextFrameLen() const;

	/**
	 * getNextFrame
	 *
	 * ��ȡһ������������֡
	 *
	 * @param[in,out] char * buffer        ������������֡�Ļ��������ɵ������ṩ
	 * @param[in] const int buffer_size    ��������С
	 *
	 * @return int ��ȡ������֡���ȡ��������ʾ��ȡ�ɹ��������ʾ��ȡʧ�ܡ�
	 *
	 * @note
	 *     �������ڴ�����쳣��ϵͳ���������£���ȡʧ�ܿ���Ϊ����ԭ��
	 *     1��������ָ��_szDataBufferΪNULL���߳���_iBufferSizeС������֡���ȣ�
	 *     2���ѻ���������޷��ж��Ƿ�Ϊһ֡����������֡��
	 */
	virtual int  getNextFrame(char * buffer, const int buffer_size);

protected:
	/**
	 * checkNextFrame
	 *
	 * �����һ������������֡
	 *
	 * @param[in] const int _iOffset        �������е�ƫ�ƣ��Ӹ��ֽڿ�ʼ���
	 */
	virtual void checkNextFrame(const int offset = 0);

	/**
	 * resetState
	 *
	 * ����״̬���������û�����
	 */
	virtual void resetState();

protected:
	int                 m_data_len;         ///< �ѻ�������ݳ���
	std::vector<char>   m_data_buffer;    ///< NALU���ݻ�����

	int                 m_next_frame_offset; ///< ��һ����������֡����ʼλ��
	int                 m_next_frame_len;    ///< ��һ����������֡�ĳ���
	bool                m_is_next_frame_complete;///< ��һ����������֡�Ƿ�����
};

#endif//__DATA_FRAME_BUFFER_BASE__


