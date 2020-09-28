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

#include <sstream>
#include <iomanip>

#include "single_inputfile.h"

#include "const_ply.h"//
#include "spcomment.h"//
#include "spbr.h"//
#include "mainfn_utility.h"

const kvs::Vector3f DEFAULT_LIGHT_POSITION (12.0, 12.0, 12.0) ;


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
	   ///KEY_CONTROL

	   ////camera Move
	   static float R = 0.05f;
	   static float L = -0.05f;
	   static float U = 0.05f;
	   static float D = 0.05f;

	   static float UP = 0.05f;
	   static float Down = -0.05f;
	   static float deg_1 = -1.0f;//-roll
	   static float deg = 1.0f;//roll

	  static kvs::Camera* Camera_n = static_cast<kvs::glut::Screen*>(screen())->scene()->camera();

	   //////Camera rotation
	   std::cout << "** Camera position : (" << Camera_n->position();
	   std::cout << ")" << std::endl;
	   std::cout << "** View Vector(      : " << Camera_n->upVector();
	   std::cout << " )" << std::endl;
	   std::cout << "** View angle(      : " << std::endl;
	   std::cout << " (" << Camera_n->viewingMatrix();
	   std::cout << " )" << std::endl;
	 
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
		   //////Object Move
		   ////const kvs::Vector3f rotation(L, 0, 0);
		   //kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
		   //object_manager->object()->multiplyXform(kvs::Xform::Translation(rotation));//Vector3
		   ////screen()->redraw();

			//  /////Camera Move
		   kvs::Vector3f T = Camera_n->position();
		   kvs::Vector3f P = Camera_n->upVector();

		   kvs::Mat4 S = Camera_n->viewingMatrix();
		   kvs::Vector3f Mm_n = kvs::Vector3f(S[0].x(), S[0].y(), S[0].z());

		   kvs::Vector3f P_m;

		   if (P[1] > P[2]) {
			   P_m = kvs::Vector3f(Mm_n[0] / 100, (Mm_n[1] + L) / 100, Mm_n[2] / 100);
		   }
		   else if (P[2] > P[1]) {
			   P_m = kvs::Vector3f(Mm_n[0] / 100, (Mm_n[1] + L) / 100, 0);
		   }

		   kvs::Xform X_m = kvs::Xform::Translation(T) * kvs::Xform::Translation(-P_m) * kvs::Xform::Translation(-T);
		   Camera_n->multiplyXform(X_m);

		   break;
	   }
	   case kvs::Key::Right:
	   {
		   // ////Object Move
		   // const kvs::Vector3f rotation(R, 0, 0);
		   // kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
		   // object_manager->object()->multiplyXform(kvs::Xform::Translation(rotation));//Vector3
		   // screen()->redraw();
		   ////// x += 0.001;


			  // ///////Camera Move
		   kvs::Vector3f T = Camera_n->position();
		   kvs::Vector3f P = Camera_n->upVector();

		   kvs::Mat4 S = Camera_n->viewingMatrix();
		   kvs::Vector3f Mm_n = kvs::Vector3f(S[0].x(), S[0].y(), S[0].z());

		   kvs::Vector3f P_m;

		   if (P[1] > P[2]) {//Z=0
			   P_m = kvs::Vector3f(Mm_n[0] / 100, (Mm_n[1] + R) / 100, Mm_n[2] / 100);
		   }
		   else if (P[2] > P[1]) {
			   P_m = kvs::Vector3f(Mm_n[0] / 100, (Mm_n[1] + R) / 100, 0);
		   }

		   kvs::Xform X_m = kvs::Xform::Translation(T) * kvs::Xform::Translation(P_m) * kvs::Xform::Translation(-T);
		   Camera_n->multiplyXform(X_m);

		   break;
	   }
	   case kvs::Key::Up:
	   {
		   ///////Object Rotation
		 //  const kvs::Vector3f rotation(0, U, 0);
			//kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
			////object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));//Matrix
			//object_manager->object()->multiplyXform(kvs::Xform::Translation(rotation));//Vector3
			//screen()->redraw();
			////U += 0.001f;

			  ///////Camera Move
		   kvs::Vector3f T = Camera_n->position();
		   kvs::Vector3f P = Camera_n->upVector();

		   kvs::Mat4 S = Camera_n->viewingMatrix();
		   kvs::Vector3f Mm_n = kvs::Vector3f(S[1].x(), S[1].y(), S[1].z());
		   kvs::Vector3f Mm_n3 = kvs::Vector3f(S[2].x(), S[2].y(), S[2].z());

		   kvs::Vector3f P_m;
		   if (P[1] > P[2]) {
			 P_m = kvs::Vector3f((Mm_n[0]+U) / 100, (Mm_n[1]+U)/ 100, Mm_n[2] / 100);
		   }
		   else {
			   P_m = kvs::Vector3f(-(Mm_n3[0] + D) / 100, -(Mm_n3[1] + D) / 100, 0);
		   }
		  
		   kvs::Xform X_m = kvs::Xform::Translation(T) * kvs::Xform::Translation(P_m)* kvs::Xform::Translation(-T);
		   Camera_n->multiplyXform(X_m);

		   break;
	   }
	   case kvs::Key::Down:
	   {
		   ///////Object Rotation
		  // const kvs::Vector3f rotation(0, D, 0);
		  // kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
		  //// object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));//Matrix
		  // object_manager->object()->multiplyXform(kvs::Xform::Translation(rotation));//Vector3
		  // screen()->redraw();

			 // ///////Camera Move
		   kvs::Vector3f  T = Camera_n->position();
		   kvs::Vector3f P = Camera_n->upVector();

		   kvs::Mat4 S = Camera_n->viewingMatrix();
		   kvs::Vector3f Mm_n = kvs::Vector3f(S[1].x(), S[1].y(), S[1].z());
		   kvs::Vector3f Mm_n3 = kvs::Vector3f(S[2].x(), S[2].y(), S[2].z());


		   kvs::Vector3f P_m;
		   if (P[1] > P[2]) {
			   P_m = kvs::Vector3f((Mm_n[0] + D) / 100, (Mm_n[1] + D) / 100, Mm_n[2] / 100);
		   }
		   else {
			   P_m = kvs::Vector3f(-(Mm_n3[0] + D) / 100, -(Mm_n3[1] + D) / 100, 0);
		   }

		   kvs::Xform X_m = kvs::Xform::Translation(T) * kvs::Xform::Translation(-P_m) * kvs::Xform::Translation(-T);
		   Camera_n->multiplyXform(X_m);

		   break;
	   }
	   case kvs::Key::u://Expansion
	   {
		   ///////Object Rotation
		   //static float UP = 1.0f;
		   //kvs::Matrix33f rotation = kvs::ScalingMatrix33<float>(UP, UP, UP);//scaling
		   //kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
		   //object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));//Matrix
		   //screen()->redraw();
		   //UP += 0.01f;

			 /////////Camera Move
		   const kvs::Vector3f MZ(0, 0, UP);
		   kvs::Vector3f  T = Camera_n->position();
		   kvs::Xform X_m = kvs::Xform::Translation(T) * kvs::Xform::Translation(MZ) * kvs::Xform::Translation(-T);
		   Camera_n->multiplyXform(X_m);

		   break;
	   }
	   case kvs::Key::d:// Reducing
	   {
		   ///////Object Rotation
		   //static float Down = 1.0f;
		   //kvs::Matrix33f rotation = kvs::ScalingMatrix33<float>(Down, Down, Down);//scaling
		   //kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
		   // object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));//Matrix
		   //screen()->redraw();
		   //Down -= 0.01f;

		  // ///////Camera Move
		  const kvs::Vector3f MZ(0, 0, Down);
		   kvs::Vector3f  T = Camera_n->position();
		   kvs::Xform X_m = kvs::Xform::Translation(T) * kvs::Xform::Translation(MZ) * kvs::Xform::Translation(-T);
		   Camera_n->multiplyXform(X_m);

		   break;
	   }
	   case kvs::Key::y://Y rotation
	   {
		  // //Object Rotation
		  // kvs::Matrix33f rotation = kvs::YRotationMatrix33<float>(deg_1);
		  // kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
		  // object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));
		  // screen()->redraw();

		   ////ƒJƒƒ‰‰ñ“]
		   kvs::Matrix33f RY = kvs::Mat3::RotationY(deg);
		   kvs::Vector3f  T = Camera_n->position();
		   kvs::Xform X = kvs::Xform::Translation(T) * kvs::Xform::Rotation(RY) * kvs::Xform::Translation(-T);
		   Camera_n->multiplyXform(X);

		   break;
	   }
	   case kvs::Key::Y://-y rotation
	   {
		 //  //Object Rotation
			//kvs::Matrix33f rotation = kvs::YRotationMatrix33<float>(deg);
		 //   kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
		 //   object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));
		 //   screen()->redraw();

		   ////ƒJƒƒ‰‰ñ“]
		   kvs::Matrix33f RY = kvs::Mat3::RotationY(deg_1);
		   kvs::Vector3f  T = Camera_n->position();
		   kvs::Xform X = kvs::Xform::Translation(T) * kvs::Xform::Rotation(RY) * kvs::Xform::Translation(-T);
		   Camera_n->multiplyXform(X);

		    break;
	   }
	   case kvs::Key::x://X rotation
	   { 
		   ////Object Rotation
		   //kvs::Matrix33f rotation = kvs::XRotationMatrix33<float>(deg_1);
		   //kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
		   //object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));//object
		   //screen()->redraw();

		   ////ƒJƒƒ‰‰ñ“]
		   kvs::Matrix33f RX = kvs::Mat3::RotationX(deg);
		   kvs::Vector3f  T = Camera_n->position();
		   kvs::Xform X = kvs::Xform::Translation(T) * kvs::Xform::Rotation(RX) * kvs::Xform::Translation(-T);
		   Camera_n->multiplyXform(X);

		   break;
	   }
	   case kvs::Key::X://-x rotation
	   {
		   ////////Object Rotation
		   //kvs::Matrix33f rotation = kvs::XRotationMatrix33<float>(deg);
		   //kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
		   //object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));
		   //screen()->redraw();

		   ////ƒJƒƒ‰‰ñ“]
		   kvs::Matrix33f RX = kvs::Mat3::RotationX(deg_1);
		   kvs::Vector3f  T = Camera_n->position();
		   kvs::Xform X = kvs::Xform::Translation(T) * kvs::Xform::Rotation(RX) * kvs::Xform::Translation(-T);
		   Camera_n->multiplyXform(X);

		   break;
	   }
	   case kvs::Key::z://z rotation
	   {
		   
		   ////OBject Rotaton
		   //kvs::Matrix33f rotation= kvs::ZRotationMatrix33<float>(deg_1);
		   //kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
		   //object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));
		   ////object_manager->multiplyXform(kvs::Xform::Rotation(rotation));
		   //screen()->redraw();

		   ////ƒJƒƒ‰‰ñ“]
		   kvs::Matrix33f RZ = kvs::Mat3::RotationZ(deg);
		   RZ = kvs::Mat3::RotationZ(deg);
		   kvs::Vector3f  T = Camera_n->position();
		   kvs::Xform X = kvs::Xform::Translation(T) * kvs::Xform::Rotation(RZ) * kvs::Xform::Translation(-T);
		   Camera_n->multiplyXform(X);

		   break;
	   }
	   case kvs::Key::Z://-z rotation
	   {
		  // //Object Rotation
		  // kvs::Matrix33f rotation = kvs::ZRotationMatrix33<float>(deg);
		  // kvs::ObjectManager* object_manager = static_cast<kvs::glut::Screen*>(screen())->scene()->objectManager();
		  // object_manager->object()->multiplyXform(kvs::Xform::Rotation(rotation));
		  // //object_manager->multiplyXform(kvs::Xform::Rotation(rotation));
		  // screen()->redraw();

		   ////ƒJƒƒ‰‰ñ“]
		   kvs::Matrix33f RZ = kvs::Mat3::RotationZ(deg_1);
		   RZ = kvs::Mat3::RotationZ(deg_1);
		   kvs::Vector3f  T = Camera_n->position();
		   kvs::Xform X = kvs::Xform::Translation(T) * kvs::Xform::Rotation(RZ) * kvs::Xform::Translation(-T);
		   Camera_n->multiplyXform(X);

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
	 std::cerr << "  UP-key: Y Move, DOWN-key: -Y Move" << std::endl;///
	 std::cerr << " RIGHT-key: -X Move, LIGHT-key: X Move" << std::endl;///
	 std::cerr << "  u-key: Z Move, d-key: -Z Move" << std::endl;///
     std::cerr << "  s-key: snapshot image (BMP)" << std::endl;
     std::cerr << "  S-key: snapshot image (PPM)" << std::endl;
     std::cerr << "  G-key: snapshot image (PGM)" << std::endl;
     std::cerr << "  q-key: quit" << std::endl;
   }

};




#endif

