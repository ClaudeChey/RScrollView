//
//  RScrollViewController.cpp
//  Noc
//
//  Created by Claude Chey on 2014. 11. 16..
//
//

#include "RScrollViewController.h"

using namespace cocos2d;
using namespace std;

enum class LayerDepth
{
    BACKGROUND=1,
    ITEM=2,
};

RScrollViewController* RScrollViewController::create()
{
    RScrollViewController* scrollViewController = new RScrollViewController();
    scrollViewController->init();
    scrollViewController->autorelease();
    scrollViewController->retain();
    return scrollViewController;
}

void RScrollViewController::destroyController()
{
    unscheduleAll();
    m_scrollView->cleanup();
    release();
}

void RScrollViewController::init()
{
    m_scrollView = Layer::create();
    m_container = Layer::create();
    m_containerItem = Layer::create();
    
    m_scrollView->addChild(m_container);
    m_container->addChild(m_containerItem, (int)LayerDepth::ITEM);
    
    setScrollTouchEvent(true);
}

void RScrollViewController::setScrollViewControllerDelegate(RScrollViewControllerDelegate* delegate)
{
    m_delegate = delegate;
}





void RScrollViewController::setScrollViewSize(const Size& size)
{
    m_scrollViewRect.size = size;
    
    calcScrollingLimitPosition();
}

void RScrollViewController::setScrollViewTouchRect(const Rect& rect)
{
    m_scrollViewTouchRectOriginal = rect;
}

void RScrollViewController::setScrollViewContentSize(const Size& size)
{
    m_contentSize = size;
    calcScrollingLimitPosition();
}

void RScrollViewController::calcScrollingLimitPosition()
{
    m_scrollMinX = m_scrollViewRect.size.width - m_contentSize.width;
    if(m_contentSize.width<m_scrollViewRect.size.width)
        m_scrollMinX = m_scrollMaxX;

    m_scrollMinY = m_scrollViewRect.size.height - m_contentSize.height;
    if(m_contentSize.height<m_scrollViewRect.size.height)
        m_scrollMinY = m_scrollMaxY;
}

void RScrollViewController::setScrollViewContentOffset(const Vec2& offset)
{
    m_containerItem->setPosition(offset);
}

void RScrollViewController::setScrollViewBackgroundLayer(Layer* background)
{
    if(m_background)
    {
        m_container->removeChild(m_background);
        m_background = nullptr;
    }
    if(background!=nullptr)
    {
        m_background = background;
        m_container->addChild(background, (int)LayerDepth::BACKGROUND);
    }
}

void RScrollViewController::setScrollDirection(RScrollViewControllerDirection direction)
{
    m_direction = direction;
}

void RScrollViewController::setScrollBounces(bool val)
{
    m_isBounceable = val;
}

void RScrollViewController::setScrollEnabled(bool val)
{
    m_isScrollEnabled = val;
}

void RScrollViewController::setScrollTouchEvent(bool val)
{
    if(val)
    {
        if(m_touchEvent) return;
        
        m_touchEvent = EventListenerTouchOneByOne::create();
        m_touchEvent->setSwallowTouches(true);
        m_touchEvent->onTouchBegan = CC_CALLBACK_2(RScrollViewController::onTouchBegan, this);
        m_touchEvent->onTouchMoved = CC_CALLBACK_2(RScrollViewController::onTouchMoved, this);
        m_touchEvent->onTouchEnded = CC_CALLBACK_2(RScrollViewController::onTouchEnded, this);
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_touchEvent, m_scrollView);
    }
    else
    {
        if(m_touchEvent==nullptr) return;
        
        Director::getInstance()->getEventDispatcher()->removeEventListener(m_touchEvent);
        m_touchEvent = nullptr;
    }
}



void RScrollViewController::addScrollViewItemVector(const vector<RScrollViewItem*> vec)
{
    vector<RScrollViewItem*>::const_iterator it = vec.begin();
    for(; it!=vec.end(); ++it)
    {
        m_containerItem->addChild(*it);
        m_vecScrollViewItem.push_back(*it);
    }
}

void RScrollViewController::addScrollViewItem(RScrollViewItem* item)
{
    m_containerItem->addChild(item);
    m_vecScrollViewItem.push_back(item);
}

void RScrollViewController::removeScrollViewItem(RScrollViewItem* item)
{
    m_containerItem->removeChild(item);
    vector<RScrollViewItem*>::iterator it = find(m_vecScrollViewItem.begin(), m_vecScrollViewItem.end(), item);
    if(it!=m_vecScrollViewItem.end())
        m_vecScrollViewItem.erase(it);
}

void RScrollViewController::removeAllScrollViewItem()
{
    m_containerItem->removeAllChildren();
}





void RScrollViewController::scrollBy(const Vec2& position, bool animated, const function<void()>& animatedComplete)
{
    Vec2 targetPos = m_container->getPosition() + position;
    if(animated==false)
        m_container->setPosition(targetPos);
    else
    {
        itemVisibleAll();
        MoveBy* move = MoveBy::create(0.3f, targetPos);
        EaseCubicActionInOut* easeMove = EaseCubicActionInOut::create(move);
        CallFunc* func = CallFunc::create([=]()->void {
            if(animatedComplete!=nullptr) animatedComplete();
            itemVisibleForIntersectionScrollView();
        });
        m_container->runAction(Sequence::create(easeMove, func, NULL));
    }
}

void RScrollViewController::scrollTo(const Vec2& position, bool animated, const function<void()>& animatedComplete)
{
    if(animated==false)
        m_container->setPosition(position);
    else
    {
        itemVisibleAll();
        MoveTo* move = MoveTo::create(0.3f, position);
        EaseCubicActionInOut* easeMove = EaseCubicActionInOut::create(move);
        CallFunc* func = CallFunc::create([=]()->void {
            if(animatedComplete!=nullptr) animatedComplete();
            itemVisibleForIntersectionScrollView();
        });
        m_container->runAction(Sequence::create(easeMove, func, NULL));
    }
}

void RScrollViewController::scrollToTop(bool animated, const function<void()>& animatedComplete)
{
    if(animated==false)
        m_container->setPositionY(m_scrollMinY);
    else
    {
        itemVisibleAll();
        MoveTo* move = MoveTo::create(0.3f, Vec2(m_scrollMinX, 0));
        EaseCubicActionIn* easeMove = EaseCubicActionIn::create(move);
        CallFunc* func = CallFunc::create([=]()->void {
            if(animatedComplete!=nullptr) animatedComplete();
            itemVisibleForIntersectionScrollView();
        });
        m_container->runAction(Sequence::create(easeMove, func, NULL));
    }
}

void RScrollViewController::stopScrolling()
{
    m_isStopScrolling = true;
    
}




Vec2 RScrollViewController::getScrollViewContentOffset()
{
    return m_containerItem->getPosition();
}

Size RScrollViewController::getScrollViewContentSize()
{
    return m_contentSize;
}

Layer* RScrollViewController::getScrollView()
{
    return m_scrollView;
}




bool RScrollViewController::onTouchBegan(Touch *touch, Event *event)
{
    if(m_scrollViewTouchRectOriginal.equals(Rect::ZERO))
    {
        m_scrollViewTouchRect.origin = m_scrollView->getPosition();
        m_scrollViewTouchRect.size = m_scrollViewRect.size;
    }
    else
        m_scrollViewTouchRect = m_scrollViewTouchRectOriginal;

    
    m_scrollViewTouchRect.origin = m_scrollView->getPosition();
    if(m_scrollViewTouchRect.containsPoint(touch->getLocation())==false)
        return false;
    
    if(m_delegate) m_delegate->onScrollTouchBegan(touch->getLocation());
    if(m_isScrollEnabled==false) return true;
    
    m_animeMove = Vec2::ZERO;
    m_decelerate = Vec2::ZERO;
    m_dragDeceleratePos = Vec2::ZERO;
    
    unschedule(schedule_selector(RScrollViewController::onLoopBounce));
    unschedule(schedule_selector(RScrollViewController::onLoopNoBounce));
    
    m_isTouchMoved = false;
    m_isPressedItem = false;
    m_itemForTouch = getItemForTouch(touch);
    if(m_itemForTouch)
    {
        m_locationForItem = touch->getLocation();
        scheduleOnce(schedule_selector(RScrollViewController::callOnTouchBeganItem), 0.05f);
    }
    return true;
}

void RScrollViewController::callOnTouchBeganItem(float dt)
{
    m_isPressedItem = true;
    m_itemForTouch->onTouchBeganItem(m_locationForItem);
}


void RScrollViewController::onTouchMoved(Touch *touch, Event *event)
{
    if(m_delegate) m_delegate->onScrollTouchMoved(touch->getLocation());
    if(m_isScrollEnabled==false) return;
    
    m_isTouchMoved = true;
    if(m_isPressedItem)
    {
        m_itemForTouch->onTouchMovedItem(touch->getLocation());
        m_isPressedItem = false;
    }
    else
        unschedule(schedule_selector(RScrollViewController::callOnTouchBeganItem));
    
    
    m_containerPos = m_container->getPosition();
    m_currPos = touch->getLocation();
    m_prevPos = touch->getPreviousLocation();
    m_diffPos = m_currPos - m_prevPos;
    Vec2 sumPos = Vec2::ZERO;

    if(isDirectionBoth() || isDirectionHorizontal())
    {
        if(m_scrollMaxX <  m_containerPos.x || m_containerPos.x < m_scrollMinX )
        {
            m_diffPos.x *= 0.4;
            m_animeMove.x = (m_currPos.x - m_prevPos.x)*5;
        }
        else
            m_animeMove.x = (m_currPos.x - m_prevPos.x)*10;
        
        sumPos.x = m_containerPos.x + m_diffPos.x;
    }
    
    if(isDirectionBoth() || isDirectionVertical())
    {
        if(m_scrollMaxY <  m_containerPos.y || m_containerPos.y < m_scrollMinY )
        {
            m_diffPos.y *= 0.4;
            m_animeMove.y = (m_currPos.y - m_prevPos.y)*5;
        }
        else
            m_animeMove.y = (m_currPos.y - m_prevPos.y)*10;
        
        sumPos.y = m_containerPos.y + m_diffPos.y;
    }
    
    
    if(m_isBounceable)
    {
        m_dragDeceleratePos = Vec2::ZERO;
        
        if(isDirectionBoth() || isDirectionHorizontal())
        {
            if(m_scrollMaxX < sumPos.x)
                m_dragDeceleratePos.x = sumPos.x - m_scrollMaxX;
            else if(sumPos.x < m_scrollMinX)
                m_dragDeceleratePos.x = sumPos.x - m_scrollMinX;
        }
        if(isDirectionBoth() || isDirectionVertical())
        {
            if(m_scrollMaxY < sumPos.y)
                m_dragDeceleratePos.y = sumPos.y - m_scrollMaxY;
            else if(sumPos.y < m_scrollMinY)
                m_dragDeceleratePos.y = sumPos.y - m_scrollMinY;
        }
        
        if(m_dragDeceleratePos.isZero()==false)
        {
            if(m_delegate)
                m_delegate->onDraggingDecelerate(m_dragDeceleratePos);
            
            m_isDragDecelerate = true;
        }
        else
            m_isDragDecelerate = false;
    }
    else
    {
        if(isDirectionBoth() || isDirectionHorizontal())
        {
            if(m_scrollMaxX < sumPos.x)
                sumPos.x = m_scrollMaxX;
            else if(sumPos.x < m_scrollMinX)
                sumPos.x = m_scrollMinX;
        }
        if(isDirectionBoth() || isDirectionVertical())
        {
            if(m_scrollMaxY < sumPos.y)
                sumPos.y = m_scrollMaxY;
            else if(sumPos.y < m_scrollMinY)
                sumPos.y = m_scrollMinY;
        }
    }
    setContainerPosition(sumPos);
    itemVisibleForIntersectionScrollView();
}

void RScrollViewController::onTouchEnded(Touch *touch, Event *event)
{
    if(m_delegate) m_delegate->onScrollTouchEnded(touch->getLocation());
    if(m_isScrollEnabled==false) return;
    
    if(m_isDragDecelerate)
        if(m_delegate)
            m_delegate->onWillEndDraggingDecelerate(m_dragDeceleratePos);
    
    if(m_isTouchMoved==false)
    {
        unschedule(schedule_selector(RScrollViewController::callOnTouchBeganItem));
        if(m_itemForTouch) m_itemForTouch->onTouchEndedItem(touch->getLocation());
    }
    
    m_loopEndHorizontal = false;
    m_loopEndVertical = false;
    if(m_isBounceable)
        schedule(schedule_selector(RScrollViewController::onLoopBounce));
    else
        schedule(schedule_selector(RScrollViewController::onLoopNoBounce));
}



void RScrollViewController::onLoopBounce(float dt)
{
    m_containerPos = m_container->getPosition();
    m_tempPos = m_animeMove * 0.1f;
    m_animeMove = m_animeMove - m_tempPos;
    m_targetPos = m_container->getPosition();
    Vec2 deceleratePos = Vec2::ZERO;
    
    if((isDirectionBoth() || isDirectionHorizontal()) && m_loopEndHorizontal==false)
    {
        if(m_scrollMaxX<m_containerPos.x) m_decelerate.x = (m_scrollMaxX - m_containerPos.x) * 0.2f;
        else if(m_containerPos.x<m_scrollMinX) m_decelerate.x = (m_scrollMinX - m_containerPos.x) * 0.2f;
        else m_decelerate.x = 0.0f;
        
        if( -0.1f < m_animeMove.x && m_animeMove.x < 0.1f && -0.1f < m_decelerate.x && m_decelerate.x < 0.1f  )
        {
            if(m_decelerate.x<0)
                m_targetPos.x = m_scrollMaxX;
            else if(0<m_decelerate.x)
                m_targetPos.x = m_scrollMinX;
            else
                m_targetPos.x = m_containerPos.x + m_tempPos.x;
            
            m_loopEndHorizontal = true;
        }
        else
        {
            m_targetPos.x = m_containerPos.x + m_tempPos.x + m_decelerate.x;

            if(m_scrollMaxX < m_targetPos.x)
                deceleratePos.x = m_targetPos.x-m_scrollMaxX;
            else if(m_targetPos.x < m_scrollMinX)
                deceleratePos.x = m_targetPos.x-m_scrollMinX;
        }
    }
    else m_loopEndHorizontal = true;
    
    if((isDirectionBoth() || isDirectionVertical()) && m_loopEndVertical==false)
    {
        if(m_scrollMaxY<m_containerPos.y) m_decelerate.y = (m_scrollMaxY - m_containerPos.y) * 0.2f;
        else if(m_containerPos.y<m_scrollMinY) m_decelerate.y = (m_scrollMinY - m_containerPos.y) * 0.2f;
        else m_decelerate.y = 0.0f;
        
        if( -0.1f < m_animeMove.y && m_animeMove.y < 0.1f && -0.1f < m_decelerate.y && m_decelerate.y < 0.1f  )
        {
            if(m_decelerate.y<0)
                m_targetPos.y = m_scrollMaxY;
            else if(0<m_decelerate.y)
                m_targetPos.y = m_scrollMinY;
            else
                m_targetPos.y = m_containerPos.y + m_tempPos.y;

            m_loopEndVertical = true;
        }
        else
        {
            m_targetPos.y = m_containerPos.y + m_tempPos.y + m_decelerate.y;
            
            if(m_scrollMaxY < m_targetPos.y)
                deceleratePos.y = m_targetPos.y-m_scrollMaxY;
            else if(m_targetPos.y < m_scrollMinY)
                deceleratePos.y = m_targetPos.y-m_scrollMinY;
        }
    }
    else m_loopEndVertical = true;
    
    
    if(m_delegate)
    {
        if(deceleratePos.isZero()==false)
            m_delegate->onScrollingDecelerate(deceleratePos);
    }
    
    if(m_isStopScrolling)
    {
        m_isStopScrolling = false;
        unschedule(schedule_selector(RScrollViewController::onLoopBounce));
    }
    else
    {
        setContainerPosition(m_targetPos);
        itemVisibleForIntersectionScrollView();

        if(m_loopEndHorizontal && m_loopEndVertical )
        {
            if(m_delegate)
                m_delegate->onDidEndScrolling(m_targetPos);
            unschedule(schedule_selector(RScrollViewController::onLoopBounce));
        }
    }
}

void RScrollViewController::onLoopNoBounce(float dt)
{
    m_containerPos = m_container->getPosition();
    m_tempPos = m_animeMove * 0.1f;
    m_animeMove = m_animeMove - m_tempPos;
    m_sumTargetPos = m_containerPos + m_tempPos;
    
    if(m_scrollMaxX<m_sumTargetPos.x)
    {
        m_sumTargetPos.x = m_scrollMaxX;
        m_loopEndHorizontal = true;
    }
    else if(m_sumTargetPos.x<m_scrollMinX)
    {
        m_sumTargetPos.x = m_scrollMinX;
        m_loopEndHorizontal = true;
    }
    
    if(m_scrollMaxY<m_sumTargetPos.y)
    {
        m_sumTargetPos.y = m_scrollMaxY;
        m_loopEndVertical = true;
    }
    else if(m_sumTargetPos.y<m_scrollMinY)
    {
        m_sumTargetPos.y = m_scrollMinY;
        m_loopEndVertical = true;
    }
    
    setContainerPosition(m_sumTargetPos);
    itemVisibleForIntersectionScrollView();
    if(m_loopEndHorizontal && m_loopEndVertical )
    {
        if(m_delegate)
            m_delegate->onDidEndScrolling(m_targetPos);
        unschedule(schedule_selector(RScrollViewController::onLoopNoBounce));
    }
}

void RScrollViewController::setContainerPosition(Vec2 position)
{
    switch (m_direction) {
        case RScrollViewControllerDirection::HORIZONTAL:
            m_container->setPositionX(position.x);
            break;
            
        case RScrollViewControllerDirection::VERTICAL:
            m_container->setPositionY(position.y);
            break;
            
        default: // BOTH
            m_container->setPosition(position);
            break;
    }
    if(m_delegate) m_delegate->onDidScrolling(position);
}



void RScrollViewController::itemVisibleAll()
{
    if(m_vecScrollViewItem.empty()) return;
    
    RScrollViewItem* item;
    vector<RScrollViewItem*>::const_iterator it = m_vecScrollViewItem.begin();
    for(; it!=m_vecScrollViewItem.end(); ++it)
    {
        item = (RScrollViewItem*)*it;
        item->setOutOfRangeOnDisplay(false);
    }
}

void RScrollViewController::itemVisibleForIntersectionScrollView()
{
    if(m_vecScrollViewItem.empty()) return;
    
    m_scrollViewRect.origin = m_scrollView->getPosition();
    
    RScrollViewItem* item;
    Rect itemRect;
    vector<RScrollViewItem*>::const_iterator it = m_vecScrollViewItem.begin();
    for(; it!=m_vecScrollViewItem.end(); ++it)
    {
        item = (RScrollViewItem*)*it;
        itemRect = item->getContentRect();
        itemRect.origin = m_containerItem->convertToWorldSpace(item->getContentRect().origin);

        if( m_scrollViewRect.intersectsRect(itemRect) )
            item->setOutOfRangeOnDisplay(false);
        else
            item->setOutOfRangeOnDisplay(true);
    }
}


RScrollViewItem* RScrollViewController::getItemForTouch(Touch *touch)
{
    if(m_vecScrollViewItem.empty()) return nullptr;
    
    Vec2 touchLocation = touch->getLocation();
    RScrollViewItem* item;
    Vec2 localPos;
    Rect itemRect;
    
    vector<RScrollViewItem*>::const_reverse_iterator it = m_vecScrollViewItem.crbegin();
    for(; it!=m_vecScrollViewItem.crend(); ++it)
    {
        item = (RScrollViewItem*)*it;
        if(item->isVisible())
        {
            localPos = m_containerItem->convertToNodeSpace(touchLocation);
            itemRect = item->getContentRect();
            if(itemRect.containsPoint(localPos))
            {
                return item;
            }
        }
    }
    return nullptr;
}









inline bool RScrollViewController::isDirectionBoth()
{
    return m_direction == RScrollViewControllerDirection::BOTH;
}

inline bool RScrollViewController::isDirectionHorizontal()
{
    return m_direction == RScrollViewControllerDirection::HORIZONTAL;
}

inline bool RScrollViewController::isDirectionVertical()
{
    return m_direction == RScrollViewControllerDirection::VERTICAL;
}




void RScrollViewController::schedule(SEL_SCHEDULE selector)
{
    Director::getInstance()->getScheduler()->schedule(selector, this, 0.0f, kRepeatForever, 0.0f, !m_scrollView->isRunning());
}

void RScrollViewController::scheduleOnce(cocos2d::SEL_SCHEDULE selector, float delay)
{
    Director::getInstance()->getScheduler()->schedule(selector, this, 0.0f, 0, delay, !m_scrollView->isRunning());
}

void RScrollViewController::unschedule(SEL_SCHEDULE selector)
{
    Director::getInstance()->getScheduler()->unschedule(selector, this);
}

void RScrollViewController::unscheduleAll()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
}

