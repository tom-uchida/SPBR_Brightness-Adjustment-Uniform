//////////////////////////////////////////
//   @file   brightness_adjustment.cpp
//   @author Tomomasa Uchida
//   @date   2019/03/15
//////////////////////////////////////////

#include <kvs/glut/Application>
#include <kvs/Version> //KVS2

#include <kvs/PointObject>

#if KVS_VERSION_MAJOR == 1
    #include <kvs/glew/ParticleBasedRenderer> //KVS1
#elif KVS_VERSION_MAJOR == 2
    #include <kvs/ParticleBasedRenderer> //KVS2
#endif

#include <kvs/glut/Screen>
#include <kvs/Camera>
#include <kvs/RotationMatrix33>

#include <cstring>
#include <iostream>

#include "single_inputfile.h"
#include "file_format.h"
#include "spbr.h"
#include "mainfn_utility.h"
#include "shuffle.h"

#include "brightness_adjustment.h"
#include <kvs/ColorImage>
#include <kvs/GrayImage>

#include "version.h"
#include <sstream>

BrightnessAdjustment::BrightnessAdjustment():
    m_snapshot_counter( 0 ),
    m_ratio_of_reference_section( 0.01f ),
    m_parameter_interval( 0.01f )
{
    // Message
    std::cout << "\n\n** BrightnessAdjustment constructor is called.\n"  << std::endl;
} // End constructor

BrightnessAdjustment::BrightnessAdjustment(FILE_FORMAT4BA file_format):
    m_file_format( file_format ),
    m_snapshot_counter( 0 ),
    m_ratio_of_reference_section( 0.01f ),
    m_parameter_interval( 0.01f )
{
    // Message
    std::cout << "\n\n** BrightnessAdjustment constructor is called."  << std::endl;

    if ( m_file_format == SPBR_ASCII4BA )
        std::cout << "**  FILE_FORMAT : SPBR_ASCII"  << "\n" << std::endl;
    else if ( m_file_format == SPBR_BINARY4BA )
        std::cout << "**  FILE_FORMAT : SPBR_BINARY" << "\n" << std::endl;
    else if ( m_file_format == PLY_ASCII4BA )
        std::cout << "**  FILE_FORMAT : PLY_ASCII"   << "\n" << std::endl;
    else if ( m_file_format == PLY_BINARY4BA )
        std::cout << "**  FILE_FORMAT : PLY_BINARY"  << "\n" << std::endl;
} // End constructor

void BrightnessAdjustment::RegisterObject( kvs::Scene* scene, int argc, char** argv, SPBR* spbr_engine, const size_t LR )
{
    scene->registerObject( CreateObject(argc, argv), CreateRenderer(spbr_engine, LR) );
} // End RegisterObject()

kvs::PointObject* BrightnessAdjustment::CreateObject(int argc, char** argv) {
    kvs::PointObject* obj = NULL;

    // Read the first data file (argv[1])
    if ( m_file_format == PLY_ASCII4BA ) {
        SPBR* spbr_engine          = new SPBR( argv[1], PLY_ASCII );
        kvs::PointObject* object   = spbr_engine;
        obj = CreateObjectCommon( argc, argv, spbr_engine, object );

    } else if ( m_file_format == PLY_BINARY4BA ) {
        SPBR* spbr_engine          = new SPBR( argv[1], PLY_BINARY );
        kvs::PointObject* object   = spbr_engine;
        obj = CreateObjectCommon( argc, argv, spbr_engine, object );

    } else if ( m_file_format == SPBR_BINARY4BA ) {
        SPBR* spbr_engine          = new SPBR( argv[1], SPBR_BINARY );
        kvs::PointObject* object   = spbr_engine;
        obj = CreateObjectCommon( argc, argv, spbr_engine, object );

    } else if ( m_file_format == SPBR_ASCII4BA ) {
        SPBR* spbr_engine          = new SPBR( argv[1], SPBR_ASCII );
        kvs::PointObject* object   = spbr_engine;
        obj = CreateObjectCommon( argc, argv, spbr_engine, object );
    }

    return obj;
} // End CreateObject()

kvs::PointObject* BrightnessAdjustment::CreateObjectCommon(int argc, char** argv, SPBR* spbr_engine, kvs::PointObject* object) {
    // Read and append the remaining files:  
    //  argv[2], argv[3], ..., argv[argc-1]
    for (int i = 3; i <= argc; i++) {
        if ( isASCII_PLY_File(argv[i - 1]) ) {
            SPBR* spbr_tmp = new SPBR(argv[i - 1], PLY_ASCII);
            object->add(*kvs::PointObject::DownCast(spbr_tmp));

        } else if ( isBINARY_PLY_File(argv[i - 1]) ) {
            SPBR* spbr_tmp = new SPBR(argv[i - 1], PLY_BINARY);
            object->add(*kvs::PointObject::DownCast(spbr_tmp));
        
        } else if ( isBinarySPBR_File(argv[i - 1]) ) {
            SPBR* spbr_tmp = new SPBR(argv[i - 1], SPBR_BINARY);
            object->add(*kvs::PointObject::DownCast(spbr_tmp));
        
        } else {
            SPBR* spbr_tmp = new SPBR(argv[i - 1], SPBR_ASCII);
            object->add(*kvs::PointObject::DownCast(spbr_tmp));
        }
    } // end for

    addBoundingBoxToScene( spbr_engine );

    // Forced shuffle
    if ( spbr_engine->isForcedShuffleOn() ) {
        Shuffle shuffle_engine( spbr_engine );
    }

    // Set name
    object->setName("Object");

    // Object rotation (Z==>X) if required
    if ( spbr_engine->isZXRotation() ) {
        double zrot_deg = spbr_engine->objectZXRotAngle (0) ; 
        double xrot_deg = spbr_engine->objectZXRotAngle (1) ; 
        ToolXform::rotateZX( object, zrot_deg, xrot_deg, kvs::Vector3f( 0, 0, 0 ) );
    }

    return object;
} // End CreateObject()

kvs::glsl::ParticleBasedRenderer* BrightnessAdjustment::CreateRenderer( SPBR* spbr_engine, const size_t LR)
{
    kvs::glsl::ParticleBasedRenderer* renderer = new kvs::glsl::ParticleBasedRenderer();
    renderer->setName( "Renderer" );
    renderer->setRepetitionLevel( LR );
    
    // Set Lambert shading or keep Phong shading
    setShadingType( spbr_engine, renderer );

    // Shading control (ON/OFF)
    if ( spbr_engine->isShading() == false ) {
        std::cout << "** Shading is off" << std::endl;
        renderer->disableShading();
    }

    // LOD control (ON/OFF)
    if ( spbr_engine->isLOD() )
        renderer->enableLODControl();

    // Particle zoom control (ON/OFF)
    if ( spbr_engine->isParticleZoomOn() == false )
        renderer->disableZooming();
    else
        renderer->enableZooming();

    // Shuffle control (ON/OFF)
    if ( spbr_engine->isParticleShuffleOn() )
        renderer->enableShuffle();
    else
        renderer->disableShuffle();

    return renderer;
} // End CreateRenderer()

void BrightnessAdjustment::ReplaceObject( kvs::Scene* scene, int argc, char** argv, SPBR* spbr_engine, const size_t LR )
{
    scene->replaceObject( "Object", CreateObject(argc, argv) );
    scene->replaceRenderer( "Renderer", CreateRenderer(spbr_engine, LR) );
} // End ReplaceObject()

void BrightnessAdjustment::SnapshotImage( kvs::Scene* scene, const std::string filename, const int repeat_level )
{
    // Snapshot
    scene->screen()->redraw();
    kvs::ColorImage color_image_tmp = scene->camera()->snapshot();

    // Save color image
    if ( m_snapshot_counter == 0 ) m_img_Color       = color_image_tmp;
    if ( m_snapshot_counter == 1 ) m_img_Color_LR1   = color_image_tmp;

    // Write color image
    // color_image_tmp.write( filename + "_LR" + kvs::String::ToString(repeat_level) + ".bmp" ); // kvs2.7
    std::ostringstream oss;
    oss << repeat_level;
    color_image_tmp.write( filename + "_LR" + oss.str() + ".bmp" );
    std::cout << "** Snapshot repeat level \"" << repeat_level << "\" image (BMP)" << std::endl;

    m_snapshot_counter++;
} // End snapshotTwoImages()

void BrightnessAdjustment::adjustBrightness( const std::string filename )
{
    // Display opening message
    displayMessage();

    // Calc number of pixels of the image
    size_t N_all = m_img_Color.numberOfPixels();
    size_t N_all_non_bgcolor = calcNumOfPixelsNonBGColor( m_img_Color );
    std::cout << "** Num. of pixels                    : " << N_all             << " (pixels)" << std::endl;
    std::cout << "** Num. of pixels non BGColor        : " << N_all_non_bgcolor << " (pixels)" << std::endl;

    // Convert color to gray
    kvs::GrayImage img_Gray( m_img_Color );
    kvs::GrayImage img_Gray_LR1( m_img_Color_LR1 );

    // ====================================
    //  STEP1 : Get max pixel value (LR=1)
    // ====================================
    kvs::UInt8 max_pixel_value_LR1 = calcMaxPixelValue( img_Gray_LR1 );
    std::cout << "** Max pixel value (LR=1)            : " << +max_pixel_value_LR1 << " (pixel value)" << std::endl;

    // =================================================
    //  STEP2 : Search for threshold pixel value (LR=1) 
    // =================================================
    size_t N_all_non_bgcolor_LR1 = calcNumOfPixelsNonBGColor( m_img_Color_LR1 );
    kvs::UInt8 threshold_pixel_value_LR1 = searchThresholdPixelValue( img_Gray_LR1, N_all_non_bgcolor_LR1, max_pixel_value_LR1 );

    // =======================================
    //  STEP3 : Adjust brightness of an image
    // =======================================
    float p = calcAdjustmentParameter( m_img_Color, threshold_pixel_value_LR1, N_all_non_bgcolor );
    p = specifyNumOfDigits( p, 4 );
    doBrightnessAdjustment( m_img_Color, p );
    std::cout << "** Adjustment parameter              : " << std::setprecision(3) << p << std::endl;

    // Write adjusted image
    std::ostringstream oss;
    oss << p;
    std::string adjusted_filename(filename + "_adjusted" + oss.str() + ".bmp");
    m_img_Color.write( adjusted_filename );
    std::cout << "** Saved adjusted image."                     << std::endl;
    std::cout << "   (PATH : ./" << adjusted_filename << ")"    << std::endl;
    std::cout << "================================\n"           << std::endl;
    
    // Exec. open command (macOS only)
#ifdef OS_MAC
    std::string EXEC("open ");
    EXEC += adjusted_filename;
    system( EXEC.c_str() );
#endif

} // End adjustBrightness()

inline void BrightnessAdjustment::displayMessage()
{
    std::cout << "\n" << std::endl;
    std::cout << BA_TITLE                       << std::endl;
    std::cout << "           "  << BA_VERSION   << std::endl;
    std::cout << "           " << BA_DATE      << std::endl;
    std::cout << "         "    << BA_AUTHOR    << "\n" << std::endl;
} // End displayMessage()

int BrightnessAdjustment::calcNumOfPixelsNonBGColor( const kvs::ColorImage& color_image )
{
    size_t counter = 0;
    kvs::RGBColor pixel;

    for ( size_t j = 0; j < color_image.height(); j++ ) {
        for ( size_t i = 0; i < color_image.width(); i++ ) {
            if ( color_image.pixel( i, j ) == m_bgcolor ) {
            } else { counter++; }
        }
    }

    return counter;
} // End calcNumOfPixelsNonBGColor()

kvs::UInt8 BrightnessAdjustment::calcMaxPixelValue( const kvs::GrayImage& gray_image )
{
    kvs::UInt8 max_pixel_value = 0;

    for ( size_t j = 0; j < gray_image.height(); j++ ) {
        for ( size_t i = 0; i < gray_image.width(); i++ ) {
            if ( gray_image.pixel( i, j ) > max_pixel_value ) 
                max_pixel_value = gray_image.pixel( i, j );
        }
    }

    return max_pixel_value;
} // End calcMaxPixelValue()

kvs::UInt8 BrightnessAdjustment::searchThresholdPixelValue(const kvs::GrayImage& gray_image, const size_t N_all_non_bgcolor_LR1, const kvs::UInt8 max_pixel_value_LR1)
{
    kvs::UInt8 threshold_pixel_value_LR1    = max_pixel_value_LR1;
    float tmp_ratio                         = 0.0f;

    // Search for threshold pixel value
    while ( tmp_ratio < m_ratio_of_reference_section ) {
        int counter = 0;
        for ( size_t j = 0; j < gray_image.height(); j++ ) {
            for ( size_t i = 0; i < gray_image.width(); i++ ) {
                if ( gray_image.pixel( i, j ) >= threshold_pixel_value_LR1 ) 
                    counter++;
            }
        }

        tmp_ratio = float(counter) / float(N_all_non_bgcolor_LR1);
        
        // Next pixel value
        threshold_pixel_value_LR1--;
    } // end while

    threshold_pixel_value_LR1++;
    std::cout   << "** Threshold pixel value (LR=1)      : " 
                << +threshold_pixel_value_LR1 << " (pixel value)" << std::endl;
    std::cout   << "** Ratio of threshold section (LR=1) : " 
                << std::setprecision(3) << tmp_ratio*100 << "(%) ( " 
                << +threshold_pixel_value_LR1 << " ~ " << +max_pixel_value_LR1 << " (pixel value) )" << std::endl;

    return threshold_pixel_value_LR1;
} // End searchThresholdPixelValue()

float BrightnessAdjustment::calcAdjustmentParameter( const kvs::ColorImage& color_image, const kvs::UInt8 threshold_pixel_value_LR1, const size_t N_all_non_bgcolor )
{
    float adjustment_parameter              = 1.0f;
    float tmp_ratio_of_reference_section    = 0.0f;
    
    std::cout << "\n** Adjusting Brightness of the image...\n" << std::endl;
    while ( tmp_ratio_of_reference_section < m_ratio_of_reference_section ) {
        // Update adjustment parameter
        adjustment_parameter += m_parameter_interval;

        tmp_ratio_of_reference_section = tempolarilyAdjustBrightness(
            /* kvs::ColorImage  */ color_image, 
            /* const float      */ adjustment_parameter, 
            /* const kvs::UInt8 */ threshold_pixel_value_LR1, 
            /* const size_t     */ N_all_non_bgcolor );
    } // end while

    // return adjustment_parameter -= m_parameter_interval;
    return adjustment_parameter;
} // End doBrightnessAdjustment()

inline float BrightnessAdjustment::tempolarilyAdjustBrightness( const kvs::ColorImage& color_image, const float p, const kvs::UInt8 threshold_pixel_value_LR1, const size_t N_all_non_bgcolor )
{
    kvs::ColorImage tmp_color_image = deepCopyColorImage( color_image );
    doBrightnessAdjustment( tmp_color_image, p );

    // Convert color to gray
    kvs::GrayImage tmp_gray_image( tmp_color_image );

    int counter = 0;
    for ( size_t j = 0; j < tmp_gray_image.height(); j++ )
        for ( size_t i = 0; i < tmp_gray_image.width(); i++ )
            if ( tmp_gray_image.pixel( i, j ) >= threshold_pixel_value_LR1 ) 
                counter++;

    float tmp_ratio = float(counter) / float(N_all_non_bgcolor);
    // std::cout   << "           ( parameter, ratio ) : ( " 
    //             << std::setprecision(3) << p << ", " 
    //             << std::setprecision(2) << 100*tmp_ratio << "(%) )" 
    //             << std::endl;

    return tmp_ratio;
} // End tempolarilyAdjustBrightness()

inline kvs::ColorImage BrightnessAdjustment::deepCopyColorImage( const kvs::ColorImage& other )
{
    kvs::ColorImage duplicated_color_image( other.width(), other.height() );

    for ( size_t j = 0; j < other.height(); j++ )
        for ( size_t i = 0; i < other.width(); i++ )
                duplicated_color_image.setPixel( i, j, other.pixel( i, j ) );

    return duplicated_color_image;
} // End deepCopyImage()

inline float BrightnessAdjustment::specifyNumOfDigits( const float p, const float digits )
{
    std::stringstream ss;
    ss << p;
    std::string p_str = ss.str();

    return stof( p_str.substr(0, digits) );
} // End specifyNumOfDigits()

void BrightnessAdjustment::doBrightnessAdjustment( kvs::ColorImage& color_image, const float p )
{
    kvs::RGBColor   pixel;
    kvs::UInt8      r, g, b;

    for ( size_t j = 0; j < color_image.height(); j++ ) {
        for ( size_t i = 0; i < color_image.width(); i++ ) {
            pixel = color_image.pixel( i, j );
            
            // Check whether the pixel value exceeds 255 after adjusting.
            if ( p*pixel.r() > 255 ) r = 255;
            else                     r = p*pixel.r();
            
            if ( p*pixel.g() > 255 ) g = 255;
            else                     g = p*pixel.g();

            if ( p*pixel.b() > 255 ) b = 255;
            else                     b = p*pixel.b();

            pixel.set( r, g, b );

            // Apply adjustment
            color_image.setPixel( i, j, pixel );
        } // end for
    } // end for
} // End doBrightnessAdjustment()