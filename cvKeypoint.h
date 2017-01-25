#ifndef SOFA_OR_COMMON_CVKEYPOINT_H
#define SOFA_OR_COMMON_CVKEYPOINT_H

#include <iostream>
#include <opencv2/core.hpp>

namespace sofa
{
namespace OR
{
namespace common
{
class cvKeypoint : public cv::KeyPoint
{
 public:
  cvKeypoint(const cv::KeyPoint &kp)
      : cv::KeyPoint(kp.pt, kp.size, kp.angle, kp.response, kp.octave,
                     kp.class_id)
  {
  }
  cvKeypoint() : cv::KeyPoint() {}
  cvKeypoint(cv::Point2f _pt, float _size, float _angle = -1,
             float _response = 0, int _octave = 0, int _class_id = -1)
      : cv::KeyPoint(_pt, _size, _angle, _response, _octave, _class_id)
  {
  }
  cvKeypoint(float x, float y, float _size, float _angle = -1,
             float _response = 0, int _octave = 0, int _class_id = -1)
      : cv::KeyPoint(x, y, _size, _angle, _response, _octave, _class_id)
  {
  }

  inline friend std::istream &operator>>(std::istream &in, cvKeypoint &s)
  {
    in >> s.pt.x >> s.pt.y >> s.size >> s.angle >> s.response >> s.octave >> s.class_id;
    return in;
  }

  inline friend std::ostream &operator<<(std::ostream &out, const cvKeypoint &s)
  {
    out << s.pt.x << ' ' << s.pt.y << ' ' << s.size << ' ' << s.angle
        << ' ' << s.response << ' ' << s.octave << ' ' << s.class_id << ' ';
    return out;
  }
};

}  // namespace common
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_COMMON_CVKEYPOINT_H
