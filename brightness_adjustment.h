////////////////////////////////////////
//   @file   brightness_adjustment.h
//   @author Tomomasa Uchida
//   @date   2019/03/15
////////////////////////////////////////

#if !defined  BRIGHTNESS_ADJUSTMENT_H
#define       BRIGHTNESS_ADJUSTMENT_H

#include "spbr.h"

class BrightnessAdjustment {
public:
    enum FILE_FORMAT4BA
    {
        SPBR_ASCII4BA  = 0,
        SPBR_BINARY4BA = 1,
        PLY_ASCII4BA   = 2,
        PLY_BINARY4BA  = 3,
    };

    // Constructor
    BrightnessAdjustment();
    BrightnessAdjustment( FILE_FORMAT4BA file_format );

    // Functions to control object and renderer
public:
    void        RegisterObject( kvs::Scene* scene, int argc, char** argv, SPBR* spbr_engine, const size_t LR );
    void        ReplaceObject( kvs::Scene* scene, int argc, char** argv, SPBR* spbr_engine, const size_t LR );
    void        SnapshotImage( kvs::Scene* scene, const std::string filename, const int repeat_level );
    size_t      getSnapshotCounter() const { return m_snapshot_counter; };
private:
    kvs::PointObject* CreateObject( int argc, char** argv );
    kvs::PointObject* CreateObjectCommon(int argc, char** argv, SPBR* spbr_engine, kvs::PointObject* object);
    kvs::glsl::ParticleBasedRenderer* CreateRenderer( SPBR* spbr_engine, const size_t LR);

    // Functions to adjust Brightness of an image
public:
    void        adjustBrightness( const std::string filename );
    void        setBackgroundColor( kvs::RGBColor bgcolor ) { m_bgcolor = bgcolor; };
private:
    void        displayMessage();
    int         calcNumOfPixelsNonBGColor( const kvs::ColorImage& image );
    kvs::UInt8  calcMaxPixelValue( const kvs::GrayImage& image );
    kvs::UInt8  searchThresholdPixelValue( const kvs::GrayImage& gray_image, const size_t N_all_non_bgcolor_LR1, const kvs::UInt8 max_pixel_value_LR1 );
    float       calcAdjustmentParameter( const kvs::ColorImage& color_image, const kvs::UInt8 threshold_pixel_value_LR1, const size_t N_all_non_bgcolor );
    float       tempolarilyAdjustBrightness( const kvs::ColorImage& color_image, const float p, const kvs::UInt8 threshold_pixel_value_LR1, const size_t N_all_non_bgcolor );
    kvs::ColorImage deepCopyColorImage( const kvs::ColorImage& other );
    float       specifyNumOfDigits( const float p, const float digits );
    void        doBrightnessAdjustment( kvs::ColorImage& color_image, const float p );
    
    //---------- DATA ----------//
    FILE_FORMAT4BA  m_file_format;
    size_t          m_snapshot_counter;
    kvs::RGBColor   m_bgcolor;
    kvs::ColorImage m_img_Color, m_img_Color_LR1;
    const float     m_ratio_of_reference_section;
    const float     m_parameter_interval;
};

#endif // end of brightness_adjustment.h