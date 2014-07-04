// Copyright (c) 2005 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef LUABIND_OBJECT_050419_HPP
#define LUABIND_OBJECT_050419_HPP

#include <boost/implicit_cast.hpp> // detail::push()
#include <boost/ref.hpp> // detail::push()
#include <boost/mpl/bool.hpp> // value_wrapper_traits specializations
#include <boost/mpl/apply_wrap.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/optional.hpp>

#include <luabind/nil.hpp>
#include <luabind/value_wrapper.hpp>
#include <luabind/detail/pcall.hpp>
#include <luabind/handle.hpp>
#include <luabind/from_stack.hpp>
#include <luabind/detail/policy.hpp>
#include <luabind/detail/stack_utils.hpp>
#include <luabind/detail/convert_to_lua.hpp> // REFACTOR
#include <luabind/typeid.hpp>

#include <boost/iterator/iterator_facade.hpp> // iterator

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/utility/enable_if.hpp>

namespace luabind {

namespace detail 
{
  namespace mpl = boost::mpl;
  
  template<class T, class ConverterGenerator>
  void push_aux(lua_State* interpreter, T& value, ConverterGenerator*)
  {
      typedef typename boost::mpl::if_<
          boost::is_reference_wrapper<T>
        , BOOST_DEDUCED_TYPENAME boost::unwrap_reference<T>::type&
        , T
      >::type unwrapped_type;

      typename mpl::apply_wrap2<
          ConverterGenerator,unwrapped_type,cpp_to_lua
      >::type cv;

      cv.apply(
          interpreter
        , boost::implicit_cast<
              BOOST_DEDUCED_TYPENAME boost::unwrap_reference<T>::type&
          >(value)
      );
  }

  template<class T, class Policies>
  void push(lua_State* interpreter, T& value, Policies const&)
  {
      typedef typename find_conversion_policy<
          0
        , Policies
      >::type converter_policy;

      push_aux(interpreter, value, (converter_policy*)0);
  }

  template<class T>
  void push(lua_State* interpreter, T& value)
  {
      push(interpreter, value, null_type());
  }

} // namespace detail

namespace adl
{
  namespace mpl = boost::mpl;
  
  template <class T>
  class object_interface;
  
  namespace is_object_interface_aux
  {
    typedef char (&yes)[1];
    typedef char (&no)[2];
    
    template <class T>
    yes check(object_interface<T>*);
    no check(void*);

    template <class T>
    struct impl 
    {
        BOOST_STATIC_CONSTANT(bool, value =
            sizeof(is_object_interface_aux::check((T*)0)) == sizeof(yes)
        );

        typedef mpl::bool_<value> type;
    };

  } // namespace detail

  template <class T>
  struct is_object_interface
    : is_object_interface_aux::impl<T>::type
  {};

  template <class R, class T, class U>
  struct enable_binary
# ifndef BOOST_NO_SFINAE
    : boost::enable_if<
          mpl::or_<
              is_object_interface<T>
            , is_object_interface<U>
          >
        , R
      >
  {};
# else
  {
      typedef R type;
  };
# endif

  template<class T, class U>
  int binary_interpreter(lua_State*& L, T const& lhs, U const& rhs
    , boost::mpl::true_, boost::mpl::true_)
  {
       L = value_wrapper_traits<T>::interpreter(lhs);
		 lua_State* L2 = value_wrapper_traits<U>::interpreter(rhs);

       // you are comparing objects with different interpreters
       // that's not allowed.
		 assert(L == L2 || L == 0 || L2 == 0);

       // if the two objects we compare have different interpreters
       // then they

       if (L != L2) return -1;
       if (L == 0) return 1;
       return 0;
  }
	
  template<class T, class U>
  int binary_interpreter(lua_State*& L, T const& x, U const&
    , boost::mpl::true_, boost::mpl::false_)
  {
       L = value_wrapper_traits<T>::interpreter(x);
       return 0;
  }

  template<class T, class U>
  int binary_interpreter(lua_State*& L, T const&, U const& x, boost::mpl::false_, boost::mpl::true_)
  {
      L = value_wrapper_traits<U>::interpreter(x);
      return 0;
  }

  template<class T, class U>
  int binary_interpreter(lua_State*& L, T const& x, U const& y)
  {
      return binary_interpreter(
          L
        , x
        , y
        , is_value_wrapper<T>()
        , is_value_wrapper<U>()
      );
  }

#define LUABIND_BINARY_OP_DEF(op, fn) \
  template<class LHS, class RHS> \
  typename enable_binary<bool,LHS,RHS>::type \
  operator op(LHS const& lhs, RHS const& rhs) \
  { \
      lua_State* L = 0; \
      switch (binary_interpreter(L, lhs, rhs)) \
      { \
          case 1: \
              return true; \
          case -1: \
              return false; \
      } \
\
      assert(L); \
\
      detail::stack_pop pop1(L, 1); \
      detail::push(L, lhs); \
      detail::stack_pop pop2(L, 1); \
      detail::push(L, rhs); \
\
      return fn(L, -1, -2) != 0; \
  }

LUABIND_BINARY_OP_DEF(==, lua_equal)
LUABIND_BINARY_OP_DEF(<, lua_lessthan)

  template<class ValueWrapper>
  std::ostream& operator<<(std::ostream& os
    , object_interface<ValueWrapper> const& v)
  {
      using namespace luabind;
      lua_State* interpreter = value_wrapper_traits<ValueWrapper>::interpreter(
          static_cast<ValueWrapper const&>(v));
      detail::stack_pop pop(interpreter, 1);
      value_wrapper_traits<ValueWrapper>::unwrap(interpreter
        , static_cast<ValueWrapper const&>(v));
		char const* p = lua_tostring(interpreter, -1);
        std::size_t len = lua_strlen(interpreter, -1);
		std::copy(p, p + len, std::ostream_iterator<char>(os));
		return os;
	}

#undef LUABIND_BINARY_OP_DEF

  template<class LHS, class RHS>
  typename enable_binary<bool,LHS,RHS>::type
  operator>(LHS const& lhs, RHS const& rhs)
  {
      return !(lhs < rhs || lhs == rhs);
  }

  template<class LHS, class RHS>
  typename enable_binary<bool,LHS,RHS>::type 
  operator<=(LHS const& lhs, RHS const& rhs)
  {
      return lhs < rhs || lhs == rhs;
  }

  template<class LHS, class RHS>
  typename enable_binary<bool,LHS,RHS>::type 
  operator>=(LHS const& lhs, RHS const& rhs)
  {
      return !(lhs < rhs);
  }

  template<class LHS, class RHS>
  typename enable_binary<bool,LHS,RHS>::type 
  operator!=(LHS const& lhs, RHS const& rhs)
  {
      return !(lhs == rhs);
  }

  template<class ValueWrapper, class Arguments>
  struct call_proxy;

  template<class Next>
  class index_proxy;

  class object;
  
  template<class Derived>
  class object_interface
  {
      struct safe_bool_type {};
  public:
      ~object_interface() {}

      call_proxy<Derived, boost::tuples::tuple<> > operator()();

      template<class A0>
      call_proxy<
          Derived
        , boost::tuples::tuple<A0 const*>
      > operator()(A0 const& a0)
      {
          typedef boost::tuples::tuple<A0 const*> arguments;

          return call_proxy<Derived, arguments>(
              derived()
            , arguments(&a0)
          );
      }

      template<class A0, class A1>
      call_proxy<
          Derived
        , boost::tuples::tuple<A0 const*, A1 const*>
      > operator()(A0 const& a0, A1 const& a1)
      {
          typedef boost::tuples::tuple<A0 const*, A1 const*> arguments;

          return call_proxy<Derived, arguments>(
              derived()
            , arguments(&a0, &a1)
          );
      }

      // The rest of the overloads are PP-generated.
      #define BOOST_PP_ITERATION_PARAMS_1 (3, \
          (3, LUABIND_MAX_ARITY, <luabind/detail/object_call.hpp>))
      #include BOOST_PP_ITERATE()

      operator safe_bool_type*() const
      {
          lua_State* L = value_wrapper_traits<Derived>::interpreter(derived());

          if (!L)
              return 0;

          value_wrapper_traits<Derived>::unwrap(L, derived());
          detail::stack_pop pop(L, 1);

          return lua_toboolean(L, -1) == 1 ? (safe_bool_type*)1 : 0;
      }

  private:
      Derived& derived()
      {
          return *static_cast<Derived*>(this);
      }

      Derived const& derived() const
      {
          return *static_cast<Derived const*>(this);
      }
  };

#ifdef LUABIND_USE_VALUE_WRAPPER_TAG
  struct iterator_proxy_tag;
#endif
  
  template<class AccessPolicy>
  class iterator_proxy
    : public object_interface<iterator_proxy<AccessPolicy> >
  {
  public:
#ifdef LUABIND_USE_VALUE_WRAPPER_TAG
      typedef iterator_proxy_tag value_wrapper_tag;
#endif

      iterator_proxy(lua_State* interpreter, handle const& table, handle const& key)
        : m_interpreter(interpreter)
        , m_table_index(lua_gettop(interpreter) + 1)
        , m_key_index(m_table_index + 1)
      {
          table.push(m_interpreter);
          key.push(m_interpreter);
      }

      iterator_proxy(iterator_proxy const& other)
        : m_interpreter(other.m_interpreter)
        , m_table_index(other.m_table_index)
        , m_key_index(other.m_key_index)
      {
          other.m_interpreter = 0;
      }

      ~iterator_proxy()
      {
          if (m_interpreter)
              lua_pop(m_interpreter, 2);
      }

		// this will set the value to nil
		iterator_proxy & operator=(luabind::detail::nil_type)
		{
          lua_pushvalue(m_interpreter, m_key_index);
			 lua_pushnil(m_interpreter);
          AccessPolicy::set(m_interpreter, m_table_index);
          return *this;
		}

      template<class T>
      iterator_proxy& operator=(T const& value)
      {
          lua_pushvalue(m_interpreter, m_key_index);
          detail::push(m_interpreter, value);
          AccessPolicy::set(m_interpreter, m_table_index);
          return *this;
      }

      template<class Key>
      index_proxy<iterator_proxy<AccessPolicy> > operator[](Key const& key)
      {
          return index_proxy<iterator_proxy<AccessPolicy> >(
              *this, m_interpreter, key
          );
      }

      // This is non-const to prevent conversion on lvalues.
      operator object();

      lua_State* interpreter() const
      {
          return m_interpreter;
      }

      // TODO: Why is it non-const?
      void push(lua_State* interpreter)
      {
          assert(interpreter == m_interpreter);
          lua_pushvalue(m_interpreter, m_key_index);
          AccessPolicy::get(m_interpreter, m_table_index);
      }

  private:
      mutable lua_State* m_interpreter;
      int m_table_index;
      int m_key_index;
  };

} // namespace adl

namespace detail
{
  struct basic_access
  {
      static void set(lua_State* interpreter, int table)
      {
          lua_settable(interpreter, table);
      }

      static void get(lua_State* interpreter, int table)
      {
          lua_gettable(interpreter, table);
      }
  };

  struct raw_access
  {
      static void set(lua_State* interpreter, int table)
      {
          lua_rawset(interpreter, table);
      }

      static void get(lua_State* interpreter, int table)
      {
          lua_rawget(interpreter, table);
      }
  };

  template<class AccessPolicy>
  class basic_iterator 
    : public boost::iterator_facade<
        basic_iterator<AccessPolicy>
      , adl::iterator_proxy<AccessPolicy>
      , boost::single_pass_traversal_tag
      , adl::iterator_proxy<AccessPolicy>
    >
  {
  public:
      basic_iterator()
        : m_interpreter(0)
      {}

      template<class ValueWrapper>
      explicit basic_iterator(ValueWrapper const& value_wrapper)
        : m_interpreter(
              value_wrapper_traits<ValueWrapper>::interpreter(value_wrapper)
          )
      {
          detail::stack_pop pop(m_interpreter, 1);
          value_wrapper_traits<ValueWrapper>::unwrap(m_interpreter, value_wrapper);

          lua_pushnil(m_interpreter);
          if (lua_next(m_interpreter, -2) != 0)
          {
              detail::stack_pop pop(m_interpreter, 2);
              handle(m_interpreter, -2).swap(m_key);
          }
          else
          {
              m_interpreter = 0;
              return;
          }

          handle(m_interpreter, -1).swap(m_table);
      }

      adl::object key() const;

  private:
      friend class boost::iterator_core_access;

      void increment()
      {
          m_table.push(m_interpreter);
          m_key.push(m_interpreter);

          detail::stack_pop pop(m_interpreter, 1);

          if (lua_next(m_interpreter, -2) != 0)
          {
              m_key.replace(m_interpreter, -2);
              lua_pop(m_interpreter, 2);
          }
          else
          {
              m_interpreter = 0;
              handle().swap(m_table);
              handle().swap(m_key);
          }
      }

      bool equal(basic_iterator const& other) const
      {
          if (m_interpreter == 0 && other.m_interpreter == 0)
              return true;

          if (m_interpreter != other.m_interpreter)
              return false;

          detail::stack_pop pop(m_interpreter, 2);
          m_key.push(m_interpreter);
          other.m_key.push(m_interpreter);
          return lua_equal(m_interpreter, -2, -1) != 0;
      }

      adl::iterator_proxy<AccessPolicy> dereference() const 
      {
          return adl::iterator_proxy<AccessPolicy>(m_interpreter, m_table, m_key);
      }

      lua_State* m_interpreter;
      handle m_table;
      handle m_key;
  };

// Needed because of some strange ADL issues.

#define LUABIND_OPERATOR_ADL_WKND(op) \
  inline bool operator op( \
      basic_iterator<basic_access> const& x \
    , basic_iterator<basic_access> const& y) \
  { \
      return boost::operator op(x, y); \
  } \
 \
  inline bool operator op( \
      basic_iterator<raw_access> const& x \
    , basic_iterator<raw_access> const& y) \
  { \
      return boost::operator op(x, y); \
  }

  LUABIND_OPERATOR_ADL_WKND(==)
  LUABIND_OPERATOR_ADL_WKND(!=)

#undef LUABIND_OPERATOR_ADL_WKND
 
} // namespace detail

namespace adl
{
 
#ifdef LUABIND_USE_VALUE_WRAPPER_TAG
  struct index_proxy_tag;    
#endif

  template<class Next>
  class index_proxy
    : public object_interface<index_proxy<Next> >
  {
  public:
#ifdef LUABIND_USE_VALUE_WRAPPER_TAG
      typedef index_proxy_tag value_wrapper_tag;
#endif

      typedef index_proxy<Next> this_type;

      template<class Key>
      index_proxy(Next const& next, lua_State* interpreter, Key const& key)
        : m_interpreter(interpreter)
        , m_key_index(lua_gettop(interpreter) + 1)
        , m_next(next)
      {
          detail::push(m_interpreter, key);
      }

      index_proxy(index_proxy const& other)
        : m_interpreter(other.m_interpreter)
        , m_key_index(other.m_key_index)
        , m_next(other.m_next)
      {
          other.m_interpreter = 0;
      }

      ~index_proxy()
      {
          if (m_interpreter)
              lua_pop(m_interpreter, 1);
      }

      // This is non-const to prevent conversion on lvalues.
      operator object();

		// this will set the value to nil
		this_type& operator=(luabind::detail::nil_type)
		{
	       value_wrapper_traits<Next>::unwrap(m_interpreter, m_next);
          detail::stack_pop pop(m_interpreter, 1);

          lua_pushvalue(m_interpreter, m_key_index);
			 lua_pushnil(m_interpreter);
          lua_settable(m_interpreter, -3);
          return *this;
		}
		
      template<class T>
      this_type& operator=(T const& value)
      {
          value_wrapper_traits<Next>::unwrap(m_interpreter, m_next);
          detail::stack_pop pop(m_interpreter, 1);

          lua_pushvalue(m_interpreter, m_key_index);
          detail::push(m_interpreter, value);
          lua_settable(m_interpreter, -3);
          return *this;
      }

      this_type& operator=(this_type const& value)
      {
          value_wrapper_traits<Next>::unwrap(m_interpreter, m_next);
          detail::stack_pop pop(m_interpreter, 1);

          lua_pushvalue(m_interpreter, m_key_index);
          detail::push(m_interpreter, value);
          lua_settable(m_interpreter, -3);
          return *this;
      }

      template<class T>
      index_proxy<this_type> operator[](T const& key)
      {
          return index_proxy<this_type>(*this, m_interpreter, key);
      }

      void push(lua_State* interpreter);

      lua_State* interpreter() const
      {
          return m_interpreter;
      }

  private:
		struct hidden_type {};
		
//      this_type& operator=(index_proxy<Next> const&);

      mutable lua_State* m_interpreter;
      int m_key_index;

      Next const& m_next;
  };

} // namespace adl

typedef detail::basic_iterator<detail::basic_access> iterator;
typedef detail::basic_iterator<detail::raw_access> raw_iterator;

#ifndef LUABIND_USE_VALUE_WRAPPER_TAG
template<class T>
struct value_wrapper_traits<adl::index_proxy<T> >
#else
template<>
struct value_wrapper_traits<adl::index_proxy_tag>
#endif
{
    typedef boost::mpl::true_ is_specialized;

    template<class Next>
    static lua_State* interpreter(adl::index_proxy<Next> const& proxy)
    {
        return proxy.interpreter();
    }

    template<class Next>
    static void unwrap(lua_State* interpreter, adl::index_proxy<Next> const& proxy)
    {
        const_cast<adl::index_proxy<Next>&>(proxy).push(interpreter);
    }
};

#ifndef LUABIND_USE_VALUE_WRAPPER_TAG
template<class AccessPolicy>
struct value_wrapper_traits<adl::iterator_proxy<AccessPolicy> >
#else
template<>
struct value_wrapper_traits<adl::iterator_proxy_tag>
#endif
{
    typedef boost::mpl::true_ is_specialized;

    template<class Proxy>
    static lua_State* interpreter(Proxy const& p)
    {
        return p.interpreter();
    }

    template<class Proxy>
    static void unwrap(lua_State* interpreter, Proxy const& p)
    {
        // TODO: Why const_cast?
        const_cast<Proxy&>(p).push(interpreter);
    }
};

namespace adl
{

  // An object holds a reference to a Lua value residing
  // in the registry.
  class object : public object_interface<object>
  {
  public:
      object()
      {}

      explicit object(handle const& other)
        : m_handle(other)
      {}

      explicit object(from_stack const& stack_reference)
        : m_handle(stack_reference.interpreter, stack_reference.index)
      {
      }

      template<class T>
      object(lua_State* interpreter, T const& value)
      {
          detail::push(interpreter, value);
          detail::stack_pop pop(interpreter, 1);
          handle(interpreter, -1).swap(m_handle);
      }

      template<class T, class Policies>
      object(lua_State* interpreter, T const& value, Policies const&)
      {
          detail::push(interpreter, value, Policies());
          detail::stack_pop pop(interpreter, 1);
          handle(interpreter, -1).swap(m_handle);
      }

      void push(lua_State* interpreter) const;
      lua_State* interpreter() const;
      bool is_valid() const;

      template<class T>
      index_proxy<object> operator[](T const& key) const
      {
          return index_proxy<object>(
              *this, m_handle.interpreter(), key
          );
      }

      void swap(object& other)
      {
          m_handle.swap(other.m_handle);
      }

  private:
      handle m_handle;
  };

  inline void object::push(lua_State* interpreter) const
  {
      m_handle.push(interpreter);
  }

  inline lua_State* object::interpreter() const
  {
      return m_handle.interpreter();
  }

  inline bool object::is_valid() const
  {
      return m_handle.interpreter() != 0;
  }

  class argument : public object_interface<argument>
  {
  public:
	  argument(from_stack const& stack_reference)
		: m_interpreter(stack_reference.interpreter)
		, m_index(stack_reference.index)
	  {
		  if (m_index < 0)
			  m_index = lua_gettop(m_interpreter) - m_index + 1;
	  }

      template<class T>
      index_proxy<argument> operator[](T const& key) const
      {
          return index_proxy<argument>(*this, m_interpreter, key);
      }

	  void push(lua_State* L) const
	  {
		  lua_pushvalue(L, m_index);
	  }

	  lua_State* interpreter() const
	  {
		  return m_interpreter;
	  }

  private:
	  lua_State* m_interpreter;
	  int m_index;
  };

} // namespace adl

using adl::object;
using adl::argument;

#ifndef LUABIND_USE_VALUE_WRAPPER_TAG
template <class ValueWrapper, class Arguments>
struct value_wrapper_traits<adl::call_proxy<ValueWrapper, Arguments> >
#else
template<>
struct value_wrapper_traits<adl::call_proxy_tag>
#endif
{
    typedef boost::mpl::true_ is_specialized;

    template<class W, class A>
    static lua_State* interpreter(adl::call_proxy<W,A> const& proxy)
    {
        return value_wrapper_traits<W>::interpreter(*proxy.value_wrapper);
    }

    template<class W, class A>
    static void unwrap(lua_State*, adl::call_proxy<W,A> const& proxy)
    {
        object result = const_cast<adl::call_proxy<W,A>&>(proxy);
        result.push(result.interpreter());
    }
};

template<>
struct value_wrapper_traits<object>
{
    typedef boost::mpl::true_ is_specialized;

    static lua_State* interpreter(object const& value)
    {
        return value.interpreter();
    }

    static void unwrap(lua_State* interpreter, object const& value)
    {
        value.push(interpreter);
    }

    static bool check(...)
    {
        return true;
    }
};

template<>
struct value_wrapper_traits<argument>
{
    typedef boost::mpl::true_ is_specialized;

    static lua_State* interpreter(argument const& value)
    {
        return value.interpreter();
    }

    static void unwrap(lua_State* interpreter, argument const& value)
    {
        value.push(interpreter);
    }

    static bool check(...)
    {
        return true;
    }
};

template<class Next>
inline void adl::index_proxy<Next>::push(lua_State* interpreter)
{
    assert(interpreter == m_interpreter);

    value_wrapper_traits<Next>::unwrap(m_interpreter, m_next);

    lua_pushvalue(m_interpreter, m_key_index);
    lua_gettable(m_interpreter, -2);
    lua_remove(m_interpreter, -2);
}

template<class Next>
inline adl::index_proxy<Next>::operator object()
{
    detail::stack_pop pop(m_interpreter, 1);
    push(m_interpreter);
    return object(from_stack(m_interpreter, -1));
}

template<class AccessPolicy>
adl::iterator_proxy<AccessPolicy>::operator object()
{
    lua_pushvalue(m_interpreter, m_key_index);
    AccessPolicy::get(m_interpreter, m_table_index);
    detail::stack_pop pop(m_interpreter, 1);
    return object(from_stack(m_interpreter, -1));
}

template<class AccessPolicy>
object detail::basic_iterator<AccessPolicy>::key() const
{
    return object(m_key);
}

namespace detail 
{

  template<
      class T
    , class ValueWrapper
    , class Policies
    , class ErrorPolicy
    , class ReturnType
  >
  ReturnType object_cast_aux(
      ValueWrapper const& value_wrapper
    , T*
    , Policies*
    , ErrorPolicy*
    , ReturnType*
  )
  {
      lua_State* interpreter = value_wrapper_traits<ValueWrapper>::interpreter(
          value_wrapper
      );

#ifndef LUABIND_NO_ERROR_CHECKING
      if (!interpreter) 
          return ErrorPolicy::handle_error(interpreter, typeid(void));
#endif

      value_wrapper_traits<ValueWrapper>::unwrap(interpreter, value_wrapper);

      detail::stack_pop pop(interpreter, 1);

      typedef typename detail::find_conversion_policy<
          0
        , Policies
      >::type converter_generator;

      typename mpl::apply_wrap2<converter_generator, T, lua_to_cpp>::type cv;

#ifndef LUABIND_NO_ERROR_CHECKING
      if (cv.match(interpreter, LUABIND_DECORATE_TYPE(T), -1) < 0)
      {
          return ErrorPolicy::handle_error(interpreter, typeid(T));
      }
#endif

      return cv.apply(interpreter, LUABIND_DECORATE_TYPE(T), -1);
  }

# ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable:4702) // unreachable code
# endif

  template<class T>
  struct throw_error_policy
  {
      static T handle_error(lua_State* interpreter, type_id const& type_info)
      {
#ifndef LUABIND_NO_EXCEPTIONS
          throw cast_failed(interpreter, type_info);
#else
          cast_failed_callback_fun e = get_cast_failed_callback();
          if (e) e(interpreter, type_info);

          assert(0 && "object_cast failed. If you want to handle this error use "
              "luabind::set_error_callback()");
          std::terminate();
#endif
          return *(typename boost::remove_reference<T>::type*)0;
      }
  };

# ifdef BOOST_MSVC
#  pragma warning(pop)
# endif

  template<class T>
  struct nothrow_error_policy
  {
      static boost::optional<T> handle_error(lua_State*, type_id const&)
      {
          return boost::optional<T>();
      }
  };

} // namespace detail

template<class T, class ValueWrapper>
T object_cast(ValueWrapper const& value_wrapper)
{
    return detail::object_cast_aux(
        value_wrapper
      , (T*)0
      , (detail::null_type*)0
      , (detail::throw_error_policy<T>*)0
      , (T*)0
    );
}

template<class T, class ValueWrapper, class Policies>
T object_cast(ValueWrapper const& value_wrapper, Policies const&)
{
    return detail::object_cast_aux(
        value_wrapper
      , (T*)0
      , (Policies*)0
      , (detail::throw_error_policy<T>*)0
      , (T*)0
    );
}

template<class T, class ValueWrapper>
boost::optional<T> object_cast_nothrow(ValueWrapper const& value_wrapper)
{
    return detail::object_cast_aux(
        value_wrapper
      , (T*)0
      , (detail::null_type*)0
      , (detail::nothrow_error_policy<T>*)0
      , (boost::optional<T>*)0
    );
}

template<class T, class ValueWrapper, class Policies>
boost::optional<T> object_cast_nothrow(ValueWrapper const& value_wrapper, Policies const&)
{
    return detail::object_cast_aux(
        value_wrapper
      , (T*)0
      , (Policies*)0
      , (detail::nothrow_error_policy<T>*)0
      , (boost::optional<T>*)0
    );
}

namespace detail
{

  template<int Index>
  struct push_args_from_tuple
  {
      template<class H, class T, class Policies>
      inline static void apply(lua_State* L, const boost::tuples::cons<H, T>& x, const Policies& p) 
      {
          convert_to_lua_p<Index>(L, *x.get_head(), p);
          push_args_from_tuple<Index+1>::apply(L, x.get_tail(), p);
      }

      template<class H, class T>
      inline static void apply(lua_State* L, const boost::tuples::cons<H, T>& x) 
      {
          convert_to_lua(L, *x.get_head());
          push_args_from_tuple<Index+1>::apply(L, x.get_tail());
      }

      template<class Policies>
      inline static void apply(lua_State*, const boost::tuples::null_type&, const Policies&) {}

      inline static void apply(lua_State*, const boost::tuples::null_type&) {}
  };

} // namespace detail

namespace adl
{

  template<class ValueWrapper, class Arguments>
  struct call_proxy
  {
      call_proxy(ValueWrapper& value_wrapper, Arguments arguments)
        : value_wrapper(&value_wrapper)
        , arguments(arguments)
      {}

      call_proxy(call_proxy const& other)
        : value_wrapper(other.value_wrapper)
        , arguments(other.arguments)
      {
          other.value_wrapper = 0;
      }

      ~call_proxy()
      {
          if (value_wrapper)
              call((detail::null_type*)0);
      }

      operator object()
      {
          return call((detail::null_type*)0);
      }

      template<class Policies>
      object operator[](Policies const&)
      {
          return call((Policies*)0);
      }

      template<class Policies>
      object call(Policies*)
      {
          lua_State* interpreter = value_wrapper_traits<ValueWrapper>::interpreter(
              *value_wrapper
          );

          value_wrapper_traits<ValueWrapper>::unwrap(
              interpreter
            , *value_wrapper
          );

          value_wrapper = 0;

          detail::push_args_from_tuple<1>::apply(interpreter, arguments, Policies());

          if (detail::pcall(interpreter, boost::tuples::length<Arguments>::value, 1))
          {
#ifndef LUABIND_NO_EXCEPTIONS
              throw luabind::error(interpreter);
#else
              error_callback_fun e = get_error_callback();
              if (e) e(interpreter);

              assert(0 && "the lua function threw an error and exceptions are disabled."
                  "if you want to handle this error use luabind::set_error_callback()");
              std::terminate();
#endif
          }

          detail::stack_pop pop(interpreter, 1);
          return object(from_stack(interpreter, -1));
      }

      mutable ValueWrapper* value_wrapper;
      Arguments arguments;
  };

  template<class Derived>
  call_proxy<Derived, boost::tuples::tuple<> >
  object_interface<Derived>::operator()()
  {
      return call_proxy<Derived, boost::tuples::tuple<> >(
          derived()
        , boost::tuples::tuple<>()
      );
  }

  // Simple value_wrapper adaptor with the sole purpose of helping with
  // overload resolution. Use this as a function parameter type instead
  // of "object" or "argument" to restrict the parameter to Lua tables.
  template <class Base = object>
  struct table : Base
  {
      table(from_stack const& stack_reference)
        : Base(stack_reference)
      {}
  };

} // namespace adl

using adl::table;

template <class Base>
struct value_wrapper_traits<adl::table<Base> >
  : value_wrapper_traits<Base>
{
    static bool check(lua_State* L, int idx)
    {
        return value_wrapper_traits<Base>::check(L, idx) &&
            lua_istable(L, idx);
    }
};

inline object newtable(lua_State* interpreter)
{
    lua_newtable(interpreter);
    detail::stack_pop pop(interpreter, 1);
    return object(from_stack(interpreter, -1));
}

// this could be optimized by returning a proxy
inline object globals(lua_State* interpreter)
{
    lua_pushvalue(interpreter, LUA_GLOBALSINDEX);
    detail::stack_pop pop(interpreter, 1);
    return object(from_stack(interpreter, -1));
}

// this could be optimized by returning a proxy
inline object registry(lua_State* interpreter)
{
    lua_pushvalue(interpreter, LUA_REGISTRYINDEX);
    detail::stack_pop pop(interpreter, 1);
    return object(from_stack(interpreter, -1));
}

template<class ValueWrapper, class K>
inline object gettable(ValueWrapper const& table, K const& key)
{
    lua_State* interpreter = value_wrapper_traits<ValueWrapper>::interpreter(
        table
    );

    value_wrapper_traits<ValueWrapper>::unwrap(interpreter, table);
    detail::stack_pop pop(interpreter, 2);
    detail::push(interpreter, key);
    lua_gettable(interpreter, -2);
    return object(from_stack(interpreter, -1));
}

template<class ValueWrapper, class K, class T>
inline void settable(ValueWrapper const& table, K const& key, T const& value)
{
    lua_State* interpreter = value_wrapper_traits<ValueWrapper>::interpreter(
        table
    );

    // TODO: Exception safe?
    
    value_wrapper_traits<ValueWrapper>::unwrap(interpreter, table);
    detail::stack_pop pop(interpreter, 1);
    detail::push(interpreter, key);
    detail::push(interpreter, value);
    lua_settable(interpreter, -3);
}

template<class ValueWrapper, class K>
inline object rawget(ValueWrapper const& table, K const& key)
{
    lua_State* interpreter = value_wrapper_traits<ValueWrapper>::interpreter(
        table
    );

    value_wrapper_traits<ValueWrapper>::unwrap(interpreter, table);
    detail::stack_pop pop(interpreter, 2);
    detail::push(interpreter, key);
    lua_rawget(interpreter, -2);
    return object(from_stack(interpreter, -1));
}

template<class ValueWrapper, class K, class T>
inline void rawset(ValueWrapper const& table, K const& key, T const& value)
{
    lua_State* interpreter = value_wrapper_traits<ValueWrapper>::interpreter(
        table
    );

    // TODO: Exception safe?
    
    value_wrapper_traits<ValueWrapper>::unwrap(interpreter, table);
    detail::stack_pop pop(interpreter, 1);
    detail::push(interpreter, key);
    detail::push(interpreter, value);
    lua_rawset(interpreter, -3);
}

template<class ValueWrapper>
inline int type(ValueWrapper const& value)
{
    lua_State* interpreter = value_wrapper_traits<ValueWrapper>::interpreter(
        value
    );

    value_wrapper_traits<ValueWrapper>::unwrap(interpreter, value);
    detail::stack_pop pop(interpreter, 1);
    return lua_type(interpreter, -1);
}

template <class ValueWrapper>
inline object getmetatable(ValueWrapper const& obj)
{
    lua_State* interpreter = value_wrapper_traits<ValueWrapper>::interpreter(
        obj
    );

    value_wrapper_traits<ValueWrapper>::unwrap(interpreter, obj);
    detail::stack_pop pop(interpreter, 2);
    lua_getmetatable(interpreter, -1);
    return object(from_stack(interpreter, -1));
}

template <class ValueWrapper1, class ValueWrapper2>
inline void setmetatable(
    ValueWrapper1 const& obj, ValueWrapper2 const& metatable)
{
    lua_State* interpreter = value_wrapper_traits<ValueWrapper1>::interpreter(
        obj
    );

    value_wrapper_traits<ValueWrapper1>::unwrap(interpreter, obj);
    detail::stack_pop pop(interpreter, 1);
    value_wrapper_traits<ValueWrapper2>::unwrap(interpreter, metatable);
    lua_setmetatable(interpreter, -2);
}

template <class ValueWrapper>
inline lua_CFunction tocfunction(ValueWrapper const& value)
{
    lua_State* interpreter = value_wrapper_traits<ValueWrapper>::interpreter(
        value
    );

    value_wrapper_traits<ValueWrapper>::unwrap(interpreter, value);
    detail::stack_pop pop(interpreter, 1);
    return lua_tocfunction(interpreter, -1);
}

template <class T, class ValueWrapper>
inline T* touserdata(ValueWrapper const& value)
{
    lua_State* interpreter = value_wrapper_traits<ValueWrapper>::interpreter(
        value
    );

    value_wrapper_traits<ValueWrapper>::unwrap(interpreter, value);
    detail::stack_pop pop(interpreter, 1);
    return static_cast<T*>(lua_touserdata(interpreter, -1));
}

template <class ValueWrapper>
inline object getupvalue(ValueWrapper const& value, int index)
{
    lua_State* interpreter = value_wrapper_traits<ValueWrapper>::interpreter(
        value
    );

    value_wrapper_traits<ValueWrapper>::unwrap(interpreter, value);
    detail::stack_pop pop(interpreter, 2);
    lua_getupvalue(interpreter, -1, index);
    return object(from_stack(interpreter, -1));
}

template <class ValueWrapper1, class ValueWrapper2>
inline void setupvalue(
    ValueWrapper1 const& function, int index, ValueWrapper2 const& value)
{
    lua_State* interpreter = value_wrapper_traits<ValueWrapper1>::interpreter(
        function
    );

    value_wrapper_traits<ValueWrapper1>::unwrap(interpreter, function);
    detail::stack_pop pop(interpreter, 1);
    value_wrapper_traits<ValueWrapper2>::unwrap(interpreter, value);
    lua_setupvalue(interpreter, -2, index);
}

template <class GetValueWrapper>
object property(GetValueWrapper const& get)
{
    lua_State* interpreter = value_wrapper_traits<GetValueWrapper>::interpreter(
        get
    );

    value_wrapper_traits<GetValueWrapper>::unwrap(interpreter, get);
    lua_pushnil(interpreter);

    lua_pushcclosure(interpreter, &detail::property_tag, 2);
    detail::stack_pop pop(interpreter, 1);

    return object(from_stack(interpreter, -1));
}

template <class GetValueWrapper, class SetValueWrapper>
object property(GetValueWrapper const& get, SetValueWrapper const& set)
{
    lua_State* interpreter = value_wrapper_traits<GetValueWrapper>::interpreter(
        get
    );

    value_wrapper_traits<GetValueWrapper>::unwrap(interpreter, get);
    value_wrapper_traits<SetValueWrapper>::unwrap(interpreter, set);

    lua_pushcclosure(interpreter, &detail::property_tag, 2);
    detail::stack_pop pop(interpreter, 1);

    return object(from_stack(interpreter, -1));

}


} // namespace luabind

#endif // LUABIND_OBJECT_050419_HPP

