#pragma once

#include "HartEng/Core/UUID.h"

#include <string>

namespace HE
{
	enum class AssetType
	{
		Scene,
		Mesh,
		Texture,
		EnvMap,
		Audio,
		Script,
		PhysicsMat,
		Other
	};

	class Asset
	{
	public:
		UUID Handle;
		AssetType Type;

		std::string FilePath;
		std::string FileName;
		std::string Extension;
		int ParentDirectory;

		Asset() = default;
		virtual ~Asset() {}
	};

	class PhysicsMaterial : public Asset
	{
	public:
		float StaticFriction;
		float DynamicFriction;
		float Bounciness;

		PhysicsMaterial() = default;
		PhysicsMaterial(float staticFriction, float dynamicFriction, float bounciness): 
			StaticFriction(staticFriction), 
			DynamicFriction(dynamicFriction), 
			Bounciness(bounciness)
		{ }

		virtual ~PhysicsMaterial() = default;
	};
}