/////////////////////
///// shuffle.h /////
/////////////////////

#ifndef SHUFFLE_H_INCLUDE
#define SHUFFLE_H_INCLUDE

#include <kvs/FilterBase>
#include <kvs/PointObject>

class Shuffle : public kvs::FilterBase, public kvs::PointObject
{
public:

    Shuffle( const kvs::PointObject* point );

    kvs::PointObject* exec( const kvs::ObjectBase* object );
};

#endif // SHUFFLE_H_INCLUDE
// end of shuffle.h
