/* Copyright (C) 2018  Adam Green (https://github.com/adamgreen)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
/* Circular queue used internally by MiP library. Overwrites oldest items once it is full.

   NOT THREAD SAFE!
   ****************
*/
#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdint.h>

template<class ElementType, uint8_t Size>
class CircularQueue
{
public:
    CircularQueue()
    {
        clear();
    }

    void clear()
    {
        m_count = 0;
        m_readIndex = 0;
        m_writeIndex = 0;
    }
    
    bool isEmpty()
    {
        return m_count == 0;
    }

    uint8_t available()
    {
        return m_count;
    }

    void push(const ElementType& element)
    {
        m_elements[m_writeIndex] = element;
        advanceWriteIndex();
        if (m_count < Size)
        {
            m_count++;
        }
        else
        {
            // Queue was full so oldest response was overwritten. Increment read index to discard oldest.
            advanceReadIndex();
        }
    }

    bool pop(ElementType& element)
    {
        if (isEmpty())
        {
            return false;
        }
        
        // Pop the oldest element from the circular queue.
        element = m_elements[m_readIndex];
        advanceReadIndex();
        m_count--;
        return true;
    }

protected:
    void advanceWriteIndex()
    {
        if (m_writeIndex == Size - 1)
        {
            // Wrap around to beginning of circular queue.
            m_writeIndex = 0;
        }
        else
        {
            m_writeIndex++;
        }
    }
    
    void advanceReadIndex()
    {
        if (m_readIndex == Size - 1)
        {
            // Wrap around to beginning of circular queue.
            m_readIndex = 0;
        }
        else
        {
            m_readIndex++;
        }
    }

    ElementType m_elements[Size];
    uint8_t     m_count;
    uint8_t     m_readIndex;
    uint8_t     m_writeIndex;
};

#endif // QUEUE_H_
