////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __XAMLPLAYER_APP_H__
#define __XAMLPLAYER_APP_H__


#include <NsCore/Noesis.h>
#include <NsApp/Application.h>
#include <NsCore/ReflectionDeclare.h>


namespace XamlPlayer
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// XAML Player Application logic
////////////////////////////////////////////////////////////////////////////////////////////////////
class App final: public NoesisApp::Application
{
public:
    App();

private:
    void OnStartUp(Noesis::BaseComponent*, const Noesis::EventArgs&);

    NS_DECLARE_REFLECTION(App, Application)
};

}

#endif
