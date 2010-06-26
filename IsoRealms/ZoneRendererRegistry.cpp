#include "ZoneRendererRegistry.h"

bool ZoneRendererRegistry::exists(const std::string& name) {
  // TODO: Implement this!
  return false;
}

IZoneRenderer* ZoneRendererRegistry::createInstance(const std::string& implementation, const std::string& instanceName) {
  if (exists(instanceName)) {
    // TODO: throw exception.
    std::cout << "ZoneRenderer exists." << std::endl;
    exit(1);
  }

  std::string mZoneRendererLocation = System::getConfigurationResource("ZoneRenderers/" + implementation + "/ZoneRenderer");
  void* mZoneRendererSO = dlopen(mZoneRendererLocation.c_str(), RTLD_LAZY | RTLD_GLOBAL);
  if (!mZoneRendererSO) {
    throw InitException("Cannot load library: " + std::string(dlerror()));
  }
  createZoneRenderer* createZoneRendererFunction = cast_voidptr_to_funcptr<createZoneRenderer*>(dlsym(mZoneRendererSO, "create"));
  const char* mDlsymError = dlerror();
  if (mDlsymError) {
    throw InitException("Cannot load symbol: " + std::string(mDlsymError));
  }
  destroyZoneRenderer* destroyZoneRendererFunction = cast_voidptr_to_funcptr<destroyZoneRenderer*>(dlsym(mZoneRendererSO, "destroy"));
  mDlsymError = dlerror();
  if (mDlsymError) {
    throw InitException("Cannot load symbol: " + std::string(mDlsymError));
  }
  cZoneRenderers[instanceName] = createZoneRendererFunction();
  cZoneRendererTypes[instanceName] = implementation;
  cDestroyFunctions[instanceName] = destroyZoneRendererFunction;
  cSOHandles[instanceName] = mZoneRendererSO;
/*  for (unsigned int i = 0; i < cListeners.size(); i++) {
    cListeners[i]->elementSetInstantiated(cElementSets[instanceName]);
  }*/
  return cZoneRenderers[instanceName];
}

IZoneRenderer* ZoneRendererRegistry::registerZoneRenderer(PluginRegistry& pluginRegistry, DOMNodeWrapper* node) {
  std::string mInstance = node->getAttribute("instance");
  std::string mType = node->getAttribute("type");
  std::string mUse = node->getAttribute("use");
  IZoneRenderer* mZoneRenderer = mType == ""
                               ? new DefaultZoneRenderer()
                               : createInstance(mType, mInstance);
/*  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "UsePlugin") {
      setPlugin(pluginRegistry, mElementSet, mNode);
    } else if (mValueAsString == "Configuration") {
      mElementSet->load(mNode);
    }
  }*/
  return mUse == "true" ? mZoneRenderer : NULL;
}

