/////////////////////////////////////////////////////////////////////////////
// Name:        tempo.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tempo.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "editorial.h"
#include "functorparams.h"
#include "horizontalaligner.h"
#include "layerelement.h"
#include "measure.h"
#include "staff.h"
#include "system.h"
#include "text.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tempo
//----------------------------------------------------------------------------

static const ClassRegistrar<Tempo> s_factory("tempo", TEMPO);

Tempo::Tempo()
    : ControlElement(TEMPO, "tempo-")
    , TextDirInterface()
    , TimeSpanningInterface()
    , AttExtender()
    , AttLang()
    , AttMidiTempo()
    , AttMmTempo()
{
    this->RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_EXTENDER);
    this->RegisterAttClass(ATT_LANG);
    this->RegisterAttClass(ATT_MIDITEMPO);
    this->RegisterAttClass(ATT_MMTEMPO);

    this->Reset();
}

Tempo::~Tempo() {}

void Tempo::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimeSpanningInterface::Reset();
    this->ResetExtender();
    this->ResetLang();
    this->ResetMidiTempo();
    this->ResetMmTempo();
}

bool Tempo::IsSupportedChild(Object *child)
{
    if (child->Is({ LB, REND, SYMBOL, TEXT })) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

int Tempo::GetDrawingXRelativeToStaff(int staffN) const
{
    int m_relativeX = 0;
    if (m_drawingXRels.find(staffN) != m_drawingXRels.end()) {
        m_relativeX = m_drawingXRels.at(staffN);
    }

    return this->GetStart()->GetDrawingX() + m_relativeX;
}

//----------------------------------------------------------------------------
// Tempo functor methods
//----------------------------------------------------------------------------

int Tempo::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = dynamic_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    double tempoTime = GetStart()->GetAlignment()->GetTime() * DURATION_4 / DUR_MAX;
    double starttime = params->m_totalTime + tempoTime;
    int tpq = params->m_midiFile->getTPQ();

    if (this->HasMidiBpm()) {
        params->m_midiFile->addTempo(0, (starttime * tpq), this->GetMidiBpm());
    }
    else if (this->HasMm()) {
        int mm = this->GetMm();
        int mmUnit = 4;
        if (this->HasMmUnit() && (this->GetMmUnit() > DURATION_breve)) {
            mmUnit = pow(2, (int)this->GetMmUnit() - 2);
        }
        if (this->HasMmDots()) {
            mmUnit = 2 * mmUnit - (mmUnit / pow(2, this->GetMmDots()));
        }
        params->m_midiFile->addTempo(0, (starttime * tpq), int(mm * 4.0 / mmUnit + 0.5));
    }

    return FUNCTOR_CONTINUE;
}

int Tempo::AdjustTempo(FunctorParams *functorParams)
{
    AdjustTempoParams *params = vrv_params_cast<AdjustTempoParams *>(functorParams);
    assert(params);

    // Get all the positioners for this object - all of them (all staves) because we can have different staff sizes
    ArrayOfFloatingPositioners positioners;
    params->m_systemAligner->FindAllPositionerPointingTo(&positioners, this);

    if (positioners.empty()) {
        return FUNCTOR_SIBLINGS;
    }

    Measure *measure = vrv_cast<Measure *>(this->GetFirstAncestor(MEASURE));
    MeasureAlignerTypeComparison alignmentComparison(ALIGNMENT_SCOREDEF_METERSIG);
    Alignment *pos
        = dynamic_cast<Alignment *>(measure->m_measureAligner.FindDescendantByComparison(&alignmentComparison, 1));

    for (auto positioner : positioners) {
        int left, right;
        int start = this->GetStart()->GetDrawingX();
        const int staffN = positioner->GetAlignment()->GetStaff()->GetN();
        if (!this->HasStartid() && (this->GetTstamp() <= 1) && pos) {
            left = measure->GetDrawingX() + pos->GetXRel();
        }
        else {
            Alignment *align = this->GetStart()->GetAlignment();
            align->GetLeftRight(staffN, left, right);
        }

        if (std::abs(left) != std::abs(VRV_UNSET)) {
            m_drawingXRels[staffN] = left - start;
        }
    }

    return FUNCTOR_CONTINUE;
}

int Tempo::ResetData(FunctorParams *functorParams)
{
    // Call parent one too
    ControlElement::ResetData(functorParams);

    m_drawingXRels.clear();

    return FUNCTOR_CONTINUE;
}

int Tempo::InitMaxMeasureDuration(FunctorParams *functorParams)
{
    InitMaxMeasureDurationParams *params = vrv_params_cast<InitMaxMeasureDurationParams *>(functorParams);
    assert(params);

    if (this->HasMidiBpm()) {
        params->m_currentTempo = this->GetMidiBpm();
    }
    else if (this->HasMm()) {
        params->m_currentTempo = Tempo::CalcTempo(this);
    }

    return FUNCTOR_CONTINUE;
}

double Tempo::CalcTempo(const AttMmTempo *attMmTempo)
{
    double tempo = MIDI_TEMPO;

    double mm = attMmTempo->GetMm();
    double mmUnit = 4;

    if (attMmTempo->HasMmUnit() && (attMmTempo->GetMmUnit() > DURATION_breve)) {
        mmUnit = pow(2, (int)attMmTempo->GetMmUnit() - 2);
    }
    if (attMmTempo->HasMmDots()) {
        double dotsUnit = 0.0;
        for (int d = 0; d < attMmTempo->GetMmDots(); d++) {
            dotsUnit += mmUnit / 4.0 / pow(2, d);
        }
        mmUnit -= dotsUnit;
    }
    if (mmUnit > 0) tempo = mm * 4.0 / mmUnit;

    return tempo;
}

} // namespace vrv
