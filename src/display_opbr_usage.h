///////////////////////////////////
///// display_pbsr_commands.h /////
///////////////////////////////////

#if !defined  DISPLAY_OPBR_COMMANDS_HH
#define       DISPLAY_OPBR_COMMANDS_HH

#include <iostream>


//-----
inline void displayOPBR_OpeningMessage ( std::ostream& out = std::cout )
{
  std::cout << std::endl;
  std::cout << OPBR_APP_TITLE << std::endl;
  std::cout << "               "  << SOPBR_VERSION << std::endl;
  std::cout << std::endl;
  std::cout << "                " << DATE << std::endl;
  std::cout << AUTHOR << std::endl;
  std::cout << INSTITUTE << "\n" << std::endl;
}


//-----
inline void displayOPBR_Usage ( std::ostream& out = std::cout )
{
    out << " USAGE : opbr file1.spbr file2.spbr ..." << std::endl;
    out << " HELP : opbr -h"           << std::endl;
    out << std::endl;
}


inline void displayOPBRCommands ( std::ostream& out ) 
{
    out << "Commands:"               << std::endl;
    out << "  #/BaseVector e1.x e1.y e1.z e2.x e2.y e2.z" << std::endl;
    out << "     * Rotation (Set body-coord base vectors in world coord.)" << std::endl;
    out << "  #/BGColorRGBByte (uByte r) (uByte g) (uByte b)" << std::endl;
    out << "  #/BoundingBox xmin ymin zmin  xmax ymax zmax "  << std::endl;
    out << "  #/CameraAngleByAxis axis_name" << std::endl;
    out << "     * axis_name: +X, +Y, +Z" << std::endl;
    out << "  #/CameraFar 1 or 0  (default: 0)" << std::endl;
    out << "     * On/off of very-far camera setting" << std::endl;
    out << "  #/CameraPosition (float x) (float y) (float z)  (default: (0,0,12))" << std::endl;
    out << "  #/CameraZoom (zoom_factor f)  (default: 1.0)" << std::endl;
    out << "  #/ColorRGB (double r) (double g) (double b)" << std::endl;
    out << "  #/ColorRGBByte (uByte r) (uByte g) (uByte b)" << std::endl;
    //    out << "  #/FPS 1 or 0  (default: 0)" << std::endl;
    //    out << "     * On/off of displaying FPS on screen" << std::endl;
    out << "  #/Grayscale ID (ID=1,2,3)" << std::endl;
    out << "     * Forcible grayscale" << std::endl;
    out << "     * 0:Off, 1:NTSC, 2:RGB-average, 3:HDTV" << std::endl;
    out << "  #/ImageResolution (uInt size)"           << std::endl;
    out << "  #/LambertShading ka kd (standard values: 0.5 0.5)"  << std::endl;
    out << "  #/LookAt (float x) (float y) (float z)  (default: (0,0,0))" << std::endl;

    out << "  #/MouseRotSpeed acceleration_factor (default: 1.0)" << std::endl;//ROTSPEED 
    out << "     * Control mouse sensitivity for rotation." << std::endl; 
    out << "  #/MouseZoomSpeed acceleration_factor (default: 1.0)" << std::endl;//ZOOMSPEED
    out << "     * Control mouse sensitivity for zoom." << std::endl; 

    out << "  #/Origin x y z " << std::endl;
    out << "     * Translation (x, y, z)" << std::endl;
    out << "  #/ObjectZXRotation zrot_angle_deg xrot_angle_deg " << std::endl;
    out << "  #/PointSize   (uInt size) " << std::endl;
    out << "  #/Shading 1 or 0 (default: 1)" << std::endl;
    out << "     * On/off of shading" << std::endl;

    //ST_BOUNDING_BOX
    out << "  #/StandardBoundingBox xmin ymin zmin  xmax ymax zmax "  << std::endl;
    out << "     * (xmin, ymin, zmin) = (-1,-1,-1)_st_coord " << std::endl;
    out << "     * (xmax, ymax, zmax) = (+1,+1,+1)_st_coord " << std::endl;
    out << "     * This command is incompatible with #/WireframBox" << std::endl;

    out << "  #/ReverseNormals 1 or 0 (default: 0)"    << std::endl;
    out << "  #/ViewAngle angle_deg (default: 45.0)" << std::endl;
    out << "  #/WireframeBox xmin ymin zmin xmax ymax zmax" << std::endl;
    out << "     * z=zmin: red, z=zmax: blue, z-parallel: green" << std::endl; 
    out << "  #/WritePLY_ASCII num_ply_column" << std::endl; 
    out << "     * Output ASCII PLY-format file" << std::endl; 
    out << "     * num_ply_column = 6: x y z Rb Gb Bb" << std::endl; 
    out << "     *                = 9: x y z nx ny nz Rb Gb Bb" << std::endl; 
    out << std::endl;
}



//-----
inline void displayOPBRDataFormat ( std::ostream& out ) 
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
    out << "Data format 3 (PLY ASCII, 3 column):"  << std::endl;
    out << "element vertex num" << std::endl;
    out << "..."  << std::endl;
    out << "end_header"  << std::endl;
    out << "x y z"   << std::endl;
    out << "..."  << std::endl;
    out << "--------------------" << std::endl;
    out << "Data format 4 (PLY ASCII, 6 column):"  << std::endl;
    out << "element vertex num" << std::endl;
    out << "..."  << std::endl;
    out << "end_header"  << std::endl;
    out << "x y z R_byte G_byte B_byte [A_byte]"   << std::endl;
    out << "..."  << std::endl;
    out << "*Note: A_byte is ignored if exists." << std::endl;
    out << "--------------------" << std::endl;
    out << "Data format 5 (PLY ASCII, 9 column):"  << std::endl;
    out << "element vertex num" << std::endl;
    out << "..."  << std::endl;
    out << "end_header"  << std::endl;
    out << "x y z nx ny nz R_byte G_byte B_byte [A_byte]"   << std::endl;
    out << "..."  << std::endl;
    out << "*Note: A_byte is ignored if exists." << std::endl;
    out << "--------------------" << std::endl;
    out << std::endl;

}// displayOPBRDataFormat()

#endif
// end of display_opbr_usage.h
