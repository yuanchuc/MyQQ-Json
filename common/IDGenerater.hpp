#ifndef __IDGENERATER_H_
#define __IDGENERATER_H_

#include <mutex>
#include <string>
#include <chrono>
#include <thread>
#include <cstdint>
#include <stdexcept>

/*
    Twitterѩ���㷨
*/
/*-------��λ---------------------------------------------��64λ---------------------------------------------��λ-------|
|-----------------------------------------------------------------------------------------------------------------------|
|    0    |    0000000000 0000000000 0000000000 0000000000 0    |         00000        |        00000        |   000000000000    |
|δʹ�� |                    41λʱ���                        |    5λDataCenterID    |    5λWorkerID        |    12λ���к�        |
|-----------------------------------------------------------------------------------------------------------------------|
|δʹ�� |                    41λʱ���                        |              10λ����ID                |    12λ���к�        |
|----------------------------------------------------------------------------------------------------------------------*/
//��������ռ��С
constexpr int SEQUENCE_BITS = 12;                //12λ���кţ������ڼ�����һ��������1����������ܲ���4096��ID
constexpr int WORKER_ID_BITS = 5;
constexpr int DATA_CENTER_ID_BITS = 5;            //5λDataCenterID��5λWorkerID����һ���ǻ���ID����10λ�������֧��1024���ڵ�
constexpr int TIMESTAMP_BITS = 41;                //41λʱ�����������69.7�� ==> (2 ^ 41) / (1000 * 60 * 60* 24 * 365) = 69.7

//������ƫ��                                    ����ǰһ������ռλ���Ⱦ�����һ����ƫ����
constexpr int SEQUENCE_ID_SHIFT = 0;
constexpr int WORK_ID_SHIFT = SEQUENCE_BITS;
constexpr int DATA_CENTER_ID_SHIFT = SEQUENCE_BITS + WORKER_ID_BITS;
constexpr int TIMESTAMP_SHIFT = SEQUENCE_BITS + WORKER_ID_BITS + DATA_CENTER_ID_BITS;

//DataCenterID��WorkerID���ȡֵ                ������ռλ���ȼ������ֵ
constexpr std::int64_t MAX_WORKER_ID = (1 << WORKER_ID_BITS) - 1;
constexpr std::int64_t MAX_DATA_CENTER_ID = (1 << DATA_CENTER_ID_BITS) - 1;

//���к�����                                    ���ڿ������к�ȡֵ��Χ
constexpr std::int64_t SEQUENCE_MASK = (1 << SEQUENCE_BITS) - 1;

//ʱ�����                                        ʱ�����Ȼ������69.7�꣬�����ֱ�Ӵ�Unixʱ���(1970.1.1)�����ֻ��֧�ֵ�2039��
//                                                ��������һ����Unix��Ԫʱ�����Ŀ�ʼʱ�䣬������ڿ�ʼʱ���ƫ�ƣ���ô֧�ֵ����ʱ����Ϊ��ʼʱ�� + 69.7��
constexpr std::int64_t START_POINT = 1625068800000LL; //2021.7.1 00:00:00  ==> 41λʱ��֧�ֵ� 2090��

//������                                        ���ϻ���������Ҫ�󣬵��ǲ�����������
class NonLockType {
public:
    void lock() {
    }
    void unlock() {
    }
};

template<typename LockType = NonLockType>
class IDGenerater final {
public:
    static IDGenerater* GetInstance(int iDataCenterID = 0, int iWorkerID = 0) {
        if (iDataCenterID < 0 || MAX_DATA_CENTER_ID < iDataCenterID) {
            throw std::invalid_argument(std::string("iDataCenterID��ӦС��0�����") + std::to_string(MAX_DATA_CENTER_ID));
        }
        if (iWorkerID < 0 || MAX_WORKER_ID < iWorkerID) {
            throw std::invalid_argument(std::string("iWorkerID��ӦС��0�����") + std::to_string(MAX_WORKER_ID));
        }

        static IDGenerater GeneraterInstance(iDataCenterID, iWorkerID);            //magic static            C++11��̬�ֲ�������ʼ���Ѿ����̰߳�ȫ��
        return &GeneraterInstance;
    }

    std::int64_t NextID() {
        std::lock_guard<LockType> lock(m_lock);
        auto i64CurTimeStamp = m_i64BootTimeStamp + GetCurrentTimeStamp<std::chrono::steady_clock>();    //��m_i64BootTimeStamp��������ʱ��

        if (i64CurTimeStamp < m_i64LastTimeStamp) {                                //ʱ����ˣ�˯�ߵ���һ������������
            i64CurTimeStamp = GetNextTimeStampBySleep();

        }
        else if (i64CurTimeStamp == m_i64LastTimeStamp) {                        //һ���������ɶ��ID
            m_i64SequenceID = (m_i64SequenceID + 1) & SEQUENCE_MASK;            //�������к�

            if (0 == m_i64SequenceID) {                                            //�ﵽ�ú��������ɵ����ID������ѭ������һ������������
                i64CurTimeStamp = GetNextTimeStampByLoop(i64CurTimeStamp);
            }
        }
        else {                                                                    //��ʱ�䣬���кŴ�ͷ��ʼ
            m_i64SequenceID = 0;
        }
        m_i64LastTimeStamp = i64CurTimeStamp;
        return ((i64CurTimeStamp - START_POINT) << TIMESTAMP_SHIFT)
            | (m_i64DataCenterID << DATA_CENTER_ID_SHIFT)
            | (m_i64WorkerID << WORK_ID_SHIFT)
            | (m_i64SequenceID << SEQUENCE_ID_SHIFT);
    }
     
private:
    template<typename ClockType>
    std::int64_t GetCurrentTimeStamp() {
        auto tpTimePoint = std::chrono::time_point_cast<std::chrono::milliseconds>(ClockType::now());    //��ȡʱ�䲢���;��ȵ�����
        return tpTimePoint.time_since_epoch().count();                                                                    //�õ�ʱ���
    }

    std::int64_t GetNextTimeStampByLoop(std::int64_t i64CurTimeStamp) {
        while (i64CurTimeStamp <= m_i64LastTimeStamp)
        {
            i64CurTimeStamp = m_i64BootTimeStamp + GetCurrentTimeStamp<std::chrono::steady_clock>();
        }
        return i64CurTimeStamp;
    }

    std::int64_t GetNextTimeStampBySleep() {
        auto dDuration = std::chrono::milliseconds(m_i64LastTimeStamp);                                                    //ʱ���Ԫ�����ھ�����ʱ���
        auto tpTime = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>(dDuration);            //�õ�ʱ���
        std::this_thread::sleep_until(tpTime);
        return m_i64BootTimeStamp + GetCurrentTimeStamp<std::chrono::steady_clock>();
    }

private:
    IDGenerater(int iDataCenterID, int iWorkerID) :m_i64DataCenterID(iDataCenterID), m_i64WorkerID(iWorkerID), m_i64SequenceID(0), m_i64LastTimeStamp(0) {
        m_i64BootTimeStamp = GetCurrentTimeStamp<std::chrono::system_clock>() - GetCurrentTimeStamp<std::chrono::steady_clock>();    //ϵͳ����ʱ��(��������ȡ����ʵ����ʱϵͳʱ��)
    }
    IDGenerater() = delete;
    ~IDGenerater() = default;

    IDGenerater(const IDGenerater& rhs) = delete;
    IDGenerater(IDGenerater&& rhs) = delete;
    IDGenerater& operator=(const IDGenerater& rhs) = delete;
    IDGenerater& operator=(IDGenerater&& rhs) = delete;

private:
    std::int64_t m_i64DataCenterID;
    std::int64_t m_i64WorkerID;
    std::int64_t m_i64SequenceID;
    std::int64_t m_i64LastTimeStamp;
    std::int64_t m_i64BootTimeStamp;
    LockType m_lock;
};

using NonLockIDGenerater = IDGenerater<>;

#endif    //!__IDGENERATER_H_
