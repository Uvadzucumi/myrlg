#ifndef VECTORS_H_INCLUDED
#define VECTORS_H_INCLUDED

namespace MyOGL{

    template <class Type>
    class Vector2{
        public:
        union{
            //Position2f position;
            //uv2f uv;
            struct{
                Type x,y;
            };
            struct{
                Type u,v;
            };
            struct{
                Type width,height;
            };
            Type data[2];
        };
        void Set(Type x_value, Type y_value){
            x=x_value;
            y=y_value;
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
        void Set(Type x_value, Type y_value, Type z_value){
            x=x_value;
            y=y_value;
            z=z_value;
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
        void Set(Type x_value, Type y_value, Type z_value, Type w_value){
            x=x_value;
            y=y_value;
            z=z_value;
            w=w_value;
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
