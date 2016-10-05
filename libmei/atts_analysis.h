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

#ifndef __VRV_ATTS_ANALYSIS_H__
#define __VRV_ATTS_ANALYSIS_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttCommonAnl
//----------------------------------------------------------------------------

class AttCommonAnl : public Att {
public:
    AttCommonAnl();
    virtual ~AttCommonAnl();

    /** Reset the default values for the attribute class **/
    void ResetCommonAnl();

    /** Read the values for the attribute class **/
    bool ReadCommonAnl(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteCommonAnl(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetCopyof(std::string copyof_) { m_copyof = copyof_; }
    std::string GetCopyof() const { return m_copyof; }
    bool HasCopyof() const;
    //
    void SetCorresp(std::string corresp_) { m_corresp = corresp_; }
    std::string GetCorresp() const { return m_corresp; }
    bool HasCorresp() const;
    //
    void SetNext(std::string next_) { m_next = next_; }
    std::string GetNext() const { return m_next; }
    bool HasNext() const;
    //
    void SetPrev(std::string prev_) { m_prev = prev_; }
    std::string GetPrev() const { return m_prev; }
    bool HasPrev() const;
    //
    void SetSameas(std::string sameas_) { m_sameas = sameas_; }
    std::string GetSameas() const { return m_sameas; }
    bool HasSameas() const;
    //
    void SetSynch(std::string synch_) { m_synch = synch_; }
    std::string GetSynch() const { return m_synch; }
    bool HasSynch() const;
    ///@}

private:
    /** Points to an element of which the current element is a copy. **/
    std::string m_copyof;
    /**
     * Used to point to other elements that correspond to this one in a generic
     * fashion.
     **/
    std::string m_corresp;
    /** Used to point to the next event(s) in a user-defined collection. **/
    std::string m_next;
    /** Points to the previous event(s) in a user-defined collection. **/
    std::string m_prev;
    /**
     * Points to an element that is the same as the current element but is not a
     * literal copy of the current element.
     **/
    std::string m_sameas;
    /** Points to elements that are synchronous with the current element. **/
    std::string m_synch;

    /* include <attsynch> */
};

//----------------------------------------------------------------------------
// AttHarmonicfunction
//----------------------------------------------------------------------------

class AttHarmonicfunction : public Att {
public:
    AttHarmonicfunction();
    virtual ~AttHarmonicfunction();

    /** Reset the default values for the attribute class **/
    void ResetHarmonicfunction();

    /** Read the values for the attribute class **/
    bool ReadHarmonicfunction(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteHarmonicfunction(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDeg(data_SCALEDEGREE deg_) { m_deg = deg_; }
    data_SCALEDEGREE GetDeg() const { return m_deg; }
    bool HasDeg() const;
    ///@}

private:
    /**
     * Captures relative scale degree information using Humdrum **deg syntax -- an
     * optional indicator of melodic approach (^ = ascending approach, v = descending
     * approach), a scale degree value (1 = tonic ...
     * 7 = leading tone), and an optional indication of chromatic alteration. The
     * amount of chromatic alternation is not indicated.
     **/
    data_SCALEDEGREE m_deg;

    /* include <attdeg> */
};

//----------------------------------------------------------------------------
// AttIntervalharmonic
//----------------------------------------------------------------------------

class AttIntervalharmonic : public Att {
public:
    AttIntervalharmonic();
    virtual ~AttIntervalharmonic();

    /** Reset the default values for the attribute class **/
    void ResetIntervalharmonic();

    /** Read the values for the attribute class **/
    bool ReadIntervalharmonic(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteIntervalharmonic(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetInth(data_INTERVAL_HARMONIC inth_) { m_inth = inth_; }
    data_INTERVAL_HARMONIC GetInth() const { return m_inth; }
    bool HasInth() const;
    ///@}

private:
    /** Encodes the harmonic interval between pitches occurring at the same time. **/
    data_INTERVAL_HARMONIC m_inth;

    /* include <attinth> */
};

//----------------------------------------------------------------------------
// AttIntervalmelodic
//----------------------------------------------------------------------------

class AttIntervalmelodic : public Att {
public:
    AttIntervalmelodic();
    virtual ~AttIntervalmelodic();

    /** Reset the default values for the attribute class **/
    void ResetIntervalmelodic();

    /** Read the values for the attribute class **/
    bool ReadIntervalmelodic(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteIntervalmelodic(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetIntm(data_INTERVAL_MELODIC intm_) { m_intm = intm_; }
    data_INTERVAL_MELODIC GetIntm() const { return m_intm; }
    bool HasIntm() const;
    ///@}

private:
    /**
     * Encodes the melodic interval from the previous pitch.
     * The value may be a general directional indication (u, d, s), an indication of
     * diatonic interval direction, quality, and size, or a precise numeric value in
     * half steps.
     **/
    data_INTERVAL_MELODIC m_intm;

    /* include <attintm> */
};

//----------------------------------------------------------------------------
// AttMelodicfunction
//----------------------------------------------------------------------------

class AttMelodicfunction : public Att {
public:
    AttMelodicfunction();
    virtual ~AttMelodicfunction();

    /** Reset the default values for the attribute class **/
    void ResetMelodicfunction();

    /** Read the values for the attribute class **/
    bool ReadMelodicfunction(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMelodicfunction(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMfunc(data_MELODICFUNCTION mfunc_) { m_mfunc = mfunc_; }
    data_MELODICFUNCTION GetMfunc() const { return m_mfunc; }
    bool HasMfunc() const;
    ///@}

private:
    /** Describes melodic function using Humdrum **embel syntax. **/
    data_MELODICFUNCTION m_mfunc;

    /* include <attmfunc> */
};

//----------------------------------------------------------------------------
// AttPitchclass
//----------------------------------------------------------------------------

class AttPitchclass : public Att {
public:
    AttPitchclass();
    virtual ~AttPitchclass();

    /** Reset the default values for the attribute class **/
    void ResetPitchclass();

    /** Read the values for the attribute class **/
    bool ReadPitchclass(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WritePitchclass(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPclass(data_PITCHCLASS pclass_) { m_pclass = pclass_; }
    data_PITCHCLASS GetPclass() const { return m_pclass; }
    bool HasPclass() const;
    ///@}

private:
    /** Holds pitch class information. **/
    data_PITCHCLASS m_pclass;

    /* include <attpclass> */
};

//----------------------------------------------------------------------------
// AttSolfa
//----------------------------------------------------------------------------

class AttSolfa : public Att {
public:
    AttSolfa();
    virtual ~AttSolfa();

    /** Reset the default values for the attribute class **/
    void ResetSolfa();

    /** Read the values for the attribute class **/
    bool ReadSolfa(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSolfa(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPsolfa(std::string psolfa_) { m_psolfa = psolfa_; }
    std::string GetPsolfa() const { return m_psolfa; }
    bool HasPsolfa() const;
    ///@}

private:
    /**
     * Contains sol-fa designation, e.g., do, re, mi, etc., in either a fixed or
     * movable Do system.
     **/
    std::string m_psolfa;

    /* include <attpsolfa> */
};

} // vrv namespace

#endif // __VRV_ATTS_ANALYSIS_H__
