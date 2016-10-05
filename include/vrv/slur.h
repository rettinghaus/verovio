/////////////////////////////////////////////////////////////////////////////
// Name:        slur.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SLUR_H__
#define __VRV_SLUR_H__

#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

class Slur : public ControlElement, public TimeSpanningInterface, public AttColor, public AttCurvature {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Slur();
    virtual ~Slur();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Slur"; }
    virtual ClassId Is() const { return SLUR; }
    ///@}

    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
