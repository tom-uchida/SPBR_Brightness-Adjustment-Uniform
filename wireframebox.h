//////////////////////////
///// wireframebox.h /////
//////////////////////////

#if !defined SSM__WIREFRAME_H_INCLUDE 
#define      SSM__WIREFRAME_H_INCLUDE  

#include <iostream>
#include <kvs/Type>
#include <kvs/ValueArray>
#include <kvs/PointObject>
#include <kvs/Bounds>
#include <kvs/LineObject>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/LineRenderer>

inline kvs::LineObject* WireframeBox ( const kvs::Vector3d& min  = kvs::Vector3d( -1.0, -1.0, -1.0),
				       const kvs::Vector3d& max  = kvs::Vector3d( +1.0, +1.0, +1.0) )
{
  // coords
  kvs::Real32 coords[24] = {  
			    (kvs::Real32)min.x(), (kvs::Real32)min.y(), (kvs::Real32)min.z(), // v0
                            (kvs::Real32)max.x(), (kvs::Real32)min.y(), (kvs::Real32)min.z(), // v1
                            (kvs::Real32)max.x(), (kvs::Real32)max.y(), (kvs::Real32)min.z(), // v2
                            (kvs::Real32)min.x(), (kvs::Real32)max.y(), (kvs::Real32)min.z(), // v3  
                            (kvs::Real32)min.x(), (kvs::Real32)min.y(), (kvs::Real32)max.z(), // v4
                            (kvs::Real32)max.x(), (kvs::Real32)min.y(), (kvs::Real32)max.z(), // v5
                            (kvs::Real32)max.x(), (kvs::Real32)max.y(), (kvs::Real32)max.z(), // v6
                            (kvs::Real32)min.x(), (kvs::Real32)max.y(), (kvs::Real32)max.z()  // v7
                           }; 

  // lines
  kvs::UInt32 connects [24] =  {
                                0, 1, // bottom
                                1, 2,
                                2, 3,
                                3, 0, 
                                4, 5, // top
                                5, 6,
                                6, 7,
                                7, 4, 
                                0, 4, // side
                                1, 5,
                                2, 6,
                                3, 7 
                              };

  // colors
  kvs::UInt8   colors [36] =  {
                                255,   0,   0, // bottom
                                255,   0,   0, 
                                255,   0,   0, 
                                255,   0,   0, 
                                  0,   0, 255, // top
                                  0,   0, 255,
                                  0,   0, 255,
                                  0,   0, 255,
				  0, 255,   0, // side
                                  0, 255,   0, 
                                  0, 255,   0, 
				  0, 255,   0
                                };

  // width
  kvs::Real32 width = 1.0 ;

  // Create a bounding box as LineObject 
  /** //KVS1
  kvs::LineObject* wireframe_box 
  = new kvs::LineObject( kvs::ValueArray<kvs::Real32> ( coords,   24 ),
                         kvs::ValueArray<kvs::UInt32> ( connects, 24 ),
                         kvs::ValueArray<kvs::UInt8>  ( colors,   36 ),
                         width,
                         kvs::LineObject::Segment,
                         kvs::LineObject::LineColor                 );
   **/
  //KVS2 //TANAKA
  kvs::LineObject* wireframe_box = new kvs::LineObject();
  wireframe_box->setCoords     ( kvs::ValueArray<kvs::Real32>( coords  , 24 ) );
  wireframe_box->setConnections( kvs::ValueArray<kvs::UInt32>( connects, 24 ) );
  wireframe_box->setColors     ( kvs::ValueArray<kvs::UInt8> ( colors  , 36 ) );
  wireframe_box->setSize       ( width );
  wireframe_box->setLineType   ( kvs::LineObject::Segment   );
  wireframe_box->setColorType  ( kvs::LineObject::LineColor );

  // update min and max coords
  wireframe_box->updateMinMaxCoords();

  // end
  return wireframe_box ;

 }


#endif
