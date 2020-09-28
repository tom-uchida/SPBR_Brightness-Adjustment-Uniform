///////////////////////
///// fpscount.h /////
///////////////////////

#if !defined  FPS_COUNT_HH
#define       FPS_COUNT_HH

#include <kvs/glut/Label>

#include <kvs/Version> //KVS2
#include <sstream> //KVS2

#if KVS_VERSION_MAJOR == 2
  #include <kvs/Scene> //KVS2
  #include <kvs/RendererManager> //KVS2
  #include <kvs/RendererBase> //KVS2
#endif


class FPSLabel : public kvs::glut::Label
{
 public:

 FPSLabel( kvs::ScreenBase* screen ):
  kvs::glut::Label( screen )
    {
      setMargin( 10 );
    }

  void screenUpdated( void )
  {
#if KVS_VERSION_MAJOR == 1
    const kvs::RendererBase* renderer = screen()->rendererManager()->renderer();//KVS1
#elif KVS_VERSION_MAJOR == 2
    const kvs::RendererBase* renderer = static_cast<kvs::glut::Screen*>( screen() )->scene()->rendererManager()->renderer();//KVS2
#endif

    std::stringstream fps;
    fps << std::setprecision(4) << renderer->timer().fps();
    setText( std::string( "fps: " + fps.str() ).c_str() );
  }
};


#endif


