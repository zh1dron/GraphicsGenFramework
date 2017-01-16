// Copyright � 2014-2016  Zhirnov Andrey. All rights reserved.

#pragma once

#include "../ThreadSafe/Referenced.h"
#include "../Types/Ptr.h"

namespace GX_STL
{
namespace File
{
	using namespace GXTypes;

	
	struct EFile
	{
		enum type
		{
			Unknown	= 0,
			HDD,
			Memory,
			Resource,	// platform depended file type
			SubFile,
			Crypted,
			Multithreaded,

			_Archive	= 0x1000,
			ZIP,
			LZMA,
		};

		inline bool IsArchive (type value)
		{
			return value > _Archive;
		}
	};


	//
	// Base File interface
	//

	class BaseFile : protected Referenced
	{
	// interface
	public:
		virtual ~BaseFile () {}

		virtual void Close () = 0;

		virtual bool SeekSet (Bytes<usize> offset) = 0;
		virtual bool SeekCur (Bytes<isize> offset) = 0;
		virtual bool SeekEnd (Bytes<usize> offset) = 0;

		virtual Bytes<usize>	RemainingSize ()	const = 0;
		virtual Bytes<usize>	Size ()				const = 0;
		virtual Bytes<usize>	Pos ()				const = 0;
		virtual bool			IsEOF ()			const = 0;
		virtual bool			IsOpened ()			const = 0;

		virtual EFile::type		GetType ()			const = 0;
		virtual StringCRef		Name ()				const = 0;
	};



	//
	// Read only File interface
	//

	class RFile : public BaseFile
	{
	// types
	public:
		SHARED_POINTER( RFile );


	// interface
	public:
		virtual Bytes<isize> ReadBuf (void * buf, Bytes<usize> size) = 0;
		
		virtual Bytes<isize> ReadBufFrom (void * buf, Bytes<usize> size, Bytes<usize> offset)
		{
			const Bytes<usize>	pos = Pos();
			SeekSet( offset );

			const Bytes<isize>	bufsize = ReadBuf( buf, size );
			SeekSet( pos );
			
			return bufsize;
		}


	// methods
	public:
		bool Read (void * buf, Bytes<usize> size)
		{
			return ReadBuf( buf, size ) == (isize) size;
		}

		template <typename T>
		bool Read (OUT T & value)
		{
			STATIC_ASSERT( not TypeTraits::IsConstOrVolatile<T> );
			STATIC_ASSERT( CompileTime::IsMemCopyFromFileAvailable<T> );
			return ReadBuf( &value, SizeOf( value ) ) == sizeof(value);
		}

		template <typename T>
		bool Read (INOUT Buffer<T> data)
		{
			STATIC_ASSERT( not TypeTraits::IsConstOrVolatile<T> );
			STATIC_ASSERT( CompileTime::IsMemCopyFromFileAvailable<T> );
			return Read( data.ptr(), data.Size() );
		}

		template <typename T>
		T ReadValue ()
		{
			T value;
			Read( value );
			return value;
		}
	};



	//
	// Write only File interface
	//

	class WFile : public BaseFile
	{
	// types
	public:
		SHARED_POINTER( WFile );


	// interface
	public:
		virtual void Flush () = 0;

		virtual Bytes<isize> WriteBuf (const void * buf, Bytes<usize> size) = 0;


	// methods
	public:
		bool Write (const void * buf, Bytes<usize> size)
		{
			return WriteBuf( buf, size ) == (isize) size;
		}

		template <typename T>
		bool Write (const T &value)
		{
			STATIC_ASSERT( CompileTime::IsMemCopyFromFileAvailable<T> );
			return WriteBuf( &value, SizeOf( value ) ) == sizeof(value);
		}

		template <typename T>
		bool Write (const Buffer<T> data)
		{
			STATIC_ASSERT( CompileTime::IsMemCopyFromFileAvailable<T> );
			return Write( data.ptr(), data.Size() );
		}

		template <typename T>
		bool Write (const TStringRef<T> data)
		{
			STATIC_ASSERT( CompileTime::IsMemCopyFromFileAvailable<T> );
			return Write( data.ptr(), data.LengthInBytes() );
		}
	};
	

}	// File


namespace GXTypes
{
	typedef SHARED_POINTER_TYPE( File::RFile )	RFilePtr;
	typedef SHARED_POINTER_TYPE( File::WFile )	WFilePtr;

}	// GXTypes
}	// GX_STL
