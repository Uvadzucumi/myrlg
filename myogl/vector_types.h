#ifndef VECTORS_H_INCLUDED
#define VECTORS_H_INCLUDED

namespace MyOGL{

    template <class Type>
    struct Vector2{
        union{
            //Position2f position;
            //uv2f uv;
            struct{
                Type x,y;
            };
            struct{
                Type u,v;
            };
            Type data[2];
        };
    };

    template <class Type>
    struct Vector3{
        union{
            //Position2f position;
            //uv2f uv;
            struct{
                Type x,y,z;
            };
            struct{
                Type r,g,b;
            };
            Type data[3];
        };
    };

    template <class Type>
    struct Vector4{
        union{
            //Position2f position;
            //uv2f uv;
            struct{
                Type left,top,width,height;
            };
            struct{
                Type x,y,z,w;
            };
            struct{
                Type r,g,b,a;
            };
            Type data[4];
        };
    };

    typedef Vector2 <int> Vector2i;
    typedef Vector2 <unsigned int> Vector2ui;
    typedef Vector2 <float> Vector2f;
    typedef Vector3 <int> Vector3i;
    typedef Vector3 <float> Vector3f;
    typedef Vector4 <int> Vector4i;
    typedef Vector4 <unsigned int> Vector4ui;
    typedef Vector4 <float> Vector4f;

}

#endif // VECTORS_H_INCLUDED
