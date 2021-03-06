// Copyright � 2014-2017  Zhirnov Andrey. All rights reserved.
/*
	TODO:
		- check align on read/write/copy/size methods
*/

#pragma once

#include "CL2Enums.h"

#if defined( COMPUTE_API_OPENCL )

namespace Engine
{
namespace Compute
{
	
	class	CL2ComputeImage;
	typedef CL2ComputeImage		ComputeImage;
	
	SHARED_POINTER( ComputeImage );
	

	class	CL2ComputeBuffer;
	typedef	CL2ComputeBuffer	ComputeBuffer;

	SHARED_POINTER( ComputeBuffer );



	//
	// Compute Image
	//

	class CL2ComputeImage : public BaseObject
	{
	// variables
	private:
		BaseObjectPtr		_shared;
		cl::cl_mem			_id;
		uint4				_dim;
		EPixelFormat::type	_format;
		ETexture::type		_target;
		EMemoryAccess::type	_flags;


	// methods
	public:
		explicit
		CL2ComputeImage (const SubSystemsRef ss);
		~CL2ComputeImage ();

		bool Create (const uint4 &dim, ETexture::type imageType, EPixelFormat::type format,
					 EMemoryAccess::type flags = EMemoryAccess::ReadWrite,
					 BinaryCBuffer data = Uninitialized,
					 BytesU xAlign = 4_b,
					 BytesU xyAlign = 4_b);
		
		bool Create (const TexturePtr &texture, EMemoryAccess::type flags, MipmapLevel level = Uninitialized);
		bool Create (const TexturePtr &texture, EMemoryAccess::type flags, ECubeMapFace::type face, MipmapLevel level = Uninitialized);

		bool SetImage (BinaryCBuffer data, const uint3 &size, const uint4 &offset = Uninitialized,
						BytesU xAlign = 4_b, BytesU xyAlign = 4_b);

		bool GetImage (OUT BinaryBuffer data, const uint3 &size, const uint4 &offset,
						BytesU xAlign = 4_b, BytesU xyAlign = 4_b);
		
		bool Copy (const ComputeBufferPtr &src, BytesU srcOffset, const uint4 &dstOffset, const uint4 &size);
		bool Copy (const ComputeImagePtr &src, const uint4 &srcOffset, const uint4 &dstOffset, const uint4 &size);
		
		bool CopyTo (const ComputeBufferPtr &dst, const uint4 &srcOffset, BytesU dstOffset, const uint4 &size);
		
		template <typename T>
		void Clear (const T &value);
		void Clear ();

		bool					IsShared ()			const	{ return _shared.IsNotNull(); }
		BaseObjectPtr const&	GetSharedObject ()	const	{ return _shared; }

		uint4 const &			Dimension ()		const	{ return _dim; }
		uint					Width ()			const	{ return _dim.x; }
		uint					Height ()			const	{ return _dim.y; }
		uint					Depth ()			const	{ return _dim.z; }
		uint					NumLayers ()		const	{ return _dim.w; }

		EPixelFormat::type		PixelFormat ()		const	{ return _format; }
		ETexture::type			ImageType ()		const	{ return _target; }
		EMemoryAccess::type		MemoryAccess ()		const	{ return _flags; }
		
		bool					IsCreated ()		const	{ return _id != null; }
		cl::cl_mem				Id ()				const	{ return _id; }

		static ComputeImagePtr  New (const TexturePtr &texture, EMemoryAccess::type flags = EMemoryAccess::ReadWrite, MipmapLevel level = Uninitialized);


	private:
		bool _CreateFromGLTex (gl::GLenum target, gl::GLuint textureId, EMemoryAccess::type flags, uint level);
		void _Destroy ();

		bool _FillImage (BinaryCBuffer pattern);
	};

	
/*
=================================================
	Clear
=================================================
*/
	template <typename T>
	inline void CL2ComputeImage::Clear (const T &value)
	{
		_FillImage( BinaryCBuffer::FromValue( value ) );
	}

	inline void CL2ComputeImage::Clear ()
	{
		if ( EPixelFormat::ValueType::IsFloat( PixelFormat() ) )
			Clear( float4( 0.0f ) );
		else
		if ( EPixelFormat::ValueType::IsSignedInteger( PixelFormat() ) )
			Clear( int4( 0 ) );
		else
		if ( EPixelFormat::ValueType::IsUnsignedInteger( PixelFormat() ) )
			Clear( uint4( 0 ) );
		else
			WARNING( "unknown pixel format to clear image" );
	}


}	// Compute
}	// Engine

#endif	// COMPUTE_API_OPENCL
