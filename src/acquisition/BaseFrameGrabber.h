#ifndef SOFACV_ACQUISITION_BASEFRAMEGRABBER_H
#define SOFACV_ACQUISITION_BASEFRAMEGRABBER_H

#include "SofaCVPlugin.h"
#include "datatypes/cvMat.h"
#include "datatypes/cvMatUtils.h"

#include "BaseGrabber.h"

#include <sofa/defaulttype/Vec.h>
#include <sofa/helper/OptionsGroup.h>

namespace sofacv
{
namespace acquisition
{
/**
 * \brief Interface for all Acquisition components outputing a cv::Mat
 */
class SOFA_SOFACV_API BaseFrameGrabber : virtual public BaseGrabber
{
 public:
  SOFA_ABSTRACT_CLASS(BaseFrameGrabber, BaseGrabber);

  sofa::Data<sofa::helper::OptionsGroup>
      d_videoMode;  ///< Determines the stereo mode if any

  sofa::Data<cvMat>
      d_fullFrame;  ///< [OUTPUT] raw frame as retrieved from the grabber
  sofa::Data<cvMat>
      d_frame1;  ///< [OUTPUT] Left image (full frame if mono)
  sofa::Data<cvMat>
      d_frame2;  ///< [OUTPUT] Right image (nothing if mono)

  // cv::CAP_PROPS
  sofa::Data<double> d_fps;  ///< [OUTPUT] Video frequency, if the device
                             ///provides the information
  sofa::Data<sofa::defaulttype::Vec2u>
      d_dimensions;  ///< [OUTPUT] Dimensions of the grabbed image

 public:
  BaseFrameGrabber();
  virtual ~BaseFrameGrabber();

  virtual void init() override
  {
    addOutput(&d_fullFrame);
    addOutput(&d_frame1);
    addOutput(&d_frame2);
    addOutput(&d_dimensions);
    addOutput(&d_fps);
    setDirtyValue();
  }

  virtual void doUpdate() override
  {

  }

protected:
  /// \brief deinterleaved image splitting function (see
  /// http://www.stereo3d.com/formats.htm)
  void split_deinterleaved(const cvMat& src, cvMat& dstL,
                           cvMat& dstR);
  /// \brief top-bottom image splitting function (see
  /// http://www.stereo3d.com/formats.htm)
  void split_top_bottom(const cvMat& src, cvMat& dstL,
                        cvMat& dstR);
  /// \brief side-by-side image splitting function (see
  /// http://www.stereo3d.com/formats.htm)
  void split_side_by_side(const cvMat& src, cvMat& dstL,
                          cvMat& dstR);
  /// \brief rotated side-by-side image splitting function (see
  /// http://www.stereo3d.com/formats.htm)
  void split_rotated_side_by_side(const cvMat& src, cvMat& dstL,
                                  cvMat& dstR);

  // splits the frames according to the specified video mode
  void splitFrames(const cvMat& src, cvMat& dstL,
                   cvMat& dstR, image::VideoMode mode);
};

}  // namespace acquisition
}  // namespace sofacv

#endif  // SOFACV_ACQUISITION_BASEFRAMEGRABBER_H
