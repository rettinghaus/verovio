/////////////////////////////////////////////////////////////////////////////
// Name:        adjustyposfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustyposfunctor.h"

//----------------------------------------------------------------------------

#include "beamspan.h"
#include "calcalignmentpitchposfunctor.h"
#include "calcstemfunctor.h"
#include "system.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustCrossStaffYPosFunctor
//----------------------------------------------------------------------------

AdjustCrossStaffYPosFunctor::AdjustCrossStaffYPosFunctor(Doc *doc) : DocFunctor(doc) {}

FunctorCode AdjustCrossStaffYPosFunctor::VisitChord(Chord *chord)
{
    if (!chord->HasCrossStaff()) return FUNCTOR_SIBLINGS;

    // For cross staff chords we need to re-calculate the stem because the staff position might have changed
    CalcAlignmentPitchPosFunctor calcAlignmentPitchPos(m_doc);
    chord->Process(calcAlignmentPitchPos);

    CalcStemFunctor calcStem(m_doc);
    chord->Process(calcStem);

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustCrossStaffYPosFunctor::VisitSystem(System *system)
{
    ArrayOfObjects *drawingList = system->GetDrawingList();
    for (auto &item : *drawingList) {
        if (item->Is(BEAMSPAN)) {
            // Here we could check that the beamSpan is actually cross-staff. Otherwise doing this is pointless
            BeamSpan *beamSpan = vrv_cast<BeamSpan *>(item);
            assert(beamSpan);
            BeamSpanSegment *segment = beamSpan->GetSegmentForSystem(system);
            if (segment) {
                segment->CalcBeam(segment->GetLayer(), segment->GetStaff(), m_doc, beamSpan, beamSpan->m_drawingPlace);
            }
        }
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
