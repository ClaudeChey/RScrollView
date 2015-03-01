//
//  RScrollViewItem.cpp
//  Nock
//
//  Created by Claude Chey on 2014. 10. 23..
//
//

#include "RScrollViewItem.h"

using namespace cocos2d;

bool RScrollViewItem::init()
{
    if(!Layer::init()) return false;
    return true;
}

Rect RScrollViewItem::getContentRect()
{
    m_contentRect.origin = getPosition();
    m_contentRect.size = getContentSize();
    return m_contentRect;
}
const Vec2 RScrollViewItem::getCenter()
{
    Size size = getContentSize();
    return Vec2(size.width/2, size.height/2);
}

void RScrollViewItem::setOutOfRangeOnDisplay(bool val)
{
    setVisible(!val);
}

void RScrollViewItem::testTouchAreaDraw()
{
    Rect r = getContentRect();
    Vec2 vertices[] =
    {
        Vec2(0,r.size.height),
        Vec2(r.size.width,r.size.height),
        Vec2(r.size.width,0),
        Vec2(0,0)
    };
    DrawNode* drawNode = DrawNode::create();
    drawNode->drawPolygon(vertices, 4, Color4F(1,1,1,0.5), 0, Color4F::BLACK);
    addChild(drawNode);
}




