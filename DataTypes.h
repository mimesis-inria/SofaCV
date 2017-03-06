//#ifndef SOFA_OR_COMMON_DATATYPES_H
//#define SOFA_OR_COMMON_DATATYPES_H

//#include <sofa/defaulttype/DataTypeInfo.h>

//namespace sofa
//{
//namespace OR
//{
//namespace common
//{

//template <class TSrcType, class TDstType>
//struct ConversionType
//{
//  typedef TSrcType SrcType;
//  typedef TDstType DstType;
//  static const char* Name() { return "unknown"; }
//};

//typedef ConversionType<common::cvKeypoint, defaulttype::Vec2i> cvKeypoint2Vec2i;
//template <>
//inline const char* cvKeypoint2Sofa::Name()
//{
//  return "cvKeypoint2Sofa";
//}

//typedef ConversionType<defaulttype::Vec2i, common::cvKeypoint> Vec2i2cvKeypoint;
//template <>
//inline const char* Sofa2cvKeypoint::Name()
//{
//  return "Sofa2cvKeypoint";
//}

//template <class ConversionType>
//  struct ConversionTypeInfo : public VirtualTypeInfo<ConversionType>
//{
//  static std::string name()
//  {
//    std::ostringstream o;
//    o << "ConversionType<" << ConversionType::SrcType::name() << "," << ConversionType::SrcType::name() << ">";
//    return o.str();
//  }
//};

//} // namespace common
//} // namespace OR
//} // namespace sofa

 
//#endif SOFA_OR_COMMON_DATATYPES_H
