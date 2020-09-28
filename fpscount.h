///////////////////////
///// fpscount.h /////
///////////////////////

#if !defined  FPS_COUNT_HH
#define       FPS_COUNT_HH

#include <kvs/Label>

#include <kvs/Version> 
#include <sstream> 

#include <kvs/Scene> 
#include <kvs/RendererManager> 
#include <kvs/RendererBase> 

#include <kvs/glut/Screen> //TANAKA

const int FPS_MARGINE   = 10;
const int FPS_FONT_SIZE = 26;

//---------------------------------------//
class FPSLabel : public kvs::Label
//---------------------------------------//
{
 public:

  FPSLabel( kvs::glut::Screen* screen , kvs::RGBColor textcolor ):
  kvs::Label( screen )
  {
    // Preparation
    setScene( screen->scene() );

    // Margine
    setMargin( FPS_MARGINE );

    // Font 
    kvs::Font font;

    //... (1) Font size
    font.setSize( FPS_FONT_SIZE );

    //... (2) Font color
    // font.setColor( kvs::RGBColor::Black() );
    font.setColor(textcolor);

    // Finalization
     setFont( font );
  }

  void screenUpdated( void )
  {
    //    setText( "FPS: %.2lf", scene()->renderer( "Renderer" )->timer().fps() );

    const kvs::RendererBase* renderer = static_cast<kvs::glut::Screen*>( screen() )->scene()->rendererManager()->renderer();//KVS2
    std::stringstream fps;
    fps << std::setprecision(4) << renderer->timer().fps();
    setText( std::string( "fps: " + fps.str() ).c_str() );
  }
};


#endif


