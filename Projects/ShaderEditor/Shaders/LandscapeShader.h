// Copyright � 2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "IShader.h"

namespace ShaderEditor
{

	//
	// Landscape Shader
	//

	class LandscapeShader : public IShader
	{
	// types
	private:
		struct ShaderData
		{
			real4x4		mvp;

			ShaderData ()
			{}
		};

		typedef UniformBlock< ShaderData >		ShaderUB;


	// variables
	private:
		ShaderProgramPtr	_program;
		ShaderUB			_landscapeUB;
		TextureUniform		_unDiffuseTex;
		TexturePtr			_diffuseTex;


	// methods
	protected:
		LandscapeShader (const SubSystemsRef ss) : IShader( ss )
		{}

		void Active () override
		{
			_unDiffuseTex.Update( _diffuseTex, 0 );

			_program->Bind();
		}

		bool SetTextures (Buffer<const TexturePtr> textures) override
		{
			CHECK_ERR( textures.Count() == 1 );

			_diffuseTex = textures[0];
			return true;
		}

		bool SetData (StringCRef name, VariantRef data) override
		{
			if ( name == "mvp" )
			{
				CHECK_ERR( data.IsType< real4x4 >() );

				_landscapeUB.Get().mvp = data.Get< real4x4 >();
				return true;
			}

			RETURN_ERR( "unknown shader variable name!" );
		}

		bool _Load (StringCRef filename)
		{
			_program = ShaderProgram::New( SubSystems() );

			CHECK_ERR( _program->Load(
				filename,
				ShaderProgram::ShaderBits_t().SetAll(),
				EShaderCompilationFlags::Default,
				VertexAttribsState()
					.Add( VertexAttribsState::AttribIndex(0), EAttribute::Float3 )	// position
					.Add( VertexAttribsState::AttribIndex(1), EAttribute::Float3 )	// normal
					.Add( VertexAttribsState::AttribIndex(2), EAttribute::Float2 ),	// texture coord
				FragmentOutputState().Set( EFragOutput::Float4, 0 )					// forward rendering
			) );
			
			CHECK_ERR( _program->CreateUniformBlock( OUT _landscapeUB, "LandscapeUB" ) );

			CHECK_ERR( _unDiffuseTex.Create( _program, "unDiffuseTexture" ) );

			return true;
		}

	public:
		static IShaderPtr  New (const SubSystemsRef ss)
		{
			SHARED_POINTER_TYPE( LandscapeShader )	ptr = BaseObject::_New( new LandscapeShader( ss ) );

			CHECK_ERR( ptr->_Load( "gl/Shaders/landscape.glsl" ), null );
			return ptr;
		}
	};


}	// ShaderEditor
