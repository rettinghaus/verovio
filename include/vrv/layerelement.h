/////////////////////////////////////////////////////////////////////////////
// Name:        layerelement.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LAYER_ELEMENT_H__
#define __VRV_LAYER_ELEMENT_H__

#include "object.h"

namespace vrv {

class Alignment;
class Beam;
class BeamElementCoord;
class Layer;
class Mensur;
class MeterSig;
class Staff;

//----------------------------------------------------------------------------
// LayerElement
//----------------------------------------------------------------------------

/**
 * This class is a base class for the Layer (<layer>) content.
 * It is not an abstract class but should not be instantiated directly.
 */
class LayerElement : public Object {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    LayerElement();
    LayerElement(std::string classid);
    virtual ~LayerElement();
    virtual void Reset();
    virtual ClassId Is() const { return LAYER_ELEMENT; }
    ///@}

    /**
     * Copy assignment for resetting pointers
     */
    LayerElement &operator=(const LayerElement &element);

    /**
     * Adjust the pname and the octave for values outside the range
     */
    static void AdjustPname(int *pname, int *oct);

    /**
     * @name Set and get the flag for indication whether it is a ScoreDef or StaffDef attribute.
     * The value is false by default. It is set to true by ScoreDef and StaffDef and used when
     * drawing the element.
     * NB In the scoreDef or staffDef itself, it can be either an attribute or an element.
     */
    ///@{
    ElementScoreDefRole GetScoreDefRole() const { return m_scoreDefRole; }
    void SetScoreDefRole(ElementScoreDefRole scoreDefRole) { m_scoreDefRole = scoreDefRole; }
    ///@}

    /**
     * @name Child type checkers.
     */
    ///@{
    /** Return true if the element is a grace note */
    bool IsGraceNote() const;
    /** Return true if the element is a note or a note child and the note has a @grace */
    bool IsCueSize();
    /** Return true if the element is a note within a ligature */
    bool IsInLigature();
    /** Return true if the element is a note or a chord within a fTrem */
    bool IsInFTrem();
    /** Return true if the element has to be aligned horizontally */
    virtual bool HasToBeAligned() const { return false; }
    /**
     * Return the beam parent if in beam
     * Look if the note or rest is in a beam.
     * Look for the first beam parent and check if the note is in its content list.
     * Looking in the content list is necessary for grace notes or imbricated beams.
     */
    Beam *IsInBeam();
    ///@}

    /**
     * Returns the drawing top and bottom taking into accound stem, etc.
     * We pass the doc as parameter in order to have access to the current drawing parameters.
     */
    int GetDrawingTop(Doc *doc, int staffSize);
    int GetDrawingBottom(Doc *doc, int staffSize);

    /**
     * Alignment getter
     */
    Alignment *GetAlignment() const { return m_alignment; }

    int GetXRel() const;

    /**
     * Returns the duration if the child element has a DurationInterface
     */
    double GetAlignmentDuration(Mensur *mensur = NULL, MeterSig *meterSig = NULL, bool notGraceOnly = true);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetVerticalAlignment(FunctorParams *functorParams);

    /**
     * See Object::AlignHorizontally
     */
    virtual int AlignHorizontally(FunctorParams *functorParams);

    /**
     * See Object::PrepareTimeSpanning
     */
    virtual int PrepareTimeSpanning(FunctorParams *functorParams);

    /**
     * See Object::SetDrawingXY
     */
    virtual int SetDrawingXY(FunctorParams *functorParams);

    /**
     * See Object::FindTimeSpanningLayerElements
     */
    virtual int FindTimeSpanningLayerElements(FunctorParams *functorParams);

    /**
     * See Object::GenerateMIDI
     */
    virtual int GenerateMIDI(FunctorParams *functorParams);
    virtual int GenerateMIDIEnd(FunctorParams *functorParams);

    /**
     * See Object::CalcMaxMeasureDuration
     */
    virtual int CalcMaxMeasureDuration(FunctorParams *functorParams);

private:
    //
public:
    /** Absolute position X. This is used for facsimile (transcription) encoding */
    int m_xAbs;
    /**
     * This stores a pointer to the corresponding BeamElementCoord(currentDur > DUR_4)
     */
    BeamElementCoord *m_beamElementCoord;
    /**
     * This stores a pointer to the cross-staff (if any) and the appropriate layer
     * Initialized in LayerElement::SetDrawingXY
     */
    Staff *m_crossStaff;
    Layer *m_crossLayer;

protected:
    Alignment *m_alignment;

private:
    /** Indicates whether it is a ScoreDef or StaffDef attribute */
    ElementScoreDefRole m_scoreDefRole;
};

} // namespace vrv

#endif
