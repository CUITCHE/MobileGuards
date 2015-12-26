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
#define SETTER(Type,variable, name) void set##name(const Type& val){this->variable = val;}
#define GETTER(Type,variable,name) const Type& get##name()const{return variable;}
#define GET_SETTER(Type,variable, name)\
	SETTER(Type,variable, name)\
	GETTER(Type,variable,name)

#define QT_MOC_GET_SETTER(Type, variable, name)\
	GET_SETTER(Type, variable, name)\
	Q_PROPERTY(Type variable READ get##name WRITE set##name)\


//把定义变量和QT的MOC PROPERTY宏声明结合在一起，
//呃，我是个大懒鬼-_-...
//变量variable，有自己的get-set方法，名为
//get##name(),set##name()，返回值或传入值均为const引用
#define QT_MOC_MINE_DEFINITION(Type, variable, name)\
	private:Type variable;\
	public: QT_MOC_GET_SETTER(Type, variable, name)\


//在类声明处书写，称为prepare模式，它适用于有数据成员
//但不需要编译器或编码人员管理类对象的生命周期。
//此模式下，checkInstance()负责生成唯一实例，
//须将类的构造函数和析构函数的控制权限设为非public。
//instance()返回类对象，
//deleteInstance()删除对象。
//可以根据需要，自定义他们的实现，也可以在类的定义处。
//调用PREPARE_INSTANCE_DEFINITION宏实现默认定义。
#define PREPARE_INSTANCE_DECLARE(Class) static Class *g_instance;\
	static void checkInstance();\
	public: static Class* instance();static void deleteInstance();\

#define PREPARE_INSTANCE_DEFINITION(Class) Class* Class::g_instance=nullptr; \
	void Class::checkInstance(){if (g_instance == nullptr){\
	std::once_flag flag_##Class;\
	std::call_once(flag_##Class,[&](){g_instance = new Class;});}}\
	Class* Class::instance(){ checkInstance(); return g_instance; }\
	void Class::deleteInstance(){ delete g_instance; g_instance = nullptr; }\

//获取指定类的实例，返回ins指针变量
template<typename Instance>
inline Instance* ___get(){ return Instance::instance(); }

#ifdef getInstance
#undef getInstance
#endif // getInstance
#define getInstance(Class) ___get<Class>()

//丢弃指定类的实例，在需要的时候，再生成新的实例
template<typename Instance>
inline void ___pop(){ Instance::deleteInstance(); }

#ifdef popInstance
#undef popInstance
#endif // getInstance
#define popInstance(Class) ___pop<Class>()

#ifdef __surrenderconsole__
#undef __surrenderconsole__
#endif
#define __surrenderconsole__ QThread::msleep(2);

#ifdef Q_lock
#undef Q_lock
#endif // Q_lock
#define Q_lock(mutex) QMutexLocker locker(mutex)

#define __normal_getsetter(Type,variable)\
	const Type& variable()const{return _##variable;}\
	Type& variable(){Q_lock(mtx); return _##variable;}

#define ENTITY_MEMBER_DEFINITION(Type, variable)\
	private:Type _##variable;\
	public:__normal_getsetter(Type, variable)

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
#endif // DEFS_H
