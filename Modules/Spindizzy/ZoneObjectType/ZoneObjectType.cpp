/*
 * Copyright 2023 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "ZoneObjectType.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  const std::string ZoneObjectType::TAG_TRAIT = "Trait";
  
  const std::string ZoneObjectType::ATTRIBUTE_EDITING_PROCESSOR = "editingProcessor";
  const std::string ZoneObjectType::ATTRIBUTE_EDITING_RENDERER  = "editingRenderer";
  const std::string ZoneObjectType::ATTRIBUTE_ID                = "id";
  const std::string ZoneObjectType::ATTRIBUTE_RUNTIME_PROCESSOR = "runtimeProcessor";
  const std::string ZoneObjectType::ATTRIBUTE_RUNTIME_RENDERER  = "runtimeRenderer";
  const std::string ZoneObjectType::ATTRIBUTE_TYPE              = "type";

  const std::string ZoneObjectType::BIND_TO_TRAIT = "Trait";
  const std::string ZoneObjectType::BIND_TO_ZONE  = "Zone";

  ZoneObjectType::ZoneObjectType(IProject* project, Spindizzy* spindizzy) :
            cDefSpindizzy(*spindizzy) {
  }
  
  ZoneObjectType::ZoneObjectType(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data) :
            ZoneObjectType(project, spindizzy) {
    for (DOMNode& mNode : node) {
      std::string mTag = mNode.getName();
      if (mTag == TAG_TRAIT) {
//         std::string mTraitID = mNode.getAttribute(ATTRIBUTE_ID);
//         IZoneObjectTypeTrait* mTrait = cDefSpindizzy.createZoneObjectTypeTrait(mNode, this);
//         cDefTypeTraits.emplace(mTraitID, mTrait);
      } else {
        throw ResourceInitException("ERROR: ZoneObjectType::ZoneObjectType: Unknown tag \"" + mTag + "\"");
      }
    }
    cDefRuntimeRendererID = node.getAttribute(ATTRIBUTE_RUNTIME_RENDERER);
    cDefEditingRendererID = node.getAttribute(ATTRIBUTE_EDITING_RENDERER);
    cDefRuntimeProcessorID = node.getAttribute(ATTRIBUTE_RUNTIME_PROCESSOR);
    cDefEditingProcessorID = node.getAttribute(ATTRIBUTE_EDITING_PROCESSOR);
  }

  void ZoneObjectType::registerAssets(IAssetRegistry* assets) {
    // TODO
  }
    
  void ZoneObjectType::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    // TODO
  }
  
  void ZoneObjectType::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    for (const std::pair<const std::string, IZoneObjectTypeTrait*>& mPair : cDefTypeTraits) {
      DOMNodeWriter mTraitNode = node->addBranch(TAG_TRAIT);
      mTraitNode.addAttribute(ATTRIBUTE_ID, mPair.first);
//      mTraitNode.addAttribute(ATTRIBUTE_TYPE, cDefSpindizzy.getID(mPair.second));
      mPair.second->save(mTraitNode);
    }
  }

  void ZoneObjectType::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool ZoneObjectType::renderIcon() const {
    return false; // TODO
  }

  std::vector<IProperty*> ZoneObjectType::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  ZoneObjectType::~ZoneObjectType() {
    cDefSpindizzy.removeAll(this);
  }
  
  void ZoneObjectType::registerAssets(ISpindizzyRegistry* registry) {
    registry->add(this, "");
    for (const std::pair<const std::string, IZoneObjectTypeTrait*>& mPair : cDefTypeTraits) {
      LocalSpindizzyRegistry mTraitRegistry(registry, mPair.first);
      mPair.second->registerAssets(&mTraitRegistry);
    }
  }  
  
  Spindizzy& ZoneObjectType::getSpindizzy() {
    return cDefSpindizzy;
  }
  
  void ZoneObjectType::registerEditor(IZoneObjectTraitEditor* editor) {
    cRuntimeEditors.emplace_back(editor);
  }
  
  IBinding* ZoneObjectType::getBinding(const std::string& id) const {
//     std::size_t mSplit = id.find('/');
//     std::string mBindTo = mSplit == std::string::npos ? id : id.substr(0, mSplit);
//     std::string mSubID = mSplit == std::string::npos ? "" : id.substr(mSplit + 1);
//     return mBindTo == BIND_TO_ZONE  ? cDefZone.getZoneBinding(mSubID)
//          : mBindTo == BIND_TO_TRAIT ? getTraitBinding(mSubID)
//          :                            nullptr;
    return nullptr;
  }
  
  std::string ZoneObjectType::getBindingID(const IBinding* binding) const {
    return "";
  }
  
  std::map<std::string, std::unique_ptr<IZoneObjectTrait>> ZoneObjectType::createTraits(ZoneObject& object) const {
    std::map<std::string, std::unique_ptr<IZoneObjectTrait>> mObjectTraits;
    for (const std::pair<const std::string, IZoneObjectTypeTrait*>& mPair : cDefTypeTraits) {
      mObjectTraits.emplace(std::string(mPair.first), mPair.second->createTrait(object));
    }
    return mObjectTraits;
  }
  
  std::string ZoneObjectType::getID(const IZoneObjectTypeTrait* trait) const {
    for (const std::pair<const std::string, IZoneObjectTypeTrait*>& mPair : cDefTypeTraits) {
      if (mPair.second == trait) {
        return mPair.first;
      }
    }
    throw ArgumentException("ERROR: ZoneObjectType::getID: Specified trait not registered with this object type");
  }
  
  std::string ZoneObjectType::getRuntimeRendererID() const {
    return cDefRuntimeRendererID;
  }

  std::string ZoneObjectType::getEditingRendererID() const {
    return cDefEditingRendererID;
  }

  std::string ZoneObjectType::getRuntimeProcessorID() const {
    return cDefRuntimeProcessorID;
  }

  std::string ZoneObjectType::getEditingProcessorID() const {
    return cDefEditingProcessorID;
  }

  IWorldEditorToolInstance* ZoneObjectType::createToolInstance(WorldEditor* editor) {
    return cEditingPens.emplace_back(std::make_unique<Pen>(*this, editor)).get();
  }

  bool ZoneObjectType::renderAssetIcon() const {
    return false;
  }

  ZoneObjectType::Pen::Pen(ZoneObjectType& parent, WorldEditor* editor) :
            cParent(parent),
            cEditor(editor) {
  }

  bool ZoneObjectType::Pen::isTool(IWorldEditorTool* tool) const {
    return tool == &cParent;
  }

  bool ZoneObjectType::Pen::renderIcon(float yaw) const {
    return cParent.renderIcon();
  }

  void ZoneObjectType::Pen::renderEditingPreview() const {
    // TODO
  }

  void ZoneObjectType::Pen::renderUI() const {
    // Nothing to do.
  }

  void ZoneObjectType::Pen::updateUI(unsigned int milliseconds) {
    // Nothing to do.
  }

  bool ZoneObjectType::Pen::inputEdit(sf::Event& event) {
//     switch (cRuntimeEditors[cEditingTrait]->inputEdit(event)) {
//       case IZoneObjectTraitEditor::InputEditResult::COMPLETED: {
//         if (cEditingTrait == 0) {
//           cPinnedZone = cEditor->getWorld()->getOrDrawZone(cEditor->getCursorCell(), cEditor);
//         }
//
//         cEditingTrait++;
//         if (cEditingTrait == cRuntimeEditors.size()) {
//           cPinnedZone->draw(cParent);
//         }
//         return true;
//       }
//
//       case IZoneObjectTraitEditor::InputEditResult::MODIFIED: {
//         return true;
//       }
//
//       case IZoneObjectTraitEditor::InputEditResult::CANCELLED: {
//         if (cEditingTrait > 0) {
//           cEditingTrait--;
//           if (cEditingTrait == 0) {
//             if (cPinnedZone.empty()) {
//               cEditor->getWorld()->remove(cPinnedZone);
//             }
//             cPinnedZone = nullptr;
//           }
//         }
//         return true;
//       }
//
//       default: {
//         return false;
//       }
//     }
    return false;
  }

  void ZoneObjectType::Pen::processCursorMovement(LiteralVertex* start, LiteralVertex* end) {
//     if (cPinnedZone != nullptr) {
//       cPinnedZone->processCursorMovement(*end);
//       cRuntimeEditors[cEditingTrait]->processCursorMovement(*end);
//     }
  }

  double ZoneObjectType::Pen::getSnapInterval() const {
    return 1.0f; // cRuntimeEditors[cEditingTrait]->getSnapInterval();
  }
  
  IBinding* ZoneObjectType::getTraitBinding(const std::string& id) const {
//     std::size_t mSplit = id.find('/');
//     std::string mBindTo = mSplit == std::string::npos ? id : id.substr(0, mSplit);
//     std::string mSubID = mSplit == std::string::npos ? "" : id.substr(mSplit + 1);
//     std::map<std::string, std::unique_ptr<IZoneObjectTrait>>::iterator mIterator = cDefTraits.find(mBindTo);
//     return mIterator != cDefTraits.end() ? mIterator->second->getTraitBinding(mSubID) : nullptr;
    return nullptr;
  }
}
