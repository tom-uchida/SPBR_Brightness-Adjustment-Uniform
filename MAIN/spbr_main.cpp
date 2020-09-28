////////////////////////////////////
///// main.cpp (spbr_main.cpp) /////
////////////////////////////////////


#include "spbr.h"
#include "version.h"
#include "display_spbr_usage.h"
#include "mainfn_utility.h"

// Prototype declaration
int  mainsub_spbr_spbrbin( int argc, char** argv );
int  mainsub_spbr_plybin(int argc, char** argv);//2018/8
int  mainsub_spbr_plyascii ( int argc, char** argv );
int  mainsub_spbr_spbrascii ( int argc, char** argv );

// MACRO
//#define DEBUG_PLY

//-----
int main(int argc, char** argv) {
  // Opening Message and Usage
  displaySPBR_OpeningMessage();
  displaySPBR_Usage();

  // Help 
  if (argc >= 2 && !strcmp(argv[1], "-h")) {
    displaySPBRCommands(std::cout);
    displaySPBRDataFormat(std::cout);
    exit(1);
  }
  else if (argc < 2) {
    exit(1);
  }

  // Check validitiy of input files
  if (!checkInputFiles(argc, argv)) {
    exit(1);
  }

  //Call a proper main routine for the input file of arg[1]:
  //  binary SPBR, binary PLY, ASCII PLY, or ASCII SPBR
  if (isBinarySPBR_File(argv[1])) {
    // binary SPBR file 
    //    std::cout << "** Binary SPBR file: " << argv[1] << std::endl;
    mainsub_spbr_spbrbin(argc, argv);
  }
  else if (isBINARY_PLY_File(argv[1])) {
    // binary PLY file
    //    std::cout << "** Binary PLY file: "  << argv[1] << std::endl;
    mainsub_spbr_plybin(argc, argv);
  } 
  else if (isASCII_PLY_File(argv[1])) {//TANAKA 2017/11/05
    // ASCII PLY file
    //    std::cout << "** ASCII PLY file: "   << argv[1] << std::endl;
    mainsub_spbr_plyascii(argc, argv);
  }
  else {
    // ASCII SPBR file
    //    std::cout << "** ASCII SPBR file: "  << argv[1] <<std::endl;
    mainsub_spbr_spbrascii(argc, argv);
  }

  //EndFn
  return 0;

}// main() for SPBR
