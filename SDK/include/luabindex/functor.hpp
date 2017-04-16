#pragma once

#include <luabind/object.hpp>
#include <luabind/function.hpp>

namespace luabindex
{
	template <class result_type>
	class functor
	{
	public:
		functor()
		{
		}

		explicit functor(const luabind::object& func) : m_func(func)
		{
		}

		explicit operator bool() const
		{
			return is_valid();
		}

		bool operator==(const functor<result_type> & other) const
		{
			return m_func == other.m_func;
		}

		template<class... Args>
		result_type operator()(Args&&... args) const
		{
			return (result_type)luabind::call_function<result_type>(m_func, std::forward<Args>(args)...);
		}

		bool is_valid() const
		{
			return m_func.is_valid();
		}

		lua_State* interpreter() const
		{
			return m_func.interpreter();
		}

		void push(lua_State* L) const
		{
			m_func.push(L);
		}

	private:
		luabind::object m_func;
	};
}

namespace luabind
{
	template <class result_type>
	struct default_converter<luabindex::functor<result_type>>
		: native_converter_base<luabindex::functor<result_type>>
	{
		static int compute_score(lua_State* L, int index)
		{
			auto type = lua_type(L, index);
			return (type == LUA_TFUNCTION  || type == LUA_TNIL) ? 0 : -1;
		}

		luabindex::functor<result_type> from(lua_State* L, int index)
		{
			if (lua_type(L, index) == LUA_TNIL)
				return luabindex::functor<result_type>();
			return luabindex::functor<result_type>(object(from_stack(L, index)));
		}

		void to(lua_State* L, luabindex::functor<result_type> const& x)
		{
			x.push(L);
		}
	};

	template <class result_type>
	struct default_converter<luabindex::functor<result_type> const&>
		: default_converter<luabindex::functor<result_type>>
	{};

	template<>
	struct default_converter<u32 const&>
		: default_converter<u32>
	{};

	template<>
	struct default_converter<u16 const&>
		: default_converter<u16>
	{};
}