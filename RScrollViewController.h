//
//  RScrollViewController.h
//  Nock
//
//  Created by Claude Chey on 2014. 11. 16..
//
//

#ifndef __Nock__RScrollViewController__
#define __Nock__RScrollViewController__

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
    void setScrollViewTouchRect(const cocos2d::Rect& rect); // absolute value on screen
    void setScrollViewContentSize(const cocos2d::Size& size);
    void setScrollViewContentOffset(const cocos2d::Vec2& offset);
    void setScrollViewBackgroundOffset(const cocos2d::Vec2& offset);
    void setScrollViewBackgroundLayer(cocos2d::Layer* background);
    void setScrollDirection(RScrollViewControllerDirection direction);
    void setScrollBounces(bool val);
    void setScrollEnabled(bool val);
    void setScrollTouchEvent(bool val);
    
    void addScrollViewItemVector(const std::vector<RScrollViewItem*>* vec);
    void addScrollViewItem(RScrollViewItem* item);
    void removeScrollViewItem(RScrollViewItem* item);
    void removeAllScrollViewItem();
    
    cocos2d::Vec2 getScrollViewContentOffset();
    cocos2d::Vec2 getScrollViewBackgroundOffset();
    cocos2d::Size getScrollViewContentSize();
    std::vector<RScrollViewItem*>* getScrollViewItemVector();
    cocos2d::Layer* getScrollView();
    
    void scrollBy(const cocos2d::Vec2& position, bool animated, const std::function<void()>& animatedComplete);
    void scrollTo(const cocos2d::Vec2& position, bool animated, const std::function<void()>& animatedComplete);
    
    void scrollToTop(bool animated, const std::function<void()>& animatedComplete);
    void scrollToBottom(bool animated, const std::function<void()>& animatedComplete);
    void scrollToLeft(bool animated, const std::function<void()>& animatedComplete);
    void scrollToRight(bool animated, const std::function<void()>& animatedComplete);
    void stopScrolling();
    
    bool isScrollViewOverTheTop();
    bool isScrollViewOverTheBottom();
    void refreshItemVisible();
    
private:
    void init();
    void calcScrollingLimitPosition();
    void createContainerBackground();
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void callOnTouchBeganItem(float dt);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void onLoopBounce(float dt);
    void onLoopNoBounce(float dt);
    void setContainerPosition(cocos2d::Vec2 position);
    
    void itemVisibleAll();
    void itemVisibleForIntersectionScrollView();
    bool isOutOfRangeOnDisplay(RScrollViewItem* item);
    
    RScrollViewItem* getItemForTouch(cocos2d::Touch *touch);
    
    bool isDirectionBoth();
    bool isDirectionHorizontal();
    bool isDirectionVertical();
    
    bool isScheduled(cocos2d::SEL_SCHEDULE selector);
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
    cocos2d::Layer* m_containerBackground = nullptr;
    cocos2d::Layer* m_background = nullptr;

    cocos2d::Rect m_scrollViewRect;
    cocos2d::Rect m_scrollViewTouchRect = cocos2d::Rect::ZERO;
    cocos2d::Size m_contentSize;
    cocos2d::Rect m_scrollViewTouchRectOriginal = cocos2d::Rect::ZERO;
    
    std::vector<RScrollViewItem*>* m_vecScrollViewItem;
    RScrollViewControllerDirection m_direction = RScrollViewControllerDirection::BOTH;

    bool m_isScrollEnabled = true;
    bool m_isStopScrolling = false;
    bool m_isBounceable = true;
    bool m_loopEndHorizontal = false;
    bool m_loopEndVertical = false;
    bool m_isDragDecelerate = false;

    const float m_stopGap = 3.0f;
    
    const float m_scrollMaxX = 0.0f;
    float m_scrollMinX = 0.0f;
    const float m_scrollMaxY = 0.0f;
    float m_scrollMinY = 0.0f;
    
    RScrollViewItem* m_itemForTouch = nullptr;
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

#endif /* defined(__Nock__RScrollViewController__) */
