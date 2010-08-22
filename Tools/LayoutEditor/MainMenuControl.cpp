/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "MainMenuControl.h"
#include "SettingsManager.h"
#include "CommandManager.h"
#include "WidgetSelectorManager.h"
#include "WidgetContainer.h"
#include "EditorWidgets.h"

namespace tools
{

	MainMenuControl::MainMenuControl() :
		mBar(nullptr),
		mPopupMenuFile(nullptr),
		mPopupMenuWidgets(nullptr)
	{
		createMainMenu();

		tools::SettingsManager::getInstance().eventSettingsChanged += MyGUI::newDelegate(this, &MainMenuControl::notifySettingsChanged);
		EditorWidgets::getInstance().eventChangeWidgets += MyGUI::newDelegate(this, &MainMenuControl::notifyChangeWidgets);
	}

	MainMenuControl::~MainMenuControl()
	{
		EditorWidgets::getInstance().eventChangeWidgets -= MyGUI::newDelegate(this, &MainMenuControl::notifyChangeWidgets);
		tools::SettingsManager::getInstance().eventSettingsChanged -= MyGUI::newDelegate(this, &MainMenuControl::notifySettingsChanged);
	}

	void MainMenuControl::createMainMenu()
	{
		MyGUI::VectorWidgetPtr menu_items = MyGUI::LayoutManager::getInstance().loadLayout("interface_menu.layout");
		MYGUI_ASSERT(menu_items.size() == 1, "Error load main menu");
		mBar = menu_items[0]->castType<MyGUI::MenuBar>();
		mBar->setCoord(0, 0, mBar->getParentSize().width, mBar->getHeight());

		// ������� ����
		MyGUI::MenuItem* menu_file = mBar->getItemById("File");
		mPopupMenuFile = menu_file->getItemChild();

		// ������ ��������� �������� ������
		const tools::VectorUString& recentFiles = tools::SettingsManager::getInstance().getRecentFiles();
		if (!recentFiles.empty())
		{
			MyGUI::MenuItem* menu_item = mPopupMenuFile->getItemById("File/Quit");
			for (tools::VectorUString::const_reverse_iterator iter = recentFiles.rbegin(); iter != recentFiles.rend(); ++iter)
			{
				mPopupMenuFile->insertItem(menu_item, *iter, MyGUI::MenuItemType::Normal, "File/RecentFiles",  *iter);
			}
			// ���� ���� �����, �� ��� ���� ���������
			mPopupMenuFile->insertItem(menu_item, "", MyGUI::MenuItemType::Separator);
		}

		// ���� ��� ��������
		MyGUI::MenuItem* menu_widget = mBar->getItemById("Widgets");
		mPopupMenuWidgets = menu_widget->createItemChild();
		//FIXME
		mPopupMenuWidgets->setPopupAccept(true);
		mPopupMenuWidgets->eventMenuCtrlAccept += MyGUI::newDelegate(this, &MainMenuControl::notifyWidgetsSelect);

		mBar->eventMenuCtrlAccept += newDelegate(this, &MainMenuControl::notifyPopupMenuAccept);

		//mInterfaceWidgets.push_back(mBar);
	}

	void MainMenuControl::notifyPopupMenuAccept(MyGUI::MenuCtrl* _sender, MyGUI::MenuItem* _item)
	{
		MyGUI::UString* data = _item->getItemData<MyGUI::UString>(false);
		if (data != nullptr)
			tools::CommandManager::getInstance().setCommandData(*data);

		if (mPopupMenuFile == _item->getMenuCtrlParent())
		{
			std::string id = _item->getItemId();

			if (id == "File/Load")
			{
				tools::CommandManager::getInstance().executeCommand("Command_FileLoad");
			}
			else if (id == "File/Save")
			{
				tools::CommandManager::getInstance().executeCommand("Command_FileSave");
			}
			else if (id == "File/SaveAs")
			{
				tools::CommandManager::getInstance().executeCommand("Command_FileSaveAs");
			}
			else if (id == "File/Clear")
			{
				tools::CommandManager::getInstance().executeCommand("Command_ClearAll");
			}
			else if (id == "File/Settings")
			{
				tools::CommandManager::getInstance().executeCommand("Command_Settings");
			}
			else if (id == "File/Test")
			{
				tools::CommandManager::getInstance().executeCommand("Command_Test");
			}
			else if (id == "File/Code_Generator")
			{
				tools::CommandManager::getInstance().executeCommand("Command_CodeGenerator");
			}
			else if (id == "File/RecentFiles")
			{
				tools::CommandManager::getInstance().executeCommand("Command_RecentFiles");
			}
			else if (id == "File/Quit")
			{
				tools::CommandManager::getInstance().executeCommand("Command_QuitApp");
			}
		}
	}

	void MainMenuControl::widgetsUpdate()
	{
		bool print_name = tools::SettingsManager::getInstance().getPropertyValue<bool>("SettingsWindow", "ShowName");
		bool print_type = tools::SettingsManager::getInstance().getPropertyValue<bool>("SettingsWindow", "ShowType");
		bool print_skin = tools::SettingsManager::getInstance().getPropertyValue<bool>("SettingsWindow", "ShowSkin");

		mPopupMenuWidgets->removeAllItems();

		for (std::vector<WidgetContainer*>::iterator iter = EditorWidgets::getInstance().widgets.begin(); iter != EditorWidgets::getInstance().widgets.end(); ++iter )
		{
			createWidgetPopup(*iter, mPopupMenuWidgets, print_name, print_type, print_skin);
		}
	}

	void MainMenuControl::createWidgetPopup(WidgetContainer* _container, MyGUI::MenuCtrl* _parentPopup, bool _print_name, bool _print_type, bool _print_skin)
	{
		bool submenu = !_container->childContainers.empty();

		_parentPopup->addItem(getDescriptionString(_container->widget, _print_name, _print_type, _print_skin), submenu ? MyGUI::MenuItemType::Popup : MyGUI::MenuItemType::Normal);
		_parentPopup->setItemDataAt(_parentPopup->getItemCount()-1, _container->widget);

		if (submenu)
		{
			MyGUI::MenuCtrl* child = _parentPopup->createItemChildAt(_parentPopup->getItemCount()-1);
			child->eventMenuCtrlAccept += MyGUI::newDelegate(this, &MainMenuControl::notifyWidgetsSelect);
			child->setPopupAccept(true);

			for (std::vector<WidgetContainer*>::iterator iter = _container->childContainers.begin(); iter != _container->childContainers.end(); ++iter )
			{
				createWidgetPopup(*iter, child, _print_name, _print_type, _print_skin);
			}
		}
	}

	void MainMenuControl::notifyWidgetsSelect(MyGUI::MenuCtrl* _sender, MyGUI::MenuItem* _item)
	{
		MyGUI::Widget* widget = *_item->getItemData<MyGUI::Widget*>();
		tools::WidgetSelectorManager::getInstance().setSelectedWidget(widget);
	}

	std::string MainMenuControl::getDescriptionString(MyGUI::Widget* _widget, bool _print_name, bool _print_type, bool _print_skin)
	{
		std::string name = "";
		std::string type = "";
		std::string skin = "";

		WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(_widget);
		if (_print_name)
		{
			if (widgetContainer->name.empty())
			{
			}
			else
			{
				// FIXME my.name ��� ����� ��� ������ ��� ������ ������� � ������
				name = "#{ColourMenuName}'" + widgetContainer->name + "' ";
			}
		}

		if (_print_type)
		{
			// FIXME my.name ��� ����� ��� ������ ��� ������ ������� � ������
			type = "#{ColourMenuType}[" + _widget->getTypeName() + "] ";
		}

		if (_print_skin)
		{
			// FIXME my.name ��� ����� ��� ������ ��� ������ ������� � ������
			skin = "#{ColourMenuSkin}" + widgetContainer->skin + " ";
		}
		return MyGUI::LanguageManager::getInstance().replaceTags(type + skin + name);
	}

	void MainMenuControl::notifyChangeWidgets()
	{
		widgetsUpdate();
	}

	void MainMenuControl::notifySettingsChanged(const MyGUI::UString& _sectionName, const MyGUI::UString& _propertyName)
	{
		if (_sectionName == "SettingsWindow")
		{
			widgetsUpdate();
		}
	}

} // namespace tools
