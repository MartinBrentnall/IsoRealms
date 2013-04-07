#include "CameraRegistry.h"

CameraRegistry::CameraRegistry() {
  cEditingCamera = NULL;
}

void CameraRegistry::setEditingCamera(ICamera* camera) {
  cEditingCamera = camera;
}

ICamera* CameraRegistry::unwrapResource(ICamera* camera) {
  return cCameras[camera];
}

ICamera* CameraRegistry::wrapResource(ICamera* camera) {
  if (cEditingCamera != NULL) {
    ICamera* mCamera = new WrappedCamera(cEditingCamera);
    cCameras[mCamera] = camera;
    return mCamera;
  }
  return camera;
}
