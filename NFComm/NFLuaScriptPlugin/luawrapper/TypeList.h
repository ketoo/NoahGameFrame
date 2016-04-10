#ifndef _TYPELIST_H
#define _TYPELIST_H

class NullType {};


//Typelist

template <class T, class U>
struct Typelist
{
   typedef T Head;
   typedef U Tail;
};

template <class TList, class T> struct IndexOf;
        
template <class T>
struct IndexOf<NullType, T>
{
   enum { value = -1 };
};
        
template <class T, class Tail>
struct IndexOf<Typelist<T, Tail>, T>
{
   enum { value = 0 };
};
        
template <class Head, class Tail, class T>
struct IndexOf<Typelist<Head, Tail>, T>
{
   private:
       enum { temp = IndexOf<Tail, T>::value };
   public:
       enum { value = (temp == -1 ? -1 : 1 + temp) };
};

#define LOKI_TYPELIST_1(T1) Typelist<T1, NullType>

#define LOKI_TYPELIST_2(T1, T2) Typelist<T1, LOKI_TYPELIST_1(T2) >

#define LOKI_TYPELIST_3(T1, T2, T3) Typelist<T1, LOKI_TYPELIST_2(T2, T3) >

#define LOKI_TYPELIST_4(T1, T2, T3, T4) \
    Typelist<T1, LOKI_TYPELIST_3(T2, T3, T4) >

#define LOKI_TYPELIST_5(T1, T2, T3, T4, T5) \
    Typelist<T1, LOKI_TYPELIST_4(T2, T3, T4, T5) >

#define LOKI_TYPELIST_6(T1, T2, T3, T4, T5, T6) \
    Typelist<T1, LOKI_TYPELIST_5(T2, T3, T4, T5, T6) >

#define LOKI_TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) \
    Typelist<T1, LOKI_TYPELIST_6(T2, T3, T4, T5, T6, T7) >

#define LOKI_TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8) \
    Typelist<T1, LOKI_TYPELIST_7(T2, T3, T4, T5, T6, T7, T8) >

#define LOKI_TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) \
    Typelist<T1, LOKI_TYPELIST_8(T2, T3, T4, T5, T6, T7, T8, T9) >

#define LOKI_TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) \
    Typelist<T1, LOKI_TYPELIST_9(T2, T3, T4, T5, T6, T7, T8, T9, T10) >

#define LOKI_TYPELIST_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) \
    Typelist<T1, LOKI_TYPELIST_10(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) >

#define LOKI_TYPELIST_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) \
    Typelist<T1, LOKI_TYPELIST_11(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12) >

#define LOKI_TYPELIST_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,T13) \
    Typelist<T1, LOKI_TYPELIST_12(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13) >

#define LOKI_TYPELIST_14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,T13,T14) \
    Typelist<T1, LOKI_TYPELIST_13(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13,T14) >
		
#define LOKI_TYPELIST_15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,T13,T14,T15) \
    Typelist<T1, LOKI_TYPELIST_14(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13,T14,T15) >		

template<int v>
struct Int2Type
{
	enum {value = v};
};

#endif