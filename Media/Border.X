xof 0303txt 0032
template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}

template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshNormals {
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template Coords2d {
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}

template MeshTextureCoords {
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}


Material PDX01_-_Default_01_-_DefaultSub0 {
 1.000000;1.000000;1.000000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Border_tlxadd.png";
 }
}

Material PDX01_-_Default_Material__1Sub1 {
 1.000000;1.000000;1.000000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Border_tlxadd.png";
 }
}

Frame NGon001 {
 

 FrameTransformMatrix {
  -0.000000,0.000000,1.997628,0.000000,0.000000,1.000000,0.000000,0.000000,-1.997628,0.000000,-0.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Frame {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,-0.500000,0.000000,1.000000;;
  }

  Mesh  {
   12;
   0.500000;0.000000;0.000000;,
   0.250000;0.000000;0.433013;,
   -0.250000;0.000000;0.433013;,
   -0.500000;0.000000;-0.000000;,
   -0.250000;0.000000;-0.433013;,
   0.250000;0.000000;-0.433013;,
   0.500000;0.500000;0.000000;,
   0.250000;0.500000;0.433013;,
   -0.250000;0.500000;0.433013;,
   -0.500000;0.500000;-0.000000;,
   -0.250000;0.500000;-0.433013;,
   0.250000;0.500000;-0.433013;;
   12;
   3;7,0,6;,
   3;0,7,1;,
   3;8,1,7;,
   3;1,8,2;,
   3;9,2,8;,
   3;2,9,3;,
   3;10,3,9;,
   3;3,10,4;,
   3;11,4,10;,
   3;4,11,5;,
   3;6,5,11;,
   3;5,6,0;;

   MeshNormals  {
    6;
    0.866025;0.000000;0.500000;,
    -0.000000;0.000000;1.000000;,
    -0.866025;0.000000;0.500000;,
    -0.866025;0.000000;-0.500000;,
    0.000000;0.000000;-1.000000;,
    0.866025;0.000000;-0.500000;;
    12;
    3;0,0,0;,
    3;0,0,0;,
    3;1,1,1;,
    3;1,1,1;,
    3;2,2,2;,
    3;2,2,2;,
    3;3,3,3;,
    3;3,3,3;,
    3;4,4,4;,
    3;4,4,4;,
    3;5,5,5;,
    3;5,5,5;;
   }

   MeshMaterialList  {
    1;
    12;
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0;
    { PDX01_-_Default_Material__1Sub1 }
   }

   MeshTextureCoords  {
    12;
    0.500000;0.597284;,
    0.980000;0.597284;,
    0.980000;0.597284;,
    0.500000;0.597284;,
    0.020000;0.597284;,
    0.020000;0.597284;,
    0.500000;0.043028;,
    0.980000;0.043028;,
    0.980000;0.043028;,
    0.500000;0.043028;,
    0.020000;0.043028;,
    0.020000;0.043028;;
   }
  }
 }
}