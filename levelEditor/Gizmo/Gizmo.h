#ifndef GIZMO_H
#define GIZMO_H

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
	};
}

#endif // GIZMO_H


