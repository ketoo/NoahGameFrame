#pragma once
#include <typeindex>
#include <iostream>
#include <type_traits>

using namespace std;
namespace mapbox
{
    namespace util
    {
        template<typename T, typename... Args>
        struct MaxType : std::integral_constant<int,
            (sizeof(T)>MaxType<Args...>::value ? sizeof(T) : MaxType<Args...>::value) >
        {};
        template<typename T>
        struct MaxType<T> : std::integral_constant<int, sizeof(T) >{};
        template < typename T, typename... List >
        struct Contains : std::true_type {};
        template < typename T, typename Head, typename... Rest >
        struct Contains<T, Head, Rest...>
            : std::conditional < std::is_same<T, Head>::value, std::true_type, Contains < T,
            Rest... >> ::type{};
        template < typename T >
        struct Contains<T> : std::false_type{};
        template<typename... Args>
        struct variantHelper;
        template<typename T, typename... Args>
        struct variantHelper<T, Args...> {
            inline static void Destroy(type_index id, void * data)
            {
                if (id == type_index(typeid(T)))
                    ((T*)(data))->~T();
                else
                    variantHelper<Args...>::Destroy(id, data);
            }
            inline static void Copy(type_index id, void *src, void *dst)
            {
                if (id == type_index(typeid(T)))
                {
                    typedef typename std::remove_reference<T>::type U;
                    new(dst)U();
                    *((T*)(dst)) = *((T*)(src));
                }
                else
                    variantHelper<Args...>::Copy(id, src,dst);
            }
        };
        template<> struct variantHelper<>  {
            inline static void Destroy(type_index id, void * data) { }
            inline static void Copy(type_index id, void * src, void *dst) { }
        };

        template<typename... Types>
        class variant
        {
            typedef variantHelper<Types...> Helper_t;
        public:
            variant(void) :m_typeIndex(typeid(void))
            {
            }
            ~variant()
            {
                Helper_t::Destroy(m_typeIndex, &m_data);
            }
            template<typename T>
            bool Is() const
            {
                return (m_typeIndex == type_index(typeid(T)));
            }
            template<typename T>
            T& get() const
            {
                if (!Is<T>())
                {
                    cout << typeid(T).name() << " is not defined. " << "current type is " <<
                        m_typeIndex.name() << endl;
                    throw std::bad_cast();
                }
                return *(T*)(&m_data);
            }
            void copy_assign(const variant& rhs)
            {
                Helper_t::Destroy(m_typeIndex, &m_data);
                Helper_t::Copy(rhs.m_typeIndex, (void*)&rhs.m_data, (void*)&m_data);
                m_typeIndex = rhs.m_typeIndex;
            }

            template <class T, class = typename std::enable_if<Contains<typename std::remove_reference<T>::type, Types...>::value>::type>
            variant& operator = (T &&value)
            {
                Helper_t::Destroy(m_typeIndex, &m_data);
                typedef typename std::remove_reference<T>::type U;
                new(m_data)U(std::forward<T>(value));
                m_typeIndex = type_index(typeid(T));
                return *this;
            }
            variant& operator = (const variant &other)
            {
                copy_assign(other);
                return *this;
            }
            //template <class T, class = typename std::enable_if<Contains<typename std::remove_reference<T>::type, Types...>::value>::type>
            //variant(T&& value) : m_typeIndex(type_index(typeid(T)))
            //{
            //    //Helper_t::Destroy(m_typeIndex, &m_data);
            //    typedef typename std::remove_reference<T>::type U;
            //    new(m_data)U(std::forward<T>(value));
            //}
        private:
            char m_data[MaxType<Types...>::value];
            std::type_index m_typeIndex;
        };
    }
}