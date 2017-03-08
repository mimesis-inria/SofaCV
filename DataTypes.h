#ifndef SOFA_OR_COMMON_DATATYPES_H
#define SOFA_OR_COMMON_DATATYPES_H

#include <sofa/defaulttype/DataTypeInfo.h>

namespace sofa
{
namespace defaulttype
{
template <class TDataType>
struct NonScalarTypeInfo
{
	typedef TDataType DataType;
	typedef DataType BaseType;
	typedef DataType ValueType;
	typedef long long ConvType;
	typedef NonScalarTypeInfo<TDataType> BaseTypeInfo;
	typedef NonScalarTypeInfo<TDataType> ValueTypeInfo;

	enum
	{
		ValidInfo = 1
	};
	enum
	{
		FixedSize = 1
	};
	enum
	{
		ZeroConstructor = 1
	};
	enum
	{
		SimpleCopy = 1
	};
	enum
	{
		SimpleLayout = 1
	};
	enum
	{
		Integer = 0
	};
	enum
	{
		Scalar = 1
	};
	enum
	{
		Text = 0
	};
	enum
	{
		CopyOnWrite = 1
	};
	enum
	{
		Container = 0
	};

	enum
	{
		Size = 1
	};
	static size_t size() { return 1; }
	static size_t byteSize() { return sizeof(DataType); }
	static size_t size(const DataType& /*data*/) { return 1; }
	static bool setSize(DataType& /*data*/, size_t /*size*/) { return false; }
	template <typename T>
	static void getValue(const DataType& /*data*/, size_t /*index*/, T& /*value*/)
	{
		//				if (index != 0) return;
		//				value = (T)data;
	}

	template <typename T>
	static void setValue(DataType& /*data*/, size_t /*index*/, const T& /*value*/)
	{
		//				if (index != 0) return;
		//				data = (DataType)value;
	}

	static void getValueString(const DataType& data, size_t index,
														 std::string& value)
	{
		if (index != 0) return;
		std::ostringstream o;
		o << data;
		value = o.str();
	}

	static void setValueString(DataType& data, size_t index,
														 const std::string& value)
	{
		if (index != 0) return;
		std::istringstream i(value);
		i >> data;
	}

	static const void* getValuePtr(const DataType& data) { return &data; }
	static void* getValuePtr(DataType& data) { return &data; }
};

}  // namespace defaulttype
}  // namespace sofa

#endif  // SOFA_OR_COMMON_DATATYPES_H
