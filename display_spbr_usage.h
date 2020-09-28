//////////////////////////////////
///// display_spbr_usage.h /////
//////////////////////////////////

#if !defined  DISPLAY_SPBR_COMMANDS_HH
#define       DISPLAY_SPBR_COMMANDS_HH

#include <iostream>

//-----
inline void displaySPBR_OpeningMessage ( std::ostream& out = std::cout )
{
  std::cout << std::endl;
  std::cout << SPBR_APP_TITLE << std::endl;
  std::cout << "               " << SOPBR_VERSION << std::endl;
  std::cout << std::endl;
  std::cout << "                " << DATE << std::endl;
  std::cout << AUTHOR << std::endl;
  std::cout << INSTITUTE << "\n" << std::endl;
}

//-----
inline void displaySPBR_Usage ( std::ostream& out = std::cout )
{
    out << " USAGE : spbr file1.spbr file2.spbr ..." << std::endl;
    out << " HELP  : spbr -h"           << std::endl;
    out << std::endl;
}


//-----
inline void displaySPBRCommands ( std::ostream& out ) 
{
    out << "Commands:"               << std::endl;
    out << "  #/BaseVector e1.x e1.y e1.z e2.x e2.y e2.z" << std::endl;
    out << "     * Rotation (Set body-coord base vectors in world coord.)" << std::endl;
    out << "  #/BGColorRGBByte (uByte r) (uByte g) (uByte b)" << std::endl;
    out << "  #/BoundingBox xmin ymin zmin  xmax ymax zmax "  << std::endl;
    out << "  #/CameraAngleByAxis axis_name" << std::endl;
    out << "     * axis_name: +X, +Y, +Z, -X, -Y, -Z" << std::endl;
    out << "  #/CameraFar 1 or 0  (default: 0)" << std::endl;
    out << "     * On/off of very-far camera setting" << std::endl;
    out << "  #/CameraPosition (float x) (float y) (float z)  (default: (0,0,12))" << std::endl;
    out << "  #/CameraZoom (zoom_factor f)  (default: 1.0)" << std::endl;
    out << "  #/ColorRGB (double r) (double g) (double b)" << std::endl;
    out << "  #/ColorRGBByte (uByte r) (uByte g) (uByte b)" << std::endl;
    out << "  #/EndHeader"  << std::endl;     
    out << "     * End of the header reagion" << std::endl;
    out << "  #/ForcedDefaultCamera 1/0 (default 0)" << std::endl;
    out << "     * All camera setting is initialized." << std::endl;
    out << "  #/ForcedShuffle  1 or 0 (default is 0)"  << std::endl;     
    out << "     * Additional shuffle after reading data" << std::endl;
    out << "  #/FPS 1 or 0  (default: 0)" << std::endl;
    out << "     * On/off of displaying FPS on screen" << std::endl;
    out << "  #/Grayscale ID (ID=0,1,2,3) (default:0)" << std::endl;
    out << "     * Forcible grayscale" << std::endl;
    out << "     * 0:Off, 1:NTSC, 2:RGB-average, 3:HDTV" << std::endl;
    out << "  #/ImageResolution (uInt size) (default: 512)" << std::endl;
    out << "  #/LambertShading [kd ka] (default: 0.6 0.4)"  << std::endl;
    out << "  #/LOD 1 or 0 (on/off of LOD (default: 1))" << std::endl;
    out << "  #/LookAt (float x) (float y) (float z)  (default: (0,0,0))" << std::endl;

    out << "  #/MouseRotSpeed acceleration_factor (default: 1.0)" << std::endl;//ROTSPEED 
    out << "     * Control mouse sensitivity for rotation." << std::endl; 
    out << "  #/MouseZoomSpeed acceleration_factor (default: 1.0)" << std::endl;//ZOOMSPEED
    out << "     * Control mouse sensitivity for zoom." << std::endl; 

    out << "  #/NumParticles  num" << std::endl;
    out << "     * This command makes sense if #/EndHeader command is used together" << std::endl;
    out << "  #/ObjectZXRotation zrot_angle_deg xrot_angle_deg " << std::endl;
    out << "  #/Origin x y z " << std::endl;
    out << "     * Translation (x, y, z)" << std::endl;
    out << "  #/OrthogonalCamera " << std::endl;
    out << "     * Use orthogonal projection for camera" << std::endl;
    out << "  #/ParticleZoom 1 or 0  (default: 1)" << std::endl;
    out << "     * On/off of particle zoom" << std::endl;
    out << "  #/PerspectiveCamera " << std::endl;
    out << "     * Use perspective projection for camera (default setting is ON))" << std::endl;
    out << "  #/PhongShading [kd ka ks shininess] (default: 0.5 0.3 0.8 100)"  << std::endl;
    out << "  #/PointSize   (uInt size) " << std::endl;
    out << "     * Available with -opaque option" << std::endl;    
    out << "  #/RepeatLevel (uInt size) (default: 1)"    << std::endl;
    out << "  #/ReverseNormals 1 or 0 (default: 0)"    << std::endl;
    out << "  #/SPBR_ASCII_Data"  << std::endl;     
    out << "     * Beginning of the header reagion" << std::endl;
    out << "  #/Shading 1 or 0 (default: 1)" << std::endl;
    out << "     * On/off of shading" << std::endl;
    out << "     * The default shading type is Lambert." << std::endl;
    out << "       Use #/LambertShading or #/PhongShading for detailed tuning." << std::endl;
    out << "  #/Shuffle 1 or 0  (default: 1)" << std::endl;
    out << "     * On/off of particle shuffling by a renderer" << std::endl;

    //ST_BOUNDING_BOX
    out << "  #/StandardBoundingBox xmin ymin zmin  xmax ymax zmax "  << std::endl;
    out << "     * (xmin, ymin, zmin) = (-1,-1,-1)_st_coord " << std::endl;
    out << "     * (xmax, ymax, zmax) = (+1,+1,+1)_st_coord " << std::endl;
    out << "     * This command is incompatible with #/WireframBox" << std::endl;

    out << "  #/ViewAngle angle_deg (default: 45.0)" << std::endl;
    out << "  #/WireframeBox xmin ymin zmin xmax ymax zmax" << std::endl;
    out << "     * z=zmin: red, z=zmax: blue, z-parallel: green" << std::endl; 
    out << "  #/WritePLY_ASCII num_ply_column" << std::endl; 
    out << "     * Output ASCII PLY-format file" << std::endl; 
    out << "     * num_ply_column = 6: x y z R_byte G_byte B_byte" << std::endl; 
    out << "     *                = 9: x y z nx ny nz R_byte G_byte B_byte" << std::endl; 
    out << std::endl;

}// displaySPBRCommands()


//PLYINPUT
//-----
inline void displaySPBRDataFormat ( std::ostream& out ) 
{
    out << "--------------------" << std::endl;
    out << "Data format 1 (SPBR ASCII):" << std::endl;
    out << "#/SPBR_ASCII_Data"  << std::endl;
    out << "#/NumParticles num  (optional)"  << std::endl;
    out << "..."                << std::endl;
    out << "#/EndHeader"  << std::endl;
    out << "x y z nx ny nz [R_byte G_byte B_byte]"   << std::endl;
    out << "..."                << std::endl;
    out << "--------------------" << std::endl;
    out << "Data format 2 (SPBR Binary):"  << std::endl;
    out << "#/SPBR_Binary_Data"  << std::endl;
    out << "#/NumParticles num"  << std::endl;
    out << "..."                << std::endl;
    out << "#/EndHeader"  << std::endl;
    out << "x y z nx ny nz R_byte G_byte B_byte"   << std::endl;
    out << "..."                << std::endl;
    out << "--------------------" << std::endl;
    out << "Data format 3 (PLY ASCII/Binary: XYZ):"  << std::endl;
    out << "ply" << std::endl;
    out << "format ascii 1.0 (or format binary_little_endian 1.0)"  << std::endl;
    out << "comment ..." << std::endl;
    out << "element vertex num_points" << std::endl;
    out << "property float x" << std::endl;
    out << "property float y" << std::endl;
    out << "property float z" << std::endl;
    out << "..."                << std::endl;
    out << "end_header"  << std::endl;
    out << "...(ASCII/Binary point data) " << std::endl;
    out << "--------------------" << std::endl;
    out << "Data format 4 (PLY ASCII/Binary: XYZRGB[A]):"  << std::endl;
    out << "ply" << std::endl;
    out << "format ascii 1.0 (or format binary_little_endian 1.0)"  << std::endl;
    out << "comment ..." << std::endl;
    out << "element vertex num_points" << std::endl;
    out << "property float x" << std::endl;
    out << "property float y" << std::endl;
    out << "property float z" << std::endl;
    out << "property uchar red" << std::endl;
    out << "property uchar green" << std::endl;
    out << "property uchar blue" << std::endl;
    out << "[property uchar alpha]" << std::endl;
    out << "..."                << std::endl;
    out << "end_header"  << std::endl;
    out << "...(ASCII/Binary point data) " << std::endl;
    out << "--------------------" << std::endl;
    out << "Data format 5 (PLY ASCII/Binary: XYZNxNyNz):"  << std::endl;
    out << "ply" << std::endl;
    out << "format ascii 1.0 (or format binary_little_endian 1.0)"  << std::endl;
    out << "comment ..." << std::endl;
    out << "element vertex num_points" << std::endl;
    out << "property float x" << std::endl;
    out << "property float y" << std::endl;
    out << "property float z" << std::endl;
    out << "property float nx" << std::endl;
    out << "property float ny" << std::endl;
    out << "property float nz" << std::endl;
    out << "..."                << std::endl;
    out << "end_header"  << std::endl;
    out << "...(ASCII/Binary point data) " << std::endl;
    out << "--------------------" << std::endl;
    out << "Data format 6 (PLY ASCII/Binary: XYZNxNyNzRGB[A]):"  << std::endl;
    out << "ply" << std::endl;
    out << "format ascii 1.0 (or format binary_little_endian 1.0)"  << std::endl;
    out << "comment ..." << std::endl;
    out << "element vertex num_points" << std::endl;
    out << "property float x" << std::endl;
    out << "property float y" << std::endl;
    out << "property float z" << std::endl;
    out << "property float nx" << std::endl;
    out << "property float ny" << std::endl;
    out << "property float nz" << std::endl;
    out << "property uchar red" << std::endl;
    out << "property uchar green" << std::endl;
    out << "property uchar blue" << std::endl;
    out << "[property uchar alpha]" << std::endl;
    out << "..."                << std::endl;
    out << "end_header"  << std::endl;
    out << "...(ASCII/Binary point data) " << std::endl;
    out << "--------------------" << std::endl;

}// displaySPBRDataFormat()

#endif
// end of display_spbr_usage.h
