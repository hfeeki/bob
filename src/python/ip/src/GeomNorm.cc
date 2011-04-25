/**
 * @file src/python/ip/src/GeomNorm.cc
 * @author <a href="mailto:Laurent.El-Shafey@idiap.ch">Laurent El Shafey</a> 
 * @date Thu 17 Mar 19:12:40 2011 
 *
 * @brief Binds the GeomNorm class to python
 */

#include <boost/python.hpp>

#include "ip/GeomNorm.h"
#include "ip/maxRectInMask.h"

using namespace boost::python;
namespace ip = Torch::ip;

static const char* GEOMNORM_DOC = "Objects of this class, after configuration, can perform a geometric normalization.";
static const char* MAXRECTINMASK2D_DOC = "Given a 2D mask (a 2D blitz array of booleans), compute the maximum rectangle which only contains true values.";


#define GEOMNORM_CALL_DEF(T) \
  .def("__call__", (void (ip::GeomNorm::*)(const blitz::Array<T,2>&, blitz::Array<double,2>&, const int, const int, const int, const int))&ip::GeomNorm::operator()<T>, (arg("input"), arg("output"), arg("rotation_center_y"), arg("rotation_center_x"), arg("crop_ref_y"), arg("crop_ref_x")), "Call an object of this type to perform a geometric normalization of an image wrt. the two given points.") \
  .def("__call__", (void (ip::GeomNorm::*)(const blitz::Array<T,2>&, const blitz::Array<bool,2>&, blitz::Array<double,2>&, blitz::Array<bool,2>&, const int, const int, const int, const int))&ip::GeomNorm::operator()<T>, (arg("input"), arg("input_mask"), arg("output"), arg("output_mask"), arg("rotation_center_y"), arg("rotation_center_x"), arg("crop_ref_y"), arg("crop_ref_x")), "Call an object of this type to perform a geometric normalization of an image wrt. the two given points, taking mask into account.")

void bind_ip_geomnorm() {
  class_<ip::GeomNorm, boost::shared_ptr<ip::GeomNorm> >("GeomNorm", GEOMNORM_DOC, init<const double, const double, const int, const int, const int, const int>((arg("rotation_angle"), arg("scaling_factor"), arg("crop_height"), arg("crop_width"), arg("crop_offset_h"), arg("crop_offset_w")), "Constructs a GeomNorm object."))
    .add_property("rotation_angle", &ip::GeomNorm::getRotationAngle, &ip::GeomNorm::setRotationAngle)
    .add_property("scaling_factor", &ip::GeomNorm::getScalingFactor, &ip::GeomNorm::setScalingFactor)
    .add_property("crop_height", &ip::GeomNorm::getCropHeight, &ip::GeomNorm::setCropHeight)
    .add_property("crop_width", &ip::GeomNorm::getCropWidth, &ip::GeomNorm::setCropWidth)
    .add_property("crop_offset_h", &ip::GeomNorm::getCropOffsetH, &ip::GeomNorm::setCropOffsetH)
    .add_property("crop_offset_w", &ip::GeomNorm::getCropOffsetW, &ip::GeomNorm::setCropOffsetW)
    GEOMNORM_CALL_DEF(uint8_t)
    GEOMNORM_CALL_DEF(uint16_t)
    GEOMNORM_CALL_DEF(double)
    ;

  def("maxRectInMask", (const blitz::TinyVector<int,4> (*)(const blitz::Array<bool,2>&))&Torch::ip::maxRectInMask, (("src")), MAXRECTINMASK2D_DOC); 
}