/////////////////////////////////////////////////////////////////////////////
// Name:        resetfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_RESETFUNCTOR_H__
#define __VRV_RESETFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// ResetDataFunctor
//----------------------------------------------------------------------------

/**
 * This class resets the drawing values before calling PrepareData after changes.
 */
class ResetDataFunctor : public MutableFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ResetDataFunctor();
    virtual ~ResetDataFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitAccid(Accid *accid) override;
    FunctorCode VisitArpeg(Arpeg *arpeg) override;
    FunctorCode VisitArtic(Artic *artic) override;
    FunctorCode VisitBeam(Beam *beam) override;
    FunctorCode VisitChord(Chord *chord) override;
    FunctorCode VisitControlElement(ControlElement *controlElement) override;
    FunctorCode VisitCustos(Custos *custos) override;
    FunctorCode VisitDot(Dot *dot) override;
    FunctorCode VisitDots(Dots *dots) override;
    FunctorCode VisitEditorialElement(EditorialElement *editorialElement) override;
    FunctorCode VisitEnding(Ending *ending) override;
    FunctorCode VisitF(F *f) override;
    FunctorCode VisitFlag(Flag *flag) override;
    FunctorCode VisitFloatingObject(FloatingObject *floatingObject) override;
    FunctorCode VisitFTrem(FTrem *fTrem) override;
    FunctorCode VisitHairpin(Hairpin *hairpin) override;
    FunctorCode VisitLayer(Layer *layer) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitLigature(Ligature *ligature) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitMRest(MRest *mRest) override;
    FunctorCode VisitNote(Note *note) override;
    FunctorCode VisitRest(Rest *rest) override;
    FunctorCode VisitSection(Section *section) override;
    FunctorCode VisitSlur(Slur *slur) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitStem(Stem *stem) override;
    FunctorCode VisitSyl(Syl *syl) override;
    FunctorCode VisitSystemMilestone(SystemMilestoneEnd *systemMilestoneEnd) override;
    FunctorCode VisitTempo(Tempo *tempo) override;
    FunctorCode VisitTuplet(Tuplet *tuplet) override;
    FunctorCode VisitTurn(Turn *turn) override;
    FunctorCode VisitVerse(Verse *verse) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif // __VRV_RESETFUNCTOR_H__
