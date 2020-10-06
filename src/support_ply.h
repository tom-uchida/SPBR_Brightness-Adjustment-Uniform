/////////////////////////
///// support_ply.h /////
/////////////////////////

#if !defined  SUPPORT_PLY_HH
#define       SUPPORT_PLY_HH

#include<fstream>
#include<cstdio>
#include<cstring>

#include "get_extension.h"
#include "const_ply.h"

const size_t BUF_SIZE = 256;
const size_t WORD_SIZE = 256;

// PLYINPUT
// Type of ply format
enum PLY_PROPERTY_TYPE { XYZ           = 0, 
                         XYZRGB        = 1, 
                         XYZNxNyNz     = 2, 
                         XYZNxNyNzRGB  = 3, 
                         XYZRGBA       = 4, 
                         XYZNxNyNzRGBA = 5 };

//#define DEBUG1

//-----
inline bool is_ply_header (const char * filename)
{
  //resultant status
  bool status = false; //initialization

  // buf
  char buf  [BUF_SIZE];  
  char word [WORD_SIZE]; 

  // input file stream
  std::ifstream fin ;   
  fin.open ( filename ); 

  // Is the file PLY format?
  fin.getline(buf, BUF_SIZE -1, '\n');
  sscanf ( buf, "%s", word );
  status = !strcmp( word, PLY_HEADER );

  // EndFn
  fin.close();
  return status;

}//is_ply_header()


//-----
inline bool search_ply_end_header ( std::ifstream& fin, std::ofstream* fout_p = 0 )
{
  //resultant status
  bool status_found = false; // initialization

  // buf
  char buf  [BUF_SIZE];  
  char word [WORD_SIZE]; 

  // search "end_header"
  while ( fin.getline(buf, BUF_SIZE -1, '\n' ) ) {
    sscanf ( buf, "%s", word );
   
    if( fout_p ) {
      (*fout_p) << buf << std::endl;
    }

    if ( !strcmp( word, PLY_END_HEADER ) ) {
      status_found = true ; //found!
      break;
    }
  }

  // EndFn
  return status_found;

}// search_ply_end_header( ifstream & )


//-----
inline bool 
checkPLY_Extension ( const std::string& filename ) 
{
  //resultant status
  bool is_ply_file_extnsion = false; // initialization

  // Check
  if ( getExtension ( filename ) == PLY_FILE_EXTENSION ) {
    is_ply_file_extnsion = true; //valid file extention!
  } 

  //EndFn
  return is_ply_file_extnsion ;

} // checkSPBR_Extension () 


//-----
inline bool isBINARY_PLY_File(const char * filename)//BINARY
{
  //resultant status
  bool status           = false; // initialization
  bool status_plyheader = false; 
  bool status_format    = false; 
  bool status_binary     = false; 
  bool status_end_header_exist = false; 

  // buf
  char buf    [BUF_SIZE];  
  char word   [WORD_SIZE] ;
  char word1  [WORD_SIZE], word2[WORD_SIZE] ; 

  // input file stream
  std::ifstream fin ;   
  fin.open ( filename ); 

  // Check the 1st line
  fin.getline(buf, BUF_SIZE -1, '\n');
  sscanf ( buf, "%s", word );
  status_plyheader = !strcmp( word, PLY_HEADER );

  // Check the 2nd line
  fin.getline( buf, BUF_SIZE -1, '\n' );
  sscanf ( buf, "%s %s", word1, word2 ); 
  status_format = !strcmp( word1, "format" );
  status_binary  = !strcmp( word2, "binary_little_endian" );//2018/7/30
  status_end_header_exist = search_ply_end_header( fin );

  // Is the file valid PLY binary format?
  status = ( status_plyheader &&\
             status_format    &&\
             status_binary     &&\
             status_end_header_exist );

  // EndFn
  fin.close();
  return status;

}//is_ply_binary()


inline bool isASCII_PLY_File(const char * filename)
{
	//resultant status
	bool status2 = false; // initialization
	bool status_plyheader2 = false;
	bool status_format2 = false;
	bool status_ascii = false;
	bool status_end_header_exist2 = false;

	// buf
	char buf[BUF_SIZE];
	char word[WORD_SIZE];
	char word1[WORD_SIZE], word2[WORD_SIZE];

	// input file stream
	std::ifstream fin;
	fin.open(filename);

	// Check the 1st line
	fin.getline(buf, BUF_SIZE - 1, '\n');
	sscanf(buf, "%s", word);
	status_plyheader2 = !strcmp(word, PLY_HEADER);

	// Check the 2nd line
	fin.getline(buf, BUF_SIZE - 1, '\n');
	sscanf(buf, "%s %s", word1, word2);
	status_format2 = !strcmp(word1, "format");
	status_ascii = !strcmp(word2, "ascii");
	status_end_header_exist2 = search_ply_end_header(fin);

	// Is the file valid PLY ascii format?
	status2 = (status_plyheader2 &&\
		status_format2    &&\
		status_ascii     &&\
		status_end_header_exist2);

	// EndFn
	fin.close();
	return status2;

}//is_ply_ascii()


//-----
inline unsigned long count_ply_vertex ( const char* filename ) //TANAKA_181230
{
  //resultant status
  unsigned long num_vertex = 0 ; // initialization //TANAKA_181230

  // buf
  char buf  [BUF_SIZE];  
  char word1[WORD_SIZE], word2[WORD_SIZE], word3[WORD_SIZE] ; 

  // input file stream
  std::ifstream fin ;   fin.open ( filename ); 

  // search "end_header"
  while ( fin.getline(buf, BUF_SIZE -1, '\n' ) ) {
    int num_words = sscanf ( buf, "%s %s %s", word1, word2, word3 );
    if ( num_words == 3 &&\
         !strcmp( word1, "element" ) &&\
         !strcmp( word2, "vertex"  )   ) 
    {
      sscanf ( word3, "%lu", & num_vertex ); //TANAKA_181230
      std::cout << "** Num. vertex (in PLY header): " << num_vertex << std::endl;
      break;
    }
  }

  // EndFn
  fin.close(); 
  return num_vertex;

}// count_ply_vertex()//TANAKA-SHREWD


//PLYINPUT
//----- 
//inline int count_ply_property(const char* filename)
inline PLY_PROPERTY_TYPE get_ply_property_type (const char* filename)
{
  //Resultant status
  PLY_PROPERTY_TYPE ply_property_type = XYZ;//Candidates:XYZ(RGB(A),NxNyNz(RGB(A))
  //  long int ply_property_type = 0; 

  // word  counter
  int num_words;
	
  // line buffer and word buffers
  char buf[BUF_SIZE];
  char word1[WORD_SIZE], word2[WORD_SIZE], word3[WORD_SIZE];

  // input file stream
  std::ifstream fin;   fin.open(filename);

  // Find line for X: 
  //  It is assumed that 
  //  the first property line is for X, i.e., 
  //  "property float x"
  while (fin.getline(buf, BUF_SIZE - 1, '\n')) {
    sscanf(buf, "%s %s %s", word1, word2, word3);
    if (!strcmp(word1, "property"))//line for X
    {
      break;
    }
  }//while

  //Skip Lines for Y and Z
  fin.getline(buf, BUF_SIZE - 1, '\n');//Y
  fin.getline(buf, BUF_SIZE - 1, '\n');//Z

  // Next, Nx or R?
  //  CASE R : property uchar red ==> Candidates are XYZRGB(A)
  //  CASE Nx: property float nx  ==> //Candidates are XYZNxNyNz(RGB(A))
  fin.getline(buf, BUF_SIZE - 1, '\n');
  num_words = sscanf(buf, "%s %s %s", word1, word2, word3);
  if (num_words == 3 && !strcmp(word1, "property")) {
    if (!strcmp(word3, "red")) {
      ply_property_type = XYZRGB;//Candidates: XYZRGB(A)
    }
    else if(!strcmp(word3, "nx")) {
      ply_property_type = XYZNxNyNz;//Candidates: XYZNxNyNz(RGB(A))
    }
  }

  // Check if the color includes A (alpha)
  if( ply_property_type == XYZRGB ) {//Candidates are XYZRGB(A)
    // Skip lines for G and B
    fin.getline(buf, BUF_SIZE - 1, '\n');//G
    fin.getline(buf, BUF_SIZE - 1, '\n');//B

    // Check if the next line is for A (alpha)
    fin.getline(buf, BUF_SIZE - 1, '\n');
    num_words = sscanf(buf, "%s %s %s", word1, word2, word3);
    if( num_words == 3 && !strcmp(word1, "property") && !strcmp(word3, "alpha") ) {
      ply_property_type = XYZRGBA; // Decided!
    } else {
      // XYZRGB; Decided!
    }
  } else if( ply_property_type == XYZNxNyNz) {//Candidates are XYZNxNyNz(RGB(A))
    // Skip lines for Ny and Nz
    fin.getline(buf, BUF_SIZE - 1, '\n');//Ny
    fin.getline(buf, BUF_SIZE - 1, '\n');//Nz

    //Check if the next line is for R (red)
    fin.getline(buf, BUF_SIZE - 1, '\n');
    num_words = sscanf(buf, "%s %s %s", word1, word2, word3);
    if ( num_words == 3 && !strcmp(word1, "property") && !strcmp(word3, "red") ) {
      ply_property_type = XYZNxNyNzRGB;//Candidates: XYZNxNyNzRGB(A)

      // Skip lines for G and B
      fin.getline(buf, BUF_SIZE - 1, '\n');//G
      fin.getline(buf, BUF_SIZE - 1, '\n');//B

      //Check if the next line is for A (alpha)
      fin.getline(buf, BUF_SIZE - 1, '\n');
      num_words = sscanf(buf, "%s %s %s", word1, word2, word3);
      if( num_words == 3 && !strcmp(word1, "property") && !strcmp(word3, "alpha") ) {
        ply_property_type = XYZNxNyNzRGBA;// Decided!
      } else  {
        // Decided: XYZNxNyNzRGB!
      }
    } else { // No color info
      // Decided: XYZNxNyNz!
    }

  } else { //  No info after XYZ
      // Decided: XYZ!
  }

  //========= 
#if defined DEBUG1
  std::cerr << "@@@@@ PLY_PROPERTY = "  << ply_property_type << std::endl;
#endif
  //=========//

  // EndFn
  fin.close();
  return ply_property_type;

} // get_ply_property_type() 


#endif 
// end of is_ply.h
