//////////////////////////
///// single_flags.h /////
/////   (Singleton)  /////
//////////////////////////

#if !defined  SINGLE_FLAGS_HH
#define       SINGLE_FLAGS_HH

#include <cstring>

//---------------//
class SingleFlags{
//---------------//
public:
  static SingleFlags* GetInstance()
  {
    static SingleFlags instance;  // only instance
    return &instance;
  } 

  // Flag 1
  void setFlag_isHeaderRead( bool status ) { m_flag_isHeaderRead = status; }
  bool flag_isHeaderRead()                 { return m_flag_isHeaderRead  ; }

 private:
  // private constructor 
  SingleFlags()
  { 
    m_flag_isHeaderRead = false ; // flag 1
  }

  // data members
  bool m_flag_isHeaderRead; // flag 1

}; // SingleFlags

#endif
// end of single_flags.h
