#ifndef SOFA_OR_COMMON_CVDMATCH_H
#define SOFA_OR_COMMON_CVDMATCH_H

#include "DataTypes.h"

#include <iostream>
#include <opencv2/core/types.hpp>

namespace sofa
{
namespace OR
{
namespace common
{
class cvDMatch : public cv::DMatch
{
 public:
  cvDMatch()
  {
    distance = -1.0f;
    imgIdx = -1;
    queryIdx = -1;
    trainIdx = -1;
  }
  cvDMatch(const cv::DMatch& o)
  {
    distance = o.distance;
    imgIdx = o.imgIdx;
    queryIdx = o.queryIdx;
    trainIdx = o.trainIdx;
  }
  cvDMatch(int _queryIdx, int _trainIdx, float _distance)
      : cv::DMatch(_queryIdx, _trainIdx, _distance)
  {
  }
  cvDMatch(int _queryIdx, int _trainIdx, int _imgIdx, float _distance)
      : cv::DMatch(_queryIdx, _trainIdx, _imgIdx, _distance)
  {
  }

  inline friend std::istream& operator>>(std::istream& in, cvDMatch& s)
  {
    in >> s.distance >> s.imgIdx >> s.queryIdx >> s.trainIdx;
    return in;
  }

  inline friend std::ostream& operator<<(std::ostream& out, const cvDMatch& s)
  {
    out << s.distance << ' '  << s.imgIdx << ' ' << s.queryIdx << ' '
        << s.trainIdx << ' ';
    return out;
  }
};

}  // namespace common
}  // namespace OR

namespace defaulttype
{
template <>
struct DataTypeInfo<OR::common::cvDMatch>
		: public NonScalarTypeInfo<OR::common::cvDMatch>
{
	static const char* name() { return "cvDMatch"; }
};

}  // namespace defaulttype

}  // namespace sofa
#endif  // SOFA_OR_COMMON_CVDMATCH_H
