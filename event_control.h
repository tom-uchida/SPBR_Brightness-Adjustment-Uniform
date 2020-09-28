////////////////////////////
///// event_control.h  /////
////////////////////////////

#if !defined  SSM__EVENT_CONTROL_INCLUDE
#define       SSM__EVENT_CONTROL_INCLUDE

#include <kvs/Version> //KVS2

#if KVS_VERSION_MAJOR == 2
  #include <kvs/Scene> //KVS2
#include <kvs/ParticleBasedRenderer> //KVS2
#endif

#include <kvs/InitializeEventListener>
#include <kvs/KeyPressEventListener>
#include <kvs/Key>
#include <kvs/glut/Screen>
#include <kvs/ColorImage>
#include <kvs/Camera>

#include <kvs/Matrix33>//
#include <kvs/Vector3>//
#include <kvs/ObjectManager>//
#include <kvs/ScalingMatrix33>//
#include <kvs/RotationMatrix33>//
#include <kvs/PointObject>
//#include <kvs/Mouse>

#include <sstream>
#include <iomanip>

#include "single_inputfile.h"

#include "const_ply.h"//
#include "spcomment.h"//
#include "spbr.h"//
#include "mainfn_utility.h"

#include "camera_control.h"

const kvs::Vector3f DEFAULT_LIGHT_POSITION (12.0, 12.0, 12.0) ;

// UCHIDA 2019/03/31
#include <kvs/TimerEventListener>
#include <kvs/glut/Timer>
#include "brightness_adjustment.h"
//------------------------------------------------------------//
class TimerEvent : public kvs::TimerEventListener {
//------------------------------------------------------------//
private:
    const int 			  m_num_of_snapshots; // LR="original" & LR="1"
    int     			  m_argc;
    char**  			  m_argv;
    size_t  			  m_repeat_level;
    std::string			  m_filename;
    kvs::Scene* 		  m_scene;
    SPBR*				  m_spbr_engine;
    BrightnessAdjustment* m_ba;

public:
    // Constructor
    TimerEvent( BrightnessAdjustment*   ba,
                int 			        argc,
                char**			        argv,
                kvs::Scene* 			scene,
                SPBR* 					spbr_engine,
                const int 				original_repeat_level ) : 
        m_num_of_snapshots( 2 ),
        m_argc( argc ),
        m_argv( argv ),
        m_repeat_level( original_repeat_level ),
        m_scene( scene ),
        m_spbr_engine( spbr_engine ),
        m_ba( ba )
    {
        // Get filename for snapshots
        SingleInputFile* m_sif = SingleInputFile::GetInstance();
        char m_filename_tmp[256];
        m_sif->GetNameBody( m_filename_tmp );
        m_filename = m_filename_tmp;
    } // End constroctor

    void update( kvs::TimeEvent* event ) {
        if ( m_ba->getSnapshotCounter() < m_num_of_snapshots ) {
            m_ba->SnapshotImage( m_scene, m_filename, m_repeat_level );

            if ( m_ba->getSnapshotCounter() == 1 ) {
                m_repeat_level = 1;
                std::cout << "** Forcibly, repeat level is set to \"1\".\n" << std::endl;
                m_ba->ReplaceObject( m_scene, m_argc, m_argv, m_spbr_engine, m_repeat_level);
                std::cout << "\n** Replaced object and renderer." << std::endl;

            } else if ( m_ba->getSnapshotCounter() == 2 ) {
                std::cout << "\n** Snapshot succeeded."       << std::endl;
                std::cout << "** SPBR ended successfully."  << std::endl;
            } // end if

        } else {
            m_ba->adjustBrightness( m_filename );
            exit(0);
        } // end if
    } // End update()
}; // End TimerEvent class

//------------------------------------------------------------//
class InitializeEvent : public kvs::InitializeEventListener {
//------------------------------------------------------------//

  void update( void )
  {
    // default light position
    kvs::Vector3f P0 = DEFAULT_LIGHT_POSITION ;

    // Set the light position 
    //... The default setting is (0, 0, 12) 
#if KVS_VERSION_MAJOR == 1
    screen()->light()->setPosition( P0.x(), P0.y(), P0.z() );//KVS1
#elif KVS_VERSION_MAJOR == 2
    static_cast<kvs::glut::Screen*>(screen())->scene()->light()->setPosition( P0.x(), P0.y(), P0.z() );//KVS2
#endif

  }

};

//------------------------------------------------------------//
class KeyPressEvent : public kvs::KeyPressEventListener
//------------------------------------------------------------//
{
   void update( kvs::KeyEvent* event )
   {
//#if KVS_VERSION_MAJOR == 1
//	   kvs::glew::rits::ParticleBasedRenderer* renderer = new kvs::glew::rits::ParticleBasedRenderer();//KVS1
//#elif KVS_VERSION_MAJOR == 2
//	   kvs::glsl::ParticleBasedRenderer* renderer = new kvs::glsl::ParticleBasedRenderer();//KVS2
//#endif
	   ///KEY_CONTROL///////////////////
	   static float m_rotation_speed = Rotation_Speed(SPBR_PARAM_FILE);
	   static float m_move_speed = Move_Speed(SPBR_PARAM_FILE);
	   static float m_updown_speed = UpDown_Speed(SPBR_PARAM_FILE);
	   static int camera_angle = CameraAngleByAxis(SPBR_PARAM_FILE);
	   static bool camera_object = Camera_Object(SPBR_PARAM_FILE);//Camera:0,Object:1
	    //std::cout << " camera_angle" << camera_angle;
	   ///////////////////

	   //////////camera Move
	   static float R = 0.05f;
	   static float L = -0.05f;
	   static float U = 0.05f;
	   static float D = 0.05f;

	   static float UP = 1.0f * m_updown_speed;
	   static float Down = -1.0f * m_updown_speed;
	  // static float deg_1 = -1.0f;//-roll
	  // static float deg = 1.0f;//roll

	   static float deg_1 = -1.0f * m_rotation_speed;//-roll
	   static float deg = 1.0f * m_rotation_speed;//roll	  
	   //////////////////////////////

	   //////////OBJECT Move
	   //static float OR = 1.0f * m_updown_speed;
	   //static float OL = -1.0f * m_updown_speed;
	   //static float OU = 1.0f * m_updown_speed;
	   //static float OD = -1.0f * m_updown_speed;

	   //static float O_UP = 0.01f * m_updown_speed;
	   //static float O_Down = 0.01f * m_updown_speed;
	   //// static float deg_1 = -1.0f;//-roll
	   //// static float deg = 1.0f;//roll

	   static float O_deg_1 = -1.0f * m_rotation_speed;//-roll
	   static float O_deg = 1.0f * m_rotation_speed;//roll	  
	   //////////////////////////////

	   //////////////////////////////
	   static kvs::Camera* Camera_n = static_cast<kvs::glut::Screen*>(screen())->scene()->camera();
	   kvs::Vector3f P_m;
	   kvs::Vector3f MZ;
	   kvs::Matrix33f RY;
	   kvs::Matrix33f RX;
	   kvs::Matrix33f RZ;
	   /////////////////////////////

	   ////////Camera rotation
	  /* std::cout << "** Camera position : (" << Camera_n->position();
	   std::cout << ")" << std::endl;
	   std::cout << "** View Vector(      : " << Camera_n->upVector();
	   std::cout << " )" << std::endl;
	   std::cout << "** View angle(      : " << std::endl;
	   std::cout << " (" << Camera_n->viewingMatrix();
	   std::cout << " )" << std::endl;*/
	  ///////////////////////////
	 
	   ///KEY_CONTROL
       switch ( event->key() )
       {
	   // push Home: CameraAngleByAxis Z
       // push l: light control
       // push o: object controls
       // push s: snapshot (BMP)
       // push S: snapshot (PPM)
       // push G: snapshot (PGM)
	   // push Left:-X Move
	   // push Right:X Move
	   // push Up:Y Move
	   // push Down:-Y Move
	   // push x:x Rotation
	   // push X:-x Rotation 
	   // push y:y Rotation
	   // push Y:-y Rotation
	   // push z:z Rotation
	   // push Z:-z Rotation 
	   // push u:Z Move
	   // push d: -Z Move

		 ///KEY_CONTROL
	   case kvs::Key::Left:
	   {
		   //if (camera_object == true) {
			  // ///////////////////Object Move//////////////////////
			  // const kvs::Vector3f rotation(OL, 0, 0);
			  //// kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
			  //// object_manager->object()->multiplyXform(kvs::Xform::Translation(rotation));//Vector3
			  // Camera_n->multiplyXform(kvs::Xform::Translation(kvs::Vec3(rotation)));//�J������Xform���X�V
			  // screen()->redraw();
			  // ///////////////////////////
		   //}
		   //else {

			   ///////////Camera Move////////////////////////////////////

			   kvs::Vector3f T = Camera_n->position();
			   kvs::Vector3f P = Camera_n->upVector();

			   kvs::Mat4 S = Camera_n->viewingMatrix();
			   kvs::Vector3f Mm_n = kvs::Vector3f(S[0].x(), S[0].y(), S[0].z());

			   //Z,-Z,X,Y,-X,-Y

				 /* if (P[1] > P[2]) {
					  P_m = kvs::Vector3f(Mm_n[0] * m_move_speed, (Mm_n[1] + L) * m_move_speed, Mm_n[2] * m_move_speed);
				  }
				  else if (P[2] > P[1]) {
					  P_m = kvs::Vector3f(Mm_n[0] * m_move_speed, (Mm_n[1] + L) * m_move_speed, 0);
				  }  */
			   if (P[1] > P[2]) {
				   P_m = kvs::Vector3f((Mm_n[0] + L) * m_move_speed, Mm_n[1] * m_move_speed, Mm_n[2] * m_move_speed);
			   }
			   else if (P[2] > P[1]) {
				   P_m = kvs::Vector3f((Mm_n[0] + L) * m_move_speed, Mm_n[1] * m_move_speed, 0);
			   }
			   kvs::Xform X_m = kvs::Xform::Translation(T) * kvs::Xform::Translation(-P_m) * kvs::Xform::Translation(-T);
			   Camera_n->multiplyXform(X_m);

			   //////////////////////////////////////
		  // }

		   break;
	   }
	   case kvs::Key::Right:
	   {
		   //if (camera_object == true) {
			  // //////Object Move////////////////////////
			  // const kvs::Vector3f rotation(OR, 0, 0);
			  //// kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
			  //// object_manager->object()->multiplyXform(kvs::Xform::Translation(rotation));//Vector3
			  // Camera_n->multiplyXform(kvs::Xform::Translation(kvs::Vec3(rotation)));//�J������Xform���X�V
			  // screen()->redraw();
			  // //// x += 0.001;
			  // ////////////////////////
		   //}
		   //else {

			   /////////Camera Move/////////////////////
			   kvs::Vector3f T = Camera_n->position();
			   kvs::Vector3f P = Camera_n->upVector();

			   kvs::Mat4 S = Camera_n->viewingMatrix();
			   kvs::Vector3f Mm_n = kvs::Vector3f(S[0].x(), S[0].y(), S[0].z());

			   //Z,-Z,X,Y,-X,-Y

				   //if (P[1] > P[2]) {//Z=0
					  // P_m = kvs::Vector3f(Mm_n[0] * m_move_speed, (Mm_n[1] + R) * m_move_speed, Mm_n[2] * m_move_speed);
				   //}
				   //else if (P[2] > P[1]) {
					  // P_m = kvs::Vector3f(Mm_n[0] * m_move_speed, (Mm_n[1] + R) * m_move_speed, 0);
				   //}
			   if (P[1] > P[2]) {//Z=0
				   P_m = kvs::Vector3f((Mm_n[0] + R) * m_move_speed, Mm_n[1] * m_move_speed, Mm_n[2] * m_move_speed);
			   }
			   else if (P[2] > P[1]) {
				   P_m = kvs::Vector3f((Mm_n[0] + R) * m_move_speed, Mm_n[1] * m_move_speed, 0);
			   }
			   kvs::Xform X_m = kvs::Xform::Translation(T) * kvs::Xform::Translation(P_m) * kvs::Xform::Translation(-T);
			   Camera_n->multiplyXform(X_m);

			   /////////////////////////
		   //}
		   break;
	   }
	   case kvs::Key::Up:
	   {
		   //if (camera_object == true) {
			  // /////Object Rotation/////////////////////////////////
			  // const kvs::Vector3f rotation(0, OU, 0);
			  // //kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
			  // //object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));//Matrix
			  // //object_manager->object()->multiplyXform(kvs::Xform::Translation(rotation));//Vector3
			  // Camera_n->multiplyXform(kvs::Xform::Translation(kvs::Vec3(rotation)));//�J������Xform���X�V
			  // screen()->redraw();
			  // //U += 0.001f;
			  // ////////////////////////////////////
		   //}
		   //else {

			   ///////Camera Move
			   kvs::Vector3f T = Camera_n->position();
			   kvs::Vector3f P = Camera_n->upVector();

			   kvs::Mat4 S = Camera_n->viewingMatrix();
			   kvs::Vector3f Mm_n = kvs::Vector3f(S[1].x(), S[1].y(), S[1].z());
			   kvs::Vector3f Mm_n3 = kvs::Vector3f(S[2].x(), S[2].y(), S[2].z());

			   if (camera_angle == 4 || camera_angle == 5) {//Z,-Z

				   if (P[1] > P[2]) {
					   P_m = kvs::Vector3f((Mm_n[0] + U) * m_move_speed, (Mm_n[1] + U) * m_move_speed, Mm_n[2] * m_move_speed);
				   }
				   else {
					   P_m = kvs::Vector3f(-(Mm_n3[0] + D) * m_move_speed, -(Mm_n3[1] + D) * m_move_speed, 0);
				   }
				   kvs::Xform X_m = kvs::Xform::Translation(T) * kvs::Xform::Translation(P_m)* kvs::Xform::Translation(-T);
				   Camera_n->multiplyXform(X_m);
			   }
			   else if (camera_angle == 0 || camera_angle == 2 || camera_angle == 1 || camera_angle == 3) {//X,Y,-X,-Y
				   MZ = kvs::Vector3f(0, 0, Down);
				   kvs::Xform X_m = kvs::Xform::Translation(T) * kvs::Xform::Translation(MZ) * kvs::Xform::Translation(-T);
				   Camera_n->multiplyXform(X_m);
			   }
			   //////////////////////
		  // }
		   break;
	   }
	   case kvs::Key::Down:
	   {
		   //if (camera_object == true) {
			  // /////////////Object Rotation
			  // const kvs::Vector3f rotation(0, OD, 0);
			  // //kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
			  // // object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));//Matrix
			  // //object_manager->object()->multiplyXform(kvs::Xform::Translation(rotation));//Vector3
			  // Camera_n->multiplyXform(kvs::Xform::Translation(kvs::Vec3(rotation)));//�J������Xform���X�V
			  // screen()->redraw();
			  // /////////////////////////////////////////////////
		   //}
		   //else {

			   ///////////Camera Move
			   kvs::Vector3f  T = Camera_n->position();
			   kvs::Vector3f P = Camera_n->upVector();

			   kvs::Mat4 S = Camera_n->viewingMatrix();
			   kvs::Vector3f Mm_n = kvs::Vector3f(S[1].x(), S[1].y(), S[1].z());
			   kvs::Vector3f Mm_n3 = kvs::Vector3f(S[2].x(), S[2].y(), S[2].z());

			   if (camera_angle == 4 || camera_angle == 5) {//Z,-Z

				   if (P[1] > P[2]) {
					   P_m = kvs::Vector3f((Mm_n[0] + D) * m_move_speed, (Mm_n[1] + D) * m_move_speed, Mm_n[2] * m_move_speed);
				   }
				   else {
					   P_m = kvs::Vector3f(-(Mm_n3[0] + D) * m_move_speed, -(Mm_n3[1] + D) * m_move_speed, 0);
				   }
				   kvs::Xform X_m = kvs::Xform::Translation(T) * kvs::Xform::Translation(-P_m) * kvs::Xform::Translation(-T);
				   Camera_n->multiplyXform(X_m);
			   }
			   else if (camera_angle == 0 || camera_angle == 2 || camera_angle == 1 || camera_angle == 3) {//X,Y,-X,-Y
				   MZ = kvs::Vector3f(0, 0, UP);
				   kvs::Xform X_m = kvs::Xform::Translation(T) * kvs::Xform::Translation(MZ) * kvs::Xform::Translation(-T);
				   Camera_n->multiplyXform(X_m);
			   }
			   //////////////
		 //  }

		   break;
	   }
	   case kvs::Key::u://Expansion
	   {
		   //if (camera_object == true) {
			  // ////////////Object Rotation
			  // static float UP_S = 1.0f;
			  // kvs::Matrix33f rotation = kvs::ScalingMatrix33<float>(UP_S, UP_S, UP_S);//scaling
			  // kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
			  // object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));//Matrix
			  // screen()->redraw();
			  // UP_S += O_UP;
			  // ///////////////////////////////////////////////
		   //}
		   //else {

			   ////////////////////Camera Move
			   kvs::Vector3f  T = Camera_n->position();
			   kvs::Vector3f P = Camera_n->upVector();

			   kvs::Mat4 S = Camera_n->viewingMatrix();
			   kvs::Vector3f Mm_n = kvs::Vector3f(S[1].x(), S[1].y(), S[1].z());
			   kvs::Vector3f Mm_n3 = kvs::Vector3f(S[2].x(), S[2].y(), S[2].z());

			   if (camera_angle == 4 || camera_angle == 5) {//Z,-Z
				   MZ = kvs::Vector3f(0, 0, UP);
				   kvs::Xform X_m = kvs::Xform::Translation(T) * kvs::Xform::Translation(MZ) * kvs::Xform::Translation(-T);
				   Camera_n->multiplyXform(X_m);
			   }
			   else if (camera_angle == 0 || camera_angle == 2 || camera_angle == 1 || camera_angle == 3) {//X,Y,-X,-Y
				   if (P[1] > P[2]) {
					   P_m = kvs::Vector3f((Mm_n[0] + U) * m_move_speed, (Mm_n[1] + U) * m_move_speed, Mm_n[2] * m_move_speed);
				   }
				   else {
					   P_m = kvs::Vector3f(-(Mm_n3[0] + D) * m_move_speed, -(Mm_n3[1] + D) * m_move_speed, 0);
				   }
				   kvs::Xform X_m = kvs::Xform::Translation(T) * kvs::Xform::Translation(P_m) * kvs::Xform::Translation(-T);
				   Camera_n->multiplyXform(X_m);
			   }
			   ///////////////////////////////////////////////
		  // }
		   break;
	   }
	   case kvs::Key::d:// Reducing
	   {
		   //if (camera_object == true) {
			  // ///////Object Rotation///////////////////////////////////
			  // static float Down_s = 1.0f;
			  // kvs::Matrix33f rotation = kvs::ScalingMatrix33<float>(Down_s, Down_s, Down_s);//scaling
			  // kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
			  // object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));//Matrix
			  // screen()->redraw();
			  // Down_s -=O_Down;
			  // ////////////////////////////////////////////
		   //}
		   //else {

			   ///////Camera Move
			   kvs::Vector3f P = Camera_n->upVector();
			   kvs::Vector3f  T = Camera_n->position();
			   kvs::Mat4 S = Camera_n->viewingMatrix();
			   kvs::Vector3f Mm_n = kvs::Vector3f(S[1].x(), S[1].y(), S[1].z());
			   kvs::Vector3f Mm_n3 = kvs::Vector3f(S[2].x(), S[2].y(), S[2].z());

			   if (camera_angle == 4 || camera_angle == 5) {//Z
				   MZ = kvs::Vector3f(0, 0, Down);
				   kvs::Xform X_m = kvs::Xform::Translation(T) * kvs::Xform::Translation(MZ) * kvs::Xform::Translation(-T);
				   Camera_n->multiplyXform(X_m);
			   }
			   else if (camera_angle == 0 || camera_angle == 2 || camera_angle == 1 || camera_angle == 3) {//X,Y,-X,-Y
				   if (P[1] > P[2]) {
					   P_m = kvs::Vector3f((Mm_n[0] + D) * m_move_speed, (Mm_n[1] + D) * m_move_speed, Mm_n[2] * m_move_speed);
				   }
				   else {
					   P_m = kvs::Vector3f(-(Mm_n3[0] + D) * m_move_speed, -(Mm_n3[1] + D) * m_move_speed, 0);
				   }
				   kvs::Xform X_m = kvs::Xform::Translation(T) * kvs::Xform::Translation(-P_m)* kvs::Xform::Translation(-T);
				   Camera_n->multiplyXform(X_m);
			   }
			   ////////////////////////////////////
		   //}
		   break;
	   }
	   case kvs::Key::y://Y rotation
	   {
		   if (camera_object == true) {
			   /////////////////////Object Rotation
			   //kvs::Matrix33f rotation = kvs::YRotationMatrix33<float>(O_deg_1);
			   //kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
			   //object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));
			   if (camera_angle == 4 || camera_angle == 5) {//Z
				   Camera_n->multiplyXform(kvs::Xform::Rotation(kvs::Mat3::RotationY(O_deg)));
			   }
			   else if (camera_angle == 0 || camera_angle == 1 || camera_angle == 2 || camera_angle == 3) {//X,Y,-Y,-X
				   Camera_n->multiplyXform(kvs::Xform::Rotation(kvs::Mat3::RotationZ(O_deg)));
			   }
			   screen()->redraw();
			   ///////////////////////////////
		   }
		   else {
			   ///////�J������]
			   if (camera_angle == 4 || camera_angle == 5) {//Z
				   RY = kvs::Mat3::RotationY(deg);
			   }
			   else if (camera_angle == 0 || camera_angle == 1 || camera_angle == 2 || camera_angle == 3) {//X,Y,-Y,-X
				   RY = kvs::Mat3::RotationZ(deg);
			   }

			   kvs::Vector3f  T = Camera_n->position();
			   kvs::Xform X = kvs::Xform::Translation(T) * kvs::Xform::Rotation(RY) * kvs::Xform::Translation(-T);
			   Camera_n->multiplyXform(X);
			   /////////////////
		   }
		   break;
	   }
	   case kvs::Key::Y://-y rotation
	   {
		   if (camera_object == true) {
			   /////////////////////Object Rotation
			   //kvs::Matrix33f rotation = kvs::YRotationMatrix33<float>(O_deg);
			   //kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
			   //object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));
			   if (camera_angle == 4 || camera_angle == 5) {//Z
				   Camera_n->multiplyXform(kvs::Xform::Rotation(kvs::Mat3::RotationY(O_deg_1)));
			   }
			   else if (camera_angle == 0 || camera_angle == 1 || camera_angle == 2 || camera_angle == 3) {//X,Y,-Y,-X
				   Camera_n->multiplyXform(kvs::Xform::Rotation(kvs::Mat3::RotationZ(O_deg_1)));
			   }
			   screen()->redraw();
			   ////////////////////////////////
		   }
		   else {
			   //////////////////////�J������]
			   if (camera_angle == 4 || camera_angle == 5) {//Z
				   RY = kvs::Mat3::RotationY(deg_1);
			   }
			   else if (camera_angle == 0 || camera_angle == 1 || camera_angle == 2 || camera_angle == 3) {//X,Y,-Y,-X
				   RY = kvs::Mat3::RotationZ(deg_1);
			   }

			   kvs::Vector3f  T = Camera_n->position();
			   kvs::Xform X = kvs::Xform::Translation(T) * kvs::Xform::Rotation(RY) * kvs::Xform::Translation(-T);
			   Camera_n->multiplyXform(X);
			   //////////////////////////////////////
		   }

		    break;
	   }
	   case kvs::Key::x://X rotation
	   { 
		   if (camera_object == true) {
			   //////////Object Rotation
			  // kvs::Matrix33f rotation = kvs::XRotationMatrix33<float>(O_deg_1);
			   //kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
			   //object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));//object
			   if (camera_angle == 4) {//Z
				   Camera_n->multiplyXform(kvs::Xform::Rotation(kvs::Mat3::RotationX(O_deg)));
			   }
			   else if (camera_angle == 0 || camera_angle == 1 || camera_angle == 3 || camera_angle == 5 || camera_angle == 2) {//-Z,-X,-Y,Y,X
				   Camera_n->multiplyXform(kvs::Xform::Rotation(kvs::Mat3::RotationX(O_deg_1)));
			   }
			   screen()->redraw();
			   /////////////////////////////////////////
		   }
		   else {
			   //////////////////////�J������]
			   if (camera_angle == 4) {//Z
				   RX = kvs::Mat3::RotationX(deg);
			   }
			   else if (camera_angle == 0 || camera_angle == 1 || camera_angle == 3 || camera_angle == 5 || camera_angle == 2) {//-Z,-X,-Y,Y,X
				   RX = kvs::Mat3::RotationX(deg_1);
			   }

			   kvs::Vector3f  T = Camera_n->position();
			   kvs::Xform X = kvs::Xform::Translation(T) * kvs::Xform::Rotation(RX) * kvs::Xform::Translation(-T);
			   Camera_n->multiplyXform(X);
			   ////////////////////////////
		   }

		   break;
	   }
	   case kvs::Key::X://-x rotation
	   {
		   if (camera_object == true) {
			   //////////Object Rotation
			   //kvs::Matrix33f rotation = kvs::XRotationMatrix33<float>(O_deg);
			   //kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
			   //object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));
			   if (camera_angle == 4) {//Z
				   Camera_n->multiplyXform(kvs::Xform::Rotation(kvs::Mat3::RotationX(O_deg_1)));
			   }
			   else if (camera_angle == 0 || camera_angle == 1 || camera_angle == 3 || camera_angle == 5 || camera_angle == 2) {//-Z,-X,-Y,Y,X
				   Camera_n->multiplyXform(kvs::Xform::Rotation(kvs::Mat3::RotationX(O_deg)));
			   }
			   screen()->redraw();
			   /////////////////////
		   }
		   else {
			   ////////////////�J������]
			   if (camera_angle == 4) {//Z
				   RX = kvs::Mat3::RotationX(deg_1);
			   }
			   else if (camera_angle == 0 || camera_angle == 1 || camera_angle == 3 || camera_angle == 5 || camera_angle == 2) {//-Z,-X,-Y,Y,X
				   RX = kvs::Mat3::RotationX(deg);
			   }

			   kvs::Vector3f  T = Camera_n->position();
			   kvs::Xform X = kvs::Xform::Translation(T) * kvs::Xform::Rotation(RX) * kvs::Xform::Translation(-T);
			   Camera_n->multiplyXform(X);
			   /////////////////////////
		   }

		   break;
	   }
	   case kvs::Key::z://z rotation
	   {
		   if (camera_object == true) {
			   ////////Object Rotaton
			   //kvs::Matrix33f rotation = kvs::ZRotationMatrix33<float>(O_deg_1);
			  // kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
			   //object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));
			   if (camera_angle == 4 || camera_angle == 5) {//Z
				   Camera_n->multiplyXform(kvs::Xform::Rotation(kvs::Mat3::RotationZ(O_deg)));
			   }
			   else if (camera_angle == 0 || camera_angle == 1 || camera_angle == 2 || camera_angle == 3) {//X,Y,-Y,-X
				   Camera_n->multiplyXform(kvs::Xform::Rotation(kvs::Mat3::RotationY(O_deg_1)));
			   }
			   screen()->redraw();
			   ///////////////////
		   }
		   else {
			   /////////�J������]
			   if (camera_angle == 4 || camera_angle == 5) {//Z
				   RZ = kvs::Mat3::RotationZ(deg);
			   }
			   else if (camera_angle == 0 || camera_angle == 1 || camera_angle == 2 || camera_angle == 3) {//X,Y,-Y,-X
				   RZ = kvs::Mat3::RotationY(deg_1);
			   }

			   kvs::Vector3f  T = Camera_n->position();
			   kvs::Xform X = kvs::Xform::Translation(T) * kvs::Xform::Rotation(RZ) * kvs::Xform::Translation(-T);
			   Camera_n->multiplyXform(X);
			   ///////////////////
		   }

		   break;
	   }
	   case kvs::Key::Z://-z rotation
	   {
		   if (camera_object == true) {
			   ///////////////Object Rotation
			   //kvs::Matrix33f rotation = kvs::ZRotationMatrix33<float>(O_deg);
			   //kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
			   //object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));
			   if (camera_angle == 4 || camera_angle == 5) {//Z
				   Camera_n->multiplyXform(kvs::Xform::Rotation(kvs::Mat3::RotationZ(O_deg_1)));
			   }
			   else if (camera_angle == 0 || camera_angle == 1 || camera_angle == 2 || camera_angle == 3) {//X,Y,-Y,-X
				   Camera_n->multiplyXform(kvs::Xform::Rotation(kvs::Mat3::RotationY(O_deg)));
			   }
			   screen()->redraw();
			   ////////////////////
		   }
		   else {
			   /////////�J������]
			   if (camera_angle == 4 || camera_angle == 5) {//Z
				   RZ = kvs::Mat3::RotationZ(deg_1);
			   }
			   else if (camera_angle == 0 || camera_angle == 1 || camera_angle == 2 || camera_angle == 3) {//X,Y,-Y,-X
				   RZ = kvs::Mat3::RotationY(deg);
			   }

			   kvs::Vector3f  T = Camera_n->position();
			   kvs::Xform X = kvs::Xform::Translation(T) * kvs::Xform::Rotation(RZ) * kvs::Xform::Translation(-T);
			   Camera_n->multiplyXform(X);
			   /////////////////
		   }
		   break;
	   }
	   ///KEY_CONTROL

         case kvs::Key::l: 
           std::cerr << "\n** Light-control mode" << std::endl;

#if KVS_VERSION_MAJOR == 1
           screen()->controlTarget() = kvs::glut::Screen::TargetLight; //KVS1
#elif KVS_VERSION_MAJOR == 2
           static_cast<kvs::glut::Screen*>( screen() )->scene()->controlTarget() = kvs::Scene::TargetLight; //KVS2
#endif

           this->displayMenu();
           break;
         case kvs::Key::o: 
           std::cerr << "\n** Object-control mode" << std::endl;

#if KVS_VERSION_MAJOR == 1
           screen()->controlTarget() = kvs::glut::Screen::TargetObject; //KVS1
#elif KVS_VERSION_MAJOR == 2
	   static_cast<kvs::glut::Screen*>( screen() )->scene()->controlTarget() = kvs::Scene::TargetObject; //KVS2
#endif

           this->displayMenu();
           break;
         case kvs::Key::s: {
           char filename_bmp [256];
           SingleInputFile* p  = SingleInputFile::GetInstance();
           p->GetBMPName ( filename_bmp );

           std::cerr << "\n** Snapshot (BMP)" << std::endl;
           kvs::ColorImage snapshot_image; 

#if KVS_VERSION_MAJOR == 1
	   snapshot_image = screen()->camera()->snapshot(); //KVS1
#elif KVS_VERSION_MAJOR == 2
	   snapshot_image = static_cast<kvs::glut::Screen*>( screen() )->scene()->camera()->snapshot();//KVS2
#endif


	   snapshot_image.write( filename_bmp );
           this->displayMenu();
           break;
	 }
         case kvs::Key::S: {
           char filename_ppm [256];
           SingleInputFile* p  = SingleInputFile::GetInstance();
           p->GetPPMName ( filename_ppm );

           std::cerr << "\n** Snapshot (PPM) " << std::endl;
           kvs::ColorImage snapshot_image; 

#if KVS_VERSION_MAJOR == 1
	   snapshot_image = screen()->camera()->snapshot();//KVS1
#elif KVS_VERSION_MAJOR == 2
	   snapshot_image = static_cast<kvs::glut::Screen*>( screen() )->scene()->camera()->snapshot();//KVS2
#endif


	   snapshot_image.write( filename_ppm );
           this->displayMenu();
           break;
	 }
         case kvs::Key::G: {
           char filename_pgm [256];
           SingleInputFile* p  = SingleInputFile::GetInstance();
           p->GetPGMName ( filename_pgm );

           std::cerr << "\n** Snapshot (PGM) " << std::endl;
           kvs::ColorImage snapshot_image; 

#if KVS_VERSION_MAJOR == 1
	   snapshot_image = screen()->camera()->snapshot(); //KVS1
#elif KVS_VERSION_MAJOR == 2
	   snapshot_image = static_cast<kvs::glut::Screen*>( screen() )->scene()->camera()->snapshot();//KVS2
#endif


	   snapshot_image.write( filename_pgm );
           this->displayMenu();
           break;
	 }
         default: 
           break;
       } // switch


   }

 public:
   void displayMenu (void ) 
   {
     std::cerr << "\nKeyboard menu:" << std::endl;
	 std::cerr << "  o-key: object control, l-key: light control" << std::endl;
	 std::cerr << "Home-key: CameraAngleByAxis Z" << std::endl;
     std::cerr << "  x-key:X Rotation, X-key: -X Rotation" << std::endl;
	 std::cerr << "  y-key:Y Rotation, Y-key: -Y Rotation" << std::endl;
	 std::cerr << "  z-key:Z Rotation, Z-key: -Z Rotation" << std::endl;
	 std::cerr << "  UP-key: Z Move, DOWN-key: -Z Move" << std::endl;///
	 std::cerr << " RIGHT-key: -X Move, LEFT-key: X Move" << std::endl;///
	 std::cerr << "  u-key: Y Move, d-key: -Y Move" << std::endl;///
     std::cerr << "  s-key: snapshot image (BMP)" << std::endl;
     std::cerr << "  S-key: snapshot image (PPM)" << std::endl;
     std::cerr << "  G-key: snapshot image (PGM)" << std::endl;
     std::cerr << "  q-key: quit" << std::endl;
   }

};




#endif

