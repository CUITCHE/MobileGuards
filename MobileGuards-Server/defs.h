#ifndef DEFS_H
#define DEFS_H
/********************************************************************
created:	2015/02/21
created:	21:2:2015   14:11
file base:	defs
author:		CHE

purpose:	全局定义
*********************************************************************/
#include <mutex>
#ifndef CLASS_MEMBER_DECLARE

#define CLASS_NAME_MEMBER(cls) cls##Private
#define CLASS_MEMBER_PRIVATE(cls) struct CLASS_NAME_MEMBER(cls)

#define CLASS_MEMBER_DECLARE(cls) \
	std::unique_ptr<CLASS_MEMBER_PRIVATE(cls)> d;
#endif

#ifndef CLASS_MEMBER_DEFINITION
#define CLASS_MEMBER_DEFINITION(cls) CLASS_MEMBER_PRIVATE(cls)
#endif

#ifndef DELETE_PTR
#define DELETE_PTR

template <typename Type>
void delete_ptr(Type p)
{
	static_assert(std::is_pointer<Type>::value, "error pointer type, p should be 1-pointer");
	if (p) {
		delete p;
	}
}

template<typename Type, typename... Args>
void delete_ptr(Type p, Args... args)
{
	delete_ptr(p);
	delete_ptr(args...);
}
#endif

#ifndef mv
#define mv(o) std::move(o)
#endif

#ifndef _sleep
#define _sleep(n) std::this_thread::sleep_for(std::chrono::milliseconds(n))
#endif

#ifndef CLASS_MEMBER_CONSTRUCTURE
#define CLASS_MEMBER_CONSTRUCTURE(var_pointer, __VA_ARGS__) var_pointer(new std::remove_pointer<decltype(var_pointer)>::type(__VA_ARGS__))
#endif

#ifndef __member
#define __member(type, var) \
	private:\
		type _##var;\
	public:\
	type& var(){return _##var;}\
	const type& var()const{return _##var;}

#endif
#endif // DEFS_H
