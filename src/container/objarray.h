/*
 * array.h - Declaration and implementation of the Array template class
 *
 * Author: Steffen Ott
 * Date: 06/30/2007
 *
 */


#ifndef _TEMPLATE_ARRAY_H_
#define _TEMPLATE_ARRAY_H_


#include <string.h> // for NULL, memcpy(), memset()
#ifndef likely
    #ifdef __GNUC__ // Check if GNUC is used
        #define likely(x)   __builtin_expect((x), 1)
        #define unlikely(x) __builtin_expect((x), 0)
    #else
        #warning No __buildin_expect() macro defined by the compiler
        #define likely(x)   (x)
        #define unlikely(x) (x)
    #endif
#endif

#ifdef __DEBUG__
    #define __ARRAY_DEBUG__
#endif


namespace penv{

/**
 * \brief The Array template class is a array implementation.
 * This array implementation only stores pointer to any type.
 * Because this array stores only pointers, it is allowed to
 * store a NULL pointer into this array.
 **/
template <typename T>
class Array
{
	public:
		Array(bool free = true);
		~Array();
    private:
        Array(Array& copy);
        Array& operator=(Array& copy);
    public:
        T*& operator[](const size_t at);

        T* ItemPtr(const size_t index);
        T& ItemRef(const size_t index);

		void Add(T* element);
		bool Insert(size_t index, T* element);
		bool Del(T*& element, bool free = true);
		bool Del(size_t index, bool free = true);
		bool Remove(T*& element, bool free = true);
		bool Remove(size_t index, bool free = true);

		void Shrink();
		void Alloc(size_t size);

		void Clear(bool free = true);


        /**
         * \brief Sets the join size, which is needed for
         * reallocation of the dynamic array.
         * \param size Join size.
         **/
        inline void SetJoinSize(size_t size)
        { m_joinsize = size; }

        /**
         * \brief Returns the join size, which is needed for
         * reallocation of the dynamic array.
         * \return Join size.
         **/
        inline size_t GetJoinSize() const
        { return (m_joinsize); }

        /**
         * \brief Sets the empty element of this array.
         * \param empty Empty element of this array.
         **/
        inline void SetEmptyItem(T* empty)
        { m_empty = empty; }

        /**
         * \brief Returns the empty element of this array.
         * \return Empty element of this array.
         **/
        inline T* GetEmptyItem() const
        { return (m_empty); }

        /**
         * \brief Returns the number of elements in the array.
         * \return Number elements in the array.
         **/
		inline size_t Count()
		{ return (m_size); }

        #ifdef __ARRAY_DEBUG__
		void Print(bool alloc = false);
		#endif

	public:
        T** m_array;
        size_t m_size;
        size_t m_allocsize;
        T* m_empty;
        size_t m_joinsize;
        bool m_free;
};




//----------------------------------------------------------------
template <typename T>
Array<T>::Array(bool free)
/**
 * \brief Constructor, initializes the Array<T> object.
 * \param free If set to true, the Del(), Remove(), Clear() and
 * Destructor, will not free any memory.
 **/
{
	// Besorge Speicher ...
	m_empty = NULL;
	m_array = NULL;
	m_joinsize = 10;
	m_array = new T*[m_joinsize];
	// .. und setzte Array Größen Werte.
	m_size = 0;
    m_allocsize = m_joinsize;
	// Initalisiere das Array
	m_array = (T**)memset((void*)m_array, 0, m_allocsize*sizeof(T*));
	m_free = free;
}



//----------------------------------------------------------------
template <typename T>
Array<T>::~Array()
/**
 * \brief Destructor.
 **/
{
	Clear(m_free);
	if (m_array != NULL) delete [] m_array;
}


//----------------------------------------------------------------
template <typename T>
Array<T>::Array(Array<T>& copy)
/**
 * \brief Copyconstructor (private).
 **/
{
	// Unbenuzt ......
}


//----------------------------------------------------------------
template <typename T>
Array<T>& Array<T>::operator=(Array<T>& copy)
/**
 * \brief Assignment operator (private).
 **/
{
	// Unbenutzt .....
	return (*this);
}


//----------------------------------------------------------------
template <typename T>
T*& Array<T>::operator[](const size_t index)
/**
 * \brief Index operator, returns a element by its index.
 * \param index Index of an element in the array.
 * \return Found element or empty (see SetEmptyItem()).
 **/
{
	if (index < 0 || index >= m_size) return (m_empty);
	return (m_array[index]);
}



//----------------------------------------------------------------
template <typename T>
T* Array<T>::ItemPtr(const size_t index)
/**
 * \brief Returns a pointer to a element by its index.
 * \param index Index of an element in the array.
 * \return Found element or empty (see SetEmptyItem()).
 **/
{
	if (index < 0 || index >= m_size) return (m_empty);
	return (m_array[index]);
}


//----------------------------------------------------------------
template <typename T>
T& Array<T>::ItemRef(const size_t index)
/**
 * \brief Returns a refrence to a element by its index.
 * \param index Index of an element in the array.
 * \return Found element or empty (see SetEmptyItem()).
 **/
{
	if (index < 0 || index >= m_size) return (*(m_empty));
	return (*(m_array[index]));
}



//----------------------------------------------------------------
template <typename T>
void Array<T>::Add(T* element)
/**
 * \brief Adds a new element to the end of the array.
 * \param element The element which should be added.
 */
{
	if (unlikely(m_size >= m_allocsize)) {
		Alloc(m_joinsize);
	}
	m_array[m_size] = element;
	m_size++;
}


//----------------------------------------------------------------
template <typename T>
bool Array<T>::Insert(size_t index, T* element)
/**
 * \brief Inserts a new element to the spezified index
 * into the array.
 * \param index Index of the new element.
 * \param element The element which should be added.
 * \return True on success; false otherwise.
 **/
{
    if (unlikely(index < 0 || index > m_size)) {
        return (false);
    }
    if (unlikely(index == m_size)) {
        Add(element);
        return (true);
    }
    if (unlikely(m_allocsize <= m_size)) {
        Alloc(m_joinsize);
    }
    size_t typesize = sizeof(T*);
    memcpy(m_array+index+1, m_array+index, (m_size-index)*typesize);
    m_array[index] = element;
	m_size++;
    return (true);
}


//----------------------------------------------------------------
template <typename T>
bool Array<T>::Del(T*& element, bool free)
/**
 * \brief Deletes a element from the array. Simply sets the element NULL
 * and does not reorder the index positions of other elements.
 * \param element The element which should be deleted.
 * \param free True if the element should be deleted.
 * \return True on success; false otherwise.
 */
{
    if (!m_free) return (false);
    // Suche Element in Array
    for (size_t i=0; i<m_size; ++i)
    {
        if (m_array[i] == element)
        {
            if (free && element != NULL) delete element;
            element = NULL;
            m_array[i] = NULL;
            return (true);
        }
    }
	return (false);
}



//----------------------------------------------------------------
template <typename T>
bool Array<T>::Del(size_t index, bool free)
/**
 * \brief Deletes a element from the array. Simply sets the element NULL
 * and does not reorder the index positions of other elements.
 * \param index The index of the element which should be deleted.
 * \param free True if the element should be deleted.
 * \return True on success; false otherwise.
 */
{
    if (!m_free) return (false);
    if (unlikely(index < 0 || index >= m_size)) return (false);
    if (unlikely(m_array[index] == NULL)) return (false);
    if (free) delete m_array[index];
    m_array[index] = NULL;
	return (true);
}



//----------------------------------------------------------------
template <typename T>
bool Array<T>::Remove(T*& element, bool free)
/**
 * \brief Removes a element from the array.
 * \param element The element which should be removed.
 * \param free True if the element should be deleted.
 * \return True on success; false otherwise.
 **/
{
    if (!m_free) return (false);
    if (unlikely(element == NULL)) return (false);
    int index = -1;
    for (size_t i=0; i<m_size; ++i)
    {
        if (m_array[i] == element) {
            index = (int)i;
            break;
        }
    }
    if (index == -1) {
        return (false);
    }
    Remove((size_t)index, free);
    element = NULL;
    return (false);
}


//----------------------------------------------------------------
template <typename T>
bool Array<T>::Remove(size_t index, bool free)
/**
 * \brief Removes a element from the array.
 * \param index The index of the element which should be deleted.
 * \param free True if the element should be deleted.
 * \return True on success; false otherwise.
 **/
{
    if (!m_free) return (false);
    if (index < 0 || index >= m_size) {
        return (false);
    }
    if (free) delete m_array[index];
    memcpy(m_array+index, m_array+index+1, (m_size-index)*sizeof(T*));
    m_size--;
    m_array[m_size] = NULL;
    return (true);
}



//----------------------------------------------------------------
template <typename T>
void Array<T>::Shrink()
/**
 * Decreases the capacity of the dynamic array until it has the size
 * of the actual number of items in the array.
 */
{
	if (unlikely(m_size >= m_allocsize)) return;
    // Initalisiere neues Array
	T** newarray = new T*[m_size];
	// Kopiere alle Elemente vom alten Array in das Neue
    newarray = (T**)memcpy(newarray, m_array, m_size*sizeof(T*));
    // Neues Array setzten und altes löschen
    delete [] m_array;
    m_array = newarray;
    // Größe setzten
    m_allocsize = m_size;
}




//----------------------------------------------------------------
template <typename T>
void Array<T>::Alloc(size_t size)
/**
 * Increases the capacity of the array by size.
 * \param size The size of which to increase the capacity.
 */
{
    if (unlikely(size == 0)) return;
	// Versuche neuen Speicher zu besorgen.
	T** newarray = new T*[m_allocsize+size];
	// Initalisiere restliche Elemente des Array
	newarray = (T**)memset((void*)(newarray), 0, (m_allocsize+size)*sizeof(T*));
	// Kopiere alle Elemente vom alten Array in das Neue
    newarray = (T**)memcpy(newarray, m_array, m_allocsize*sizeof(T*));
    // Neues Array setzten und altes löschen
    delete [] m_array;
    m_array = newarray;
    // Größe setzten
    m_allocsize += size;
}


//----------------------------------------------------------------
template <typename T>
void Array<T>::Clear(bool free)
/**
 * Removes all elements from the array.
 * \param free True if the element should be deleted.
 */
{
    if (likely(m_array != NULL && free))
    {
        for (size_t i=0; i<m_size; i++)
        {
            if (likely(m_array[i] != NULL))
            {
                delete m_array[i];
                m_array[i] = NULL;
            }
        }
    }
    m_size = 0;
}

#ifdef __ARRAY_DEBUG__
    // Predefines for printf methods
    #ifdef __WXDEBUG__
        #define PENV_ARRAY_DEBUG_PRINTF0(msg)      wxLogMessage(_T(msg))
        #define PENV_ARRAY_DEBUG_PRINTF1(msg, var) wxLogMessage(_T(msg), var)
    #else
        #include <cstdio>
        #define PENV_ARRAY_DEBUG_PRINTF0(msg)      printf(msg)
        #define PENV_ARRAY_DEBUG_PRINTF1(msg, var) printf(msg, var)
    #endif
//----------------------------------------------------------------
template <typename T>
void Array<T>::Print(bool alloc)
/**
 * \brief Prints out debug information for the array.
 * \param alloc True if all elements including the capacity
 * should be printed.
 **/
{
    PENV_ARRAY_DEBUG_PRINTF0("-------------------------------------\n");
    PENV_ARRAY_DEBUG_PRINTF1("size = %u\n", m_size);
    PENV_ARRAY_DEBUG_PRINTF1("alloc size = %u\n", m_allocsize);
    PENV_ARRAY_DEBUG_PRINTF0("array = \n");
    for (size_t i=0; i<m_size; ++i)
    {
        if (m_array[i] == NULL) PENV_ARRAY_DEBUG_PRINTF0("NULL ");
        //else PENV_ARRAY_DEBUG_PRINTF1("%s ", m_array[i]->GetCommandId().c_str());
        else PENV_ARRAY_DEBUG_PRINTF1("%p ", m_array[i]);
    }
    if (alloc)
    {
        PENV_ARRAY_DEBUG_PRINTF0("// ");
        for (size_t i=m_size; i<m_allocsize; ++i)
        {
            if (m_array[i] == NULL) PENV_ARRAY_DEBUG_PRINTF0("NULL ");
            //else PENV_ARRAY_DEBUG_PRINTF1("%s ", m_array[i]->GetCommandId().c_str());
            else PENV_ARRAY_DEBUG_PRINTF1("%p ", m_array[i]);
        }
    }
    PENV_ARRAY_DEBUG_PRINTF0("\n");
}
#endif // __ARRAY_DEBUG__



}//namespace penv

#endif // _TEMPLATE_ARRAY_H_


