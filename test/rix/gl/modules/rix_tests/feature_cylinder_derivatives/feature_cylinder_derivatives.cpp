// Copyright (c) 2012-2016, NVIDIA CORPORATION. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include <test/testfw/manager/Manager.h>
#include "feature_cylinder_derivatives.h"

#include <dp/gl/RenderTarget.h>
#include <dp/util/Image.h>
#include <dp/math/Trafo.h>

#include <test/rix/core/framework/RiXBackend.h>
#include <test/rix/core/helpers/GeometryHelper.h>
#include <test/rix/core/helpers/TextureHelper.h>

#include <limits>

//Automatically add the test to the module's global test list
REGISTER_TEST("feature_cylinder_derivatives", "All the possible geometric shapes with the cylinder helper function", create_feature_cylinder_derivatives);

using namespace dp;
using namespace math;
using namespace rix::core;
using namespace rix::util;

Feature_cylinder_derivatives::Feature_cylinder_derivatives()
  : m_renderData(nullptr)
  , m_rix(nullptr)
{
}

Feature_cylinder_derivatives::~Feature_cylinder_derivatives()
{
}

bool Feature_cylinder_derivatives::onInit()
{
  DP_ASSERT( dynamic_cast<test::framework::RiXBackend*>(&(*m_backend)) );
  m_rix = static_cast<test::framework::RiXBackend*>(&(*m_backend))->getRenderer();
  m_displayTarget.inplaceCast<dp::gl::RenderTarget>()->setClearColor( 0.46f, 0.72f, 0.0f, 1.0f );

  m_renderData = new test::framework::RenderDataRiX;

  glEnable(GL_DEPTH_TEST);

  createScene();

  return true;
}

bool Feature_cylinder_derivatives::onRun( unsigned int idx )
{
  render(m_renderData, m_displayTarget);

  return true;
}

bool Feature_cylinder_derivatives::onClear()
{
  delete m_renderData;

  return true;
}

void Feature_cylinder_derivatives::createScene()
{
  const char * vertexShader = ""
    "#version 400\n"
    "layout(location=0) in vec3 Position;\n"
    "layout(location=2) in vec3 Normal;\n"
    "layout(location=8) in vec2 TexCoord;\n\n"

    "uniform mat4 model2world;\n"
    "uniform mat4 model2worldIT;\n\n"

    "uniform mat4 world2view;\n"
    "uniform mat4 view2world;\n"
    "uniform mat4 view2clip;\n"
    "uniform mat4 world2clip;\n\n"

    "out vec3 vPosition;\n"
    "out vec2 vTexCoord;\n"
    "out vec3 vEyePos;\n"
    "out vec3 vNormal;\n"
    "out vec3 vTangent;\n"
    "out vec3 vBinormal;\n\n"

    "void main(void)\n"
    "{\n"
    "  vec4 worldPos = model2world * vec4( Position, 1.0 );\n\n"

    "  vPosition     = worldPos.xyz;\n"
    "  vTexCoord     = TexCoord;\n"
    "  vNormal       = ( model2worldIT * vec4( Normal, 0.0 ) ).xyz;\n\n"

    "  //Get the translation part of the inverse view matrix\n"
    "  vEyePos       = vec3( view2world[3][0], view2world[3][1], view2world[3][2] );\n"
    "  gl_Position   = world2clip * worldPos;\n"
    "}\n";

  const char * fragmentTextureShader = "" 
    "#version 400\n"
    "uniform sampler2D diffuseTex;\n"
    "uniform vec3 lightDir;\n\n"

    "in vec3 vPosition;\n"
    "in vec2 vTexCoord;\n"
    "in vec3 vNormal;\n"
    "in vec3 vEyePos;\n"
    "layout(location = 0, index = 0) out vec4 Color;\n\n"

    "// Phong lighting\n"
    "vec3 eval(in vec3 wo, in vec3 ns, in vec3 wi, \n"
    "          in vec3 ambientColor, in vec3 diffuseColor, in vec3 specularColor,\n"
    "          in float ambient, in float diffuse, in float specular, in float exponent)\n"
    "{\n"
    "  float shine = 0.0;\n"
    "  float ns_dot_wi = max(0.0, dot(ns, wi));\n"
    "  if(0.0 < ns_dot_wi)\n"
    "  {\n"
    "  // Phong\n"
    "    vec3 R = reflect(-wi, ns);\n"
    "    float r_dot_wo = max(0.0, dot(R, wo));\n"
    "    shine = 0.0 < exponent ? pow(r_dot_wo, exponent) : 1.0;\n"
    "  }\n"
    "  return ambient * ambientColor +\n"
    "         ns_dot_wi * diffuse * diffuseColor +\n"
    "         shine * specular * specularColor;\n"
    "}\n\n"

    "void main(void)\n"
    "{\n"
    "  vec3 emissiveColor;\n"
    "  vec3 ambientColor;\n"
    "  vec3 diffuseColor;\n"
    "  vec3 specularColor;\n"
    "  float ambient;\n"
    "  float diffuse;\n"
    "  float specular;\n"
    "  float exponent;\n"

    "  emissiveColor = vec3(0.0, 0.0, 0.0);\n"
    "  ambientColor  = vec3(0.0, 0.0, 0.0);\n"
    "  diffuseColor  = texture(diffuseTex, vTexCoord).xyz;\n"
    "  specularColor = vec3(1.0, 1.0, 1.0);\n"
    "  exponent      = 16.0;\n"
    "  ambient       = 0.0;\n"
    "  diffuse       = 1.0;\n"
    "  specular      = 1.0;\n\n"

    "  //Direction from our Eye to the fragment at hand in tangent space\n"
    "  vec3 wo = normalize(vEyePos - vPosition);\n\n"

    "  //Normalized light direction in tangent space\n"
    "  vec3 wi = normalize(lightDir);\n\n"

    "  //Normal at fragment at hand read from the normal map\n"
    "  vec3 ns = normalize(vNormal);\n"
    "  vec3 rgb = eval( wo, ns, wi, \n"
    "              ambientColor, diffuseColor, specularColor, \n"
    "              ambient, diffuse, specular, exponent);\n"
    "  Color = vec4( rgb, 1.0 );\n\n"

    "}\n";

  //Geometry
  GeometryDataSharedPtr cylinderDataNormal  = createCylinder( ATTRIB_POSITION | ATTRIB_NORMAL | ATTRIB_TEXCOORD0, 64 );
  GeometryDataSharedPtr cylinderDataCut     = createCylinder( ATTRIB_POSITION | ATTRIB_NORMAL | ATTRIB_TEXCOORD0, 64, 8, 3.0f*PI_HALF );
  GeometryDataSharedPtr cylinderDataCutTube = createCylinder( ATTRIB_POSITION | ATTRIB_NORMAL | ATTRIB_TEXCOORD0, 64, 8, 3.0f*PI_HALF, 0.5f );
  
  GeometrySharedHandle cylinderNormal = rix::util::generateGeometry(cylinderDataNormal, m_rix);
  GeometrySharedHandle cylinderCut = rix::util::generateGeometry(cylinderDataCut, m_rix);
  GeometrySharedHandle cylinderCutTube = rix::util::generateGeometry(cylinderDataCutTube, m_rix);

  // Container Descriptors

  ProgramParameter vertexConstProgramParameters[] = {
    ProgramParameter("world2view", ContainerParameterType::MAT4X4),
    ProgramParameter("view2world", ContainerParameterType::MAT4X4),
    ProgramParameter("view2clip",  ContainerParameterType::MAT4X4),
    ProgramParameter("world2clip", ContainerParameterType::MAT4X4)
  };

  ProgramParameter vertexVarProgramParameters[] = {
    ProgramParameter("model2world", ContainerParameterType::MAT4X4),
    ProgramParameter("model2worldIT", ContainerParameterType::MAT4X4)
  };

  ProgramParameter fragmentTexturedProgramParameters[] = {
    ProgramParameter("diffuseTex", ContainerParameterType::SAMPLER) 
  };

  ProgramParameter fragmentConstProgramParameters[] = {
    ProgramParameter("lightDir", ContainerParameterType::FLOAT3)
  };

  ContainerDescriptorSharedHandle vertConstContainerDescriptor =
    m_rix->containerDescriptorCreate( ProgramParameterDescriptorCommon( vertexConstProgramParameters, 
    sizeof testfw::core::array(vertexConstProgramParameters) ) );

  ContainerDescriptorSharedHandle vertVarContainerDescriptor =
    m_rix->containerDescriptorCreate( ProgramParameterDescriptorCommon( vertexVarProgramParameters, 
    sizeof testfw::core::array(vertexVarProgramParameters) ) );

  ContainerDescriptorSharedHandle fragTexturedContainerDescriptor =
    m_rix->containerDescriptorCreate( ProgramParameterDescriptorCommon( fragmentTexturedProgramParameters,
    sizeof testfw::core::array(fragmentTexturedProgramParameters) ) );

  ContainerDescriptorSharedHandle fragConstContainerDescriptor =
    m_rix->containerDescriptorCreate( ProgramParameterDescriptorCommon( fragmentConstProgramParameters,
    sizeof testfw::core::array(fragmentConstProgramParameters) ) );


  // Program

  ProgramShaderCode vertShader( vertexShader, ShaderType::VERTEX_SHADER );
  ProgramShaderCode fragTexturedShader( fragmentTextureShader, ShaderType::FRAGMENT_SHADER );

  ContainerDescriptorSharedHandle vertContainerDescriptors[] = { vertConstContainerDescriptor, vertVarContainerDescriptor };
  ContainerDescriptorSharedHandle fragTexturedContainerDescriptors[] = { fragTexturedContainerDescriptor, fragConstContainerDescriptor  };

  ProgramDescription vertProgramDescription( vertShader, vertContainerDescriptors, sizeof testfw::core::array(vertContainerDescriptors) );
  ProgramDescription fragTexturedProgramDescription( fragTexturedShader, fragTexturedContainerDescriptors, sizeof testfw::core::array(fragTexturedContainerDescriptors) );

  ProgramSharedHandle vertProgram = m_rix->programCreate( vertProgramDescription );
  ProgramSharedHandle fragTexturedProgram = m_rix->programCreate( fragTexturedProgramDescription );


  // Program Pipeline

  ProgramSharedHandle programsTextured[] = {vertProgram, fragTexturedProgram};
  ProgramPipelineSharedHandle programTexturedPipeline = m_rix->programPipelineCreate( programsTextured, sizeof testfw::core::array(programsTextured) );


  // Containers

  ContainerSharedHandle vertViewProjContainer          = m_rix->containerCreate( vertConstContainerDescriptor );
  ContainerSharedHandle fragConstContainer             = m_rix->containerCreate( fragConstContainerDescriptor );

  ContainerSharedHandle vertCylinderNormalTransformContainer  = m_rix->containerCreate( vertVarContainerDescriptor );
  ContainerSharedHandle fragCylinderNormalMaterialContainer   = m_rix->containerCreate( fragTexturedContainerDescriptor );

  ContainerSharedHandle vertCylinderCutTransformContainer = m_rix->containerCreate( vertVarContainerDescriptor );
  ContainerSharedHandle fragCylinderCutMaterialContainer  = m_rix->containerCreate( fragTexturedContainerDescriptor );

  ContainerSharedHandle vertCylinderCutTubeTransformContainer  = m_rix->containerCreate( vertVarContainerDescriptor );
  ContainerSharedHandle fragCylinderCutTubeMaterialContainer   = m_rix->containerCreate( fragTexturedContainerDescriptor );

  // Container Data

  Mat44f view2Clip = makeFrustum<float>(-0.1f, 0.1f, -0.1f*m_height/m_width, 0.1f*m_height/m_width, 0.1f, 50.0f);
  Mat44f world2View = makeLookAt<float>( Vec3f(0.0f, 3.0f, 4.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f) );
  Mat44f world2ViewI = world2View;
  world2ViewI.invert();
  Mat44f world2Clip = world2View * view2Clip;

  Trafo model2world0;
  model2world0.setTranslation( Vec3f(-3.0f, 0.0f, 0.0f) );
  Mat44f model2world0IT = ~model2world0.getInverse();

  Trafo model2world1;
  model2world1.setTranslation( Vec3f(0.0f, 0.0f, 0.0f) );
  model2world1.setOrientation( Quatf( Vec3f(0.0f, 1.0f, 0.0), 5.0f*PI_QUARTER) );
  Mat44f model2world1IT = ~model2world1.getInverse();

  Trafo model2world2;
  model2world2.setTranslation( Vec3f(3.0f, 0.0f, 0.0f) );
  model2world2.setOrientation( Quatf( Vec3f(0.0f, 1.0f, 0.0), 4.5f*PI_QUARTER) );
  Mat44f model2world2IT = ~model2world2.getInverse();

  float bluish[4] = { 0.3f, 0.0f, 1.0f, 1.0f };
  float redish[4] = { 1.0f, 0.0f, 0.3f, 1.0f };
  float greenish[4] = { 0.3f, 1.0f, 0.3f, 1.0f };


  // Geometry Instances

  GeometryInstanceSharedHandle geometryInstance0 = m_rix->geometryInstanceCreate();
  m_rix->geometryInstanceSetGeometry( geometryInstance0, cylinderNormal );
  m_rix->geometryInstanceSetProgramPipeline( geometryInstance0, programTexturedPipeline );
  m_rix->geometryInstanceUseContainer( geometryInstance0, vertViewProjContainer );
  m_rix->geometryInstanceUseContainer( geometryInstance0, vertCylinderNormalTransformContainer );
  m_rix->geometryInstanceUseContainer( geometryInstance0, fragConstContainer );
  m_rix->geometryInstanceUseContainer( geometryInstance0, fragCylinderNormalMaterialContainer );

  GeometryInstanceSharedHandle geometryInstance1 = m_rix->geometryInstanceCreate();
  m_rix->geometryInstanceSetGeometry( geometryInstance1, cylinderCut );
  m_rix->geometryInstanceSetProgramPipeline( geometryInstance1, programTexturedPipeline );
  m_rix->geometryInstanceUseContainer( geometryInstance1, vertViewProjContainer );
  m_rix->geometryInstanceUseContainer( geometryInstance1, vertCylinderCutTransformContainer );
  m_rix->geometryInstanceUseContainer( geometryInstance1, fragConstContainer );
  m_rix->geometryInstanceUseContainer( geometryInstance1, fragCylinderCutMaterialContainer );

  GeometryInstanceSharedHandle geometryInstance2 = m_rix->geometryInstanceCreate();
  m_rix->geometryInstanceSetGeometry( geometryInstance2, cylinderCutTube );
  m_rix->geometryInstanceSetProgramPipeline( geometryInstance2, programTexturedPipeline );
  m_rix->geometryInstanceUseContainer( geometryInstance2, vertViewProjContainer );
  m_rix->geometryInstanceUseContainer( geometryInstance2, vertCylinderCutTubeTransformContainer );
  m_rix->geometryInstanceUseContainer( geometryInstance2, fragConstContainer );
  m_rix->geometryInstanceUseContainer( geometryInstance2, fragCylinderCutTubeMaterialContainer );



  // Render Group

  RenderGroupSharedHandle renderGroup = m_rix->renderGroupCreate();
  m_renderData->setRenderGroup(renderGroup);
  m_rix->renderGroupAddGeometryInstance( renderGroup, geometryInstance0 );
  m_rix->renderGroupAddGeometryInstance( renderGroup, geometryInstance1 );
  m_rix->renderGroupAddGeometryInstance( renderGroup, geometryInstance2 );


  // Get container entries

  ContainerEntry containerEntryView2clip   = m_rix->containerDescriptorGetEntry( vertConstContainerDescriptor, "view2clip" );
  ContainerEntry containerEntryWorld2view  = m_rix->containerDescriptorGetEntry( vertConstContainerDescriptor, "world2view" );
  ContainerEntry containerEntryView2world  = m_rix->containerDescriptorGetEntry( vertConstContainerDescriptor, "view2world" );
  ContainerEntry containerEntryWorld2clip  = m_rix->containerDescriptorGetEntry( vertConstContainerDescriptor, "world2clip" );

  ContainerEntry containerEntryModel2world = m_rix->containerDescriptorGetEntry( vertVarContainerDescriptor, "model2world" );
  ContainerEntry containerEntryModel2worldIT = m_rix->containerDescriptorGetEntry( vertVarContainerDescriptor, "model2worldIT" );

  ContainerEntry containerEntryTexture     = m_rix->containerDescriptorGetEntry( fragTexturedContainerDescriptor, "diffuseTex" );
  ContainerEntry containerEntryLightDir    = m_rix->containerDescriptorGetEntry( fragConstContainerDescriptor, "lightDir" );


  // Set Container Data

  m_rix->containerSetData( vertViewProjContainer,  containerEntryView2clip,  ContainerDataRaw( 0, view2Clip.getPtr(),   16*sizeof(float) ) );
  m_rix->containerSetData( vertViewProjContainer,  containerEntryWorld2view, ContainerDataRaw( 0, world2View.getPtr(),  16*sizeof(float) ) );
  m_rix->containerSetData( vertViewProjContainer,  containerEntryView2world, ContainerDataRaw( 0, world2ViewI.getPtr(), 16*sizeof(float) ) );
  m_rix->containerSetData( vertViewProjContainer,  containerEntryWorld2clip, ContainerDataRaw( 0, world2Clip.getPtr(),  16*sizeof(float) ) );

  SamplerStateDataCommon samplerStateDataCommon( SamplerStateFilterMode::NEAREST, SamplerStateFilterMode::NEAREST );
  SamplerStateSharedHandle samplerStateHandle = m_rix->samplerStateCreate(samplerStateDataCommon);
  TextureSharedHandle diffuseMap = dp::rix::util::generateTexture( m_rix, dp::rix::util::createTextureGradient( Vec2ui(128, 128), Vec4f(1.0, 0.0f, 0.0f, 1.0), Vec4f(0.0, 1.0f, 0.0f, 1.0), Vec4f(0.0, 0.0f, 1.0f, 1.0) ), dp::PixelFormat::RGBA, dp::DataType::UNSIGNED_INT_32, InternalTextureFormat::RGBA8 );

  rix::core::SamplerSharedHandle samplerDiffuseMap = m_rix->samplerCreate();
  m_rix->samplerSetTexture( samplerDiffuseMap, diffuseMap );
  m_rix->samplerSetSamplerState( samplerDiffuseMap, samplerStateHandle );

  m_rix->containerSetData( vertCylinderNormalTransformContainer,   containerEntryModel2world,   ContainerDataRaw( 0, model2world0.getMatrix().getPtr(), 16*sizeof(float) ) );
  m_rix->containerSetData( vertCylinderNormalTransformContainer,   containerEntryModel2worldIT, ContainerDataRaw( 0, model2world0IT.getPtr(), 16*sizeof(float) ) );
  m_rix->containerSetData( fragCylinderNormalMaterialContainer,    containerEntryTexture,       ContainerDataSampler( samplerDiffuseMap ) );

  m_rix->containerSetData( vertCylinderCutTransformContainer,   containerEntryModel2world,   ContainerDataRaw( 0, model2world1.getMatrix().getPtr(), 16*sizeof(float) ) );
  m_rix->containerSetData( vertCylinderCutTransformContainer,   containerEntryModel2worldIT, ContainerDataRaw( 0, model2world1IT.getPtr(), 16*sizeof(float) ) );
  m_rix->containerSetData( fragCylinderCutMaterialContainer,    containerEntryTexture,       ContainerDataSampler( samplerDiffuseMap ) );

  m_rix->containerSetData( vertCylinderCutTubeTransformContainer,   containerEntryModel2world,   ContainerDataRaw( 0, model2world2.getMatrix().getPtr(), 16*sizeof(float) ) );
  m_rix->containerSetData( vertCylinderCutTubeTransformContainer,   containerEntryModel2worldIT, ContainerDataRaw( 0, model2world2IT.getPtr(), 16*sizeof(float) ) );
  m_rix->containerSetData( fragCylinderCutTubeMaterialContainer,    containerEntryTexture,       ContainerDataSampler( samplerDiffuseMap ) );


  Vec3f lightDirection(-0.5f, 1.0f, 1.0f);

  m_rix->containerSetData( fragConstContainer, containerEntryLightDir, ContainerDataRaw( 0, &lightDirection[0], 3*sizeof(float) ) );

}
