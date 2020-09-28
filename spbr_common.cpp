//////////////////////////////
///// spbr_spbrascii.cpp /////
//////////////////////////////

#include "spbr.h"

const double MOUSE_MIN_SPEED = 0.01 ;

//-----
void 
SPBR::setCameraPosition ( double Ex, double Ey, double Ez )
{
  m_cameraPosition.set( Ex, Ey, Ez );
  std::cout << "** Camera position : (" << Ex << "," << Ey << "," << Ez ;
  std::cout << ")" << std::endl;
}


//-----
void 
SPBR::setLookAt ( double LAx, double LAy, double LAz )
{
  m_lookAt.set( LAx, LAy, LAz );
  std::cout << "** Look-at position: (" << LAx << "," << LAy << "," << LAz;
  std::cout << ")" << std::endl;
}


//-----
void 
SPBR::setViewAngle ( double angle_deg )
{
  m_viewAngle = angle_deg ;
  std::cout << "** View-angle      : " << m_viewAngle << " [deg]" << std::endl;
}


//----- 
void 
SPBR::setFlagCameraFar ( int flag ) 
{
  if( flag ) { 
    m_flagCameraFar = true ;  
    setCameraPosition ( 0.0, 0.0, CAMERA_FAR_DISTANCE )  ;  
    setViewAngle ( CAMERA_FAR_VIEW_ANGLE );
    std::cout << "** Camera distance is made very far." << std::endl;
  }
  else       { 
    m_flagCameraFar = false ;  
  }

}


//-----
void 
SPBR::setCameraZoom ( double f_zoom ) 
{
  double d0 = DEFAULT_CAMERA_DISTANCE; 
  double d  = d0 / fabs(f_zoom) ;

  setCameraPosition ( 0.0, 0.0, d )  ;  
  std::cout << "** Camera distance is reset to " << d << std::endl;
}


//-----
void 
SPBR::setColor ( double R, double G, double B )
{
  this->setColor( (unsigned int)(255.0 * R), 
                  (unsigned int)(255.0 * G), 
                  (unsigned int)(255.0 * B) ) ;
}

//-----
void 
SPBR::setColor ( unsigned int Rb, unsigned int Gb, unsigned int Bb )
{
  setFlagForcedColor ( true ); //FORCED_COLOR

  m_Rb = Rb ; if ( m_Rb > 255 ) { m_Rb = 255 ;}
  m_Gb = Gb ; if ( m_Gb > 255 ) { m_Gb = 255 ;}
  m_Bb = Bb ; if ( m_Bb > 255 ) { m_Bb = 255 ;}

#if defined DEBUG_COLOR
  std::cout << "** Color (byte) is set to (" ;
  std::cout << m_Rb << " " << m_Gb << " " << m_Bb;
  std::cout << ")."  << std::endl;
#endif

}

//-----
void 
SPBR::setFlagUseNormals ( int flag ) 
{
  if( flag ) { 
    m_flagUseNormals = true;  

#if defined DEBUG_SET_NORMALS
    std::cout << "** Use normal vectors for shading." ;
    std::cout << std::endl;
#endif

  }
  else       { 
    m_flagUseNormals = false; 

#if defined DEBUG_SET_NORMALS
    std::cout << "** Do not use normal vectors for shading." ;
    std::cout << std::endl;
#endif
  }

}

//-----
void 
SPBR::setFlagFPS ( int flag ) 
{
  if( flag ) { 
    m_flagFPS = true;  
    std::cout << "** Display FPS on screen." ;
    std::cout << std::endl;
  }
  else       { 
    m_flagFPS = false; 
    std::cout << "** Do not display FPS on screen." ;
    std::cout << std::endl;
  }

}


//-----
void 
SPBR::setFlagLOD ( int flag ) 
{
  if( flag ) { 
    m_flagLOD = true;  
    std::cout << "** LOD is ON" << std::endl;
  }
  else       { 
    m_flagLOD = false ;  
    std::cout << "** LOD is OFF" << std::endl;
  }

}


//-----
void 
SPBR::setFlagShuffle ( int flag ) 
{
  if( flag ) { 
    m_flagShuffle = true;  
    std::cout << "** Particle shuffling is ON" << std::endl;
  }
  else       { 
    m_flagShuffle = false ;  
    std::cout << "** Particle shuffling is OFF" << std::endl;
  }

}


//----- //TANAKA
void 
SPBR::setFlagForcedShuffle ( int flag ) 
{
  if( flag ) { 
    m_flagForcedShuffle = true;  
    std::cout << "** Forced particle shuffling is ON" << std::endl;
  }
  else       { 
    m_flagForcedShuffle = false ;  
    std::cout << "** Forced particle shuffling is OFF" << std::endl;
  }

}


//-----
void 
SPBR::setPointSize ( unsigned int size ) 
{ 
  m_pointSize = size ;
  if ( m_pointSize < 1 ) {  m_pointSize = 1 ; }

  std::cout << "** Point size is set to \"" ;
  std::cout << m_pointSize << "\"." << std::endl;
  std::cout << "   (Used in OPBR and ignored in SPBR.)" <<  std::endl;
}


//-----
void 
SPBR::setRepeatLevel  ( unsigned int level ) 
{ 
  m_repeatLevel = level; 
  if ( m_repeatLevel < 1 ) {  m_repeatLevel = 1 ; }

  std::cout << "** Repeat level is set to \"" ;
  std::cout << m_repeatLevel << "\"." <<  std::endl;

}

//-----
void 
SPBR::setWireframeBox   ( double xmin, double ymin, double zmin, 
                              double xmax, double ymax, double zmax  )
{
  m_minWBPosition.set( xmin, ymin, zmin );
  m_maxWBPosition.set( xmax, ymax, zmax );
  std::cout << "** Wireframe box is set to: " ;
  std::cout << m_minWBPosition << "  " << m_maxWBPosition << std::endl;
  std::cout << "   (z=zmin: red, z=zmax: blue, z-parallel: green)" << std::endl; 
}




//-----
void 
SPBR::normalizeVector( double* x_p, double* y_p, double* z_p  )
{
  // x, y, z, and norm
  double x = *x_p; 
  double y = *y_p; 
  double z = *z_p; 
  double norm = sqrt( x*x + y*y + z*z );

  // normalization
  if( norm > 0 ) {
    *x_p /= norm;  *y_p /= norm ;  *z_p /= norm ;  
  } 

}

//-----
void 
SPBR::setBodyCoordOrigin ( const kvs::Vector3d& origin ) 
{
  m_O = origin ;
} 

//-----
void   
SPBR::setBodyCoordBaseVector ( const kvs::Vector3d& e1, 
                                     const kvs::Vector3d& e2 )
{
  // e1, e1
  m_e1 = e1       ; m_e2 = e2;
  m_e1.normalize(); m_e2.normalize();

  m_e3 = m_e1.cross (m_e2) ;
  m_e3.normalize();
} 


//-----
kvs::Vector3d 
SPBR::doCoordTransformation( const kvs::Vector3d& P )
{
  // calc 
  kvs::Vector3d PP = (P.x() * m_e1) + (P.y() * m_e2) + (P.z() * m_e3) + m_O ;

  // end
  return PP ;
}  

//-----
void  
SPBR::doCoordTransformation( double *x, double* y, double* z,  
                                  double *nx, double* ny, double* nz )
{
  //Original body-coord orgin, position, and normal vectors
  kvs::Vector3d O  ( 0.0, 0.0, 0.0 );
  kvs::Vector3d P  ( *x , *y , *z  ); 
  kvs::Vector3d N  ( *nx, *ny, *nz ); 

  //Transformed body-coord orgin, position, and normal vectors
  kvs::Vector3d OO = doCoordTransformation ( O ) ; 
  kvs::Vector3d PP = doCoordTransformation ( P ) ; 
  kvs::Vector3d NN = doCoordTransformation ( N ) ; 
  NN -= OO ; 

  //Results
  // ... Position
  *x = PP.x(); 
  *y = PP.y(); 
  *z = PP.z(); 

  // ... Normal vectors
  *nx = NN.x(); 
  *ny = NN.y(); 
  *nz = NN.z(); 
}  


//-----
void 
SPBR::setObjZXRot ( double zrot_angle_deg, double xrot_angle_deg ) 
{
  // set angle
  m_objZXRotAngle[0] = zrot_angle_deg ; 
  m_objZXRotAngle[1] = xrot_angle_deg ; 

  // set flag
  m_flagObjZXRot = true ;

  // Message
  std::cout << "** RotateZ(" <<  zrot_angle_deg << " deg) ==> ";
  std::cout << "RotateX(" <<  xrot_angle_deg << " deg)" << std::endl;
}


//-----
void   
SPBR::setBoundingBox ( const kvs::Vector3f& minPoint, 
                             const kvs::Vector3f& maxPoint )   

{
  m_flagForcedBoundingBox = true ;
  m_forcedMinPoint = minPoint ;
  m_forcedMaxPoint = maxPoint ;  

  // Message 
  std::cout << "** Forced bounding box is set to:\n  ";
  std::cout << "(" <<   m_forcedMinPoint << ") - ";
  std::cout << "(" <<   m_forcedMaxPoint << ")" << std::endl;
}  


//ST_BOUNDING_BOX
//-----
void   
SPBR::setStandardBoundingBox ( const kvs::Vector3f& minPoint, 
                               const kvs::Vector3f& maxPoint )   

{
  // Set the bounding box information
  m_flagForcedStandardBoundingBox = true ;
  m_forcedStandardMinPoint = minPoint ;
  m_forcedStandardMaxPoint = maxPoint ;  

  // Message 
  std::cout << "** Forced standard bounding box is set to:\n  ";
  std::cout << "(" <<   m_forcedStandardMinPoint << ") - ";
  std::cout << "(" <<   m_forcedStandardMaxPoint << ")" << std::endl;
}  


//ST_BOUNDING_BOX
//-----
void 
SPBR::setForcedMinMaxCoords( void )
{
  // Forced bounding box
  if( isForcedBoundingBox() ) { 
    kvs::Vector3f min = forcedMinPoint() ;
    kvs::Vector3f max = forcedMaxPoint() ;

    kvs::PointObject::setMinMaxObjectCoords   ( min, max );
    kvs::PointObject::setMinMaxExternalCoords ( min, max );
  }

  //ST_BOUNDING_BOX
  // Forced standard bounding box
  if( isForcedStandardBoundingBox() ) { 
    // Get the current bounding box
    kvs::Vector3f min_org = this->minObjectCoord() ;
    kvs::Vector3f max_org = this->maxObjectCoord() ;

    // Scaling factor to transform the standard coords to
    // the world coords: 
    // (max_world.x - min_world.x)/ (1.0 - (-1.0))
    double scale_x = 0.5 * std::fabs (max_org.x() -  min_org.x()) ;
    double scale_y = 0.5 * std::fabs (max_org.y() -  min_org.y()) ;
    double scale_z = 0.5 * std::fabs (max_org.z() -  min_org.z()) ;

    // Get the user-defined min and max points 
    //  in the standard coords.
    kvs::Vector3f min_st = forcedStandardMinPoint() ;
    kvs::Vector3f max_st = forcedStandardMaxPoint() ;

    // Calc the user-defined min and max points
    //  in the world coords.
    double min_x  = min_org.x() + scale_x * (min_st.x() - (-1.0));
    double min_y  = min_org.y() + scale_y * (min_st.y() - (-1.0));
    double min_z  = min_org.z() + scale_z * (min_st.z() - (-1.0));

    double max_x  = min_org.x() + scale_x * (max_st.x() - (-1.0));
    double max_y  = min_org.y() + scale_y * (max_st.y() - (-1.0));
    double max_z  = min_org.z() + scale_z * (max_st.z() - (-1.0));

    kvs::Vector3f min (min_x, min_y, min_z);
    kvs::Vector3f max (max_x, max_y, max_z);

    // Set the user-defined min and max points to the scene.
    kvs::PointObject::setMinMaxObjectCoords   ( min, max );
    kvs::PointObject::setMinMaxExternalCoords ( min, max );
  }


}

//-----
void 
SPBR::setFlagParticleZoom ( int flag ) 
{
  // Set flag  m_flagParticleZoom
  if( flag ) { 
    m_flagParticleZoom = true;  
    std::cout << "** Particle zoom is ON." ;
    std::cout << std::endl;
  }
  else       { 
    m_flagParticleZoom = false; 
    std::cout << "** Particle zoom is OFF." ;
    std::cout << std::endl;
  }

}


//-----
void 
SPBR::setLambertShading ( double kd, double ka ) 
{ 
  m_flagLambertShading = true ;
  m_kd = kd ; m_ka = ka;

  if ( m_kd < 0.0 ) { m_kd = 0.0 ;}
  if ( m_kd > 1.0 ) { m_kd = 1.0 ;}

  if ( m_ka < 0.0 ) { m_ka = 0.0 ;}
  if ( m_ka > 1.0 ) { m_ka = 1.0 ;}
}


//-----
void 
SPBR::setPhongShading( double kd, double ka, double ks, int shininess )  

{ 
  m_flagLambertShading = false ;
  m_kd = kd ; m_ka = ka;
  m_ks = ks ; m_shininess = shininess;

  if ( m_kd < 0.0 ) { m_kd = 0.0 ;}
  if ( m_kd > 1.0 ) { m_kd = 1.0 ;}

  if ( m_ka < 0.0 ) { m_ka = 0.0 ;}
  if ( m_ka > 1.0 ) { m_ka = 1.0 ;}

  if ( m_ks < 0.0 ) { m_ks = 0.0 ;}
  if ( m_ks > 1.0 ) { m_ks = 1.0 ;}

  if ( m_shininess < 1 ) { m_shininess = 1;}  

}

//-----
bool SPBR::setCameraAngleByAxis ( char* axis_name, double* zrot_angle_deg, double* xrot_angle_deg ) 
{
  // local variables
  bool is_angle_set = false ;

  // Set proper rotation
  //   Camera from +X 
  if( !strcmp( axis_name, "+X") || !strcmp( axis_name, "X") || 
      !strcmp( axis_name, "+x") || !strcmp( axis_name, "x")    ) {
    *zrot_angle_deg = -90.0; *xrot_angle_deg = -90.0;
    is_angle_set = true ;
    setObjZXRot ( *zrot_angle_deg, *xrot_angle_deg ) ;          

  } else 
  //  Camera from +Y [OK]
  if( !strcmp( axis_name, "+Y") || !strcmp( axis_name, "Y") || 
      !strcmp( axis_name, "+y") || !strcmp( axis_name, "y")    ) {
    *zrot_angle_deg = 180.0; *xrot_angle_deg = -90.0;
    is_angle_set = true ;
    setObjZXRot ( *zrot_angle_deg, *xrot_angle_deg ) ;
  } else 
  //  Camera from +Z [OK] 
  if( !strcmp( axis_name, "+Z") || !strcmp( axis_name, "Z") || 
      !strcmp( axis_name, "+z") || !strcmp( axis_name, "z")    ) {
      // Do nothing
    is_angle_set = false ;
  } else 
  //  Camera from -X [OK]
  if( !strcmp( axis_name, "-X") || !strcmp( axis_name, "-x") ) {
      // Do nothing
    *zrot_angle_deg = +90.0; *xrot_angle_deg = -90.0;
    is_angle_set = true ;
    setObjZXRot ( *zrot_angle_deg, *xrot_angle_deg ) ;
  } else 
  //  Camera from -Y [OK]
  if( !strcmp( axis_name, "-Y") || !strcmp( axis_name, "-y") ) {
      // Do nothing
    *zrot_angle_deg = 0.0; *xrot_angle_deg = -90.0;
    is_angle_set = true ;
    setObjZXRot ( *zrot_angle_deg, *xrot_angle_deg ) ;
  } else 
  //  Camera from -Z 
  if( !strcmp( axis_name, "-Z") || !strcmp( axis_name, "-z") ) {
      // Do nothing
    *zrot_angle_deg = 180.0; *xrot_angle_deg = 180.0;
    is_angle_set = true ;
    setObjZXRot ( *zrot_angle_deg, *xrot_angle_deg ) ;
  } 

  //EndFn
  return is_angle_set;

} // setCameraAngleByAxis()


// GRAYSCALE
//-----
bool SPBR::getGrayscaleColor ( unsigned int Rb, unsigned int Gb, unsigned int Bb, 
                              unsigned int * pGb                                )
{
  // local 
  bool   gray_status = false; // initialized to non-grayscale (color)
  double R_tmp = (double)Rb, G_tmp = (double)Gb, B_tmp = (double)Bb;
  double gray_tmp = R_tmp; // initialization

  if (m_grayscale_id == 3) {
    //HDTV
    R_tmp = pow(R_tmp, 1.075)*0.222015;
    G_tmp = pow(G_tmp, 1.075)*0.706655;
    B_tmp = pow(B_tmp, 1.075)*0.071330;
    
    gray_tmp = pow(R_tmp + G_tmp + B_tmp, 1.0 / 1.075);

    gray_status = true;

  }
  else if (m_grayscale_id == 1) {
    //NTSC
    gray_tmp = (0.298912*R_tmp + 0.586611*G_tmp + 0.114478*B_tmp);
    gray_status = true;
  }
  else if (m_grayscale_id == 2) {
    //RGB Average
    gray_tmp = (R_tmp + B_tmp + G_tmp) / 3.0;
    gray_status = true;
  }
  else if(m_grayscale_id == 0) {
    // Color: Do nothing
    gray_tmp = R_tmp ;
    gray_status = false;
  }

  // Set result  
  if (gray_status) {
    *pGb = (unsigned int) gray_tmp;
  }

  // EndFn
  return gray_status;

}//getGrayscaleColor()

/****
	  //GRAYSCALE
	  if (m_grayscale == 3) {
		   ///HDTV 係数による加重平均と補正
		  Rb = pow(Rb, 1.075)*0.222015;
		  Gb = pow(Gb, 1.075)*0.706655;
		  Bb = pow(Bb, 1.075)*0.071330;
		  *(pcolors++) = pow(Rb + Gb + Bb, 1 / 1.075);
		  *(pcolors++) = pow(Rb + Gb + Bb, 1 / 1.075);
		  *(pcolors++) = pow(Rb + Gb + Bb, 1 / 1.075);
	  }
	  else if (m_grayscale == 1) {
		  /// NTSC 係数による加重平均法
		*(pcolors++) = (0.298912*Rb+ 0.586611*Gb+ 0.114478*Bb);
		*(pcolors++) = (0.298912*Rb + 0.586611*Gb + 0.114478*Bb);
		*(pcolors++) = (0.298912*Rb + 0.586611*Gb + 0.114478*Bb);
	  }
	  else if (m_grayscale == 2) {
		  //// 単純平均法
		  *(pcolors++) = (Rb + Bb + Gb) / 3;
		  *(pcolors++) = (Rb + Bb + Gb) / 3;
		  *(pcolors++) = (Rb + Bb + Gb) / 3;
	  }
	  else if(m_grayscale == 0) {
		  *(pcolors++) = Rb;
		  *(pcolors++) = Gb;
		  *(pcolors++) = Bb;
	  }
**/


//-----
void SPBR::setMouseRotSpeed ( double mouse_rot_speed ) //ROTSPEED
{
  m_mouse_rot_speed = mouse_rot_speed ;
  if( m_mouse_rot_speed < MOUSE_MIN_SPEED ) { m_mouse_rot_speed = MOUSE_MIN_SPEED; }
}

//-----
void SPBR::setMouseZoomSpeed ( double mouse_zoom_speed ) //ZOOMSPEED
{
  m_mouse_zoom_speed = mouse_zoom_speed ;
  if( m_mouse_zoom_speed < MOUSE_MIN_SPEED ) { m_mouse_zoom_speed = MOUSE_MIN_SPEED; }
}

