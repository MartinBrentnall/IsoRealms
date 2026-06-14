/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Modules/Basics/Basics.h"

#include "SequenceTrackRegistry.h"

namespace IsoRealms::Basics {
  SequenceTrackRegistry::SequenceTrackRegistry(IComponentTypeRegistry& registry) :
            cAction(registry.getResourceMetadata("SequenceTrackAction")),
            cAudio( registry.getResourceMetadata("SequenceTrackAudio")),
            cColour(registry.getResourceMetadata("SequenceTrackColour")),
            cFloat( registry.getResourceMetadata("SequenceTrackFloat")),
            cScreen(registry.getResourceMetadata("SequenceTrackScreen")) {
    add(&cAction, "Action", "Action");
    add(&cAudio,  "Audio",  "Audio");
    add(&cColour, "Colour", "Colour");
    add(&cFloat,  "Float",  "Float");
    add(&cScreen, "Screen", "Screen");
  }
}
