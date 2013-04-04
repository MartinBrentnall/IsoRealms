#include "SpindizzyCameras.h"

void SpindizzyCameras::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Camera") {
      SpindizzyCamera* mCamera = new SpindizzyCamera();
      cCameras.push_back(mCamera);
      std::string mName = mNode->getAttribute("name");
      IArgumentSource* mArgumentSource = new ArgumentSourceCustom<SpindizzyCamera>(mCamera);
      runtimeContext->add(mCamera, mName, mNode);
      runtimeContext->registerArgument("Camera", mName, mArgumentSource);
    }
  }
}

void SpindizzyCameras::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cCameras.size(); i++) {
    DOMNodeWriter* mCameraNode = node->addBranch("Camera");
    std::string mResourceName = resourceLocator->getPath(cCameras[i]);
    mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
    mCameraNode->addAttribute("name", mResourceName);
    cCameras[i]->save(mCameraNode, resourceLocator);
  }
}

extern "C" IPlugin* create() {
  return new SpindizzyCameras();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
