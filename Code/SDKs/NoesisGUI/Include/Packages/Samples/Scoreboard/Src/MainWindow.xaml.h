////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __SCOREBOARD_MAINWINDOW_H__
#define __SCOREBOARD_MAINWINDOW_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/UserControl.h>


namespace Scoreboard
{

////////////////////////////////////////////////////////////////////////////////////////////////////
class MainWindow final: public Noesis::UserControl
{
public:
    MainWindow();

private:
    void InitializeComponent();
    void OnInitialized(BaseComponent*, const Noesis::EventArgs&);

private:
    NS_DECLARE_REFLECTION(MainWindow, UserControl)
};


}


#endif
