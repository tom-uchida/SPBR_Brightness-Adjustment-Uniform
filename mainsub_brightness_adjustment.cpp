//////////////////////////////////////////////////
//   @file   mainsub_brightness_adjustment.cpp
//   @author Tomomasa Uchida
//   @date   2019/03/16
//////////////////////////////////////////////////

#include <kvs/glut/Application>
#include <kvs/Version> //KVS2

#if KVS_VERSION_MAJOR == 1
    #include <kvs/glew/ParticleBasedRenderer> //KVS1
#elif KVS_VERSION_MAJOR == 2
    #include <kvs/ParticleBasedRenderer> //KVS2
#endif

#include <kvs/glut/Screen>

#include <cstring>
#include <iostream>

#include "event_control.h"
#include "file_format.h"
#include "spbr.h"
#include "version.h"
#include "display_opbr_usage.h"

#include "toolxform.h"
#include "shuffle.h"

#include "mainfn_utility.h"

#include "brightness_adjustment.h"
#include <kvs/glut/Timer>

int mainsub_brightness_adjustment(
    kvs::glut::Application*             app,
    int                                 argc,
    char**                              argv,
    SPBR*                               spbr_engine,
    kvs::PointObject*                   object,
    BrightnessAdjustment::FILE_FORMAT4BA file_format )
{
    // Instantiate class BrightnessAdjustment
    BrightnessAdjustment* ba = new BrightnessAdjustment( file_format );

    // Create screen
    kvs::glut::Screen screen( app );

    // Register object and renderer
    unsigned int original_repeat_level = spbr_engine->repeatLevel();
    ba->RegisterObject( screen.scene(), argc, argv, spbr_engine, original_repeat_level);

    // Set camera type (orthogonal/perspective) and 
    //   other camera parameters: 
    //   camera position, look-at position, and view angle
    setCameraParameters( spbr_engine, &screen ); 

    // Set image resolution to the screen
    unsigned int img_resoln = spbr_engine->imageResolution();
    screen.setGeometry( 0, 0, img_resoln, img_resoln );

    // Forcibly, set background color "Black"
    screen.setBackgroundColor( kvs::RGBColor(0, 0, 0) );
    std::cout << "** Forcibly, background color is set to \"black\"." << std::endl;
    ba->setBackgroundColor( kvs::RGBColor(0, 0, 0) );

    // Set window title
    setWindowTitle ( SPBR_WINDOW_TITLE, argv[1], &screen ) ; 

    // Add initialize event
    InitializeEvent init;
    screen.addEvent( &init );

    // Add keypress event
    KeyPressEvent   key;
    screen.addEvent( &key );

    // Add timer event
    TimerEvent timer_event(
        /* BrightnessAdjustment* */  ba, 
        /* int                   */  argc, 
        /* char**                */  argv, 
        /* kvs::Scene*           */  screen.scene(), 
        /* SPBR*                 */  spbr_engine, 
        /* const int             */  original_repeat_level);
    const int sec = 1; // sec
    kvs::glut::Timer timer( sec*1000 ); // msec
    screen.addTimerEvent( &timer_event, &timer );
    screen.addEvent( &timer_event );

    std::cout << "** Executing particle-based rendering...\n" << std::endl;

    // Create and show the window
    screen.show();

    return ( app->run() );
} // End mainsub_spbr()

// end of mainsub_brightness_adjustment.cpp