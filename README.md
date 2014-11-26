RScrollView is ScrollView for cocos2d-x 3.2

Simple usage :
RScrollViewController* svc = RScrollViewController::create();
svc->setScrollViewSize( "Input your scroll size" );
svc->setScrollViewContentSize( "Input your contents size" );
addChild(svc->getScrollView()); // Impotant


Methods :

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

// set item
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
cocos2d::Layer* getScrollView();

