/////////////////////////////////////
///// mainsub_spbr_plyascii.cpp /////
/////////////////////////////////////

#include <kvs/glut/Application>
#include <kvs/Version> //KVS2

#if KVS_VERSION_MAJOR == 1
  #include <kvs/glew/ParticleBasedRenderer> //KVS1
#elif KVS_VERSION_MAJOR == 2
  #include <kvs/ParticleBasedRenderer> //KVS2
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


//#include "display_usage.h"
#include "event_control.h"
#include "shuffle.h"
#include "version.h"

#include "mainfn_utility.h"

// UCHIDA 2019/03/31
// Prototype declaration
int mainsub_brightness_adjustment(
    kvs::glut::Application*             app,
    int                                 argc,
    char**                              argv,
    SPBR*                               spbr_engine,
    kvs::PointObject*                   object,
    BrightnessAdjustment::FILE_FORMAT4BA file_format );

//#define DEBUG_MAIN

//-----
int mainsub_spbr_plyascii ( int argc, char** argv )
{
  // Create an application and events
  kvs::glut::Application app( argc, argv );

  // Read the first data file (argv[1])
  SPBR* spbr_engine          = new SPBR( argv[1], PLY_ASCII );
  kvs::PointObject* object = spbr_engine        ;

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

  // Forced shuffle
  if( spbr_engine->isForcedShuffleOn () ) {
    Shuffle shuffle_engine ( spbr_engine );
  }

#if defined DEBUG_MAIN
  std::cout << *object << std::endl;
#endif

  //===== END OF CREATING THE POINT OBJECT =====//

    // UCHIDA 2019/03/31
    // Adjust brightness
    if ( spbr_engine->isBrightnessAdjustment() ) {
        return mainsub_brightness_adjustment(
                /* kvs::glut::Application*  */  &app, 
                /* int                      */  argc, 
                /* char**                   */  argv, 
                /* SPBR*                    */  spbr_engine, 
                /* kvs::PointObject*        */  object,
                /* FILE_FORMAT4BA           */  BrightnessAdjustment::PLY_ASCII4BA );
    }

#if KVS_VERSION_MAJOR == 1
  kvs::glew::rits::ParticleBasedRenderer* renderer = new kvs::glew::rits::ParticleBasedRenderer();//KVS1
#elif KVS_VERSION_MAJOR == 2
  kvs::glsl::ParticleBasedRenderer* renderer = new kvs::glsl::ParticleBasedRenderer();//KVS2
#endif

  // Set repeat level
  renderer->setRepetitionLevel ( spbr_engine->repeatLevel()   );

  // Set Lambert shading or keep Phong shading
  setShadingType ( spbr_engine, renderer ) ;

  // Shading control (ON/OFF)
  if( spbr_engine->isShading() == false ) {
    std::cout << "** Shading is off" << std::endl;
    renderer->disableShading();
  }

  // LOD control (ON/OFF)
  if (spbr_engine->isLOD() ) {
    renderer->enableLODControl();
  }

  // Particle zoom control (ON/OFF)
  if ( spbr_engine->isParticleZoomOn() == false ) {
    renderer->disableZooming();
  } else {
    renderer->enableZooming();
  }

  // Shuffle control (ON/OFF)
  if( spbr_engine->isParticleShuffleOn () ) {
    renderer->enableShuffle ();
  } else {
    renderer->disableShuffle ();
  }

  // Create a screen and register 
  //  the point object and the renderer 
  kvs::glut::Screen screen( &app );
  screen.registerObject( object, renderer );

  // Object rotation (Z==>X) if required
  if ( spbr_engine->isZXRotation() ) {
    double zrot_deg = spbr_engine->objectZXRotAngle (0) ; 
    double xrot_deg = spbr_engine->objectZXRotAngle (1) ; 
    ToolXform::rotateZX( object, zrot_deg, xrot_deg, kvs::Vector3f( 0, 0, 0 ) );
  }

  // Create wireframe-box line object if required
  drawWireframeBox( spbr_engine, &screen );

  // Set camera type (orthogonal/perspective) and 
  //   other camera parameters: 
  //   camera position, look-at position, and view angle
  setCameraParameters ( spbr_engine, &screen ); 

  // Set image resolution to the screen
  unsigned int img_resoln = spbr_engine->imageResolution();
  screen.setGeometry( 0, 0, img_resoln, img_resoln );

  // Display particle size (only for displaying it in console)
  displayParticleSize ( spbr_engine, &screen ); 

  // Set background color
#if KVS_VERSION_MAJOR == 1
  screen.background()->setColor( spbr_engine->backGroundColor() );//KVS1   
#elif KVS_VERSION_MAJOR == 2
  screen.setBackgroundColor( spbr_engine->backGroundColor() );//KVS2
#endif

  // Mouse rotation speed //ROTSPEED
  double mouse_rot_speed = spbr_engine->mouseRotSpeed(); // get scaling factor 
  double virtual_sphere_size = screen.scene()->mouse()->trackball().size();
  virtual_sphere_size /= mouse_rot_speed ; 
  screen.scene()->mouse()->trackball().setSize( virtual_sphere_size );

  // Mouse zoom speed //ZOOMSPEED
  double mouse_zoom_speed = spbr_engine->mouseZoomSpeed(); // get scaling factor 
  mouse_zoom_speed *= screen.scene()->mouse()->trackball().scalingFactor(); 
  screen.scene()->mouse()->trackball().setScalingFactor( mouse_zoom_speed );

  // Set window title
  setWindowTitle ( SPBR_WINDOW_TITLE, argv[1], &screen ) ; 

  // Draw FPS count inside the view window
  drawFPS ( spbr_engine, &screen );

  // Add events to the screen
  InitializeEvent  init;
  KeyPressEvent    key;
  screen.addEvent( &init );
  screen.addEvent( &key );

  // Display mene in console 
  std::cout << "** Executing particle-based rendering..." << std::endl;
  key.displayMenu();  // display menu in console

  // Create and show the window
  screen.show();

  // Start
  return( app.run() );

} // mainsub_spbr_plyascii ()

