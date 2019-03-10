////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __MENU3D_MAINMENU_H__
#define __MENU3D_MAINMENU_H__


#include <NsCore/Noesis.h>
#include <NsGui/UserControl.h>


namespace Menu3D
{

////////////////////////////////////////////////////////////////////////////////////////////////////
class MainMenu final: public Noesis::UserControl
{
public:
    MainMenu();

private:
    void InitializeComponent();

    NS_DECLARE_REFLECTION(MainMenu, UserControl)
};

}


#endif
