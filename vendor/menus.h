#pragma once

#include <functional>
#include <string>

#define Menus_INTERFACE "IMenusApi"

#define ITEM_HIDE 0
#define ITEM_DEFAULT 1
#define ITEM_DISABLED 2

typedef std::function<void(const char* szItem, const char* szTranslate, int iItem, int iSlot)> MenuCallbackFunc;

struct Items
{
    int iType;
    std::string sBack;
    std::string sText;
};

struct Menu
{
    std::string szTitle;	
    std::vector<Items> hItems;
	MenuCallbackFunc hFunc = nullptr;
	bool bBack = true;
    void clear() {
        szTitle.clear();
        hItems.clear();
        hFunc = nullptr;
    }
};

struct MenuPlayer
{
    bool bEnabled;
    int iList;
    Menu hMenu;
    int iEnd;

    void clear() {
        bEnabled = false;
        iList = 0;
        hMenu.clear();
        iEnd = 0;
    }
};

class IMenusApi
{
public:
	virtual void AddItemMenu(Menu& hMenu, const char* sBack, const char *sText, ...) = 0;
	virtual void DisplayPlayerMenu(Menu& hMenu, int iSlot, bool bClose = true) = 0;
	virtual void SetTitleMenu(Menu& hMenu, const char *sText, ...) = 0;
	virtual void SetBackMenu(Menu& hMenu, bool bBack) = 0;
	virtual void SetCallback(Menu& hMenu, MenuCallbackFunc func) = 0;
    virtual void ClosePlayerMenu(int iSlot) = 0;
    virtual bool IsMenuOpen(int iSlot) = 0;
};