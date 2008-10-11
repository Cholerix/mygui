/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __MYGUI_SCROLL_VIEW_H__
#define __MYGUI_SCROLL_VIEW_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class _MyGUIExport ScrollView : public Widget
	{
		// ��� ������ ��������� ������������
		friend class factory::ScrollViewFactory;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		ScrollView(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);
		virtual ~ScrollView();

	public:
		//! @copydoc Widget::setTextAlign
		virtual void setTextAlign(Align _align);

		//! @copydoc Widget::setPosition(const IntCoord& _coord)
		void setPosition(const IntCoord& _coord);
		//! @copydoc Widget::setSize(const IntSize& _size)
		void setSize(const IntSize& _size);
		//! @copydoc Widget::setPosition(int _left, int _top)
		void setPosition(int _left, int _top) { Widget::setPosition(IntPoint(_left, _top)); }
		//! @copydoc Widget::setPosition(int _left, int _top, int _width, int _height)
		void setPosition(int _left, int _top, int _width, int _height) { setPosition(IntCoord(_left, _top, _width, _height)); }
		//! @copydoc Widget::setSize(int _width, int _height)
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }

		/** Show VScroll when canvas size larger than ScrollView */
		void showVScroll(bool _show) { mShowVScroll = _show; updateView(); }
		/** Get Show VScroll flag */
		bool isShowVScroll() { return mShowVScroll; }

		/** Show HScroll when canvas size larger than ScrollView */
		void showHScroll(bool _show) { mShowHScroll = _show; updateView(); }
		/** Get Show HScroll flag */
		bool isShowHScroll() { return mShowHScroll; }

		/** Get canvas align */
		Align getCanvasAlign() { return mAlignCanvas; }
		/** Set canvas align */
		void setCanvasAlign(Align _align) { mAlignCanvas = _align; updateView(); }

		/** Get canvas size */
		IntSize getCanvasSize() { return mWidgetCanvas->getSize(); }
		/** Set canvas size */
		void setCanvasSize(const IntSize & _size) { mWidgetCanvas->setSize(_size); updateView(); }
		/** Set canvas size */
		void setCanvasSize(int _width, int _height) { mWidgetCanvas->setSize(_width, _height); updateView(); }

		// �������������� ��� ���������� ������
		virtual WidgetPtr _createWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name);

		// �������������� ��� �������� ����� ������
		virtual EnumeratorWidgetPtr getEnumerator();

	protected:

		void notifyMouseSetFocus(WidgetPtr _sender, WidgetPtr _old);
		void notifyMouseLostFocus(WidgetPtr _sender, WidgetPtr _new);
		void notifyMousePressed(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyMouseReleased(WidgetPtr _sender, int _left, int _top, MouseButton _id);

		void notifyScrollChangePosition(WidgetPtr _sender, size_t _position);
		void notifyMouseWheel(WidgetPtr _sender, int _rel);

		virtual void onKeyLostFocus(WidgetPtr _new);
		virtual void onKeySetFocus(WidgetPtr _old);

		void updateScrollViewState();

		// ���������� �������������
		void updateView();

		void updateScroll();

	protected:
		bool mIsFocus;
		bool mIsPressed;

		VScrollPtr mVScroll;
		HScrollPtr mHScroll;

		bool mShowHScroll;
		bool mShowVScroll;

		size_t mVRange;
		size_t mHRange;

		WidgetPtr mWidgetCanvas;
		Align mAlignCanvas;

	}; // class ScrollView : public Widget

} // namespace MyGUI

#endif // __MYGUI_SCROLL_VIEW_H__
