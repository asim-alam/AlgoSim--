#ifndef CORE_H
#define CORE_H

typedef unsigned int uint32;
typedef int int32;
typedef unsigned char uint8;

template <typename K, typename V>
struct Pair
{
    K first;
    V second;
    Pair()
    {
    }
    Pair(K f, V s)
    {
        first = f;
        second = s;
    }
    bool operator<(const Pair &other) const
    {
        return first < other.first;
    }

    bool operator>(const Pair &other) const
    {
        return first > other.first;
    }
};

template <typename T>
class Vector
{
private:
    T *data;
    int sizex;
    int capacity;

public:
    Vector()
    {
        data = nullptr;
        sizex = 0;
        capacity = 0;
    }

    ~Vector()
    {
        delete[] data;
    }

    void push_back(T value)
    {
        if (sizex == capacity)
        {
            int newCapacity = (capacity == 0) ? 4 : capacity * 2;
            T *newData = new T[newCapacity];

            for (int i = 0; i < sizex; i++)
                newData[i] = data[i];

            delete[] data;
            data = newData;
            capacity = newCapacity;
        }

        data[sizex] = value;
        sizex++;
    }

    void pop_back()
    {
        if (sizex > 0)
            sizex--;
    }

    T get(int index)
    {
        return data[index];
    }

    int size()
    {
        return sizex;
    }

    T &operator[](int index) { return data[index]; }
    const T &operator[](int index) const { return data[index]; }

    bool empty() const { return sizex == 0; }
    void clear() { sizex = 0; }

    T *begin() { return data; }
    T *end() { return data + sizex; }
};

template <typename T>
class Stack
{
private:
    Vector<T> vec;

public:
    void push(const T &val)
    {
        vec.push_back(val);
    }

    T pop()
    {
        if (vec.empty())
        {
            return T();
        }

        T value = vec.get(vec.size() - 1);
        vec.pop_back();
        return value;
    }

    T &top()
    {
        return vec.get(vec.size() - 1);
    }

    bool empty() const
    {
        return vec.empty();
    }

    int size() const
    {
        return vec.size();
    }
};
template <typename T>
struct ListNode
{
    T data;
    ListNode *next;
    ListNode *prev;
    ListNode(const T &val) : data(val), next(nullptr), prev(nullptr) {}
};

template <typename T>
class Queue
{
private:
    ListNode<T> *head;
    ListNode<T> *tail;
    int count;

public:
    Queue() : head(nullptr), tail(nullptr), count(0) {}
    ~Queue()
    {
        while (head)
        {
            ListNode<T> *temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push(const T &val)
    {
        ListNode<T> *node = new ListNode<T>(val);
        if (!tail)
        {
            head = tail = node;
        }
        else
        {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        count++;
    }

    void pop()
    {
        if (!head)
            return;
        ListNode<T> *temp = head;
        head = head->next;
        if (head)
            head->prev = nullptr;
        else
            tail = nullptr;
        delete temp;
        count--;
    }

    T &front() { return head->data; }
    bool empty() const { return count == 0; }
    int size() const { return count; }
};

template <typename T>
class PriorityQueue
{
private:
    Vector<T> heap;

    void heapifyUp(int index)
    {
        if (index == 0)
            return;
        int parent = (index - 1) / 2;
        if (heap[index] < heap[parent])
        {
            T temp = heap[index];
            heap[index] = heap[parent];
            heap[parent] = temp;
            heapifyUp(parent);
        }
    }

    void heapifyDown(int index)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < heap.size() && heap[left] < heap[smallest])
            smallest = left;
        if (right < heap.size() && heap[right] < heap[smallest])
            smallest = right;

        if (smallest != index)
        {
            T temp = heap[index];
            heap[index] = heap[smallest];
            heap[smallest] = temp;
            heapifyDown(smallest);
        }
    }

public:
    void push(const T &val)
    {
        heap.push_back(val);
        heapifyUp(heap.size() - 1);
    }

    void pop()
    {
        if (heap.empty())
            return;
        heap[0] = heap[heap.size() - 1];
        heap.pop_back();
        heapifyDown(0);
    }

    T &top() { return heap[0]; }
    bool empty() const { return heap.empty(); }
    int size() const { return heap.size(); }
    void clear() { heap.clear(); }
};

#endif