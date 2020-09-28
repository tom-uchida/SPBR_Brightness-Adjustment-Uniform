/////////////////////////////
///// mainfun_utility.h /////
/////////////////////////////

#if !defined  MAINFUN_UTILITY_HH
#define       MAINFUN_UTILITY_HH

#include <kvs/Version> //KVS2

#if KVS_VERSION_MAJOR == 1
  #include <kvs/glew/ParticleBasedRenderer> //KVS1
#elif KVS_VERSION_MAJOR == 2
  #include <kvs/ParticleBasedRenderer> //KVS2
#endif

#if KVS_VERSION_MAJOR == 1
  #include <kvs/glew/PointRenderer> //KVS1
#elif KVS_VERSION_MAJOR == 2
  #include <kvs/PointRenderer> //KVS2
#endif

#include <iostream>
#include <kvs/glut/Screen>
#include <kvs/InitializeEventListener>
#include <kvs/KeyPressEventListener>
#include <kvs/Vector3>

#include "camera_info.h"
#include "const_spbr.h"
#include "fpscount.h"
#include "get_extension.h"
#include "spbr.h"
#include "spcomment.h"
#include "toolxform.h"
#include "wireframebox.h"

#include "support_ply.h"//TANAKA 2017/11/05

//-----
#if KVS_VERSION_MAJOR == 1
//KVS1
inline void setShadingType ( SPBR*                              spbr_engine,   
                             kvs::glew::rits::ParticleBasedRenderer* renderer, 
                             std::ostream& out = std::cout                   ) 
#elif KVS_VERSION_MAJOR == 2
//KVS2
inline void setShadingType ( SPBR*                              spbr_engine,   
                             kvs::glsl::ParticleBasedRenderer* renderer  , 
                             std::ostream& out = std::cout                ) 
#endif
{

  // Set Lambert shading
  if( spbr_engine->isLambertShading() ) {
    double ka = spbr_engine->ka(), kd = spbr_engine->kd();
    renderer->setShader ( kvs::Shader::Lambert( ka, kd ) );
    out << "** Lambert Shading: ";
    out << "kd = " << kd << ", ";
    out << "ka = " << ka << std::endl;
  } else {
    out << "** Phong Shading: " ;
    double ka = spbr_engine->ka(), kd = spbr_engine->kd();
    double ks = spbr_engine->ks();
    int    shininess = spbr_engine->shininess();
    renderer->setShader ( kvs::Shader::Phong( ka, kd, ks, (float)shininess ) );
    out << "kd = " << kd << ", ";
    out << "ka = " << ka << ", ";
    out << "ks = " << ks << ", ";
    out << "shininess = " << shininess << std::endl;
  }

}// setShadingType(SPBR)


//-----
#if KVS_VERSION_MAJOR == 1
//KVS1
inline void setShadingType ( SPBR*                              spbr_engine,   
                             kvs::glew::rits::PointRenderer* renderer, 
                             std::ostream& out = std::cout                   ) 
#elif KVS_VERSION_MAJOR == 2
//KVS2
inline void setShadingType ( SPBR*                              spbr_engine,   
                             kvs::glsl::PointRenderer* renderer  , 
                             std::ostream& out = std::cout                ) 
#endif
{

  // Set Lambert shading
  if( spbr_engine->isLambertShading() ) {
    double ka = spbr_engine->ka(), kd = spbr_engine->kd();
    renderer->setShader ( kvs::Shader::Lambert( ka, kd ) );
    out << "** Lambert Shading: ";
    out << "kd = " << kd << ", ";
    out << "ka = " << ka << std::endl;
  } else {
    out << "** Phong Shading: " ;
    double ka = spbr_engine->ka(), kd = spbr_engine->kd();
    double ks = spbr_engine->ks();
    int    shininess = spbr_engine->shininess();
    renderer->setShader ( kvs::Shader::Phong( ka, kd, ks, (float)shininess ) );
    out << "kd = " << kd << ", ";
    out << "ka = " << ka << ", ";
    out << "ks = " << ks << ", ";
    out << "shininess = " << shininess << std::endl;
  }

}// setShadingType(OPBR)


//-----
// Note: This function should be called after visualized objects 
//       are created within spbr_engin and image resolution is set to 
//       screen.
inline void displayParticleSize ( SPBR*               spbr_engine, 
                                  kvs::glut::Screen* screen, 
                                  std::ostream&      out = std::cout )
{
  // Camera database
  CameraInfo cameraInfo ;

  // Bounding box information
  kvs::Vector3f min_coord = spbr_engine->minCoord();
  kvs::Vector3f max_coord = spbr_engine->maxCoord();

  // Set the bounding-box information to the camera database 
  cameraInfo.set( screen->scene()->camera(), 
                  min_coord.x(), min_coord.y(), min_coord.z(), 
                  max_coord.x(), max_coord.y(), max_coord.z() ) ; 

  // Calc particle width
  double particle_diameter = cameraInfo.pixelWidth(screen->scene()->camera());

  // Display particle size (width and cross section)
  std::cout << "** particle_diameter    : " << particle_diameter << std::endl;
  std::cout << "** particle_crosssection: " << particle_diameter * particle_diameter << std::endl;

}// displayParticleSize()


//----- 
inline void setCameraParameters ( SPBR*              spbr_engine, 
                                  kvs::glut::Screen* screen, 
                                  std::ostream&      out = std::cout )

{
  // Set camera type (orthogonal/perspective)
  if( spbr_engine->CameraProjectionType() == kvs::Camera::Orthogonal ) {
    screen->scene()->camera()->setProjectionType ( kvs::Camera::Orthogonal );
  } 
  if( spbr_engine->CameraProjectionType() == kvs::Camera::Perspective ) {
    screen->scene()->camera()->setProjectionType ( kvs::Camera::Perspective );
  } 

  // Set camera parameters 
  //   camera position, look-at position, and view angle
  screen->scene()->camera()->setPosition( spbr_engine->cameraPosition() );
  out << "** Camera position : (" << screen->scene()->camera()->position() ;
  out << ")" << std::endl;
  screen->scene()->camera()->setLookAt  ( spbr_engine->lookAt() );
  out << "** Look-at position: (" << screen->scene()->camera()->lookAt() ;
  out << ")" << std::endl;
  screen->scene()->camera()->setFieldOfView ( spbr_engine->viewAngle() );
  //screen->scene()->camera()->setFieldOfView(viewAngle());
  out << "** View angle      : " << screen->scene()->camera()->fieldOfView() ;
  out << " [deg]" << std::endl;

}//setCameraParameters () 


//----- 
inline void setWindowTitle ( const char* title, 
                             const char* datafile, 
                             kvs::glut::Screen* screen  ) 
{
  // Set window title
  char WINDOW_TITLE[256];
  strcpy( WINDOW_TITLE, title );
//  strcat( WINDOW_TITLE, "  with GLSL" );
  strcat( WINDOW_TITLE, "  (" );
  strcat( WINDOW_TITLE, datafile );
  strcat( WINDOW_TITLE, ")" );
  screen->setTitle( WINDOW_TITLE );

}//setWindowTitle()


//-----
inline void drawFPS( SPBR*               spbr_engine,   
                    kvs::glut::Screen*  screen      ) 
{

  FPSLabel* fps_label = new FPSLabel ( screen );
  if( spbr_engine->isFPSDisplayed()) {
    kvs::RGBColor textcolor( 255 - spbr_engine->bg_Rb() ,
			     255 - spbr_engine->bg_Gb() ,
			     255 - spbr_engine->bg_Bb()  );
    fps_label->setTextColor(textcolor);
    fps_label->show();
  }

}// drawFPS()


//-----
inline void drawWireframeBox ( SPBR*               spbr_engine, 
                               kvs::glut::Screen* screen      ) 
{
  // Create wireframe-box line object if required
  if ( spbr_engine->isDrawWireframeBox() ) {
    // initantiate a line object
    kvs::LineObject* line_object = NULL ;
    line_object  
      = WireframeBox( spbr_engine->wireframeBoxMinPosition(),
                      spbr_engine->wireframeBoxMaxPosition() );

    // registration
    screen->registerObject( line_object );

  }//if(flagDrawWireframeBox())

}// drawWireframeBox()


//ST_BOUNDING_BOX
//-----
inline void addBoundingBoxToScene ( SPBR*  spbr_engine ) 
{
  spbr_engine->updateMinMaxCoords();
  spbr_engine->setForcedMinMaxCoords(); //ST_BOUNDING_BOX
}


//-----
inline bool 
isFileExistent ( const std::string& filename ) 
{
  // local variables
  bool is_file_existent = false;
  //  std::ifstream fin_tmp( filename ) ;
  std::ifstream fin_tmp( filename.c_str() ) ; // TANAKA

  // Check 
  if( !fin_tmp ) {
    is_file_existent = false;
    std::cerr << "ERROR: File  " << filename ;
    std::cerr << "  does not exist." << std::endl;
  } else {
    is_file_existent = true;
  }

  // EndFn
  fin_tmp.close();
  return is_file_existent;

} // isFileExistent ()


//-----
inline bool 
checkSPBR_Extension ( const std::string& filename ) 
{
  // local variables
  bool is_spbr_file_extnsion = false;

  // Check //TANAKA 2017/11/05
  if ( getExtension ( filename ) == SPBR_FILE_EXTENSION ||\
       getExtension ( filename ) == PLY_FILE_EXTENSION      ) {
    is_spbr_file_extnsion = true;

  } else {
    std::cerr << "ERROR: Wrong extension of a given SPBR File: ." ;
    std::cerr << getExtension ( filename ) << std::endl;
    std::cerr << "       Use ."  << SPBR_FILE_EXTENSION;
    std::cerr << "." << std::endl;
  }

  //EndFn
  return is_spbr_file_extnsion ;

} // checkSPBR_Extension () 


//-----
inline bool 
isBinarySPBR_File ( const char* filename )
{
  // local variables
  bool           flag_is_binary_data = false ; // return value
  char           buf[ SPBR_BUF_SIZE ] ;        // line buffer
  std::ifstream  reader ;                      // input file stream

  // Open input file stream
  reader.open ( filename );

  // Read the first line of the given data file
  reader.getline( buf, SPBR_BUF_SIZE - 1 , '\n' );
 
  // Decide if the data file is binary 
  if ( !strncmp( buf, SPBR_BINARY_DATA_COMMAND, strlen(SPBR_BINARY_DATA_COMMAND) ) ) {
     flag_is_binary_data = true ;
  } 

  // EndFn
  reader.close ();
  return flag_is_binary_data;

}// isBinarySPBR_File ()


//-----
inline bool 
isASCII_SPBR_File ( const char* filename )
{
  // local variables
  bool           flag_is_ascii_data = false ; // return value
  char           buf[ SPBR_BUF_SIZE ] ;       // line buffer
  std::ifstream  reader ;                     // input file stream

  // Open input file stream
  reader.open ( filename );

  // Read the first line of the given data file
  reader.getline( buf, SPBR_BUF_SIZE - 1 , '\n' );
 
  // Decide if the data file is binary 
  if ( !strncmp( buf, SPBR_ASCII_DATA_COMMAND, strlen(SPBR_ASCII_DATA_COMMAND) ) ) { 
     flag_is_ascii_data = true ;
  } 

  // EndFn
  reader.close ();
  return flag_is_ascii_data ;

}// isASCII_SPBR_File ()


//-----//TANAKA 2017/11/05
inline bool 
checkInputFiles ( int argc, char** argv ) 
{
  // resultant status
  bool status = false ; // initialization

  // For each input file, ...
  for( int i=2; i<=argc; i++ ) {
    // local
    bool status_spbr, status_ply ;
    
    // Check if the file exists
    status = isFileExistent ( argv[i-1] ) ;
    if (!status ) { break; }

    // Check if the file has the correct extension
    status_spbr = checkSPBR_Extension ( argv[i-1] );
    status_ply  = checkPLY_Extension  ( argv[i-1] );
    status = ( status_spbr || status_ply ) ;
    if (!status ) { break; }

    // Check if the file has one of the correct header cooments
    status = ( isBinarySPBR_File( argv[i-1] ) ||\
               isASCII_SPBR_File( argv[i-1] ) ||\
               isASCII_PLY_File ( argv[i-1] ) ||\
		       isBINARY_PLY_File(argv[i - 1])   );//BINARY
    if (!status ) { 
      std::cerr << "ERROR: Data type of  " << argv[i-1] ;
      std::cerr << "  is unknown." << std::endl;
      break; 
    }

  } // for

  // EndFn
  return status;

} // checkInputFiles ()  


#endif
// end of mainfn_utility.h
