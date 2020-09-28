///////////////////////////
///// spbr_binary.cpp /////
///////////////////////////

#include "spbr.h"
#include "single_inputfile.h" 

#include "spcomment.h"

const unsigned int DEFAULT_REPEAT_LEVEL     = 1 ;
const unsigned int DEFAULT_IMAGE_RESOLUTION = 512 ;
const double       DEFAULT_VIEW_ANGLE       = 45.0 ;
const unsigned int DEFAULT_GRAY_LEVEL       = 128 ;
const unsigned int DEFAULT_BG_GRAY_LEVEL    = 255 ;


SPBR::SPBR( const char* input_file, SPBR_BINARY_FORMAT file_format )  : 
    m_cameraPosition (0.0, 0.0, DEFAULT_CAMERA_DISTANCE) ,
    m_lookAt         (0.0, 0.0, 0.0 )             ,
    m_viewAngle      ( DEFAULT_VIEW_ANGLE )       ,
    m_flagCameraFar  ( false )                    , 
    m_repeatLevel(DEFAULT_REPEAT_LEVEL)           ,
    m_Rb(DEFAULT_GRAY_LEVEL)                      , 
    m_Gb(DEFAULT_GRAY_LEVEL)                      , 
    m_Bb(DEFAULT_GRAY_LEVEL)                      , 
    m_flagReverseNormals   (false)                ,
    m_flagUseNormals       (true)                 , 
    m_flagForcedColor      (false)                ,
    m_flagParticleZoom     (true)                 , 
    m_numParticles(0)                             , 
    m_pointSize(1)                                ,
    m_flagDrawWireframeBox (false)                , 
    m_minWBPosition        ( 0.0, 0.0, 0.0 )      , 
    m_maxWBPosition        ( 0.0, 0.0, 0.0 )      ,  
    m_flagFPS              (false)                , 
    m_BG_Rb(DEFAULT_BG_GRAY_LEVEL)                , 
    m_BG_Gb(DEFAULT_BG_GRAY_LEVEL)                , 
    m_BG_Bb(DEFAULT_BG_GRAY_LEVEL)                ,
    m_imageResolution ( DEFAULT_IMAGE_RESOLUTION ),
    m_flagCoordTransformed (false)                ,
    m_O  (0.0, 0.0, 0.0)                          , 
    m_e1 (1.0, 0.0, 0.0)                          , 
    m_e2 (0.0, 1.0, 0.0)                          , 
    m_e3 (0.0, 0.0, 1.0)                          ,
    m_flagLOD              (true)                 ,
    m_flagObjZXRot         (false)                ,
    m_flagForcedBoundingBox(false)                , 
    m_forcedMinPoint(-1.0, -1.0, -1.0 )           ,
    m_forcedMaxPoint(+1.0, +1.0, +1.0 )           ,
    m_flagForcedStandardBoundingBox(false)        , // ST_BOUNDING_BOX
    m_forcedStandardMinPoint(-1.0, -1.0, -1.0 )   ,
    m_forcedStandardMaxPoint(+1.0, +1.0, +1.0 )   ,
    m_flagShuffle          (true)                 ,
    m_flagForcedShuffle    (false)                ,
    m_flagCameraProjectionType (kvs::Camera::Perspective), 
    m_flagLambertShading (true), 
    m_kd(DEFAULT_PHONG_KD), m_ka(DEFAULT_PHONG_KA), 
    m_ks(DEFAULT_PHONG_KS), m_shininess(DEFAULT_PHONG_SHININESS), 
    m_grayscale_id(0),
    m_unirand(),
    m_mouse_rot_speed(1.0) , //ROTSPEED
    m_mouse_zoom_speed(1.0), //ZOOMSPEED
    m_num_output_ply_column(0)
{
  //---- Message
  //  std::cout << "** " << message_string << std::endl; 
  std::cout <<"** SPBR constructor for SPBR binary format is called." << std::endl;

  // Input file name
  strcpy( m_input_file, input_file );  
  SingleInputFile* p  = SingleInputFile::GetInstance(); 
  p->SetName( m_input_file ); 
  std::cout << "** Binary SPBR file: " << m_input_file << std::endl;

  // Initialize ZX rotation parameters
  m_objZXRotAngle[0] = 0.0;  m_objZXRotAngle[1] = 0.0;  

  // Ceate a point object from a binary input file 
  generatePointObject_Binary( this );
}


//-----
void 
SPBR::generatePointObject_Binary( kvs::PointObject* point_object ) 
{
  // Open file input stream
  m_finBin.open( m_input_file );
  if( !m_finBin ) {
    std::cerr << "ERROR: Cannot open " << m_input_file << std::endl;
    exit(1);
  }

  // Read header
  readHeader_Binary();

  // Read point data
  readPointData_Binary();
  
  // Close output stream
  m_finBin.close();

  // Set bounding box of the pointObject
  setForcedMinMaxCoords();
}


//-----
void 
SPBR::readHeader_Binary()
{
  // Read data
  m_numParticles = 0 ; // clear particle counter
  char       buf[ SPBR_BUF_SIZE ] ;
  char       dummy [64] ; 
  bool       flag_is_end_header_read = false;

  // Check if Binary at 1st Line
  m_finBin.getline( buf, SPBR_BUF_SIZE - 1 , '\n' );
  if ( strncmp( buf, SPBR_BINARY_DATA_COMMAND, strlen(SPBR_BINARY_DATA_COMMAND) ) ) {
    std::cerr << m_input_file << " is not a binary data file" << std::endl;
    exit(1);
  } else {
    std::cout << "** SPBR binary header is detected." << std::endl;
  }


  // Read header commands
  while (  m_finBin.getline( buf, SPBR_BUF_SIZE - 1 , '\n' ) ) {
    
    // Blank line
    if( !strlen (buf) ) { 
      continue ; // skip this line
    } else 
      // Commmand or comment line
    if( buf[0] == '#' ) {
	
      // Command
      if(buf[1] == '/') { 
        // UCHIDA 2019/3/31
        //----- Brightness Adjustment ----- 
        if ( !strncmp( buf, BRIGHTNESS_ADJUSTMENT, strlen(BRIGHTNESS_ADJUSTMENT) ) ) { 
            int flag;
            sscanf ( buf, "%s %d", dummy, &flag );
            setFlagBrightnessAdjustment( flag );
        } else
        //----- Origin ----- [OK]
        if ( !strncmp( buf, ORIGIN_COMMAND, strlen(ORIGIN_COMMAND) ) ) { 
          double Ox, Oy, Oz ;
          sscanf ( buf, "%s %lg %lg %lg", dummy, &Ox, &Oy, &Oz );
          setBodyCoordOrigin( kvs::Vector3d( Ox, Oy, Oz ) );
          m_flagCoordTransformed = true ;
        }  else
        //----- BaseVector ----- [OK]
        if ( !strncmp( buf, BASE_VECTOR_COMMAND, strlen(BASE_VECTOR_COMMAND) ) ) { 
          double e1x, e1y, e1z, e2x, e2y, e2z ;
          sscanf ( buf, "%s %lg %lg %lg %lg %lg %lg", dummy, 
                         &e1x, &e1y, &e1z, &e2x, &e2y, &e2z );
          setBodyCoordBaseVector( kvs::Vector3d( e1x, e1y, e1z ), 
                                  kvs::Vector3d( e2x, e2y, e2z )  );
          m_flagCoordTransformed = true ;
        }  else
        //----- BGColorRGBByte (renamed from BGColorByteRGB) ----- [OK]
        if ( !strncmp( buf, BG_COLOR_BYTE_COMMAND, strlen(BG_COLOR_BYTE_COMMAND) ) ) { 
          unsigned int R, G, B;
          sscanf ( buf, "%s %u %u %u", dummy, &R, &G, &B );
          setBackGroundColor (R, G, B);
	  //          m_BG_Rb = R;  m_BG_Gb = G; m_BG_Bb = B ;
        }  else
        //----- ColorRGBByte (renamed from ColorByteRGB) ----- [OK]
        // NOTE: The ColorRGBByte block must be written before the ColorRGB block
        if ( !strncmp( buf, COLOR_BYTE_COMMAND, strlen(COLOR_BYTE_COMMAND) ) ) { 
	    unsigned int Rb, Gb, Bb;
            sscanf ( buf, "%s %u %u %u", dummy, &Rb, &Gb, &Bb );
            setColor( Rb, Gb, Bb ); // set m_Rb, m_Gb, m_Bb
        } else
        //----- ColorRGB ----- [OK]
        if ( !strncmp( buf, COLOR_COMMAND, strlen(COLOR_COMMAND) ) ) { 
          double R, G, B;
          sscanf ( buf, "%s %lg %lg %lg", dummy, &R, &G, &B );
          setColor( R, G, B ); // set m_Rb, m_Gb, m_Bb
        }  else
        //----- ReverseNormals ----- [OK]
        if ( !strncmp( buf, REVERSE_NORMALS_COMMAND, strlen(REVERSE_NORMALS_COMMAND) ) ) { 
	  int flag ;
          sscanf ( buf, "%s %d", dummy, &flag );
          setFlagReverseNormals ( flag ) ;
        } else
        //----- Shading (renamed from UseNormals) ----- [OK]
        if ( !strncmp( buf, SHADING_COMMAND, strlen(SHADING_COMMAND) ) ) { 
	  int flag = 1 ; // default
          sscanf ( buf, "%s %d", dummy, &flag );
          setFlagUseNormals ( flag ) ;
        } else
        //----- FPS ----- [OK]
        if ( !strncmp( buf, FPS_COMMAND, strlen(FPS_COMMAND) ) ) { 
	  int flag ;
          sscanf ( buf, "%s %d", dummy, &flag );
          setFlagFPS ( flag ) ;
        } else
        //----- LOD ----- [OK]
        if ( !strncmp( buf, LOD_COMMAND, strlen(LOD_COMMAND) ) ) { 
	  int flag ;
          sscanf ( buf, "%s %d", dummy, &flag );
          setFlagLOD ( flag ) ;
        } else
        //----- ImageResolution ----- [OK]
        if ( !strncmp( buf, IMAGE_RESOLUTION_COMMAND, strlen(IMAGE_RESOLUTION_COMMAND) ) ) { 
          unsigned int resolution;
          sscanf ( buf, "%s %u", dummy, &resolution );
          m_imageResolution = resolution;
        }  else
        //----- ParticleZoom ----- [OK]
        if ( !strncmp( buf, PARTICLE_ZOOM_COMMAND, strlen(PARTICLE_ZOOM_COMMAND) ) ) { 
	  int flag ;
          sscanf ( buf, "%s %d", dummy, &flag );
	  setFlagParticleZoom ( flag ) ;
        } else
        //----- RepeatLevel ----- [OK]
        if ( !strncmp( buf, REPEAT_LEVEL_COMMAND, strlen(REPEAT_LEVEL_COMMAND) ) ) { 
            unsigned int repeat_level;
            sscanf ( buf, "%s %u", dummy, &repeat_level );
            this->setRepeatLevel ( repeat_level ) ;
        } else 
        //----- WireframeBox ----- [OK]
        if ( !strncmp( buf, WIREFRAME_BOX_COMMAND, strlen(WIREFRAME_BOX_COMMAND) ) ) { 
          double xmin, ymin, zmin, xmax, ymax, zmax ;
          sscanf ( buf, "%s %lg %lg %lg %lg %lg %lg", dummy, &xmin, &ymin, &zmin, &xmax, &ymax, &zmax ) ;
	  setWireframeBox ( xmin, ymin, zmin, xmax, ymax, zmax ) ;
          m_flagDrawWireframeBox = true ;
        } else 
        //----- BoundingBox ----- [OK]
        if ( !strncmp( buf, BOUNDING_BOX_COMMAND, strlen(BOUNDING_BOX_COMMAND) ) ) { 
          double xmin, ymin, zmin, xmax, ymax, zmax ;
          sscanf ( buf, "%s %lg %lg %lg %lg %lg %lg", dummy, &xmin, &ymin, &zmin, &xmax, &ymax, &zmax ) ;
	  kvs::Vector3f minPoint ( xmin, ymin, zmin );
	  kvs::Vector3f maxPoint ( xmax, ymax, zmax );
	  setBoundingBox ( minPoint, maxPoint ) ;

        } else 
        //----- StandardBoundingBox ----- [OK] //ST_BOUNDING_BOX
        if ( !strncmp( buf, STANDARD_BOUNDING_BOX_COMMAND, strlen(BOUNDING_BOX_COMMAND) ) ) { 
          double xmin, ymin, zmin, xmax, ymax, zmax ;
          sscanf ( buf, "%s %lg %lg %lg %lg %lg %lg", dummy, &xmin, &ymin, &zmin, &xmax, &ymax, &zmax ) ;
	  kvs::Vector3f minPoint ( xmin, ymin, zmin );
	  kvs::Vector3f maxPoint ( xmax, ymax, zmax );
	  setStandardBoundingBox ( minPoint, maxPoint ) ;

        } else 
        //----- CameraPosition ----- [OK]
        if ( !strncmp( buf, CAMERA_POSITION_COMMAND, strlen(CAMERA_POSITION_COMMAND) ) ) { 
          double Ex, Ey, Ez ;
          sscanf ( buf, "%s %lg %lg %lg", dummy, &Ex, &Ey, &Ez );
	  m_cameraPosition.set( Ex, Ey, Ez );
        } else 
        //----- LookAt ----- [OK]
        if ( !strncmp( buf, LOOK_AT_COMMAND, strlen(LOOK_AT_COMMAND) ) ) { 
          double LAx, LAy, LAz ;
          sscanf ( buf, "%s %lg %lg %lg", dummy, &LAx, &LAy, &LAz );
	  m_lookAt.set( LAx, LAy, LAz );
        } else 
        //----- ViewAngle ----- [OK]
        if ( !strncmp( buf, VIEW_ANGLE_COMMAND, strlen(VIEW_ANGLE_COMMAND) ) ) { 
          double angle_deg ;
          sscanf ( buf, "%s %lg", dummy, &angle_deg );
          setViewAngle ( angle_deg );
        }  else
        //----- CameraFar ----- [OK]
        if ( !strncmp( buf, CAMERA_FAR_COMMAND, strlen(CAMERA_FAR_COMMAND) ) ) { 
	  int flag ;
          sscanf ( buf, "%s %d", dummy, &flag );
	  setFlagCameraFar ( flag ) ;
        } else 
        //----- CameraZoom ----- [OK]
        if ( !strncmp( buf, CAMERA_ZOOM_COMMAND, strlen(CAMERA_ZOOM_COMMAND) ) ) { 
          double f_zoom ;
          sscanf ( buf, "%s %lf", dummy, &f_zoom );
	  setCameraZoom ( f_zoom ) ;
        } else 
        //----- ObjectZXRot ----- [OK]
        if ( !strncmp( buf, OBJECT_ZX_ROT_COMMAND, strlen(OBJECT_ZX_ROT_COMMAND) ) ) { 
	  double zrot_angle_deg, xrot_angle_deg ;
          sscanf( buf, "%s %lg %lg", dummy, &zrot_angle_deg, &xrot_angle_deg );

          setObjZXRot ( zrot_angle_deg, xrot_angle_deg ) ;

        } else 
        //----- CameraAngleByAxis ----- [OK] //TANAKA
          //   +X, X: ==> #/ObjectZXRotation -90 -90
          //   +Y, Y: ==> #/ObjectZXRotation 180 -90
          //   +Z, Z: ==> Default
        if ( !strncmp( buf, CAMERA_ANGLE_BY_AXIS_COMMAND, strlen(CAMERA_ANGLE_BY_AXIS_COMMAND) ) ){ 
	  double zrot_angle_deg = 0.0 ;
          double xrot_angle_deg = 0.0 ;
          char axis_name[4];
          bool status;

          // Read the axis
          sscanf( buf, "%s %s", dummy, axis_name );

          // Set proper rotation
          status = setCameraAngleByAxis ( axis_name, &zrot_angle_deg, &xrot_angle_deg );

          // Message
          if( status ) {
	    std::cout << "** Camera is set on axis " << axis_name << std::endl;
	  }

        } else 
        //----- Shuffle ----- [OK]
        if ( !strncmp( buf, SHUFFLE_COMMAND, strlen(SHUFFLE_COMMAND) ) ) { 
	  int flag ;
          sscanf ( buf, "%s %d", dummy, &flag );
	  setFlagShuffle ( flag ) ;
	} else
        //----- ForcedShuffle ----- [OK]
        if ( !strncmp( buf, FORCED_SHUFFLE_COMMAND, strlen(FORCED_SHUFFLE_COMMAND) ) ) { 
	  int flag ;
          sscanf ( buf, "%s %d", dummy, &flag );
	  setFlagForcedShuffle ( flag ) ;
        } else 
        //----- OrhtogonalCamera ----- [OK]
        if ( !strncmp( buf, ORTHOGONAL_CAMERA_COMMAND, strlen(ORTHOGONAL_CAMERA_COMMAND) ) ) { 
          setOrthogonalCamera () ;
        } else 
        //----- PerspectiveCamera ----- [OK]
        if ( !strncmp( buf, PERSPECTIVE_CAMERA_COMMAND, strlen(PERSPECTIVE_CAMERA_COMMAND) ) ) { 
          setPerspectiveCamera () ;
        } else 
        //----- LambertShading [OK]
        if ( !strncmp( buf, LAMBERT_SHADING_COMMAND, strlen(LAMBERT_SHADING_COMMAND) ) ) { 
	  double kd, ka ;
          int num_words = sscanf ( buf, "%s %lg %lg", dummy, &kd, &ka );
          if( num_words == 1 ) { kd = DEFAULT_LAMBERT_KD; ka = DEFAULT_LAMBERT_KA; }
          setLambertShading ( kd, ka ) ;
        } else 
        //----- PhongShading [OK]
        if ( !strncmp( buf, PHONG_SHADING_COMMAND, strlen(PHONG_SHADING_COMMAND) ) ) { 
	  double kd, ka, ks;
          int     shininess;
          int num_words = sscanf ( buf, "%s %lg %lg %lg %d", dummy, &kd, &ka, &ks, &shininess );
          if( num_words == 1 ) { 
            kd = DEFAULT_PHONG_KD; ka = DEFAULT_PHONG_KA; 
            ks = DEFAULT_PHONG_KS; shininess = DEFAULT_PHONG_SHININESS;
          }
          setPhongShading ( kd, ka, ks, shininess ) ;
        } else 
        //----- NumParticles [OK]
        if ( !strncmp( buf, NUM_PARTICLES_COMMAND , strlen(NUM_PARTICLES_COMMAND) ) ) { 
          unsigned long n_tmp;//TANAKA_181230
          sscanf ( buf, "%s %lu", dummy, &n_tmp );//TANAKA_181230
          m_numParticles = (unsigned long)n_tmp; //TANAKA_181230
          std::cout << "** #/NumParticles command is found: ";
          std::cout <<  m_numParticles << " points." << std::endl;
        } else 
        //----- Point size (for OPBR) -----
        if ( !strncmp( buf, POINT_SIZE_COMMAND, strlen(POINT_SIZE_COMMAND) ) ) { 
            unsigned int point_size;
            sscanf ( buf, "%s %u", dummy, &point_size );
            this->setPointSize ( point_size ) ;

        } else 
        //----- Write PLY  -----
        if ( !strncmp( buf, WRITE_PLY_ASCII_COMMAND, strlen(WRITE_PLY_ASCII_COMMAND) ) ) { 
	  unsigned int num_ply_column;
          int narg = sscanf ( buf, "%s %u", dummy, &num_ply_column );
          if( narg < 2 ) { num_ply_column = 6; }
          this->setPlyAsciiOutput(num_ply_column);

        } else 
        //----- Grayscale ----- //GRAYSCALE
        if ( !strncmp( buf, GRAYSCALE_COMMAND, strlen(GRAYSCALE_COMMAND) ) ) { 
            unsigned int grayscale_id = 0;
            sscanf ( buf, "%s %u", dummy, &grayscale_id );
            m_grayscale_id = grayscale_id;
            if( m_grayscale_id > 3 || m_grayscale_id < 0 ) {
              m_grayscale_id = 0;
	    }
        } else 
        //----- Mouse rotation speed -----// //ROTSPEED
        if ( !strncmp( buf, MOUSE_ROT_SPEED_COMMAND, strlen(MOUSE_ROT_SPEED_COMMAND) ) ) { 
            double rot_speed = 1.0; // initialize to the default value
            sscanf ( buf, "%s %lg", dummy, &rot_speed );
            setMouseRotSpeed ( rot_speed );// set m_mouse_rot_speed

        } else 
        //----- Mouse zoom speed -----// //ZOOMSPEED
        if ( !strncmp( buf, MOUSE_ZOOM_SPEED_COMMAND, strlen(MOUSE_ZOOM_SPEED_COMMAND) ) ) { 
            double zoom_speed = 1.0; // initialize to the default value
            sscanf ( buf, "%s %lg", dummy, &zoom_speed );
            setMouseZoomSpeed ( zoom_speed );// set m_mouse_zoom_speed

        } else 
        //---- EndHeader [OK]
        if ( !strncmp ( buf, END_HEADER_COMMAND, strlen(END_HEADER_COMMAND) ) ) {
          flag_is_end_header_read = true ;
          break; 
        } else
        //----------------------- Unknown command
        {
		// Do nothing
        }

      } // if(buf[1] == '/')
	
    } // if( buf[0] == '#' ) 

  }//while

  // Error ==> terminate the program
  if( !flag_is_end_header_read ) {
    std::cerr << "ERROR: Cannot find " << END_HEADER_COMMAND << std::endl;
    exit(1);
  }

}//readHeader_Binary()


//-----
void 
SPBR::readPointData_Binary() 
{

  // Message
  std::cout << "** Global-parameter setting is completed:" << std::endl;
  std::cout << "**   Number of particles : " << m_numParticles << std::endl;
  std::cout << "**   Use of normal vectors: " ;
  std::cout << (m_flagUseNormals ? "Yes": "No") ;
  std::cout << std::endl;
  std::cout << "**   Repeat level         : " << m_repeatLevel << std::endl;

  // ValueArrays for coords, normals
  kvs::ValueArray<kvs::Real32> coords  ( m_numParticles * 3 );
  kvs::Real32* pcoords = coords.pointer(); //point to the top of the array 

  kvs::ValueArray<kvs::Real32> normals ( m_numParticles * 3 );
  kvs::Real32* pnormals = normals.pointer(); //point to the top of the array 

  kvs::ValueArray<kvs::UInt8> colors   ( m_numParticles * 3 );
  kvs::UInt8* pcolors = colors.pointer(); //point to the top of the array 

  kvs::Real32 po[3], pn[3];
  kvs::UInt8 co[3];
  
  // Read data
  for(int i=0; i < m_numParticles; i++) {
    m_finBin.read( (char*)po, sizeof(kvs::Real32)*3 );
    m_finBin.read( (char*)pn, sizeof(kvs::Real32)*3 );
    m_finBin.read( (char*)co, sizeof(kvs::UInt8)*3 );

    // Invert orientation of the normal vector, if required
    if( isNormalsReversed() ) { 
      pn[0] *= (-1.0); pn[1] *= (-1.0); pn[2] *= (-1.0);
    }

    // Overwrite forced color if any
    if ( isForcedColor () ) { 
       co[0] = m_Rb, co[1] = m_Gb, co[2] = m_Bb ;
    }

    // Coordinate transformation if required
    if( m_flagCoordTransformed == true ) {
      double x=po[0], y=po[1], z=po[2],  nx=pn[0], ny=pn[1], nz=pn[2]; 
      doCoordTransformation( &x, &y, &z,  &nx, &ny, &nz );
      po[0] = x ;  po[1] = y ; po[2] = z ;  
      pn[0] = nx;  pn[1] = ny; pn[2] = nz; 
    }

    // GRAYSCALE
    //  grayscale control
    unsigned int gray_b;
    if( getGrayscaleColor ( co[0], co[1], co[2], &gray_b ) ) {
      co[0] = gray_b; co[1] = gray_b; co[2] = gray_b;
    }

    // Save the read point data
    *(pcoords++) = po[0];
    *(pcoords++) = po[1];
    *(pcoords++) = po[2];

    *(pnormals++) = pn[0];
    *(pnormals++) = pn[1];
    *(pnormals++) = pn[2];

    *(pcolors++) = co[0];
    *(pcolors++) = co[1];
    *(pcolors++) = co[2];
  }

  // Set the read data to the point object
  SuperClass::setCoords( coords );
  SuperClass::setNormals( normals );
  SuperClass::setColors ( colors  );
  SuperClass::setSize( m_pointSize );
  SuperClass::updateMinMaxCoords (); 

  // Message
  std::cout << "** PointObject is ready." << std::endl;
  std::cout << "** Area of point cloud: \n  " ;
  std::cout << BOUNDING_BOX_COMMAND << "  ";
  std::cout << minCoord() << "  ";
  std::cout << maxCoord() << std::endl; 

  /********
  // PLY output 
  if( m_num_output_ply_column >= 3 ) {
    writePlyAscii ( coords, normals, colors );
  }
  ********/

}//readPointData_Binary() 

// end of spbr_binary.cpp

