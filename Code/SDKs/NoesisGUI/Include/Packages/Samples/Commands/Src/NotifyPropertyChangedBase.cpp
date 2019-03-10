////////////////////////////////////////////////////////////////////////////////////////////////////
// Noesis Engine - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "NotifyPropertyChangedBase.h"

#include <NsCore/ReflectionImplement.h>


using namespace Commands;
using namespace Noesis;


////////////////////////////////////////////////////////////////////////////////////////////////////
PropertyChangedEventHandler& NotifyPropertyChangedBase::PropertyChanged()
{
    return _propertyChanged;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void NotifyPropertyChangedBase::OnPropertyChanged(const char* name)
{
    if (_propertyChanged)
    {
        _propertyChanged(this, PropertyChangedEventArgs(Symbol(name)));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_IMPLEMENT_REFLECTION(NotifyPropertyChangedBase)
{
    NsImpl<INotifyPropertyChanged>();
}
