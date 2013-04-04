#ifndef ABSTRACT_VERTICAL_COMPONENT_H
#define ABSTRACT_VERTICAL_COMPONENT_H

#include "AbstractRectangularComponent.h"
#include "IComponentHeightCalculator.h"

class AbstractVerticalComponent:public IComponentHeightCalculator,
                                public AbstractRectangularComponent {
};

#endif
