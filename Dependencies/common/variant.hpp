#ifndef MAPBOX_UTIL_VARIANT_HPP
#define MAPBOX_UTIL_VARIANT_HPP

#include <cassert>
#include <cstddef>   // size_t
#include <new>       // operator new
#include <stdexcept> // runtime_error
#include <string>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <utility>

// clang-format off
// [[deprecated]] is only available in C++14, use this for the time being
#if __cplusplus <= 201103L
# ifdef __GNUC__
#  define MAPBOX_VARIANT_DEPRECATED __attribute__((deprecated))
# elif defined(_MSC_VER)
#  define MAPBOX_VARIANT_DEPRECATED __declspec(deprecated)
# else
#  define MAPBOX_VARIANT_DEPRECATED
# endif
#else
#  define MAPBOX_VARIANT_DEPRECATED [[deprecated]]
#endif


#ifdef _MSC_VER
 // https://msdn.microsoft.com/en-us/library/bw1hbe6y.aspx
 #ifdef NDEBUG
  #define VARIANT_INLINE __forceinline
 #else
  #define VARIANT_INLINE __declspec(noinline)
 #endif
#else
 #ifdef NDEBUG
  #define VARIANT_INLINE inline __attribute__((always_inline))
 #else
  #define VARIANT_INLINE __attribute__((noinline))
 #endif
#endif
// clang-format on

#define VARIANT_MAJOR_VERSION 1
#define VARIANT_MINOR_VERSION 1
#define VARIANT_PATCH_VERSION 0

#define VARIANT_VERSION (VARIANT_MAJOR_VERSION * 100000) + (VARIANT_MINOR_VERSION * 100) + (VARIANT_PATCH_VERSION)

namespace mapbox {
namespace util {

template <typename T>
class recursive_wrapper
{

    T* p_;

    void assign(T const& rhs)
    {
        this->get() = rhs;
    }

  public:
    using type = T;

    /**
     * Default constructor default initializes the internally stored value.
     * For POD types this means nothing is done and the storage is
     * uninitialized.
     *
     * @throws std::bad_alloc if there is insufficient memory for an object
     *         of type T.
     * @throws any exception thrown by the default constructur of T.
     */
    recursive_wrapper()
        : p_(new T){};

    ~recursive_wrapper() noexcept { delete p_; };

    recursive_wrapper(recursive_wrapper const& operand)
        : p_(new T(operand.get())) {}

    recursive_wrapper(T const& operand)
        : p_(new T(operand)) {}

    recursive_wrapper(recursive_wrapper&& operand)
        : p_(new T(std::move(operand.get()))) {}

    recursive_wrapper(T&& operand)
        : p_(new T(std::move(operand))) {}

    inline recursive_wrapper& operator=(recursive_wrapper const& rhs)
    {
        assign(rhs.get());
        return *this;
    }

    inline recursive_wrapper& operator=(T const& rhs)
    {
        assign(rhs);
        return *this;
    }

    inline void swap(recursive_wrapper& operand) noexcept
    {
        T* temp = operand.p_;
        operand.p_ = p_;
        p_ = temp;
    }

    recursive_wrapper& operator=(recursive_wrapper&& rhs) noexcept
    {
        swap(rhs);
        return *this;
    }

    recursive_wrapper& operator=(T&& rhs)
    {
        get() = std::move(rhs);
        return *this;
    }

    T& get()
    {
        assert(p_);
        return *get_pointer();
    }

    T const& get() const
    {
        assert(p_);
        return *get_pointer();
    }

    T* get_pointer() { return p_; }

    const T* get_pointer() const { return p_; }

    operator T const&() const { return this->get(); }

    operator T&() { return this->get(); }

}; // class recursive_wrapper

template <typename T>
inline void swap(recursive_wrapper<T>& lhs, recursive_wrapper<T>& rhs) noexcept
{
    lhs.swap(rhs);
}

// XXX This should derive from std::logic_error instead of std::runtime_error.
//     See https://github.com/mapbox/variant/issues/48 for details.
class bad_variant_access : public std::runtime_error
{

  public:
    explicit bad_variant_access(const std::string& what_arg)
        : runtime_error(what_arg) {}

    explicit bad_variant_access(const char* what_arg)
        : runtime_error(what_arg) {}

}; // class bad_variant_access

template <typename R = void>
struct MAPBOX_VARIANT_DEPRECATED static_visitor
{
    using result_type = R;

  protected:
    static_visitor() {}
    ~static_visitor() {}
};

namespace detail {

static constexpr std::size_t invalid_value = std::size_t(-1);

template <typename T, typename... Types>
struct direct_type;

template <typename T, typename First, typename... Types>
struct direct_type<T, First, Types...>
{
    static constexpr std::size_t index = std::is_same<T, First>::value
                                             ? sizeof...(Types)
                                             : direct_type<T, Types...>::index;
};

template <typename T>
struct direct_type<T>
{
    static constexpr std::size_t index = invalid_value;
};

template <typename T, typename... Types>
struct convertible_type;

template <typename T, typename First, typename... Types>
struct convertible_type<T, First, Types...>
{
    static constexpr std::size_t index = std::is_convertible<T, First>::value
                                             ? sizeof...(Types)
                                             : convertible_type<T, Types...>::index;
};

template <typename T>
struct convertible_type<T>
{
    static constexpr std::size_t index = invalid_value;
};

template <typename T, typename... Types>
struct value_traits
{
    using value_type = typename std::remove_reference<T>::type;
    static constexpr std::size_t direct_index = direct_type<value_type, Types...>::index;
    static constexpr bool is_direct = direct_index != invalid_value;
    static constexpr std::size_t index = is_direct ? direct_index : convertible_type<value_type, Types...>::index;
    static constexpr bool is_valid = index != invalid_value;
    static constexpr std::size_t tindex = is_valid ? sizeof...(Types)-index : 0;
    using target_type = typename std::tuple_element<tindex, std::tuple<void, Types...>>::type;
};

// check if T is in Types...
template <typename T, typename... Types>
struct has_type;

template <typename T, typename First, typename... Types>
struct has_type<T, First, Types...>
{
    static constexpr bool value = std::is_same<T, First>::value || has_type<T, Types...>::value;
};

template <typename T>
struct has_type<T> : std::false_type
{
};

template <typename T, typename... Types>
struct is_valid_type;

template <typename T, typename First, typename... Types>
struct is_valid_type<T, First, Types...>
{
    static constexpr bool value = std::is_convertible<T, First>::value || is_valid_type<T, Types...>::value;
};

template <typename T>
struct is_valid_type<T> : std::false_type
{
};

template <typename T, typename R = void>
struct enable_if_type
{
    using type = R;
};

template <typename F, typename V, typename Enable = void>
struct result_of_unary_visit
{
    using type = typename std::result_of<F(V&)>::type;
};

template <typename F, typename V>
struct result_of_unary_visit<F, V, typename enable_if_type<typename F::result_type>::type>
{
    using type = typename F::result_type;
};

template <typename F, typename V, typename Enable = void>
struct result_of_binary_visit
{
    using type = typename std::result_of<F(V&, V&)>::type;
};

template <typename F, typename V>
struct result_of_binary_visit<F, V, typename enable_if_type<typename F::result_type>::type>
{
    using type = typename F::result_type;
};

template <std::size_t arg1, std::size_t... others>
struct static_max;

template <std::size_t arg>
struct static_max<arg>
{
    static const std::size_t value = arg;
};

template <std::size_t arg1, std::size_t arg2, std::size_t... others>
struct static_max<arg1, arg2, others...>
{
    static const std::size_t value = arg1 >= arg2 ? static_max<arg1, others...>::value : static_max<arg2, others...>::value;
};

template <typename... Types>
struct variant_helper;

template <typename T, typename... Types>
struct variant_helper<T, Types...>
{
    VARIANT_INLINE static void destroy(const std::size_t type_index, void* data)
    {
        if (type_index == sizeof...(Types))
        {
            reinterpret_cast<T*>(data)->~T();
        }
        else
        {
            variant_helper<Types...>::destroy(type_index, data);
        }
    }

    VARIANT_INLINE static void move(const std::size_t old_type_index, void* old_value, void* new_value)
    {
        if (old_type_index == sizeof...(Types))
        {
            new (new_value) T(std::move(*reinterpret_cast<T*>(old_value)));
        }
        else
        {
            variant_helper<Types...>::move(old_type_index, old_value, new_value);
        }
    }

    VARIANT_INLINE static void copy(const std::size_t old_type_index, const void* old_value, void* new_value)
    {
        if (old_type_index == sizeof...(Types))
        {
            new (new_value) T(*reinterpret_cast<const T*>(old_value));
        }
        else
        {
            variant_helper<Types...>::copy(old_type_index, old_value, new_value);
        }
    }
};

template <>
struct variant_helper<>
{
    VARIANT_INLINE static void destroy(const std::size_t, void*) {}
    VARIANT_INLINE static void move(const std::size_t, void*, void*) {}
    VARIANT_INLINE static void copy(const std::size_t, const void*, void*) {}
};

template <typename T>
struct unwrapper
{
    static T const& apply_const(T const& obj) { return obj; }
    static T& apply(T& obj) { return obj; }
};

template <typename T>
struct unwrapper<recursive_wrapper<T>>
{
    static auto apply_const(recursive_wrapper<T> const& obj)
        -> typename recursive_wrapper<T>::type const&
    {
        return obj.get();
    }
    static auto apply(recursive_wrapper<T>& obj)
        -> typename recursive_wrapper<T>::type&
    {
        return obj.get();
    }
};

template <typename T>
struct unwrapper<std::reference_wrapper<T>>
{
    static auto apply_const(std::reference_wrapper<T> const& obj)
        -> typename std::reference_wrapper<T>::type const&
    {
        return obj.get();
    }
    static auto apply(std::reference_wrapper<T>& obj)
        -> typename std::reference_wrapper<T>::type&
    {
        return obj.get();
    }
};

template <typename F, typename V, typename R, typename... Types>
struct dispatcher;

template <typename F, typename V, typename R, typename T, typename... Types>
struct dispatcher<F, V, R, T, Types...>
{
    VARIANT_INLINE static R apply_const(V const& v, F&& f)
    {
        if (v.template is<T>())
        {
            return f(unwrapper<T>::apply_const(v.template get<T>()));
        }
        else
        {
            return dispatcher<F, V, R, Types...>::apply_const(v, std::forward<F>(f));
        }
    }

    VARIANT_INLINE static R apply(V& v, F&& f)
    {
        if (v.template is<T>())
        {
            return f(unwrapper<T>::apply(v.template get<T>()));
        }
        else
        {
            return dispatcher<F, V, R, Types...>::apply(v, std::forward<F>(f));
        }
    }
};

template <typename F, typename V, typename R, typename T>
struct dispatcher<F, V, R, T>
{
    VARIANT_INLINE static R apply_const(V const& v, F&& f)
    {
        return f(unwrapper<T>::apply_const(v.template get<T>()));
    }

    VARIANT_INLINE static R apply(V& v, F&& f)
    {
        return f(unwrapper<T>::apply(v.template get<T>()));
    }
};

template <typename F, typename V, typename R, typename T, typename... Types>
struct binary_dispatcher_rhs;

template <typename F, typename V, typename R, typename T0, typename T1, typename... Types>
struct binary_dispatcher_rhs<F, V, R, T0, T1, Types...>
{
    VARIANT_INLINE static R apply_const(V const& lhs, V const& rhs, F&& f)
    {
        if (rhs.template is<T1>()) // call binary functor
        {
            return f(unwrapper<T0>::apply_const(lhs.template get<T0>()),
                     unwrapper<T1>::apply_const(rhs.template get<T1>()));
        }
        else
        {
            return binary_dispatcher_rhs<F, V, R, T0, Types...>::apply_const(lhs, rhs, std::forward<F>(f));
        }
    }

    VARIANT_INLINE static R apply(V& lhs, V& rhs, F&& f)
    {
        if (rhs.template is<T1>()) // call binary functor
        {
            return f(unwrapper<T0>::apply(lhs.template get<T0>()),
                     unwrapper<T1>::apply(rhs.template get<T1>()));
        }
        else
        {
            return binary_dispatcher_rhs<F, V, R, T0, Types...>::apply(lhs, rhs, std::forward<F>(f));
        }
    }
};

template <typename F, typename V, typename R, typename T0, typename T1>
struct binary_dispatcher_rhs<F, V, R, T0, T1>
{
    VARIANT_INLINE static R apply_const(V const& lhs, V const& rhs, F&& f)
    {
        return f(unwrapper<T0>::apply_const(lhs.template get<T0>()),
                 unwrapper<T1>::apply_const(rhs.template get<T1>()));
    }

    VARIANT_INLINE static R apply(V& lhs, V& rhs, F&& f)
    {
        return f(unwrapper<T0>::apply(lhs.template get<T0>()),
                 unwrapper<T1>::apply(rhs.template get<T1>()));
    }
};

template <typename F, typename V, typename R, typename T, typename... Types>
struct binary_dispatcher_lhs;

template <typename F, typename V, typename R, typename T0, typename T1, typename... Types>
struct binary_dispatcher_lhs<F, V, R, T0, T1, Types...>
{
    VARIANT_INLINE static R apply_const(V const& lhs, V const& rhs, F&& f)
    {
        if (lhs.template is<T1>()) // call binary functor
        {
            return f(unwrapper<T1>::apply_const(lhs.template get<T1>()),
                     unwrapper<T0>::apply_const(rhs.template get<T0>()));
        }
        else
        {
            return binary_dispatcher_lhs<F, V, R, T0, Types...>::apply_const(lhs, rhs, std::forward<F>(f));
        }
    }

    VARIANT_INLINE static R apply(V& lhs, V& rhs, F&& f)
    {
        if (lhs.template is<T1>()) // call binary functor
        {
            return f(unwrapper<T1>::apply(lhs.template get<T1>()),
                     unwrapper<T0>::apply(rhs.template get<T0>()));
        }
        else
        {
            return binary_dispatcher_lhs<F, V, R, T0, Types...>::apply(lhs, rhs, std::forward<F>(f));
        }
    }
};

template <typename F, typename V, typename R, typename T0, typename T1>
struct binary_dispatcher_lhs<F, V, R, T0, T1>
{
    VARIANT_INLINE static R apply_const(V const& lhs, V const& rhs, F&& f)
    {
        return f(unwrapper<T1>::apply_const(lhs.template get<T1>()),
                 unwrapper<T0>::apply_const(rhs.template get<T0>()));
    }

    VARIANT_INLINE static R apply(V& lhs, V& rhs, F&& f)
    {
        return f(unwrapper<T1>::apply(lhs.template get<T1>()),
                 unwrapper<T0>::apply(rhs.template get<T0>()));
    }
};

template <typename F, typename V, typename R, typename... Types>
struct binary_dispatcher;

template <typename F, typename V, typename R, typename T, typename... Types>
struct binary_dispatcher<F, V, R, T, Types...>
{
    VARIANT_INLINE static R apply_const(V const& v0, V const& v1, F&& f)
    {
        if (v0.template is<T>())
        {
            if (v1.template is<T>())
            {
                return f(unwrapper<T>::apply_const(v0.template get<T>()),
                         unwrapper<T>::apply_const(v1.template get<T>())); // call binary functor
            }
            else
            {
                return binary_dispatcher_rhs<F, V, R, T, Types...>::apply_const(v0, v1, std::forward<F>(f));
            }
        }
        else if (v1.template is<T>())
        {
            return binary_dispatcher_lhs<F, V, R, T, Types...>::apply_const(v0, v1, std::forward<F>(f));
        }
        return binary_dispatcher<F, V, R, Types...>::apply_const(v0, v1, std::forward<F>(f));
    }

    VARIANT_INLINE static R apply(V& v0, V& v1, F&& f)
    {
        if (v0.template is<T>())
        {
            if (v1.template is<T>())
            {
                return f(unwrapper<T>::apply(v0.template get<T>()),
                         unwrapper<T>::apply(v1.template get<T>())); // call binary functor
            }
            else
            {
                return binary_dispatcher_rhs<F, V, R, T, Types...>::apply(v0, v1, std::forward<F>(f));
            }
        }
        else if (v1.template is<T>())
        {
            return binary_dispatcher_lhs<F, V, R, T, Types...>::apply(v0, v1, std::forward<F>(f));
        }
        return binary_dispatcher<F, V, R, Types...>::apply(v0, v1, std::forward<F>(f));
    }
};

template <typename F, typename V, typename R, typename T>
struct binary_dispatcher<F, V, R, T>
{
    VARIANT_INLINE static R apply_const(V const& v0, V const& v1, F&& f)
    {
        return f(unwrapper<T>::apply_const(v0.template get<T>()),
                 unwrapper<T>::apply_const(v1.template get<T>())); // call binary functor
    }

    VARIANT_INLINE static R apply(V& v0, V& v1, F&& f)
    {
        return f(unwrapper<T>::apply(v0.template get<T>()),
                 unwrapper<T>::apply(v1.template get<T>())); // call binary functor
    }
};

// comparator functors
struct equal_comp
{
    template <typename T>
    bool operator()(T const& lhs, T const& rhs) const
    {
        return lhs == rhs;
    }
};

struct less_comp
{
    template <typename T>
    bool operator()(T const& lhs, T const& rhs) const
    {
        return lhs < rhs;
    }
};

template <typename Variant, typename Comp>
class comparer
{
  public:
    explicit comparer(Variant const& lhs) noexcept
        : lhs_(lhs) {}
    comparer& operator=(comparer const&) = delete;
    // visitor
    template <typename T>
    bool operator()(T const& rhs_content) const
    {
        T const& lhs_content = lhs_.template get<T>();
        return Comp()(lhs_content, rhs_content);
    }

  private:
    Variant const& lhs_;
};

// True if Predicate matches for all of the types Ts
template <template <typename> class Predicate, typename... Ts>
struct static_all_of : std::is_same<std::tuple<std::true_type, typename Predicate<Ts>::type...>,
                                    std::tuple<typename Predicate<Ts>::type..., std::true_type>>
{
};

// True if Predicate matches for none of the types Ts
template <template <typename> class Predicate, typename... Ts>
struct static_none_of : std::is_same<std::tuple<std::false_type, typename Predicate<Ts>::type...>,
                                     std::tuple<typename Predicate<Ts>::type..., std::false_type>>
{
};

} // namespace detail

struct no_init
{
};

template <typename... Types>
class variant
{
    static_assert(sizeof...(Types) > 0, "Template parameter type list of variant can not be empty");
    static_assert(detail::static_none_of<std::is_reference, Types...>::value, "Variant can not hold reference types. Maybe use std::reference?");

  private:
    static const std::size_t data_size = detail::static_max<sizeof(Types)...>::value;
    static const std::size_t data_align = detail::static_max<alignof(Types)...>::value;

    using first_type = typename std::tuple_element<0, std::tuple<Types...>>::type;
    using data_type = typename std::aligned_storage<data_size, data_align>::type;
    using helper_type = detail::variant_helper<Types...>;

    std::size_t type_index;
    data_type data;

  public:
    VARIANT_INLINE variant() noexcept(std::is_nothrow_default_constructible<first_type>::value)
        : type_index(sizeof...(Types)-1)
    {
        static_assert(std::is_default_constructible<first_type>::value, "First type in variant must be default constructible to allow default construction of variant");
        new (&data) first_type();
    }

    VARIANT_INLINE variant(no_init) noexcept
        : type_index(detail::invalid_value) {}

    // http://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers
    template <typename T, typename Traits = detail::value_traits<T, Types...>,
              typename Enable = typename std::enable_if<Traits::is_valid>::type>
    VARIANT_INLINE variant(T&& val) noexcept(std::is_nothrow_constructible<typename Traits::target_type, T&&>::value)
        : type_index(Traits::index)
    {
        new (&data) typename Traits::target_type(std::forward<T>(val));
    }

    VARIANT_INLINE variant(variant<Types...> const& old)
        : type_index(old.type_index)
    {
        helper_type::copy(old.type_index, &old.data, &data);
    }

    VARIANT_INLINE variant(variant<Types...>&& old) noexcept(std::is_nothrow_move_constructible<std::tuple<Types...>>::value)
        : type_index(old.type_index)
    {
        helper_type::move(old.type_index, &old.data, &data);
    }

  private:
    VARIANT_INLINE void copy_assign(variant<Types...> const& rhs)
    {
        helper_type::destroy(type_index, &data);
        type_index = detail::invalid_value;
        helper_type::copy(rhs.type_index, &rhs.data, &data);
        type_index = rhs.type_index;
    }

    VARIANT_INLINE void move_assign(variant<Types...>&& rhs)
    {
        helper_type::destroy(type_index, &data);
        type_index = detail::invalid_value;
        helper_type::move(rhs.type_index, &rhs.data, &data);
        type_index = rhs.type_index;
    }

  public:
    VARIANT_INLINE variant<Types...>& operator=(variant<Types...>&& other)
    {
        move_assign(std::move(other));
        return *this;
    }

    VARIANT_INLINE variant<Types...>& operator=(variant<Types...> const& other)
    {
        copy_assign(other);
        return *this;
    }

    // conversions
    // move-assign
    template <typename T>
    VARIANT_INLINE variant<Types...>& operator=(T&& rhs) noexcept
    {
        variant<Types...> temp(std::forward<T>(rhs));
        move_assign(std::move(temp));
        return *this;
    }

    // copy-assign
    template <typename T>
    VARIANT_INLINE variant<Types...>& operator=(T const& rhs)
    {
        variant<Types...> temp(rhs);
        copy_assign(temp);
        return *this;
    }

    template <typename T>
    VARIANT_INLINE bool is() const
    {
        static_assert(detail::has_type<T, Types...>::value, "invalid type in T in `is<T>()` for this variant");
        return type_index == detail::direct_type<T, Types...>::index;
    }

    VARIANT_INLINE bool valid() const
    {
        return type_index != detail::invalid_value;
    }

    template <typename T, typename... Args>
    VARIANT_INLINE void set(Args&&... args)
    {
        helper_type::destroy(type_index, &data);
        type_index = detail::invalid_value;
        new (&data) T(std::forward<Args>(args)...);
        type_index = detail::direct_type<T, Types...>::index;
    }

    // get<T>()
    template <typename T, typename std::enable_if<
                              (detail::direct_type<T, Types...>::index != detail::invalid_value)>::type* = nullptr>
    VARIANT_INLINE T& get()
    {
        if (type_index == detail::direct_type<T, Types...>::index)
        {
            return *reinterpret_cast<T*>(&data);
        }
        else
        {
            throw bad_variant_access("in get<T>()");
        }
    }

    template <typename T, typename std::enable_if<
                              (detail::direct_type<T, Types...>::index != detail::invalid_value)>::type* = nullptr>
    VARIANT_INLINE T const& get() const
    {
        if (type_index == detail::direct_type<T, Types...>::index)
        {
            return *reinterpret_cast<T const*>(&data);
        }
        else
        {
            throw bad_variant_access("in get<T>()");
        }
    }

    // get<T>() - T stored as recursive_wrapper<T>
    template <typename T, typename std::enable_if<
                              (detail::direct_type<recursive_wrapper<T>, Types...>::index != detail::invalid_value)>::type* = nullptr>
    VARIANT_INLINE T& get()
    {
        if (type_index == detail::direct_type<recursive_wrapper<T>, Types...>::index)
        {
            return (*reinterpret_cast<recursive_wrapper<T>*>(&data)).get();
        }
        else
        {
            throw bad_variant_access("in get<T>()");
        }
    }

    template <typename T, typename std::enable_if<
                              (detail::direct_type<recursive_wrapper<T>, Types...>::index != detail::invalid_value)>::type* = nullptr>
    VARIANT_INLINE T const& get() const
    {
        if (type_index == detail::direct_type<recursive_wrapper<T>, Types...>::index)
        {
            return (*reinterpret_cast<recursive_wrapper<T> const*>(&data)).get();
        }
        else
        {
            throw bad_variant_access("in get<T>()");
        }
    }

    // get<T>() - T stored as std::reference_wrapper<T>
    template <typename T, typename std::enable_if<
                              (detail::direct_type<std::reference_wrapper<T>, Types...>::index != detail::invalid_value)>::type* = nullptr>
    VARIANT_INLINE T& get()
    {
        if (type_index == detail::direct_type<std::reference_wrapper<T>, Types...>::index)
        {
            return (*reinterpret_cast<std::reference_wrapper<T>*>(&data)).get();
        }
        else
        {
            throw bad_variant_access("in get<T>()");
        }
    }

    template <typename T, typename std::enable_if<
                              (detail::direct_type<std::reference_wrapper<T const>, Types...>::index != detail::invalid_value)>::type* = nullptr>
    VARIANT_INLINE T const& get() const
    {
        if (type_index == detail::direct_type<std::reference_wrapper<T const>, Types...>::index)
        {
            return (*reinterpret_cast<std::reference_wrapper<T const> const*>(&data)).get();
        }
        else
        {
            throw bad_variant_access("in get<T>()");
        }
    }

    // This function is deprecated because it returns an internal index field.
    // Use which() instead.
    MAPBOX_VARIANT_DEPRECATED VARIANT_INLINE std::size_t get_type_index() const
    {
        return type_index;
    }

    VARIANT_INLINE int which() const noexcept
    {
        return static_cast<int>(sizeof...(Types)-type_index - 1);
    }

    // visitor
    // unary
    template <typename F, typename V, typename R = typename detail::result_of_unary_visit<F, first_type>::type>
    auto VARIANT_INLINE static visit(V const& v, F&& f)
        -> decltype(detail::dispatcher<F, V, R, Types...>::apply_const(v, std::forward<F>(f)))
    {
        return detail::dispatcher<F, V, R, Types...>::apply_const(v, std::forward<F>(f));
    }
    // non-const
    template <typename F, typename V, typename R = typename detail::result_of_unary_visit<F, first_type>::type>
    auto VARIANT_INLINE static visit(V& v, F&& f)
        -> decltype(detail::dispatcher<F, V, R, Types...>::apply(v, std::forward<F>(f)))
    {
        return detail::dispatcher<F, V, R, Types...>::apply(v, std::forward<F>(f));
    }

    // binary
    // const
    template <typename F, typename V, typename R = typename detail::result_of_binary_visit<F, first_type>::type>
    auto VARIANT_INLINE static binary_visit(V const& v0, V const& v1, F&& f)
        -> decltype(detail::binary_dispatcher<F, V, R, Types...>::apply_const(v0, v1, std::forward<F>(f)))
    {
        return detail::binary_dispatcher<F, V, R, Types...>::apply_const(v0, v1, std::forward<F>(f));
    }
    // non-const
    template <typename F, typename V, typename R = typename detail::result_of_binary_visit<F, first_type>::type>
    auto VARIANT_INLINE static binary_visit(V& v0, V& v1, F&& f)
        -> decltype(detail::binary_dispatcher<F, V, R, Types...>::apply(v0, v1, std::forward<F>(f)))
    {
        return detail::binary_dispatcher<F, V, R, Types...>::apply(v0, v1, std::forward<F>(f));
    }

    ~variant() noexcept // no-throw destructor
    {
        helper_type::destroy(type_index, &data);
    }

    // comparison operators
    // equality
    VARIANT_INLINE bool operator==(variant const& rhs) const
    {
        assert(valid() && rhs.valid());
        if (this->which() != rhs.which())
        {
            return false;
        }
        detail::comparer<variant, detail::equal_comp> visitor(*this);
        return visit(rhs, visitor);
    }

    VARIANT_INLINE bool operator!=(variant const& rhs) const
    {
        return !(*this == rhs);
    }

    // less than
    VARIANT_INLINE bool operator<(variant const& rhs) const
    {
        assert(valid() && rhs.valid());
        if (this->which() != rhs.which())
        {
            return this->which() < rhs.which();
        }
        detail::comparer<variant, detail::less_comp> visitor(*this);
        return visit(rhs, visitor);
    }
    VARIANT_INLINE bool operator>(variant const& rhs) const
    {
        return rhs < *this;
    }
    VARIANT_INLINE bool operator<=(variant const& rhs) const
    {
        return !(*this > rhs);
    }
    VARIANT_INLINE bool operator>=(variant const& rhs) const
    {
        return !(*this < rhs);
    }
};

// unary visitor interface
// const
template <typename F, typename V>
auto VARIANT_INLINE apply_visitor(F&& f, V const& v) -> decltype(V::visit(v, std::forward<F>(f)))
{
    return V::visit(v, std::forward<F>(f));
}

// non-const
template <typename F, typename V>
auto VARIANT_INLINE apply_visitor(F&& f, V& v) -> decltype(V::visit(v, std::forward<F>(f)))
{
    return V::visit(v, std::forward<F>(f));
}

// binary visitor interface
// const
template <typename F, typename V>
auto VARIANT_INLINE apply_visitor(F&& f, V const& v0, V const& v1) -> decltype(V::binary_visit(v0, v1, std::forward<F>(f)))
{
    return V::binary_visit(v0, v1, std::forward<F>(f));
}

// non-const
template <typename F, typename V>
auto VARIANT_INLINE apply_visitor(F&& f, V& v0, V& v1) -> decltype(V::binary_visit(v0, v1, std::forward<F>(f)))
{
    return V::binary_visit(v0, v1, std::forward<F>(f));
}

// getter interface
template <typename ResultType, typename T>
ResultType& get(T& var)
{
    return var.template get<ResultType>();
}

template <typename ResultType, typename T>
ResultType const& get(T const& var)
{
    return var.template get<ResultType>();
}
} // namespace util
} // namespace mapbox

#endif // MAPBOX_UTIL_VARIANT_HPP
