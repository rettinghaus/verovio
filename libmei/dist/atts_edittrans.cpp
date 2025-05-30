/////////////////////////////////////////////////////////////////////////////
// Authors:     Laurent Pugin and Rodolfo Zitellini
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
//
// Code generated using a modified version of libmei
// by Andrew Hankinson, Alastair Porter, and Others
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// NOTE: this file was generated with the Verovio libmei version and
// should not be edited because changes will be lost.
/////////////////////////////////////////////////////////////////////////////

#include "atts_edittrans.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AttAgentIdent
//----------------------------------------------------------------------------

AttAgentIdent::AttAgentIdent() : Att()
{
    this->ResetAgentIdent();
}

void AttAgentIdent::ResetAgentIdent()
{
    m_agent = "";
}

bool AttAgentIdent::ReadAgentIdent(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("agent")) {
        this->SetAgent(StrToStr(element.attribute("agent").value()));
        if (removeAttr) element.remove_attribute("agent");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAgentIdent::WriteAgentIdent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAgent()) {
        element.append_attribute("agent") = StrToStr(this->GetAgent()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAgentIdent::HasAgent() const
{
    return (m_agent != "");
}

//----------------------------------------------------------------------------
// AttReasonIdent
//----------------------------------------------------------------------------

AttReasonIdent::AttReasonIdent() : Att()
{
    this->ResetReasonIdent();
}

void AttReasonIdent::ResetReasonIdent()
{
    m_reason = "";
}

bool AttReasonIdent::ReadReasonIdent(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("reason")) {
        this->SetReason(StrToStr(element.attribute("reason").value()));
        if (removeAttr) element.remove_attribute("reason");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttReasonIdent::WriteReasonIdent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasReason()) {
        element.append_attribute("reason") = StrToStr(this->GetReason()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttReasonIdent::HasReason() const
{
    return (m_reason != "");
}

} // namespace vrv
