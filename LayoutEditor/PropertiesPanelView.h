/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/
#ifndef __PROPERTIES_PANEL_VIEW_H__
#define __PROPERTIES_PANEL_VIEW_H__

#include <MyGUI.h>
#include "PanelView/PanelView.h"
#include "PanelView/PanelViewWindow.h"
#include "PanelMainProperties.h"
#include "PanelProperties.h"
#include "PanelItems.h"
#include "PanelUserData.h"

extern int grid_step;//FIXME_HOOK

class PropertiesPanelView : public PanelViewWindow
{
public:
	PropertiesPanelView();

	virtual void initialise();

	// widget editing
	void notifyRectangleResize(MyGUI::WidgetPtr _sender);
	void notifyRectangleKeyPressed(MyGUI::WidgetPtr _sender, MyGUI::KeyCode _key, MyGUI::Char _char);

	void update(MyGUI::WidgetPtr _current_widget);

	void createPropertiesWidgetsPair(MyGUI::WidgetPtr _window, Ogre::String _property, Ogre::String _value, std::string _type, int y);
	MyGUI::WindowPtr getWidgetRectangle(){return current_widget_rectangle;};

	MyGUI::IntCoord convertCoordToParentCoord(MyGUI::IntCoord coord, MyGUI::WidgetPtr widget); // ��� ����� � ������ ��� �������

	void setPositionText(const std::string & _caption){propertiesElement[1]->setCaption(_caption);}
	void toggleRelativeMode(){mPanelMainProperties.notifyToggleRelativeMode();}

	typedef MyGUI::delegates::CDelegate1<bool> EventInfo_Recreate;
	EventInfo_Recreate eventRecreate;
private:
	void notifyApplyProperties(MyGUI::WidgetPtr _sender);
	void notifyApplyPropertiesCombo(MyGUI::WidgetPtr _widget); // calls previous method

	// properties window
	MyGUI::VectorWidgetPtr propertiesText;
	MyGUI::VectorWidgetPtr propertiesElement;

	PanelMainProperties mPanelMainProperties;
	PanelProperties mPanelTypeProperties;
	PanelProperties mPanelGeneralProperties;
	PanelProperties mPanelEvents;
	PanelItems mPanelItems;
	PanelUserData mPanelUserData;

	MyGUI::WidgetPtr current_widget;
	MyGUI::WindowPtr current_widget_rectangle;
	
	// widget was moved by keyboard arrows
	bool arrow_move;
};

#endif // __PROPERTIES_PANEL_VIEW_H__
