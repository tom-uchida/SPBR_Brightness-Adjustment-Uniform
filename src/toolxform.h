///////////////////////
///// toolxform.h /////
///////////////////////

#if !defined TOOL_XFORM_H
#define TOOL_XFORM_H


#include <kvs/Vector3>
#include <kvs/Matrix33>
#include <kvs/ObjectBase>
#include <kvs/XformControl>

//----- 
class ToolXform {

 public:

  //----- constructor
  ToolXform() { ; }

  //-----
  static void rotateX( kvs::ObjectBase* object, 
                       double xrot_deg, 
                       const kvs::Vector3f& rot_center ) 
  {
    kvs::Matrix33f R = kvs::Matrix33f::RotationX( xrot_deg );
    kvs::Vector3f  T = rot_center;
    kvs::Xform X = kvs::Xform::Translation( T ) * kvs::Xform::Rotation( R ) * kvs::Xform::Translation( -T );
    object->multiplyXform( X );

  }

  //-----
  static void rotateY( kvs::ObjectBase* object, 
                       double yrot_deg, 
                       const kvs::Vector3f& rot_center ) 
  {
    kvs::Matrix33f R = kvs::Matrix33f::RotationY( yrot_deg );
    kvs::Vector3f  T = rot_center;
    kvs::Xform X = kvs::Xform::Translation( T ) * kvs::Xform::Rotation( R ) * kvs::Xform::Translation( -T );
    object->multiplyXform( X );

  }

  //-----
  static void rotateZ( kvs::ObjectBase* object, 
                       double zrot_deg, 
                       const kvs::Vector3f& rot_center ) 
  {
    kvs::Matrix33f R = kvs::Matrix33f::RotationZ( zrot_deg );
    kvs::Vector3f  T = rot_center;
    kvs::Xform X = kvs::Xform::Translation( T ) * kvs::Xform::Rotation( R ) * kvs::Xform::Translation( -T );
    object->multiplyXform( X );

  }


  //-----
  static void rotateZX( kvs::ObjectBase* object, 
			double zrot_deg, double xrot_deg,
                        const kvs::Vector3f& rot_center  ) 
  {
    kvs::Matrix33f RZ = kvs::Matrix33f::RotationZ( zrot_deg );
    kvs::Matrix33f RX = kvs::Matrix33f::RotationX( xrot_deg );

    kvs::Vector3f  T = rot_center;
    kvs::Xform X = kvs::Xform::Translation( T ) * kvs::Xform::Rotation( RX ) * kvs::Xform::Rotation( RZ ) * kvs::Xform::Translation( -T );
    object->multiplyXform( X );

  }


}; // ToolXform

////////////// end of toolxformh
#endif
