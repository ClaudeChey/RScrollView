RScrollView
=========

RScrollView is scrollview for cocos2d-x 3.2

 - RScrollViewController
 - RScrollViewItem

RScrollViewItem is display item on scrollview.  
RScrollViewItem touch events can be receive (onTouchBeganItem, onTouchMovedItem, onTouchEndedItem).  
When you use the RScrollViewItem recommend that you create by inheritance.




##Simple usage##


    RScrollViewController* svc = RScrollViewController::create();
    svc->setScrollViewSize( "Input your scroll size" );
    svc->setScrollViewContentSize( "Input your contents size" );
    svc->addScrollViewItem( "Your item on scrollview" );
    addChild(svc->getScrollView()); // Important



##Public Methods##

    // scrollview configration
    destroyController();
    setScrollViewControllerDelegate(RScrollViewControllerDelegate* delegate);
    setScrollViewSize(const cocos2d::Size& size);
    setScrollViewTouchRect(const cocos2d::Rect& rect);
    setScrollViewContentSize(const cocos2d::Size& size);
    setScrollViewContentOffset(const cocos2d::Vec2& offset);
    setScrollViewBackgroundLayer(cocos2d::Layer* background);
    setScrollDirection(RScrollViewControllerDirection direction);
    setScrollBounces(bool val);
    setScrollEnabled(bool val);
    setScrollTouchEvent(bool val);
    

    // add/remove item
    addScrollViewItemVector(const std::vector<RScrollViewItem*> vec);
    addScrollViewItem(RScrollViewItem* item);
    removeScrollViewItem(RScrollViewItem* item);
    removeAllScrollViewItem();
    

    // scrollview scrolling
    scrollBy(const cocos2d::Vec2& position, bool animated, const std::function<void()>& animatedComplete);
    scrollTo(const cocos2d::Vec2& position, bool animated, const std::function<void()>& animatedComplete);
    scrollToTop(bool animated, const std::function<void()>& animatedComplete);
    stopScrolling();
    

    // get methods
    cocos2d::Vec2 getScrollViewContentOffset();
    cocos2d::Size getScrollViewContentSize();
    std::vector<RScrollViewItem*> getScrollViewItem();
    cocos2d::Layer* getScrollView();

    void refreshItemVisible();



##RScrollViewControllerDelegate##
If you need `RScrollViewController` delegate with `setScrollViewControllerDelegate`.

    void onDidScrolling(cocos2d::Vec2 position);
    void onDidEndScrolling(cocos2d::Vec2 position);
    void onScrollTouchBegan(cocos2d::Vec2 position);
    void onScrollTouchMoved(cocos2d::Vec2 position);
    void onScrollTouchEnded(cocos2d::Vec2 position);
    void onDraggingDecelerate(cocos2d::Vec2 deceleratePosition);
    void onScrollingDecelerate(cocos2d::Vec2 deceleratePosition);
    void onWillEndDraggingDecelerate(cocos2d::Vec2 deceleratePosition);  



##RScrollViewControllerDirection##
You can specify the scrolling direction with `setScrollDirection `.

    RScrollViewControllerDirection::BOTH
    RScrollViewControllerDirection::VERTICAL
    RScrollViewControllerDirection::HORIZONTAL
