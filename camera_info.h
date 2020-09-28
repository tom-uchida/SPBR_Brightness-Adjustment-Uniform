////////////////////////
///// CameraInfo.h /////
////////////////////////

#if !defined  SSM__CAMERA_INFO_H_INCLUDE
#define       SSM__CAMERA_INFO_H_INCLUDE

#include<kvs/Vector3>
#include <kvs/CellByCellParticleGenerator>

//----------------//
class CameraInfo {
//----------------//

 public:
 void set ( kvs::Camera*  camera, 
            double min_x, double min_y, double min_z , 
            double max_x, double max_y, double max_z      );

  GLdouble* modelviewMatrix ( void ) { return m_modelview  ; }
  GLdouble* projectionMatrix( void ) { return m_projection ; }
  GLint*    viewportMatrix  ( void ) { return m_viewport   ; }
  float     bbCenterDepth   ( void ) { return m_bb_center_depth ;}

  // KVS2
  double pixelWidth( const kvs::Camera* camera )
  {
    const float subpixel_level = 1 ; // ensemble averaging
    return kvs::CellByCellParticleGenerator::CalculateSubpixelLength( subpixel_level, *camera, *m_bb_obj );
  }

 private:
  kvs::Vector3f m_bb_min ;
  kvs::Vector3f m_bb_max ;

  kvs::PointObject* m_bb_obj ;

  GLdouble m_modelview [16]; 
  GLdouble m_projection[16]; 
  GLint    m_viewport  [ 4]; 

  float    m_bb_center_depth ;


};

//-----
inline 
void CameraInfo::set ( kvs::Camera*  camera, 
                       double min_x , 
                       double min_y , 
                       double min_z , 
                       double max_x , 
                       double max_y , 
                       double max_z             )
{
  // min and max coordinates of the bounding box
  m_bb_min.set ( (float)min_x, (float)min_y, (float)min_z ); 
  m_bb_max.set ( (float)max_x, (float)max_y, (float)max_z ); 

  // Create a dummy object
  m_bb_obj = new kvs::PointObject();
  m_bb_obj->setMinMaxExternalCoords( m_bb_min, m_bb_max );
  m_bb_obj->updateNormalizeParameters();

  // Calc OpenGL matrices // KVS1
  //  ::GetModelviewMatrix ( camera, m_bb_obj, &(m_modelview) );
  //  ::GetProjectionMatrix( camera, &(m_projection) );
  //  ::GetViewport( camera, &(m_viewport) );

   //Calc OpenGL matrices // KVS2
  kvs::detail::GetModelviewMatrix ( camera, m_bb_obj, &(m_modelview) );
  kvs::detail::GetProjectionMatrix( camera, &(m_projection) );
  kvs::detail::GetViewport( camera, &(m_viewport) );

  // Calc distance betwen the eye and the object center: 
  //  It is normalized to have value between 0 and 1.
  /* KVS1 
  m_bb_center_depth  = 
   kvs::CellByCellParticleGenerator::CalculateObjectDepth(m_bb_obj,
                                                          m_modelview, 
                                                          m_projection,
                                                          m_viewport   );
  */
    //kvs::CellByCellParticleGenerator::CalculateObjectDepth(*camera, *m_bb_obj);//KVS2

}// set()


#endif
// end of CameraInfo.h
