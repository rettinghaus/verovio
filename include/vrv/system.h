/////////////////////////////////////////////////////////////////////////////
// Name:        system.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SYSTEM_H__
#define __VRV_SYSTEM_H__

#include "aligner.h"
#include "drawinginterface.h"
#include "editorial.h"
#include "object.h"

namespace vrv {

class BoundaryEnd;
class DeviceContext;
class Ending;
class Measure;
class ScoreDef;

//----------------------------------------------------------------------------
// System
//----------------------------------------------------------------------------

/**
 * This class represents a system in a laid-out score (Doc).
 * A System is contained in a Page.
 * It contains Staff objects.
*/
class System : public Object, public DrawingListInterface {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    System();
    virtual ~System();
    virtual void Reset();
    virtual std::string GetClassName() const { return "System"; }
    virtual ClassId Is() const { return SYSTEM; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    virtual void AddChild(Object *object);
    ///@}

    /**
     * @name Set and get the labels drawing width (normal and abbreviated)
     */
    ///@{
    int GetDrawingLabelsWidth() const { return m_drawingLabelsWidth; }
    void SetDrawingLabelsWidth(int width);
    int GetDrawingAbbrLabelsWidth() const { return m_drawingAbbrLabelsWidth; }
    void SetDrawingAbbrLabelsWidth(int width);
    ///@}

    /**
     * Return the height of the system
     */
    int GetHeight() const;

    /**
     * Return the default horizontal spacing of system.
     */
    int GetVerticalSpacing() const;

    /**
     * Return the index position of the system in its page parent
     */
    int GetSystemIdx() const { return Object::GetIdx(); }

    void SetCurrentFloatingPositioner(int staffN, FloatingObject *object, int x, int y);

    /**
     * @name Setter and getter of the drawing scoreDef
     */
    ///@{
    ScoreDef *GetDrawingScoreDef() const { return m_drawingScoreDef; }
    void SetDrawingScoreDef(ScoreDef *drawingScoreDef);
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::UnsetCurrentScoreDef
     */
    virtual int UnsetCurrentScoreDef(FunctorParams *functorParams);

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

    /**
     * See Object::ResetVerticalAlignment
     */
    virtual int ResetVerticalAlignment(FunctorParams *functorParams);

    /**
     * See Object::AlignHorizontally
     */
    virtual int AlignHorizontally(FunctorParams *functorParams);

    /**
     * See Object::AlignVertically
     */
    virtual int AlignVertically(FunctorParams *functorParams);

    /**
     * See Object::SetAligmentYPos
     */
    virtual int SetAligmentYPos(FunctorParams *functorParams);

    /**
     * See Object::IntegrateBoundingBoxYShift
     */
    virtual int IntegrateBoundingBoxYShift(FunctorParams *functorParams);

    /**
     * See Object::AlignSystems
     */
    virtual int AlignSystems(FunctorParams *functorParams);

    /**
     * See Object::AlignMeasures
     */
    virtual int AlignMeasures(FunctorParams *functorParams);
    virtual int AlignMeasuresEnd(FunctorParams *functorParams);

    /**
     * See Object::JustifyX
     */
    virtual int JustifyX(FunctorParams *functorParams);

    /**
     * See Object::CalcStaffOverlap
     */
    virtual int CalcStaffOverlap(FunctorParams *functorParams);

    /**
     * See Object::AdjustFloatingPostioners
     */
    virtual int AdjustFloatingPostioners(FunctorParams *functorParams);

    /**
     * See Object::CastOffPages
     */
    virtual int CastOffPages(FunctorParams *functorParams);

    /**
     * See Object::UnCastOff
     */
    virtual int UnCastOff(FunctorParams *functorParams);

    /**
     * See Object::SetDrawingXY
     */
    virtual int SetDrawingXY(FunctorParams *functorParams);

    /**
     * See Object::CastOffSystemsEnd
     */
    virtual int CastOffSystemsEnd(FunctorParams *functorParams);

public:
    SystemAligner m_systemAligner;

public:
    /** System left margin (MEI scoredef@system.leftmar). Saved if != 0 */
    int m_systemLeftMar;
    /** System right margin (MEI scoredef@system.rightmar). Saved if != 0 */
    int m_systemRightMar;
    /**
     * The Y absolute position of the staff for facsimile (transcription) encodings.
     * This is the top left corner of the system.
     */
    int m_yAbs;
    /**
     * The Y relative position of the system.
     * It is used internally when calculating the layout and it is not stored in the file.
     */
    int m_drawingYRel;
    /**
     * The x absolute position of the  system for facsimile layouts.
     * This is the top left corner of the system.
     */
    int m_xAbs;
    /**
     * The X relative position of the system.
     * It is used internally when calculating the layout and it is not stored in the file.
     */
    int m_drawingXRel;
    /**
     * The width used by the labels at the left of the system.
     * It is used internally when calculating the layout and it is not stored in the file.
     */
    int m_drawingLabelsWidth;
    /** The width used by the abbreviated labels */
    int m_drawingAbbrLabelsWidth;
    /**
     * @name The total width of the system.
     * It is computed during the layout processed and used for calculating the justification ratio.
     */
    ///@{
    int m_drawingTotalWidth;
    int m_drawingJustifiableWidth;
    ///@}

private:
    ScoreDef *m_drawingScoreDef;
};

} // namespace vrv

#endif
