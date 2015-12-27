#ifndef synthetise_H__
#define synthetise_H__

#ifndef __Key
	#define __Key
#endif

#ifndef __Value
	#define __Value
#endif

template<typename _ValueType>
void synthtiseJSON(QVariantMap &keyValue, const QString &key __Key, const _ValueType &value __Value)
{
	keyValue.insert(key, value);
}

#endif // synthetise_H__
