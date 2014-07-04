// Copyright (c) 2003 Daniel Wallin and Arvid Norberg

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


#ifndef LUABIND_CLASS_HPP_INCLUDED
#define LUABIND_CLASS_HPP_INCLUDED

/*
	ISSUES:
	------------------------------------------------------

	* solved for member functions, not application operator *
	if we have a base class that defines a function a derived class must be able to
	override that function (not just overload). Right now we just add the other overload
	to the overloads list and will probably get an ambiguity. If we want to support this
	each method_rep must include a vector of type_info pointers for each parameter.
	Operators do not have this problem, since operators always have to have
	it's own type as one of the arguments, no ambiguity can occur. Application
	operator, on the other hand, would have this problem.
	Properties cannot be overloaded, so they should always be overridden.
	If this is to work for application operator, we really need to specify if an application
	operator is const or not.

	If one class registers two functions with the same name and the same
	signature, there's currently no error. The last registered function will
	be the one that's used.
	How do we know which class registered the function? If the function was
	defined by the base class, it is a legal operation, to override it.
	we cannot look at the pointer offset, since it always will be zero for one of the bases.



	TODO:
	------------------------------------------------------

 	finish smart pointer support
		* the adopt policy should not be able to adopt pointers to held_types. This
		must be prohibited.
		* name_of_type must recognize holder_types and not return "custom"

	document custom policies, custom converters

	store the instance object for policies.

	support the __concat metamethod. This is a bit tricky, since it cannot be
	treated as a normal operator. It is a binary operator but we want to use the
	__tostring implementation for both arguments.
	
*/

#include <luabind/prefix.hpp>
#include <luabind/config.hpp>

#include <string>
#include <map>
#include <vector>
#include <cassert>

#include <boost/bind.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_member_object_pointer.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/logical.hpp>

#include <luabind/config.hpp>
#include <luabind/scope.hpp>
#include <luabind/back_reference.hpp>
#include <luabind/function.hpp>
#include <luabind/dependency_policy.hpp>
#include <luabind/detail/constructor.hpp>
#include <luabind/detail/call.hpp>
#include <luabind/detail/deduce_signature.hpp>
#include <luabind/detail/compute_score.hpp>
#include <luabind/detail/primitives.hpp>
#include <luabind/detail/property.hpp>
#include <luabind/detail/typetraits.hpp>
#include <luabind/detail/class_rep.hpp>
#include <luabind/detail/call.hpp>
#include <luabind/detail/object_rep.hpp>
#include <luabind/detail/calc_arity.hpp>
#include <luabind/detail/call_member.hpp>
#include <luabind/detail/enum_maker.hpp>
#include <luabind/detail/operator_id.hpp>
#include <luabind/detail/pointee_typeid.hpp>
#include <luabind/detail/link_compatibility.hpp>
#include <luabind/detail/inheritance.hpp>
#include <luabind/typeid.hpp>

// to remove the 'this' used in initialization list-warning
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4355)
#endif

namespace boost
{

  template <class T> class shared_ptr;

} // namespace boost

namespace luabind
{	
	namespace detail
	{
		struct unspecified {};

		template<class Derived> struct operator_;

		struct you_need_to_define_a_get_const_holder_function_for_your_smart_ptr {};
	}

	template<class T, class X1 = detail::unspecified, class X2 = detail::unspecified, class X3 = detail::unspecified>
	struct class_;

	// TODO: this function will only be invoked if the user hasn't defined a correct overload
	// maybe we should have a static assert in here?
	inline detail::you_need_to_define_a_get_const_holder_function_for_your_smart_ptr*
	get_const_holder(...)
	{
		return 0;
	}

	template <class T>
	boost::shared_ptr<T const>* get_const_holder(boost::shared_ptr<T>*)
	{
		return 0;
	}

    template <
        BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(
            LUABIND_MAX_BASES, class A, detail::null_type)
    >
    struct bases
    {};

    typedef bases<detail::null_type> no_bases;

	namespace detail
	{
        template <class T>
        struct is_bases
          : mpl::false_
        {};

        template <BOOST_PP_ENUM_PARAMS(LUABIND_MAX_BASES, class A)>
        struct is_bases<bases<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_BASES, A)> >
          : mpl::true_
        {};

        template <class T, class P>
        struct is_unspecified
          : mpl::apply1<P, T>
        {};

        template <class P>
        struct is_unspecified<unspecified, P>
          : mpl::true_
        {};

        template <class P>
        struct is_unspecified_mfn
        {
            template <class T>
            struct apply
              : is_unspecified<T, P>
            {};
        };

		template<class Predicate>
		struct get_predicate
		{
            typedef mpl::protect<is_unspecified_mfn<Predicate> > type;
		};

        template <class Result, class Default>
        struct result_or_default
        {
            typedef Result type;
        };

        template <class Default>
        struct result_or_default<unspecified, Default>
        {
            typedef Default type;
        };

		template<class Parameters, class Predicate, class DefaultValue>
		struct extract_parameter
		{
			typedef typename get_predicate<Predicate>::type pred;
			typedef typename boost::mpl::find_if<Parameters, pred>::type iterator;
            typedef typename result_or_default<
                typename iterator::type, DefaultValue
            >::type type;
		};

		// prints the types of the values on the stack, in the
		// range [start_index, lua_gettop()]

		LUABIND_API std::string stack_content_by_name(lua_State* L, int start_index);
	
		struct LUABIND_API create_class
		{
			static int stage1(lua_State* L);
			static int stage2(lua_State* L);
		};

	} // detail

	namespace detail {

		template<class T>
		struct static_scope
		{
			static_scope(T& self_) : self(self_)
			{
			}

			T& operator[](scope s) const
			{
				self.add_inner_scope(s);
				return self;
			}

		private:
			template<class U> void operator,(U const&) const;
			void operator=(static_scope const&);
			
			T& self;
		};

		struct class_registration;

		struct LUABIND_API class_base : scope
		{
		public:
			class_base(char const* name);		

			struct base_desc
			{
				type_id type;
				int ptr_offset;
			};

			void init(
                type_id const& type, class_id id
              , type_id const& wrapped_type, class_id wrapper_id);

            void add_base(type_id const& base, cast_function cast);

			void add_member(registration* member);
			void add_default_member(registration* member);

			const char* name() const;

			void add_static_constant(const char* name, int val);
			void add_inner_scope(scope& s);

            void add_cast(class_id src, class_id target, cast_function cast);

		private:
			class_registration* m_registration;
		};

// MSVC complains about member being sensitive to alignment (C4121)
// when F is a pointer to member of a class with virtual bases.
# ifdef BOOST_MSVC
#  pragma pack(push)
#  pragma pack(16)
# endif

		template <class Class, class F, class Policies>
		struct memfun_registration : registration
		{
			memfun_registration(char const* name, F f, Policies const& policies)
			  : name(name)
			  , f(f)
			  , policies(policies)
			{}

			void register_(lua_State* L) const
			{
				object fn = make_function(
					L, f, deduce_signature(f, (Class*)0), policies);

				add_overload(
					object(from_stack(L, -1))
				  , name
				  , fn
				);
			}

			char const* name;
			F f;
			Policies policies;
		};

# ifdef BOOST_MSVC
#  pragma pack(pop)
# endif

        template <class P, class T>
        struct default_pointer
        {
            typedef P type;
        };

        template <class T>
        struct default_pointer<null_type, T>
        {
            typedef std::auto_ptr<T> type;
        };

        template <class Class, class Pointer, class Signature, class Policies>
        struct constructor_registration : registration
        {
            constructor_registration(Policies const& policies)
              : policies(policies)
            {}

            void register_(lua_State* L) const
            {
                typedef typename default_pointer<Pointer, Class>::type pointer;

                object fn = make_function(
                    L
                  , construct<Class, pointer, Signature>(), Signature()
                  , policies
                );

                add_overload(
                    object(from_stack(L, -1))
                  , "__init"
                  , fn
                );
            }

            Policies policies;
        };

        template <class T>
        struct reference_result
          : mpl::if_<
                mpl::or_<boost::is_pointer<T>, is_primitive<T> >
              , T
              , typename boost::add_reference<T>::type
            >
        {};

        template <class T, class Policies>
        struct inject_dependency_policy
          : mpl::if_<
                is_primitive<T>
              , Policies
              , policy_cons<dependency_policy<0, 1>, Policies>
            >
        {};

        template <
            class Class
          , class Get, class GetPolicies
          , class Set = null_type, class SetPolicies = null_type
        >
        struct property_registration : registration
        {
            property_registration(
                char const* name
              , Get const& get
              , GetPolicies const& get_policies
              , Set const& set = Set()
              , SetPolicies const& set_policies = SetPolicies()
            )
              : name(name)
              , get(get)
              , get_policies(get_policies)
              , set(set)
              , set_policies(set_policies)
            {}

            void register_(lua_State* L) const
            {
                object context(from_stack(L, -1));
                register_aux(
                    L
                  , context
                  , make_get(L, get, boost::is_member_object_pointer<Get>())
                  , set
                );
            }

            template <class F>
            object make_get(lua_State* L, F const& f, mpl::false_) const
            {
                return make_function(
                    L, f, deduce_signature(f, (Class*)0), get_policies);
            }

            template <class T, class D>
            object make_get(lua_State* L, D T::* mem_ptr, mpl::true_) const
            {
                typedef typename reference_result<D>::type result_type;
                typedef typename inject_dependency_policy<
                    D, GetPolicies>::type policies;

                return make_function(
                    L
                  , access_member_ptr<T, D, result_type>(mem_ptr)
                  , mpl::vector2<result_type, Class const&>()
                  , policies()
                );
            }

            template <class F>
            object make_set(lua_State* L, F const& f, mpl::false_) const
            {
                return make_function(
                    L, f, deduce_signature(f, (Class*)0), set_policies);
            }

            template <class T, class D>
            object make_set(lua_State* L, D T::* mem_ptr, mpl::true_) const
            {
                return make_function(
                    L
                  , access_member_ptr<T, D>(mem_ptr)
                  , mpl::vector3<void, Class&, D const&>()
                  , set_policies
                );
            }

            template <class S>
            void register_aux(
                lua_State* L, object const& context
              , object const& get_, S const&) const
            {
                context[name] = property(
                    get_
                  , make_set(L, set, boost::is_member_object_pointer<Set>())
                );
            }

            void register_aux(
                lua_State*, object const& context
              , object const& get_, null_type) const
            {
                context[name] = property(get_);
            }

            char const* name;
            Get get;
            GetPolicies get_policies;
            Set set;
            SetPolicies set_policies;
        };

	} // namespace detail

	// registers a class in the lua environment
	template<class T, class X1, class X2, class X3>
	struct class_: detail::class_base 
	{
		typedef class_<T, X1, X2, X3> self_t;

	private:

		template<class A, class B, class C, class D>
		class_(const class_<A,B,C,D>&);

	public:

        typedef boost::mpl::vector4<X1, X2, X3, detail::unspecified> parameters_type;

		// WrappedType MUST inherit from T
		typedef typename detail::extract_parameter<
		    parameters_type
		  , boost::is_base_and_derived<T, boost::mpl::_>
		  , detail::null_type
		>::type WrappedType;

		typedef typename detail::extract_parameter<
		    parameters_type
		  , boost::mpl::not_<
		        boost::mpl::or_<
                    detail::is_bases<boost::mpl::_>
                  , boost::is_base_and_derived<boost::mpl::_, T>
                  , boost::is_base_and_derived<T, boost::mpl::_>
				>
			>
		  , detail::null_type
		>::type HeldType;

        template <class Src, class Target>
        void add_downcast(Src*, Target*, boost::mpl::true_)
        {
            add_cast(
                detail::registered_class<Src>::id
              , detail::registered_class<Target>::id
              , detail::dynamic_cast_<Src, Target>::execute
            );
        }

        template <class Src, class Target>
        void add_downcast(Src*, Target*, boost::mpl::false_)
        {}

		// this function generates conversion information
		// in the given class_rep structure. It will be able
		// to implicitly cast to the given template type
		template<class To>
		void gen_base_info(detail::type_<To>)
		{
            add_base(typeid(To), detail::static_cast_<T, To>::execute);
            add_cast(
                detail::registered_class<T>::id
              , detail::registered_class<To>::id
              , detail::static_cast_<T, To>::execute
            );

            add_downcast((To*)0, (T*)0, boost::is_polymorphic<To>());
		}

		void gen_base_info(detail::type_<detail::null_type>)
		{}

#define LUABIND_GEN_BASE_INFO(z, n, text) gen_base_info(detail::type_<BaseClass##n>());

		template<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_BASES, class BaseClass)>
		void generate_baseclass_list(detail::type_<bases<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_BASES, BaseClass)> >)
		{
			BOOST_PP_REPEAT(LUABIND_MAX_BASES, LUABIND_GEN_BASE_INFO, _)
		}

#undef LUABIND_GEN_BASE_INFO

		class_(const char* name): class_base(name), scope(*this)
		{
#ifndef NDEBUG
			detail::check_link_compatibility();
#endif
		   	init(); 
		}

		template<class F>
		class_& def(const char* name, F f)
		{
			return this->virtual_def(
				name, f, detail::null_type()
			  , detail::null_type(), boost::mpl::true_());
		}

		// virtual functions
		template<class F, class DefaultOrPolicies>
		class_& def(char const* name, F fn, DefaultOrPolicies default_or_policies)
		{
			return this->virtual_def(
				name, fn, default_or_policies, detail::null_type()
			  , LUABIND_MSVC_TYPENAME detail::is_policy_cons<DefaultOrPolicies>::type());
		}

		template<class F, class Default, class Policies>
		class_& def(char const* name, F fn
			, Default default_, Policies const& policies)
		{
			return this->virtual_def(
				name, fn, default_
			  , policies, boost::mpl::false_());
		}

		template<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, class A)>
		class_& def(constructor<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, A)> sig)
		{
            return this->def_constructor(&sig, detail::null_type());
		}

		template<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, class A), class Policies>
		class_& def(constructor<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, A)> sig, const Policies& policies)
		{
            return this->def_constructor(&sig, policies);
		}

        template <class Getter>
        class_& property(const char* name, Getter g)
        {
            this->add_member(
                new detail::property_registration<T, Getter, detail::null_type>(
                    name, g, detail::null_type()));
            return *this;
        }

        template <class Getter, class MaybeSetter>
        class_& property(const char* name, Getter g, MaybeSetter s)
        {
            return property_impl(
                name, g, s
              , boost::mpl::bool_<detail::is_policy_cons<MaybeSetter>::value>()
            );
        }

        template<class Getter, class Setter, class GetPolicies>
        class_& property(const char* name, Getter g, Setter s, const GetPolicies& get_policies)
        {
            typedef detail::property_registration<
                T, Getter, GetPolicies, Setter, detail::null_type
            > registration_type;

            this->add_member(
                new registration_type(name, g, get_policies, s));
            return *this;
        }

        template<class Getter, class Setter, class GetPolicies, class SetPolicies>
        class_& property(
            const char* name
          , Getter g, Setter s
          , GetPolicies const& get_policies
          , SetPolicies const& set_policies)
        {
            typedef detail::property_registration<
                T, Getter, GetPolicies, Setter, SetPolicies
            > registration_type;

            this->add_member(
                new registration_type(name, g, get_policies, s, set_policies));
            return *this;
        }

        template <class C, class D>
        class_& def_readonly(const char* name, D C::*mem_ptr)
        {
            typedef detail::property_registration<T, D C::*, detail::null_type>
                registration_type;

            this->add_member(
                new registration_type(name, mem_ptr, detail::null_type()));
            return *this;
        }

        template <class C, class D, class Policies>
        class_& def_readonly(const char* name, D C::*mem_ptr, Policies const& policies)
        {
            typedef detail::property_registration<T, D C::*, Policies>
                registration_type;

            this->add_member(
                new registration_type(name, mem_ptr, policies));
            return *this;
        }

        template <class C, class D>
        class_& def_readwrite(const char* name, D C::*mem_ptr)
        {
            typedef detail::property_registration<
                T, D C::*, detail::null_type, D C::*
            > registration_type;

            this->add_member(
                new registration_type(
                    name, mem_ptr, detail::null_type(), mem_ptr));
            return *this;
        }

        template <class C, class D, class GetPolicies>
        class_& def_readwrite(
            const char* name, D C::*mem_ptr, GetPolicies const& get_policies)
        {
            typedef detail::property_registration<
                T, D C::*, GetPolicies, D C::*
            > registration_type;

            this->add_member(
                new registration_type(
                    name, mem_ptr, get_policies, mem_ptr));
            return *this;
        }

        template <class C, class D, class GetPolicies, class SetPolicies>
        class_& def_readwrite(
            const char* name
          , D C::*mem_ptr
          , GetPolicies const& get_policies
          , SetPolicies const& set_policies
        )
        {
            typedef detail::property_registration<
                T, D C::*, GetPolicies, D C::*, SetPolicies
            > registration_type;

            this->add_member(
                new registration_type(
                    name, mem_ptr, get_policies, mem_ptr, set_policies));
            return *this;
        }

		template<class Derived, class Policies>
		class_& def(detail::operator_<Derived>, Policies const& policies)
		{
			return this->def(
				Derived::name()
			  , &Derived::template apply<T, Policies>::execute
			  , policies
			);
		}

		template<class Derived>
		class_& def(detail::operator_<Derived>)
		{
			return this->def(
				Derived::name()
			  , &Derived::template apply<T, detail::null_type>::execute
			);
		}

		detail::enum_maker<self_t> enum_(const char*)
		{
			return detail::enum_maker<self_t>(*this);
		}
		
		detail::static_scope<self_t> scope;
		
	private:
		void operator=(class_ const&);

        void add_wrapper_cast(detail::null_type*)
        {}

        template <class U>
        void add_wrapper_cast(U*)
        {
            add_cast(
                detail::registered_class<U>::id
              , detail::registered_class<T>::id
              , detail::static_cast_<U,T>::execute
            );

            add_downcast((T*)0, (U*)0, boost::is_polymorphic<T>());
        }

		void init()
		{
			typedef typename detail::extract_parameter<
					parameters_type
				,	boost::mpl::or_<
							detail::is_bases<boost::mpl::_>
						,	boost::is_base_and_derived<boost::mpl::_, T>
					>
				,	no_bases
			>::type bases_t;

			typedef typename 
				boost::mpl::if_<detail::is_bases<bases_t>
					,	bases_t
					,	bases<bases_t>
				>::type Base;
	
            class_base::init(
                typeid(T)
              , detail::registered_class<T>::id
              , typeid(WrappedType)
              , detail::registered_class<WrappedType>::id
            );

            add_wrapper_cast((WrappedType*)0);

			generate_baseclass_list(detail::type_<Base>());
		}

		template<class Getter, class GetPolicies>
		class_& property_impl(const char* name,
									 Getter g,
									 GetPolicies policies,
									 boost::mpl::bool_<true>)
		{
            this->add_member(
                new detail::property_registration<T, Getter, GetPolicies>(
                    name, g, policies));
			return *this;
		}

		template<class Getter, class Setter>
		class_& property_impl(const char* name,
									 Getter g,
									 Setter s,
									 boost::mpl::bool_<false>)
		{
            typedef detail::property_registration<
                T, Getter, detail::null_type, Setter, detail::null_type
            > registration_type;

            this->add_member(
                new registration_type(name, g, detail::null_type(), s));
			return *this;
		}

		// these handle default implementation of virtual functions
		template<class F, class Policies>
		class_& virtual_def(char const* name, F const& fn
			, Policies const&, detail::null_type, boost::mpl::true_)
		{
			this->add_member(
				new detail::memfun_registration<T, F, Policies>(
					name, fn, Policies()));
			return *this;
		}

		template<class F, class Default, class Policies>
		class_& virtual_def(char const* name, F const& fn
			, Default const& default_, Policies const&, boost::mpl::false_)
		{
			this->add_member(
				new detail::memfun_registration<T, F, Policies>(
					name, fn, Policies()));

			this->add_default_member(
				new detail::memfun_registration<T, Default, Policies>(
					name, default_, Policies()));

			return *this;
		}

        template<class Signature, class Policies>
		class_& def_constructor(Signature*, Policies const&)
        {
            typedef typename Signature::signature signature;

            typedef typename boost::mpl::if_<
                boost::is_same<WrappedType, detail::null_type>
              , T
              , WrappedType
            >::type construct_type;

            this->add_member(
                new detail::constructor_registration<
                    construct_type, HeldType, signature, Policies>(
                        Policies()));

            this->add_default_member(
                new detail::constructor_registration<
                    construct_type, HeldType, signature, Policies>(
                        Policies()));

            return *this;
        }
	};

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // LUABIND_CLASS_HPP_INCLUDED

