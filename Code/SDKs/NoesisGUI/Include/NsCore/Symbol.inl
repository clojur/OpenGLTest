////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
// [CR #518]
////////////////////////////////////////////////////////////////////////////////////////////////////


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Symbol::Symbol(): mIndex(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Symbol::Symbol(uint32_t index): mIndex(index)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t Symbol::GetId() const
{
    return mIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Symbol& Symbol::SetId(uint32_t id)
{
    mIndex = id;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Symbol::IsNull() const
{
    return mIndex == 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Symbol::operator==(const Symbol& symbol) const
{
    return mIndex == symbol.mIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Symbol::operator!=(const Symbol& symbol) const
{
    return mIndex != symbol.mIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Symbol::operator<(const Symbol& symbol) const
{
    return mIndex < symbol.mIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Symbol::operator>(const Symbol& symbol) const
{
    return mIndex > symbol.mIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Symbol::operator<=(const Symbol& symbol) const
{
    return mIndex <= symbol.mIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Symbol::operator>=(const Symbol& symbol) const
{
    return mIndex >= symbol.mIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Symbol::operator uint32_t() const
{
    return mIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Symbol Symbol::Null()
{
    return Symbol(uint32_t(0));
}

}