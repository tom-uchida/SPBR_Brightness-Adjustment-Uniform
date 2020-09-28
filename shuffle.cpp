///////////////////////
///// shuffle.cpp /////
///////////////////////

#include "shuffle.h"
#include <kvs/ValueArray>
#include <algorithm>


//-----
Shuffle::Shuffle( const kvs::PointObject* point )
{
    this->exec( point );
}


//-----
kvs::PointObject* Shuffle::exec( const kvs::ObjectBase* object )
{
    // Treat the given "object" as a point object. 
    const kvs::PointObject* point = kvs::PointObject::DownCast( object );

    // Get the number of points in the PointObject.
    //    const size_t npoints = point->nvertices(); // KVS1
    const size_t npoints = point->numberOfVertices(); // KVS2

    // Create shuffled indices for temporal arrays
    kvs::ValueArray<int> index( npoints ); // We need npoints indeces.
    for ( size_t i = 0; i < npoints; i++ ) index[i] = static_cast<int>( i );
    std::random_shuffle( index.begin(), index.end() );

    // Create the shuffled "coords" array.
    {
      // Temporary array for coords (initialized to empty)
      //   x0 y0 z0 x1 y1 z1 x2 y2 z2 ... 
      kvs::ValueArray<kvs::Real32> coords( npoints * 3 ); 

      // Define a pointer to an element of the array. 
      //   It is initialized to &(coords[0]).
      kvs::Real32* pcoords = coords.pointer(); 

      // Set coords of the index[i]-th point 
      //   as the i-th elemeht of the array.
      for ( size_t i = 0; i < npoints; i++ )
      {
        // Set coords of the index[i]-th point 
        //   as the i-th elemeht of the array
        const kvs::Vector3f v = point->coord( index[i] );
        *(pcoords++) = v.x();
        *(pcoords++) = v.y();
        *(pcoords++) = v.z();
      }

      // Replace coords of the point object with suffled result
      setCoords( coords ); 
    }


    // Create the shuffled color array.
    //    if ( point->ncolors() == 1 ) setColor( point->color() );//KVS1
    //    else if ( point->ncolors() > 1 )                        //KVS1
    if ( point->numberOfColors() == 1 ) setColor( point->color() );//KVS2
    else if ( point->numberOfColors() > 1 )                       //KVS2 
    {
        // Temporary array for colors (initialized to empty)
        //   r0 g0 b0 r1 g1 b1 r2 g2 b2 ... 
        kvs::ValueArray<kvs::UInt8> colors( npoints * 3 );

        // Define a pointer to an element of the array. 
        //   It is initialized to &(colors[0]).
        kvs::UInt8* pcolors = colors.pointer(); 

        // Set colors of the index[i]-th point 
        //   as the i-th elemeht of the array
        for ( size_t i = 0; i < npoints; i++ )
        {
            const kvs::RGBColor c = point->color( index[i] );
            *(pcolors++) = c.r();
            *(pcolors++) = c.g();
            *(pcolors++) = c.b();
        }

        // Replace colors of the point object with suffled result
        setColors( colors );
    }


    // Create the shuffled surface normal array.
    //    if ( point->nnormals() > 1 )  // KVS1 
    if ( point->numberOfNormals() > 1 ) // KVS2
    {
        // Temporary array for normals (initialized to empty)
        //   nx0 ny0 nz0 nx1 ny1 nz1 nx2 ny2 nz2 ... 
        kvs::ValueArray<kvs::Real32> normals( npoints * 3 );
                                         
        // Define a pointer to an element of the array. 
        //   It is initialized to &(normals[0]).
        kvs::Real32* pnormals = normals.pointer(); 

        // Set normals of the index[i]-th point 
        //   as the i-th elemeht of the array
        for ( size_t i = 0; i < npoints; i++ )
        {
            const kvs::Vector3f n = point->normal( index[i] );
            *(pnormals++) = n.x();
            *(pnormals++) = n.y();
            *(pnormals++) = n.z();
        }

        // Replace normals of the point object with suffled result
        setNormals( normals );
    }

    //?????
    //    setSize( 1 );

    // Copy the original bounding-box information to the shuffled point set
    setMinMaxObjectCoords  ( point->minObjectCoord(), 
                             point->maxObjectCoord()   );
    setMinMaxExternalCoords( point->minExternalCoord(), 
                             point->maxExternalCoord() );

    // EndFn
    return( this );

}// exec()

// end of shuffle.cpp

