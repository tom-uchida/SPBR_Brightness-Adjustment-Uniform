///////////////////////
///// const_ply.h /////
///////////////////////

#if !defined  CONST_PLY_HH
#define       CONST_PLY_HH

//TANAKA-SHREWD
#include"os_select.h"
//  for Mac/Linux
#if defined OS_UNIX
const char SPBR_PARAM_FILE[] = ".param.spbr";
#endif
//  for Windows
#if defined OS_WIN
const char SPBR_PARAM_FILE[] = "C:\\Users\\Public\\Documents\\win_spbr\\param.spbr";
#endif


const char PLY_HEADER[] = "ply";
const char PLY_END_HEADER[] = "end_header";
const char PLY_FILE_EXTENSION[] = "ply";

#endif
//end of const_ply.h
