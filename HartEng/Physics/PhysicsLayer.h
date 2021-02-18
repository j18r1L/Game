#pragma once

#include <string>
#include <vector>
#include <stdint.h>

namespace HE
{
	struct PhysicsLayer
	{
		uint32_t LayerID;
		std::string Name;
		uint32_t BitValue;
		int32_t CollidesWith = 0;

		bool IsValid() const
		{
			return LayerID >= 0 && !Name.empty() && BitValue > 0;
		}
	};

	class PhysicsLayerManager
	{
	private:
		static std::vector<PhysicsLayer> s_Layers;
		static PhysicsLayer s_NullLayer;

		static uint32_t GetNextLayerID();

	public:
		static uint32_t AddLayer(const std::string& name, bool setCollisions = true);
		static void RemoveLayer(uint32_t layerID);

		static void SetLayerCollision(uint32_t layerId, uint32_t otherLayer, bool shouldCollide);
		static std::vector<PhysicsLayer> GetLayerCollisions(uint32_t layerID);

		static const std::vector<PhysicsLayer>& GetLayers() { return s_Layers; }

		static PhysicsLayer& GetLayer(uint32_t layerID);
		static PhysicsLayer& GetLayer(const std::string& layerName);
		static uint32_t GetLayerCount() { return s_Layers.size(); }

		static bool ShouldCollide(uint32_t layer1, uint32_t layer2);
		static bool IsLayerValid(uint32_t layerID);

	



	};
}