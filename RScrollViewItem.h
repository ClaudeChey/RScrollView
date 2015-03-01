//
//  RScrollViewItem.h
//  Nock
//
//  Created by Claude Chey on 2014. 10. 23..
//
//

#ifndef __Nock__RScrollViewItem__
#define __Nock__RScrollViewItem__

#include <stdio.h>
#include <cocos2d.h>

class RScrollViewItem : public cocos2d::Layer
{
public:
    CREATE_FUNC(RScrollViewItem);
    virtual bool init();
    cocos2d::Rect getContentRect();
    const cocos2d::Vec2 getCenter();

    void setOutOfRangeOnDisplay(bool val);
    void testTouchAreaDraw();
    
    virtual void onTouchBeganItem(cocos2d::Vec2 location) {};
    virtual void onTouchMovedItem(cocos2d::Vec2 location) {};
    virtual void onTouchEndedItem(cocos2d::Vec2 location) {};
    
    
private:
    cocos2d::Rect m_contentRect = cocos2d::Rect::ZERO;
};

#endif /* defined(__Nock__RScrollViewItem__) */
