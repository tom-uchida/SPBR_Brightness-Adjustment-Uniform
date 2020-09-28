///////////////////////
///// spcomment.h /////
///////////////////////

#if !defined  SPECIAL_COMMENT_HH
#define       SPECIAL_COMMENT_HH


// #/Origin O.x O.y O.z
const char ORIGIN_COMMAND           [] = "#/Origin";

// #/BGColorRGBByte (uByte r) (uByte g) (uByte b) 
const char BG_COLOR_BYTE_COMMAND    [] = "#/BGColorRGBByte" ;//RENAMED from #/BGColorByteRGB

// #/BaseVector e1.x e1.y e1.z e2.x e2.y e2.z 
const char BASE_VECTOR_COMMAND      [] = "#/BaseVector";

// #/ColorRGBByte (uByte r) (uByte g) (uByte b)
const char COLOR_BYTE_COMMAND       [] = "#/ColorRGBByte" ;//FORCED COLOR, RENAMED from #/ColorByteRGB, 

//GRAYSCALE
// #/Grayscale id (id=1,2,3): Forcible grayscale //TANAKA-SHREWD
//  ID = 0 (Off), 1 (NTSC), 2 (RGB-average), 3 (HDTV)
const char GRAYSCALE_COMMAND[] = "#/Grayscale";

// #/ColorRGB (double r) (double g) (double b)
const char COLOR_COMMAND            [] = "#/ColorRGB" ; //FORCED COLOR

// #/ForcedShuffle  1 or 0 (default: 0)
const char FORCED_SHUFFLE_COMMAND       [] = "#/ForcedShuffle" ;

// #/UseKVSDefaultCamera 1/0 (default 0) 
//const char USE_KVS_DEFAULT_CAMERA_COMMAND [] = "#/UseKVSDefaultCamera" ;//ABOLISHED

// #/Shading 1 or 0  (default: 1)
const char SHADING_COMMAND       [] = "#/Shading" ;//RENAMED from #/UseNormals

// #/PointSize (uInt size)
const char POINT_SIZE_COMMAND       [] = "#/PointSize" ;

// #/RepeatLevel (uInt size) (default: 1)
const char REPEAT_LEVEL_COMMAND     [] = "#/RepeatLevel" ;

// #/ImageResolution (uInt size)
const char IMAGE_RESOLUTION_COMMAND [] = "#/ImageResolution" ;

// #/WireframeBox  xmin ymin zmin xmax ymax zmax 
const char WIREFRAME_BOX_COMMAND    [] = "#/WireframeBox" ;

// #/FPS 1 or 0 (default: 0) 
const char FPS_COMMAND              [] = "#/FPS" ;

// #/LOD 1 or 0  (default: 1)
const char LOD_COMMAND              [] = "#/LOD" ;

// #/CameraPosition  x y z  (default: (0,0,12))
const char CAMERA_POSITION_COMMAND  [] = "#/CameraPosition" ;

// #/LookAt          x y z  (default: (0,0,0))
const char LOOK_AT_COMMAND          [] = "#/LookAt" ;

// #/ParticleZoom 1 or 0  (default: 1)
const char PARTICLE_ZOOM_COMMAND [] = "#/ParticleZoom" ;

// #/CameraFar 1 or 0 (default: 0)
const char CAMERA_FAR_COMMAND [] = "#/CameraFar" ;

// #/ViewAngle (double angle_deg) (default: 0)
const char VIEW_ANGLE_COMMAND [] = "#/ViewAngle" ;

// #/ObjectZXRotation (double zrot_angle_deg double xrot_angle_deg ) 
//                     (default: 0 0)
const char OBJECT_ZX_ROT_COMMAND [] = "#/ObjectZXRotation" ;

// #/BoundingBox xmin ymin zmin  xmax ymax zmax
const char BOUNDING_BOX_COMMAND [] = "#/BoundingBox" ;

//ST_BOUNDING_BOX
// #/StandardBoundingBox xmin ymin zmin  xmax ymax zmax
// (xmin, ymin, zmin) = (-1,-1,-1)_standard_coord 
// (xmax, ymax, zmax) = (+1,+1,+1)_standard_coord 
const char STANDARD_BOUNDING_BOX_COMMAND [] = "#/StandardBoundingBox" ;

// #/ReverseNormals 1 or 0 (default: 0)
const char REVERSE_NORMALS_COMMAND [] = "#/ReverseNormals" ;

// #/Shuffle 1 or 0 (default: 1)
const char SHUFFLE_COMMAND [] = "#/Shuffle" ;

// #/UseParticleByParticleColorByteRGB (default: 1)
//const char USE_PARTICLE_BY_PARTICLE_COLOR_BYTE_RGB [] = "#/UseParticleByParticleColorByteRGB" ;//ABOLISHED

// #/LambertShading kd ka (default: 0.6, 0.4)
//   Default shading type
const char LAMBERT_SHADING_COMMAND [] = "#/LambertShading";//ARG CHANGED

// #/PhongShading kd ka ks s (default: 0.5, 0.3, 0.8, 100)
const char PHONG_SHADING_COMMAND [] = "#/PhongShading" ; // NEW

// #/CameraZoom zoom_factor (default: 1.0)
const char CAMERA_ZOOM_COMMAND [] = "#/CameraZoom" ;  

// #/OrthogonalCamera 
const char ORTHOGONAL_CAMERA_COMMAND [] = "#/OrthogonalCamera" ;  

// #/PerspectiveCamera 
const char PERSPECTIVE_CAMERA_COMMAND [] = "#/PerspectiveCamera" ;  

// #/NumParticles: Number of rendered 3D Points 
const char NUM_PARTICLES_COMMAND [] = "#/NumParticles";//COMMON TO ASCII AND BIN

// #/SPBR_ASCII_Data: 1st line of an ascii data file
const char SPBR_ASCII_DATA_COMMAND [] = "#/SPBR_ASCII_Data" ;//NEW

// #/SPBR_Binary_Data: 1st line of a binary data file
const char SPBR_BINARY_DATA_COMMAND [] = "#/SPBR_Binary_Data" ;//NEW

// #/EndHeader: common to ASCII and Binary modes 
const char END_HEADER_COMMAND [] = "#/EndHeader" ; //COMMON TO ASCII AND BIN

// #/WritePLY_ASCII num_ply_column
// num_ply_column
//   6: x y z r g b
//   9: x y z nx ny nz Rb Gb Bb
const char WRITE_PLY_ASCII_COMMAND [] = "#/WritePLY_ASCII" ; 

// #/CameraAngleByAxis axis
//  axis: 
//   +X, X: #/ObjectZXRotation -90 -90
//   +Y, Y: #/ObjectZXRotation 180 -90
//   +Z, Z: Default
//   -X   : #/ObjectZXRotation +90 -90
//   -Y   : #/ObjectZXRotation   0 -90
//   -Z   : #/ObjectZXRotation 180 180
const char CAMERA_ANGLE_BY_AXIS_COMMAND [] = "#/CameraAngleByAxis" ; 

//const char SPEED_CONTROL_COMMAND[] = "#/SpeedControl";

// #/MouseRotSpeed value (default:1.0) 
const char MOUSE_ROT_SPEED_COMMAND[] = "#/MouseRotSpeed";// 

// #/MouseZoomSpeed value (default:1.0) 
const char MOUSE_ZOOM_SPEED_COMMAND[] = "#/MouseZoomSpeed";// ZOOMSPEED


/////shrewd
const char ROTATION_CONTROL_COMMAND[] = "#/RotationControl";
const char MOVE_CONTROL_COMMAND[] = "#/MoveControl";
const char UPDOWN_CONTROL_COMMAND[] = "#/UpDownControl";

const char CAMERA_OBJECT_COMMAND[] = "#/CameraObject";
////shrewd


// UCHIDA 2019/03/31
// #/BrightnessAdjustment 1 or 0 (default: 0)
const char BRIGHTNESS_ADJUSTMENT[] = "#/BrightnessAdjustment"; 

// end of spcomment.h
#endif
