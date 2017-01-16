// Copyright � 2014-2016  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Resource.h"

namespace Engine
{
namespace Base
{

	//
	// Loader interface
	//

	class IResourceLoader : public BaseObject
	{
	// types
	protected:

		struct ResourceFileHeader : public CompileTime::PODStruct
		{
		// variables
		private:
			CompileTime::Signature4::value_t	signature;
			uint								version;

		// methods
		public:
			ResourceFileHeader (CompileTime::Signature4::value_t signature, uint version) :
				signature( signature ),
				version( _engine_hidden_::BuildResourceVersion( ENGINE_VERSION, version ) )
			{}

		protected:
			bool _Check (const ResourceFileHeader &right) const
			{
				return	this->signature == right.signature and
						this->version	== right.version;
			}
		};


	// interface
	public:
		explicit
		IResourceLoader (const SubSystemsRef ss) : BaseObject( ss )
		{}

		virtual bool Load (OUT ResourcePtr &resource, PackFileID fileID, const RFilePtr &file) = 0;

		virtual EResource::type GetType () const = 0;
	};


	SHARED_POINTER( IResourceLoader );


}	// Base
}	// Engine