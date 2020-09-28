////////////////////////////
///// camera_control.h /////
////////////////////////////

#include<fstream>
#include<cstdio>
#include<cstring>

#include "get_extension.h"
#include "const_ply.h"

const size_t BUF_SIZE_2 = 256;
const size_t WORD_SIZE_2 = 256;

enum CAMERA_PROPERTY_TYPE {
	 X = 0,
	X_ = 1,
	 Y = 2,
	Y_ = 3,
	 Z = 4,
	Z_ = 5
};


//-----
inline bool Camera_Object(const char * filename)
{
	//resultant status
	bool status = false; //Camera

	// buf
	char buf[SPBR_BUF_SIZE];
	char word1[WORD_SIZE_2], word2[WORD_SIZE_2];

	// input file stream
	std::ifstream fin;
	fin.open(SPBR_PARAM_FILE);

	// Read SPBR commands
	while (fin.getline(buf, SPBR_BUF_SIZE - 1, '\n')) {

		// Blank line
		if (!strlen(buf)) {
			continue; // skip this line
		}
		else
			// Commmand or comment line
			if (buf[0] == '#') {
				//search "end_header"

				int num_words = sscanf(buf, "%s %s", word1, word2);
				if (num_words == 2 && \
					!strcmp(word1, CAMERA_OBJECT_COMMAND))
				{
					if (num_words == 2 && \
						!strcmp(word2, "1"))//Object
					{
						status = true;
					}
				}
			}
	}//while

	// EndFn
	fin.close();
	return status;

}//Camera_Object()


/////event_cpntrol-shrewd
//-----
inline float Rotation_Speed(const char * filename)
{
	float rotation_speed = 1.0;

	char       buf[SPBR_BUF_SIZE];
	char word1[WORD_SIZE_2], word2[WORD_SIZE_2];
	std::ifstream fin;
	fin.open(SPBR_PARAM_FILE);

	//
		// Read SPBR commands
	while (fin.getline(buf, SPBR_BUF_SIZE - 1, '\n')) {

		// Blank line
		if (!strlen(buf)) {
			continue; // skip this line
		}
		else
			// Commmand or comment line
			if (buf[0] == '#') {
				//search "end_header"

				int num_words = sscanf(buf, "%s %s", word1, word2);
				if (num_words == 2 && \
					!strcmp(word1, ROTATION_CONTROL_COMMAND))
				{
					sscanf(word2, "%g", &rotation_speed);
					std::cout << "** Rotation Speed = " << rotation_speed << std::endl;
					break;

				}
			}
	}//while
	// EndFn
	fin.close();
	return rotation_speed;

}// Rotation_Speed()


//-----
inline float Move_Speed(const char * filename)
{
	float move_speed = 1.0;

	char       buf[SPBR_BUF_SIZE];
	char word1[WORD_SIZE_2], word2[WORD_SIZE_2];
	std::ifstream fin;
	fin.open(SPBR_PARAM_FILE);

	//
		// Read SPBR commands
	while (fin.getline(buf, SPBR_BUF_SIZE - 1, '\n')) {

		// Blank line
		if (!strlen(buf)) {
			continue; // skip this line
		}
		else
			// Commmand or comment line
			if (buf[0] == '#') {
				//search "end_header"

				int num_words = sscanf(buf, "%s %s", word1, word2);
				if (num_words == 2 && \
					!strcmp(word1, MOVE_CONTROL_COMMAND))
				{
					sscanf(word2, "%g", &move_speed);
					std::cout << "** Move Speed = " << move_speed << std::endl;
					break;

				}
			}
	}//while
	// EndFn
	fin.close();
	return move_speed;

}// Move_Speed()


//-----
inline float UpDown_Speed(const char * filename)
{
	float updown_speed = 1.0;

	char       buf[SPBR_BUF_SIZE];
	char word1[WORD_SIZE_2], word2[WORD_SIZE_2];
	std::ifstream fin;
	fin.open(SPBR_PARAM_FILE);

	//
		// Read SPBR commands
	while (fin.getline(buf, SPBR_BUF_SIZE - 1, '\n')) {

		// Blank line
		if (!strlen(buf)) {
			continue; // skip this line
		}
		else
			// Commmand or comment line
			if (buf[0] == '#') {
				//search "end_header"

				int num_words = sscanf(buf, "%s %s", word1, word2);
				if (num_words == 2 && \
					!strcmp(word1, UPDOWN_CONTROL_COMMAND))
				{
					sscanf(word2, "%g", &updown_speed);
					std::cout << "** UpDown Speed = " << updown_speed << std::endl;
					break;

				}
			}
	}//while
	// EndFn
	fin.close();
	return updown_speed;

}// UpDown_Speed()
//////////event_cpntrol-shrewd


//-----
inline  CAMERA_PROPERTY_TYPE CameraAngleByAxis(const char* filename)
{
	//Resultant status
	CAMERA_PROPERTY_TYPE camera_property_type = Z;//Candidates:XYZ(RGB(A),NxNyNz(RGB(A))
	//resultant status
	//long int num_property= 0; // xyz

	 // word  counter
	// buf
	char buf[BUF_SIZE];
	char word1[WORD_SIZE], word2[WORD_SIZE];

	// input file stream
	std::ifstream fin;   fin.open(filename);

	while (fin.getline(buf, SPBR_BUF_SIZE - 1, '\n')) {

		// Blank line
		if (!strlen(buf)) {
			continue; // skip this line
		}
		else
			// Commmand or comment line
			if (buf[0] == '#') {
				//search "end_header"
				int num_words = sscanf(buf, "%s %s", word1, word2);
				if (num_words == 2 && \
					!strcmp(word1, CAMERA_ANGLE_BY_AXIS_COMMAND)) {

				  if ( (num_words == 2) &&		\
				       (!strcmp(word2, "+X") || !strcmp(word2, "X") ||
					!strcmp(word2, "+x") || !strcmp(word2, "x"))   )
					{
					  camera_property_type = X;
					}
					else if
					  ( (num_words == 2) &&		\
					    (!strcmp(word2, "+Y") || !strcmp(word2, "Y") ||
					     !strcmp(word2, "+y") || !strcmp(word2, "y")) )
					{
					  camera_property_type = Y;
					}
					else if
					  ( (num_words == 2) &&		\
					    (!strcmp(word2, "+Z") || !strcmp(word2, "Z") ||
					     !strcmp(word2, "+z") || !strcmp(word2, "z")) )
					{
					  camera_property_type = Z;
					}
					else if
					  ( (num_words == 2) &&		\
					    (!strcmp(word2, "-X") || !strcmp(word2, "-x")) )
					{
					  camera_property_type = X_;
					}
					else if
					  ( (num_words == 2) &&		\
					    (!strcmp(word2, "-Y") || !strcmp(word2, "-y")) )
					{
					  camera_property_type = Y_;
					}
					else if
					  ( (num_words == 2) &&		\
					    (!strcmp(word2, "-Z") || !strcmp(word2, "-z")) )
					{
					  camera_property_type = Z_;
					}
				}
			}
	}//while
	// EndFn
	fin.close();
	return camera_property_type;

}// CameraAngleByAxis()
