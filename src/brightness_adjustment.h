////////////////////////////////////////
//   @file   brightness_adjustment.h
//   @author Tomomasa Uchida
//   @date   2020/09/30
////////////////////////////////////////

#if !defined  BRIGHTNESS_ADJUSTMENT_H
#define       BRIGHTNESS_ADJUSTMENT_H

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
    BrightnessAdjustment( const FILE_FORMAT4BA file_format );

    // Functions to control object and renderer
public:
    void   RegisterObject( kvs::Scene* scene, int argc, char** argv, SPBR* spbr_engine, const size_t repeat_level );
    void   ReplaceObject( kvs::Scene* scene, int argc, char** argv, SPBR* spbr_engine, const size_t repeat_level );
    void   SnapshotImage( kvs::Scene* scene, const std::string filename, const int repeat_level );
    size_t getSnapshotCounter() const { return m_snapshot_counter; };
private:
    kvs::PointObject* CreateObject( int argc, char** argv );
    kvs::PointObject* CreateObjectCommon( int argc, char** argv, SPBR* spbr_engine );
    kvs::glsl::ParticleBasedRenderer* CreateRenderer( SPBR* spbr_engine, const size_t repeat_level );

    // Functions to adjust brightness of an image
public:
    void        AdjustBrightness( const std::string filename );
    // void        setBackgroundColor( const kvs::RGBColor bgcolor ) { m_bgcolor = bgcolor; };
private:
    void        displayMessage() const;
    int         calcNumberOfPixelsNonBGColor( const kvs::ColorImage& image ) const;
    kvs::UInt8  calcMaxPixelValue( const kvs::GrayImage& image ) const;
    kvs::UInt8  searchThresholdPixelValue( const kvs::GrayImage& gray_image, const size_t npixels_non_bgcolor_LR1, const kvs::UInt8 max_pixel_value_LR1 ) const;
    float       calcAdjustmentParameter( const kvs::ColorImage& color_image, const kvs::UInt8 threshold_pixel_value_LR1, const size_t npixels_non_bgcolor );
    float       calcTemporaryPercent( const kvs::ColorImage& color_image, const float p_current, const kvs::UInt8 threshold_pixel_value_LR1, const size_t npixels_non_bgcolor );
    kvs::ColorImage deepCopyColorImage( const kvs::ColorImage& other ) const;
    float       specifyNumberOfDigits( const float p, const float digits ) const;
    void        doBrightnessAdjustment( kvs::ColorImage& color_image, const float p ) const;
    float       calcFinalPercent( const kvs::ColorImage& color_image, const kvs::UInt8 threshold_pixel_value_LR1, const size_t npixels_non_bgcolor ) const;
    void        writeAdjustedImage( const std::string filename , const kvs::ColorImage& color_image, const float p_final ) const;
    void        execOpenCommand( const std::string filename ) const;
    
    //---------- DATA ----------//
    const FILE_FORMAT4BA    m_file_format;
    const kvs::RGBColor     m_bgcolor;
    size_t                  m_snapshot_counter;
    kvs::ColorImage         m_color_image, m_color_image_LR1;
};

#endif // end of brightness_adjustment.h