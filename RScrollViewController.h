//
//  RScrollViewController.h
//  Noc
//
//  Created by Claude Chey on 2014. 11. 16..
//
//

#ifndef __Noc__RScrollViewController__
#define __Noc__RScrollViewController__

#include <stdio.h>
#include <cocos2d.h>
#include "RScrollViewItem.h"


class RScrollViewControllerDelegate
{
public:
    virtual void onDidScrolling(cocos2d::Vec2 position) = 0;
    virtual void onDidEndScrolling(cocos2d::Vec2 position) = 0;
    virtual void onScrollTouchBegan(cocos2d::Vec2 position) = 0;
    virtual void onScrollTouchMoved(cocos2d::Vec2 position) = 0;
    virtual void onScrollTouchEnded(cocos2d::Vec2 position) = 0;
    virtual void onDraggingDecelerate(cocos2d::Vec2 deceleratePosition) = 0;
    virtual void onScrollingDecelerate(cocos2d::Vec2 deceleratePosition) = 0;
    virtual void onWillEndDraggingDecelerate(cocos2d::Vec2 deceleratePosition) = 0;
};



enum class RScrollViewControllerDirection
{
    BOTH=0,
    VERTICAL,
    HORIZONTAL
};


/*
 To do
 - directionalLockEnabled
 
 */

class RScrollViewController : public cocos2d::Ref
{
public:
    static RScrollViewController* create();
    void destroyController();
    void setScrollViewControllerDelegate(RScrollViewControllerDelegate* delegate);
    
    void setScrollViewSize(const cocos2d::Size& size);
    void setScrollViewTouchRect(const cocos2d::Rect& rect);
    void setScrollViewContentSize(const cocos2d::Size& size);
    void setScrollViewContentOffset(const cocos2d::Vec2& offset);
    void setScrollViewBackgroundLayer(cocos2d::Layer* background);
    void setScrollDirection(RScrollViewControllerDirection direction);
    void setScrollBounces(bool val);
    void setScrollEnabled(bool val);
    void setScrollTouchEvent(bool val);
    
    void addScrollViewItemVector(const std::vector<RScrollViewItem*> vec);
    void addScrollViewItem(RScrollViewItem* item);
    void removeScrollViewItem(RScrollViewItem* item);
    void removeAllScrollViewItem();
    
    void scrollBy(const cocos2d::Vec2& position, bool animated, const std::function<void()>& animatedComplete);
    void scrollTo(const cocos2d::Vec2& position, bool animated, const std::function<void()>& animatedComplete);
    void scrollToTop(bool animated, const std::function<void()>& animatedComplete);
    void stopScrolling();
    
    cocos2d::Vec2 getScrollViewContentOffset();
    cocos2d::Size getScrollViewContentSize();
    cocos2d::Layer* getScrollView();
    
private:
    void init();
    void calcScrollingLimitPosition();
    void createContainerBackground();
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void callOnTouchBeganItem(float dt);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void setContainerPosition(cocos2d::Vec2 position);
    
    void onLoopBounce(float dt);
    void onLoopNoBounce(float dt);
    
    void itemVisibleAll();
    void itemVisibleForIntersectionScrollView();
    RScrollViewItem* getItemForTouch(cocos2d::Touch *touch);
    
    bool isDirectionBoth();
    bool isDirectionHorizontal();
    bool isDirectionVertical();
    
    void schedule(cocos2d::SEL_SCHEDULE selector);
    void scheduleOnce(cocos2d::SEL_SCHEDULE selector, float delay);
    void unschedule(cocos2d::SEL_SCHEDULE selector);
    void unscheduleAll();
    
private:
    RScrollViewControllerDelegate* m_delegate = nullptr;
    cocos2d::EventListenerTouchOneByOne* m_touchEvent = nullptr;
    
    cocos2d::Layer* m_scrollView;
    cocos2d::Layer* m_container;
    cocos2d::Layer* m_containerItem;
    cocos2d::Layer* m_background = nullptr;

    cocos2d::Rect m_scrollViewRect;
    cocos2d::Rect m_scrollViewTouchRect = cocos2d::Rect::ZERO;
    cocos2d::Size m_contentSize;
    
    std::vector<RScrollViewItem*> m_vecScrollViewItem;
    RScrollViewControllerDirection m_direction = RScrollViewControllerDirection::BOTH;

    bool m_isScrollEnabled = true;
    bool m_isStopScrolling = false;
    bool m_isBounceable = true;
    bool m_loopEndHorizontal = false;
    bool m_loopEndVertical = false;
    bool m_isDragDecelerate = false;

    const float m_scrollMaxX = 0.0f;
    float m_scrollMinX = 0.0f;
    const float m_scrollMaxY = 0.0f;
    float m_scrollMinY = 0.0f;
    
    RScrollViewItem* m_itemForTouch;
    bool m_isPressedItem;
    bool m_isTouchMoved = false;
    
    cocos2d::Vec2 m_locationForItem;
    cocos2d::Vec2 m_dragDeceleratePos;
    cocos2d::Vec2 m_animeMove;
    cocos2d::Vec2 m_decelerate;
    
    cocos2d::Vec2 m_containerPos;
    cocos2d::Vec2 m_currPos;
    cocos2d::Vec2 m_prevPos;
    cocos2d::Vec2 m_diffPos;
    cocos2d::Vec2 m_tempPos;
    cocos2d::Vec2 m_targetPos;
    cocos2d::Vec2 m_sumTargetPos;
};

#endif /* defined(__Noc__RScrollViewController__) */