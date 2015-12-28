#ifndef DEFS_H
#define DEFS_H
/********************************************************************
created:	2015/02/21
created:	21:2:2015   14:11
file base:	defs
author:		CHE

purpose:	ȫ�ֶ���
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


//�Ѷ��������QT��MOC PROPERTY�����������һ��
//�������Ǹ�������-_-...
//����variable�����Լ���get-set��������Ϊ
//get##name(),set##name()������ֵ����ֵ��Ϊconst����
#define QT_MOC_MINE_DEFINITION(Type, variable, name)\
	private:Type variable;\
	public: QT_MOC_GET_SETTER(Type, variable, name)\


//������������д����Ϊprepareģʽ���������������ݳ�Ա
//������Ҫ�������������Ա�����������������ڡ�
//��ģʽ�£�checkInstance()��������Ψһʵ����
//�뽫��Ĺ��캯�������������Ŀ���Ȩ����Ϊ��public��
//instance()���������
//deleteInstance()ɾ������
//���Ը�����Ҫ���Զ������ǵ�ʵ�֣�Ҳ��������Ķ��崦��
//����PREPARE_INSTANCE_DEFINITION��ʵ��Ĭ�϶��塣
#define PREPARE_INSTANCE_DECLARE(Class) static Class *g_instance;\
	static void checkInstance();\
	public: static Class* instance();static void deleteInstance();\

#define PREPARE_INSTANCE_DEFINITION(Class) Class* Class::g_instance=nullptr; \
	void Class::checkInstance(){if (g_instance == nullptr){\
	std::once_flag flag_##Class;\
	std::call_once(flag_##Class,[&](){g_instance = new Class;});}}\
	Class* Class::instance(){ checkInstance(); return g_instance; }\
	void Class::deleteInstance(){ delete g_instance; g_instance = nullptr; }\

//��ȡָ�����ʵ��������insָ�����
template<typename Instance>
inline Instance* ___get(){ return Instance::instance(); }

#ifdef getInstance
#undef getInstance
#endif // getInstance
#define getInstance(Class) ___get<Class>()

//����ָ�����ʵ��������Ҫ��ʱ���������µ�ʵ��
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

#ifndef _sleep
#define _sleep(n) std::this_thread::sleep_for(std::chrono::milliseconds(n))
#endif
#endif // DEFS_H
