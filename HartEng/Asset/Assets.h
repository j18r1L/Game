#pragma once

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
		PhysicsMat
	};

	class Asset
	{
	public:
		uint32_t Handle;
		AssetType Type;

		std::string FilePath;
		std::string FileName;
		std::string Extension;
		int ParentDirectory;

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
	};
}