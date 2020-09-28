//////////////////////////////////////
///// mainsub_opbr_plyascii.cpp  /////
//////////////////////////////////////

#include <kvs/glut/Application>
#include <kvs/Version> //KVS2

#if KVS_VERSION_MAJOR == 1
  #include <kvs/glew/PointRenderer> //KVS1
#elif KVS_VERSION_MAJOR == 2
  #include <kvs/PointRenderer> //KVS2
#endif

#include <kvs/glut/Screen>
#include <kvs/RotationMatrix33>

#include <cstring>
#include <iostream>

#include "fpscount.h"

#include "event_control.h"
#include "wireframebox.h"

#include "spbr.h"
#include "version.h"
#include "display_opbr_usage.h"

#include "toolxform.h"

#include "mainfn_utility.h"


//#define DEBUG_MAIN


//-----
int mainsub_opbr_plyascii ( int argc, char** argv )
{
  // Create an application
  kvs::glut::Application app( argc, argv );
  InitializeEvent  init;
  KeyPressEvent key;

  // Read the first data file (argv[1])
  SPBR* spbr_engine         = new SPBR( argv[1], PLY_ASCII );
  kvs::PointObject* object = spbr_engine ;

  // Read and append the remaining files:  
  //  argv[2], argv[3], ..., argv[argc-1]
  for (int i = 3; i <= argc; i++) {
    if ( isASCII_PLY_File(argv[i - 1]) ) {
      SPBR* spbr_tmp = new SPBR(argv[i - 1], PLY_ASCII);
      object->add(*kvs::PointObject::DownCast(spbr_tmp));
    }
    else if ( isBINARY_PLY_File(argv[i - 1]) ) {
      SPBR* spbr_tmp = new SPBR(argv[i - 1], PLY_BINARY);
      object->add(*kvs::PointObject::DownCast(spbr_tmp));
    }
    else if ( isBinarySPBR_File(argv[i - 1]) ) {
      SPBR* spbr_tmp = new SPBR(argv[i - 1], SPBR_BINARY);
      object->add(*kvs::PointObject::DownCast(spbr_tmp));
    }
    else {
      SPBR* spbr_tmp = new SPBR(argv[i - 1], SPBR_ASCII);
      object->add(*kvs::PointObject::DownCast(spbr_tmp));
    }

  }//for 

  // Set the total bounding box
  //   Note: This updates the total bounding box of the 
  //         read point objects.
  addBoundingBoxToScene ( spbr_engine ) ;


#if defined DEBUG_MAIN
  std::cout << *object << std::endl;
#endif

#if KVS_VERSION_MAJOR == 1
  kvs::glew::PointRenderer* renderer = new kvs::glew::PointRenderer();//KVS1
#elif KVS_VERSION_MAJOR == 2
  kvs::glsl::PointRenderer* renderer = new kvs::glsl::PointRenderer(); //KVS2
#endif

  // Set Lambert shading or keep Phong shading
  setShadingType ( spbr_engine, renderer ) ;

  // Shading control
  if( spbr_engine->isShading() == false ) {
    std::cout << "** Shading is off" << std::endl;
    renderer->disableShading();
  }

  // Create a screen and make registration
  kvs::glut::Screen screen( &app );
  screen.registerObject( object, renderer );

  // Mouse rotation speed //ROTSPEED
  double mouse_rot_speed = spbr_engine->mouseRotSpeed(); // get scaling factor 
  double virtual_sphere_size = screen.scene()->mouse()->trackball().size();
  virtual_sphere_size /= mouse_rot_speed ; 
  screen.scene()->mouse()->trackball().setSize( virtual_sphere_size );

  // Mouse zoom speed //ZOOMSPEED
  double mouse_zoom_speed = spbr_engine->mouseZoomSpeed(); // get scaling factor 
  mouse_zoom_speed *= screen.scene()->mouse()->trackball().scalingFactor(); 
  screen.scene()->mouse()->trackball().setScalingFactor( mouse_zoom_speed );

  /////2018/10/17
  //// Object rotation (Z==>X)
  if ( spbr_engine->isZXRotation() ) {
    double zrot_deg = spbr_engine->objectZXRotAngle (0) ; 
    double xrot_deg = spbr_engine->objectZXRotAngle (1) ; 
    ToolXform::rotateZX( object, zrot_deg, xrot_deg, kvs::Vector3f( 0, 0, 0 ) );
  } // if object rotation (Z==>X)   
 
  // Create wireframe-box line object if required
  drawWireframeBox( spbr_engine, &screen );

  // Set camera type (orthogonal/perspective) and 
  //   other camera parameters: 
  //   camera position, look-at position, and view angle
  setCameraParameters ( spbr_engine, &screen ); 

  // Window title
  char WINDOW_TITLE[256];
  strcpy( WINDOW_TITLE, OPBR_WINDOW_TITLE );
  strcat( WINDOW_TITLE, "  with GLSL" );
  strcat( WINDOW_TITLE, "  (" );
  strcat( WINDOW_TITLE, argv[1] );
  strcat( WINDOW_TITLE, ")" );

  // Set camera type (orthogonal/perspective) and 
  //   other camera parameters: 
  //   camera position, look-at position, and view angle
  //setCameraParameters ( spbr_engine, &screen ); 

  // Draw
  unsigned int img_resoln = spbr_engine->imageResolution();
  screen.setGeometry( 0, 0, img_resoln, img_resoln );

#if KVS_VERSION_MAJOR == 1
  screen.background()->setColor( spbr_engine->backGroundColor() ); //KVS1   
#elif KVS_VERSION_MAJOR == 2
  screen.setBackgroundColor    ( spbr_engine->backGroundColor() );//KVS2
#endif

  screen.setTitle( WINDOW_TITLE );
  screen.addEvent( &init );
  screen.addEvent( &key );
  screen.show();
  std::cout << "** Executing particle-based rendering..." << std::endl;
  key.displayMenu();  

  // FPS count
  /***** //TANAKA
  FPSLabel fpslabel( &screen );
  if( spbr_engine->isFPSDisplayed()) {
    kvs::RGBColor textcolor( 255 - spbr_engine->bg_Rb() ,
			     255 - spbr_engine->bg_Gb() ,
			     255 - spbr_engine->bg_Bb()  );
    fpslabel.setTextColor(textcolor);
    fpslabel.show();
  }
  *****/
  
  // Start
  return( app.run() );

} // mainsub_opbr_plyascii ()
