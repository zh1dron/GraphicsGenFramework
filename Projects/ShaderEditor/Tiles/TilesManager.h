// Copyright � 2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Tile.h"

namespace ShaderEditor
{

	//
	// Tiles Manager
	//

	class TilesManager : public BaseObject
	{
	// types
	private:
		struct MeshVertex
		{
		// variables
			float3	position;
			float3	normal;
			float2	texcoord;

		// methods
			MeshVertex () {}

			MeshVertex (const float3 &position, const float3 &normal, const float2 &texcoord);
			
			static void GetAttribs (OUT VertexAttribsDesc &desc);
		};


	// variables
	private:
		Shared::FreeCameraController	_controller;
		//Shared::FlightCameraController	_controller;
		
		Array< TilePtr >				_tiles;
		Queue< TilePtr >				_pendingToInit;
		RectI							_tilesRect;

		ITileInitializerPtr				_initializer;
		real							_maxTileSize;
		int								_globalZoom;	// if tile is too small, than use global scale to increase view distance
		uint							_maxZoom;		// 0 - maxTileSize, 1 - maxTileSize/2, ...
		uint							_maxTiles;		// in 0 zoom level


	// methods
	public:
		TilesManager (const SubSystemsRef ss);
		~TilesManager ();

		bool Initialize ();
		void Deinitialize ();
		bool Reload ();

		void ResetCamera ();

		void Draw (const RenderTargetPtr &rt);
		void Update (Time<double> dt);

		void SetTileInitializer (const ITileInitializerPtr &initializer);

		static MeshGenerator::Mesh CreateTilePlane (const SubSystemsRef ss, uint vertices);

	private:
		void _ClearTiles ();
		void _UpdateTiles ();
		void _InitializeTiles ();
		void _UpdateGlobalScale ();
		real _GetGlobalScale () const;
	};


}	// ShaderEditor
