#pragma once

#include "HartEng/HartEng.h"

namespace HE
{
	class Gizmo
	{
	private:
		PerspectiveCameraController* m_CameraController = nullptr;

	public:
		Gizmo() = default;
		void Draw(Entity* entity);

		void SetCamera(PerspectiveCameraController* cameraController);

		bool IsUsing() const;

		bool IsOver() const;
	};
}



