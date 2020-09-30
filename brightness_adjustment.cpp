//////////////////////////////////////////
//   @file   brightness_adjustment.cpp
//   @author Tomomasa Uchida
//   @date   2020/09/30
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

const float RATIO_IN_REFERENCE_SECTION = 0.01f;
const float PARAMETER_INTERVAL = 0.01f;

BrightnessAdjustment::BrightnessAdjustment( FILE_FORMAT4BA file_format ):
    m_file_format( file_format ),
    m_snapshot_counter( 0 )
    // m_ratio_of_reference_section( 0.01f ),
    // m_parameter_interval( 0.01f )
{
    // Message
    std::cout << "\n*** BrightnessAdjustment constructor is called." << std::endl;

    // Display the file format of the input data
    if ( m_file_format == SPBR_ASCII4BA )
        std::cout << "*** (FILE_FORMAT: SPBR_ASCII)\n"  << std::endl;
    else if ( m_file_format == SPBR_BINARY4BA )
        std::cout << "*** (FILE_FORMAT: SPBR_BINARY)\n" << std::endl;
    else if ( m_file_format == PLY_ASCII4BA )
        std::cout << "*** (FILE_FORMAT: PLY_ASCII)\n"   << std::endl;
    else if ( m_file_format == PLY_BINARY4BA )
        std::cout << "*** (FILE_FORMAT: PLY_BINARY)\n"  << std::endl;
} // End constructor

void BrightnessAdjustment::RegisterObject( kvs::Scene* scene, int argc, char** argv, SPBR* spbr_engine, const size_t repeat_level )
{
    scene->registerObject( CreateObject( argc, argv ), CreateRenderer( spbr_engine, repeat_level ) );
} // End RegisterObject()

kvs::PointObject* BrightnessAdjustment::CreateObject( int argc, char** argv ) {
    kvs::PointObject* object = NULL;

    // Read the first data file (argv[1])
    if ( m_file_format == PLY_ASCII4BA ) {
        SPBR* spbr_engine = new SPBR( argv[1], PLY_ASCII );
        object = CreateObjectCommon( argc, argv, spbr_engine );

    } else if ( m_file_format == PLY_BINARY4BA ) {
        SPBR* spbr_engine = new SPBR( argv[1], PLY_BINARY );
        object = CreateObjectCommon( argc, argv, spbr_engine );

    } else if ( m_file_format == SPBR_BINARY4BA ) {
        SPBR* spbr_engine = new SPBR( argv[1], SPBR_BINARY );
        object = CreateObjectCommon( argc, argv, spbr_engine );

    } else if ( m_file_format == SPBR_ASCII4BA ) {
        SPBR* spbr_engine = new SPBR( argv[1], SPBR_ASCII );
        object = CreateObjectCommon( argc, argv, spbr_engine );
    } // end if

    return object;
} // End CreateObject()

kvs::PointObject* BrightnessAdjustment::CreateObjectCommon( int argc, char** argv, SPBR* spbr_engine ) {
    kvs::PointObject* object = spbr_engine;

    // Read and append the remaining files:  
    //  argv[2], argv[3], ..., argv[argc-1]
    for ( int i = 3; i <= argc; i++ ) {
        if ( isASCII_PLY_File( argv[i - 1] ) ) {
            SPBR* spbr_tmp = new SPBR( argv[i - 1], PLY_ASCII );
            object->add( *kvs::PointObject::DownCast( spbr_tmp ) );

        } else if ( isBINARY_PLY_File( argv[i - 1] ) ) {
            SPBR* spbr_tmp = new SPBR( argv[i - 1], PLY_BINARY );
            object->add( *kvs::PointObject::DownCast( spbr_tmp ) );
        
        } else if ( isBinarySPBR_File( argv[i - 1] ) ) {
            SPBR* spbr_tmp = new SPBR( argv[i - 1], SPBR_BINARY );
            object->add( *kvs::PointObject::DownCast( spbr_tmp ) );
        
        } else {
            SPBR* spbr_tmp = new SPBR( argv[i - 1], SPBR_ASCII );
            object->add( *kvs::PointObject::DownCast( spbr_tmp ) );
        } // end if
    } // end for

    addBoundingBoxToScene( spbr_engine );

    // Forced shuffle
    if ( spbr_engine->isForcedShuffleOn() ) {
        Shuffle shuffle_engine( spbr_engine );
    }

    // Set object name
    object->setName( "Object" );

    // Object rotation (Z==>X) if required
    if ( spbr_engine->isZXRotation() ) {
        double zrot_deg = spbr_engine->objectZXRotAngle(0);
        double xrot_deg = spbr_engine->objectZXRotAngle(1);
        ToolXform::rotateZX( object, zrot_deg, xrot_deg, kvs::Vector3f( 0, 0, 0 ) );
    }

    return object;
} // End CreateObject()

kvs::glsl::ParticleBasedRenderer* BrightnessAdjustment::CreateRenderer( SPBR* spbr_engine, const size_t repeat_level )
{
    kvs::glsl::ParticleBasedRenderer* renderer = new kvs::glsl::ParticleBasedRenderer();

    // Set rendere name
    renderer->setName( "Renderer" );

    // Set repeat level
    renderer->setRepetitionLevel( repeat_level );
    
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

void BrightnessAdjustment::ReplaceObject( kvs::Scene* scene, int argc, char** argv, SPBR* spbr_engine, const size_t repeat_level )
{
    scene->replaceObject( "Object", CreateObject( argc, argv ) );
    scene->replaceRenderer( "Renderer", CreateRenderer( spbr_engine, repeat_level ) );
} // End ReplaceObject()

void BrightnessAdjustment::SnapshotImage( kvs::Scene* scene, const std::string filename, const int repeat_level )
{
    // Snapshot
    scene->screen()->redraw();
    kvs::ColorImage color_image_tmp = scene->camera()->snapshot();

    // Save the snapshot image
    if ( m_snapshot_counter == 0 ) m_color_image     = color_image_tmp;
    if ( m_snapshot_counter == 1 ) m_color_image_LR1 = color_image_tmp;

    // Write the snapshot image
    color_image_tmp.write( filename + "_LR" + kvs::String::ToString( repeat_level ) + ".bmp" );

    // Update snapshot counter
    m_snapshot_counter++;
} // End SnapshotImage()

void BrightnessAdjustment::AdjustBrightness( const std::string filename )
{
    // Display opening message
    displayMessage();

    // Calc number of pixels of the image
    const size_t npixels             = m_color_image.numberOfPixels();
    const size_t npixels_non_bgcolor = calcNumberOfPixelsNonBGColor( m_color_image );
    std::cout   << "*** Number of pixels                  : " 
                << npixels             << " (pixels)" << std::endl;
    std::cout   << "*** Number of pixels non-BGColor      : " 
                << npixels_non_bgcolor << " (pixels)" << std::endl;

    // Convert color to gray
    const kvs::GrayImage gray_image_LR1( m_color_image_LR1 );

    // ===================================
    //  STEP1: Get max pixel value (LR=1)
    // ===================================
    const kvs::UInt8 max_pixel_value_LR1 = calcMaxPixelValue( gray_image_LR1 );
    std::cout   << "*** Max pixel value (LR=1)            : " 
                << +max_pixel_value_LR1 << " (pixel value)" << std::endl;

    // ================================================
    //  STEP2: Search for threshold pixel value (LR=1) 
    // ================================================
    const size_t npixels_non_bgcolor_LR1 = calcNumberOfPixelsNonBGColor( m_color_image_LR1 );
    const kvs::UInt8 threshold_pixel_value_LR1 = searchThresholdPixelValue( gray_image_LR1, npixels_non_bgcolor_LR1, max_pixel_value_LR1 );

    // =======================================
    //  STEP3: Adjust brightness of the image
    // =======================================
    std::cout << "\n*** Doing \"Brightness Adjustment\"...\n" << std::endl;
    float p = calcAdjustmentParameter( m_color_image, threshold_pixel_value_LR1, npixels_non_bgcolor );
    p = specifyNumberOfDigits( p, 4 );
    doBrightnessAdjustment( m_color_image, p );
    std::cout   << "*** Adjustment parameter              : " 
                << std::setprecision(3) << p << std::endl;

    // Write adjusted image
    std::ostringstream oss;
    oss << p*100;
    std::string adjusted_image_filename( filename + "_adjusted" + oss.str() + ".bmp" );
    m_color_image.write( adjusted_image_filename );
    std::cout << "*** Saved adjusted image."                     << std::endl;
    std::cout << "    (PATH: " << adjusted_image_filename << ")" << std::endl;
    std::cout << "===========================================\n" << std::endl;
    
    // Exec. open command (macOS only)
#ifdef OS_MAC
    std::string EXEC( "open " );
    EXEC += adjusted_image_filename;
    system( EXEC.c_str() );
#endif

} // End AdjustBrightness()

inline void BrightnessAdjustment::displayMessage() const
{
    std::cout << "\n\n";
    std::cout << BA_TITLE << "\n";
    std::cout << "                " << BA_DATE << "\n";
    std::cout << "              " << BA_AUTHOR << "\n\n";
} // End displayMessage()

int BrightnessAdjustment::calcNumberOfPixelsNonBGColor( const kvs::ColorImage& color_image ) const
{
    size_t non_bgcolor_counter = 0;

    for ( size_t j = 0; j < color_image.height(); j++ ) {
        for ( size_t i = 0; i < color_image.width(); i++ ) {
            if ( color_image.pixel( i, j ) == m_bgcolor ) {
            } else { non_bgcolor_counter++; }
        }
    }

    return non_bgcolor_counter;
} // End calcNumOfPixelsNonBGColor()

kvs::UInt8 BrightnessAdjustment::calcMaxPixelValue( const kvs::GrayImage& gray_image ) const
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

kvs::UInt8 BrightnessAdjustment::searchThresholdPixelValue( const kvs::GrayImage& gray_image, const size_t npixels_non_bgcolor_LR1, const kvs::UInt8 max_pixel_value_LR1 ) const
{
    kvs::UInt8 threshold_pixel_value_LR1 = max_pixel_value_LR1;
    float ratio_tmp = 0.0f;

    // Search for threshold pixel value
    while ( ratio_tmp < RATIO_IN_REFERENCE_SECTION ) {
        int counter = 0;
        for ( size_t j = 0; j < gray_image.height(); j++ ) {
            for ( size_t i = 0; i < gray_image.width(); i++ ) {
                if ( gray_image.pixel( i, j ) >= threshold_pixel_value_LR1 ) 
                    counter++;
            }
        }

        ratio_tmp = float( counter ) / float( npixels_non_bgcolor_LR1 );
        
        // Next pixel value
        threshold_pixel_value_LR1--;
    } // end while
    threshold_pixel_value_LR1++;

    std::cout   << "*** Threshold pixel value (LR=1)      : " 
                << +threshold_pixel_value_LR1 << " (pixel value)" << std::endl;
    std::cout   << "*** Ratio in reference section (LR=1) : " 
                << std::setprecision(3) << ratio_tmp*100 << "(%) ( " 
                << +threshold_pixel_value_LR1 << " ~ " << +max_pixel_value_LR1 << " (pixel value) )" << std::endl;

    return threshold_pixel_value_LR1;
} // End searchThresholdPixelValue()

float BrightnessAdjustment::calcAdjustmentParameter( const kvs::ColorImage& color_image, const kvs::UInt8 threshold_pixel_value_LR1, const size_t npixels_non_bgcolor )
{
    float adjustment_parameter = 1.0f;
    float ratio_tmp = 0.0f;
    
    while ( ratio_tmp < RATIO_IN_REFERENCE_SECTION ) {
        // Update adjustment parameter
        adjustment_parameter += PARAMETER_INTERVAL;

        ratio_tmp = calcRatioTempolarily(
            /* kvs::ColorImage  */ color_image, 
            /* const float      */ adjustment_parameter, 
            /* const kvs::UInt8 */ threshold_pixel_value_LR1, 
            /* const size_t     */ npixels_non_bgcolor );
    } // end while

    // return adjustment_parameter -= m_parameter_interval;
    return adjustment_parameter;
} // End calcAdjustmentParameter()

inline float BrightnessAdjustment::calcRatioTempolarily( const kvs::ColorImage& color_image, const float p_current, const kvs::UInt8 threshold_pixel_value_LR1, const size_t npixels_non_bgcolor )
{
    kvs::ColorImage color_image_tmp = deepCopyColorImage( color_image );
    doBrightnessAdjustment( color_image_tmp, p_current );

    // Convert color to gray
    const kvs::GrayImage gray_image_tmp( color_image_tmp );

    int counter = 0;
    for ( size_t j = 0; j < gray_image_tmp.height(); j++ )
        for ( size_t i = 0; i < gray_image_tmp.width(); i++ )
            if ( gray_image_tmp.pixel( i, j ) >= threshold_pixel_value_LR1 ) 
                counter++;

    const float ratio_tmp = float( counter ) / float( npixels_non_bgcolor );
    return ratio_tmp;
} // End calcRatioTempolarily()

inline kvs::ColorImage BrightnessAdjustment::deepCopyColorImage( const kvs::ColorImage& other ) const
{
    kvs::ColorImage duplicated_color_image( other.width(), other.height() );

    for ( size_t j = 0; j < other.height(); j++ )
        for ( size_t i = 0; i < other.width(); i++ )
            duplicated_color_image.setPixel( i, j, other.pixel( i, j ) );

    return duplicated_color_image;
} // End deepCopyColorImage()

inline float BrightnessAdjustment::specifyNumberOfDigits( const float p, const float digits ) const
{
    std::stringstream ss;
    ss << p;
    std::string p_str = ss.str();

    return stof( p_str.substr( 0, digits ) );
} // End specifyNumberOfDigits()

inline void BrightnessAdjustment::doBrightnessAdjustment( kvs::ColorImage& color_image, const float p ) const
{
    kvs::RGBColor   pixel;
    kvs::UInt8      r, g, b;

    for ( size_t j = 0; j < color_image.height(); j++ ) {
        for ( size_t i = 0; i < color_image.width(); i++ ) {
            pixel = color_image.pixel( i, j );
            
            // Check whether the pixel value exceeds 255 after adjusting.
            if ( pixel.r() * p > 255 ) r = 255;
            else                       r = pixel.r() * p;
            
            if ( pixel.g() * p > 255 ) g = 255;
            else                       g = pixel.g() * p;

            if ( pixel.b() * p > 255 ) b = 255;
            else                       b = pixel.b() * p;

            // Apply adjustment
            pixel.set( r, g, b );
            color_image.setPixel( i, j, pixel );
        } // end for
    } // end for
} // End doBrightnessAdjustment()