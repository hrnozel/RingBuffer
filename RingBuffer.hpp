#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP

#include <mutex>
#include <memory>
#include <cstdint>

namespace buffer {

template <class T> class RingBuffer {
protected:
  std::uint32_t m_head = 0;
  std::uint32_t m_tail = 0;
  std::uint32_t m_size = 0;
  bool m_isOverwriteEnabled = false;
  bool m_isFull = false;
  bool m_isBufferOverflow = false;

  mutable std::mutex m_mutex;

  std::unique_ptr<T[]> m_buffer;

public:
  RingBuffer(std::uint32_t size, bool overwriteEnabled)
      : m_buffer(new T[size]), m_size(size),
        m_isOverwriteEnabled(overwriteEnabled){};

  ~RingBuffer() {};

    [[nodiscard]] bool enqueue(const T &item)
    {
        std::unique_lock lock{m_mutex};

        if ((m_isOverwriteEnabled) || (m_isFull == false)) {
            if (m_isFull) {
            m_head = (m_head + 1) % m_size;
            m_isBufferOverflow = true;
            }

            m_buffer[m_tail] = item;
            m_tail = (m_tail + 1) % m_size;
            m_isFull = (m_head == m_tail);
        }
        else
        {
          return false;
        }
    };
    [[nodiscard]] bool dequeue(T &item) 
    {
        std::unique_lock lock{m_mutex};

        m_isBufferOverflow = false;

        if (isEmpty()) {
            return false;
        } else {
            item = m_buffer[m_head];
            m_head = (m_head + 1) % m_size;
            m_isFull = false;

            return true;
        }
    }
    inline T &front() { return m_buffer[m_head]; };
    bool get(T &item, std::uint32_t index)
    {
        std::unique_lock lock{m_mutex};

        auto bufferIndex = ((m_head + index) % m_size);

        if (index < getSize()) {
            item = m_buffer[bufferIndex];
            return true;
        } else {
            return false;
        }
    }
  
    inline bool isEmpty() { return ((!m_isFull) && (m_head == m_tail)); }
    bool isFull() { return m_isFull; }
    bool isBufferOverflow() { return m_isBufferOverflow; }
    std::uint32_t getSize() 
    {
        std::uint32_t size = m_size;
        if (!m_isFull) {
            if (m_tail >= m_head) {
            size = (m_tail - m_head);
            } else {
            size = (m_size - m_head - m_tail);
            }
        }

        return size;
    }

    std::uint32_t getCapacity()
    {
        return m_size;
    }

    void empty() 
    {
        std::unique_lock lock{m_mutex};

        m_head = 0;
        m_tail = 0;
        m_isFull = false;
        m_isBufferOverflow = false;
    }
};

} // buffer
