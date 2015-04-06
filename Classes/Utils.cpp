//
//  Utils.cpp
//
//  Created by Tien Dao Cong on 12/20/14.
//  Copyright (c) 2014 TienDC. All rights reserved.
//

#include "Utils.h"


USING_NS_CC;


// Check to see if the given node contains the pt
bool NodeContainsPoint(Node* node, const Vec2& pt)
{
    const Size& nodeSz = node->getContentSize();
    Rect bbox = Rect(0, 0, nodeSz.width, nodeSz.height);
    Vec2 locationInNodeSpace = node->convertToNodeSpace(pt);
    return bbox.containsPoint(locationInNodeSpace);
}
