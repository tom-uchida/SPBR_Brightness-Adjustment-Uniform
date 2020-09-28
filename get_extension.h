///////////////////////////
///// get_extension.h /////
///////////////////////////


#ifndef _GET_EXTENSION__
#define _GET_EXTENSION__

#include <iostream>
#include <cstring>
#include <sstream> 

inline std::string getExtension(const std::string &path) {
  std::string ext;
  size_t pos1 = path.rfind('.');
  if(pos1 != std::string::npos){
    ext = path.substr(pos1+1, path.size()-pos1);
    std::string::iterator itr = ext.begin();
    while(itr != ext.end()){
      *itr = tolower(*itr);
      itr++;
    }
    itr = ext.end()-1;
    while(itr != ext.begin()){
      if(*itr == 0 || *itr == 32){
	ext.erase(itr--);
      }
      else{
	itr--;
      }
    }
  }

  return ext;
}
#endif

// end of get_extension.h
