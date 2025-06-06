/////////////////////////////////////////////////////////////////////////////
// Name:        iomusxml.h
// Author:      Laurent Pugin and Klaus Rettinghaus
// Created:     22/09/2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOMUSXML_H__
#define __VRV_IOMUSXML_H__

#include <map>
#include <optional>
#include <queue>
#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "attdef.h"
#include "iobase.h"
#include "metersig.h"
#include "vrvdef.h"

//----------------------------------------------------------------------------

#include "pugixml.hpp"

namespace vrv {

class Arpeg;
class BeamSpan;
class BracketSpan;
class Clef;
class ControlElement;
class Dir;
class Dynam;
class F;
class Fermata;
class Fb;
class Gliss;
class Hairpin;
class Harm;
class InstrDef;
class KeySig;
class Label;
class LabelAbbr;
class Layer;
class LayerElement;
class Measure;
class MeterSigGrp;
class Octave;
class Pedal;
class Section;
class Slur;
class StaffGrp;
class Tempo;
class Tie;
class Trill;

//----------------------------------------------------------------------------
// namespace for local MusicXML classes
//----------------------------------------------------------------------------

#ifndef NO_MUSICXML_SUPPORT

namespace musicxml {

    struct OpenSlur {
        OpenSlur(const std::string &measureNum, short int number, curvature_CURVEDIR curvedir)
        {
            m_measureNum = measureNum;
            m_number = number;
            m_curvedir = curvedir;
        }

        std::string m_measureNum;
        short int m_number;
        curvature_CURVEDIR m_curvedir;
    };

    struct CloseSlur {
        CloseSlur(const std::string &measureNum, short int number, curvature_CURVEDIR curvedir)
        {
            m_measureNum = measureNum;
            m_number = number;
            m_curvedir = curvedir;
        }

        std::string m_measureNum;
        short int m_number;
        curvature_CURVEDIR m_curvedir;
    };

    struct OpenSpanner {
        OpenSpanner(const int &dirN, const int &lastMeasureCount)
        {
            m_dirN = dirN;
            m_lastMeasureCount = lastMeasureCount;
        }

        int m_dirN;
        int m_lastMeasureCount;
    };

    struct OpenArpeggio {
        OpenArpeggio(const int &arpegN, const Fraction &timeStamp)
        {
            m_arpegN = arpegN;
            m_timeStamp = timeStamp;
        }

        int m_arpegN;
        Fraction m_timeStamp;
    };

    struct EndingInfo {
        EndingInfo(const std::string &endingNumber, const std::string &endingType, const std::string &endingText)
        {
            m_endingNumber = endingNumber;
            m_endingType = endingType;
            m_endingText = endingText;
        }

        std::string m_endingNumber;
        std::string m_endingType;
        std::string m_endingText;
    };

    struct ClefChange {
        ClefChange(const std::string &measureNum, Staff *staff, Layer *layer, Clef *clef, const int &scoreOnset,
            bool afterBarline)
        {
            m_measureNum = measureNum;
            m_staff = staff;
            m_layer = layer;
            m_clef = clef;
            m_scoreOnset = scoreOnset;
            m_afterBarline = afterBarline;
        }

        std::string m_measureNum;
        Staff *m_staff;
        Layer *m_layer;
        Clef *m_clef;
        int m_scoreOnset; // the score position of clef change
        bool m_afterBarline = false; // musicXML attribute
    };

    struct OpenDashes {
        OpenDashes(const int dirN, int staffNum, const int measureCount)
        {
            m_dirN = dirN;
            m_staffNum = staffNum;
            m_measureCount = measureCount;
        }

        int m_dirN; // direction number
        int m_staffNum;
        int m_measureCount; // measure number of dashes start
    };

    struct OpenTie {
        OpenTie(Tie *tie, Note *note, int layerNum)
        {
            m_tie = tie;
            m_note = note;
            m_layerNum = layerNum;
        }

        Tie *m_tie = NULL;
        Note *m_note = NULL;
        int m_layerNum = 0;
    };

    struct CloseTie {
        CloseTie(Note *note, int layerNum)
        {
            m_note = note;
            m_layerNum = layerNum;
        }

        Note *m_note = NULL;
        int m_layerNum = 0;
    };

} // namespace musicxml

#endif // NO_MUSICXML_SUPPORT

//----------------------------------------------------------------------------
// MusicXmlInput
//----------------------------------------------------------------------------

class MusicXmlInput : public Input {
public:
    // constructors and destructors
    MusicXmlInput(Doc *doc);
    virtual ~MusicXmlInput();

private:
    /*
     * Objects that were not successfully added and should be destroyed at the end of the import
     */
    ListOfObjects m_garbage;

#ifndef NO_MUSICXML_SUPPORT
public:
    bool Import(const std::string &musicxml) override;

private:
    /*
     * Top level method called from ImportFile or Import
     */
    bool ReadMusicXml(pugi::xml_node root);

    /*
     * Method to fill MEI header with title
     */
    void ReadMusicXmlTitle(pugi::xml_node title);

    /*
     * @name Top level methods for reading MusicXML part and measure elements.
     */
    ///@{
    bool ReadMusicXmlPart(pugi::xml_node node, Section *section, short int nbStaves, const short int staffOffset);
    bool ReadMusicXmlMeasure(pugi::xml_node node, Section *section, Measure *measure, short int nbStaves,
        const short int staffOffset, int index);
    ///@}

    /*
     * Methods for reading the first MusicXML attributes element as MEI staffDef.
     * Returns the number of staves in the part.
     */
    short int ReadMusicXmlPartAttributesAsStaffDef(
        pugi::xml_node node, StaffGrp *staffGrp, const short int staffOffset);

    /*
     * @name Methods for reading the content of a MusicXML measure.
     */
    ///@{
    void ReadMusicXmlAttributes(pugi::xml_node, Section *section, Measure *measure, const std::string &measureNum);
    void ReadMusicXmlBackup(pugi::xml_node, Measure *measure, const std::string &measureNum);
    void ReadMusicXmlBarLine(pugi::xml_node, Measure *measure, const std::string &measureNum);
    void ReadMusicXmlDirection(
        pugi::xml_node, Measure *measure, const std::string &measureNum, const short int staffOffset);
    void ReadMusicXmlFigures(pugi::xml_node, Measure *measure, const std::string &measureNum);
    void ReadMusicXmlForward(pugi::xml_node, Measure *measure, const std::string &measureNum);
    void ReadMusicXmlHarmony(pugi::xml_node, Measure *measure, const std::string &measureNum);
    void ReadMusicXmlNote(
        pugi::xml_node, Measure *measure, const std::string &measureNum, const short int staffOffset, Section *section);
    void ReadMusicXmlPrint(pugi::xml_node, Section *section);
    bool ReadMusicXmlBeamsAndTuplets(const pugi::xml_node &node, Layer *layer, bool isChord);
    void ReadMusicXmlTupletStart(const pugi::xml_node &node, const pugi::xml_node &tupletStart, Layer *layer);
    void ReadMusicXmlBeamStart(const pugi::xml_node &node, const pugi::xml_node &beamStart, Layer *layer);
    void ReadMusicXMLMeterSig(const pugi::xml_node &node, Object *parent);
    void ReadMusicXmlTies(const pugi::xml_node &node, Layer *layer, Note *note, const std::string &measureNum);
    ///@}

    /**
     * Process all clef change queue and add clefs to corresponding places in the score
     */
    void ProcessClefChangeQueue(Section *section);

    /**
     * Add clef changes to all layers of a given measure, staff, and time stamp
     */
    void AddClefs(Measure *measure, const musicxml::ClefChange &clefChange);

    /**
     * Add clef as layer element to specified layer and #sameas clefs to previous layers, if needed
     */
    void InsertClefToLayer(Staff *staff, Layer *layer, Clef *clef, int scoreOnSet);

    /*
     * @name Helper function to insert clef into correct position in layer/other parent based on the insertAfter
     * variable.
     */
    ///@{
    void InsertClefIntoObject(Object *layerElement, Clef *clef, Layer *layer, int scoreOnset, bool insertAfter);
    void InsertClefIntoObject(Object *parent, Clef *clef, Object *relevantChild, bool insertAfter);
    ///@}

    /*
     * Add a Measure to the section.
     * If the measure already exists it will move all its content.
     * The measure can contain only staves. Other elements must be stacked on m_floatingElements.
     */
    void AddMeasure(Section *section, Measure *measure, int i);

    /*
     * Add a Layer element to the layer or to the LayerElement at the top of m_elementStack.
     */
    void AddLayerElement(Layer *layer, LayerElement *element, int duration = 0);

    /*
     * Returns the appropriate layer for a node looking at its MusicXML staff and voice elements.
     */
    Layer *SelectLayer(pugi::xml_node node, Measure *measure);

    /*
     * Returns the appropriate first layer of a staff.
     */
    Layer *SelectLayer(short int staffNb, Measure *measure);

    /*
     * Returns the layer with \@n=layerNb on the staff.
     * Creates the layer if not found.
     */
    Layer *SelectLayer(short int layerNb, Staff *staff);

    /*
     * @name Methods for converting the content of MusicXML attributes.
     */
    ///@{
    Clef *ConvertClef(const pugi::xml_node &clef);
    KeySig *ConvertKey(const pugi::xml_node &key);
    ///@}

    /*
     * Remove the last ClassId element on top of m_elementStack.
     * For example, when closing a beam, we need to remove it from the stack, but it is not
     * necessary the top one (for example we can have an opened chord there).
     */
    void RemoveLastFromStack(ClassId classId, Layer *layer);

    /**
     * Check if element of classId type is present on the specified layer
     */
    bool IsInStack(ClassId classId, Layer *layer);

    /*
     * @name Helper methods for checking presence of values of attributes or elements
     */
    ///@{
    bool HasAttributeWithValue(const pugi::xml_node node, const std::string &attribute, const std::string &value) const;
    bool IsElement(const pugi::xml_node node, const std::string &name) const;
    bool HasContentWithValue(const pugi::xml_node node, const std::string &value) const;
    ///@}

    /*
     * @name Helper method to check whether an ending measure is already present in m_endingStack.
     */
    ///@{
    bool NotInEndingStack(const Measure *measure) const;
    ///@}

    /*
     * @name Helper methods for retrieving attribute values or element content
     */
    ///@{
    std::string GetContent(const pugi::xml_node node) const;
    std::string GetContentOfChild(const pugi::xml_node node, const std::string &child) const;
    ///@}

    /*
     * @name Methods for opening and closing ties and slurs.
     * Opened ties and slurs are stacked together with musicxml::OpenTie
     * and musicxml::OpenSlur objects.
     * Slur starts and ends are matched based on its number.
     */
    ///@{
    void OpenTie(Note *note, Tie *tie, int layerNum);
    void CloseTie(Note *note, int layerNum);
    void OpenSlur(Measure *measure, short int number, Slur *slur, curvature_CURVEDIR dir);
    void CloseSlur(Measure *measure, short int number, LayerElement *element, curvature_CURVEDIR dir);
    void CloseBeamSpan(Staff *staff, Layer *layer, LayerElement *element);
    void MatchTies(bool matchLayers);
    ///@}

    /*
     * @name Helper methods for rendering text elements
     */
    ///@{
    ///@}
    std::string GetWordsOrDynamicsText(const pugi::xml_node node) const;
    void TextRendition(const pugi::xpath_node_set words, ControlElement *element) const;
    std::string StyleLabel(pugi::xml_node display);
    void PrintMetronome(pugi::xml_node metronome, Tempo *tempo);

    /*
     * @name Helper methods for filling in space elements
     */
    ///@{
    ///@}
    void FillSpace(Layer *layer, int dur);

    /*
     * @name Helper method for generating additional IDs
     */
    ///@{
    ///@}
    void GenerateID(pugi::xml_node node);

    /*
     * @name Helper method for meterSigGrp. Separates beat/beat-type into MeterSig and adds them to the MeterSigGrp.
     * Returns total meterCount and meterUnit for the group
     */
    ///@{
    std::pair<std::vector<int>, int> GetMeterSigGrpValues(const pugi::xml_node &node, MeterSigGrp *parent);
    ///@}

    /*
     * @name Helper method for multirests. Returns number of measure hidden by MRest before
     * measure with certain index
     */
    ///@{
    int GetMrestMeasuresCountBeforeIndex(int index) const;
    ///@}

    /*
     * @name Helper method for multirests. Checks whether measure should be hidden as part of MRest
     */
    ///@{
    bool IsMultirestMeasure(int index) const;
    ///@}

    /*
     * @name Helper method for styling fermatas
     */
    ///@{
    void SetFermataExternalSymbols(Fermata *fermata, const std::string &shape);
    void ShapeFermata(Fermata *fermata, pugi::xml_node node);
    ///@}

    /*
     * @name Helper method for getting glyph numbers for ornaments based on approach/depart attributes
     */
    ///@{
    std::string GetOrnamentGlyphNumber(int attributes) const;
    ///@}

    /*
     * @name Helper method for setting \@staff attribute for chords
     */
    ///@{
    void SetChordStaff(Layer *layer);
    ///@}

    /*
     * @name Helper method for comparing written/gestural accidental attributes
     */
    ///@{
    static bool IsSameAccidWrittenGestural(data_ACCIDENTAL_WRITTEN written, data_ACCIDENTAL_GESTURAL gestural);
    ///@}

    /*
     * @name Helper for detecting the slur curve direction
     */
    static curvature_CURVEDIR CombineCurvedir(curvature_CURVEDIR startDir, curvature_CURVEDIR stopDir);

    /*
     * @name Methods for converting MusicXML values to MEI attributes.
     */
    ///@{
    static data_ACCIDENTAL_WRITTEN ConvertAccidentalToAccid(const std::string &value);
    static data_ACCIDENTAL_GESTURAL ConvertAlterToAccid(const float value);
    static data_ARTICULATION ConvertArticulations(const std::string &value);
    static data_BARRENDITION ConvertStyleToRend(const std::string &value, const bool repeat);
    static data_BOOLEAN ConvertWordToBool(const std::string &value);
    static data_DURATION ConvertTypeToDur(const std::string &value);
    static data_HEADSHAPE_list ConvertNotehead(const std::string &value);
    static data_LINESTARTENDSYMBOL ConvertLineEndSymbol(const std::string &value);
    static data_MIDIVALUE ConvertDynamicsToMidiVal(const float dynamics);
    static data_PITCHNAME ConvertStepToPitchName(const std::string &value);
    static data_TEXTRENDITION ConvertEnclosure(const std::string &value);
    static beamRend_FORM ConvertBeamFanToForm(const std::string &value);
    static curvature_CURVEDIR InferCurvedir(const pugi::xml_node slurOrTie);
    static fermataVis_SHAPE ConvertFermataShape(const std::string &value);
    static pedalLog_DIR ConvertPedalTypeToDir(const std::string &value);
    static tupletVis_NUMFORMAT ConvertTupletNumberValue(const std::string &value);
    static std::u32string ConvertTypeToVerovioText(const std::string &value);
    static std::string ConvertAlterToSymbol(const std::string &value, bool plusMinus = false);
    static std::string ConvertKindToSymbol(const std::string &value);
    static std::string ConvertKindToText(const std::string &value);
    static std::string ConvertDegreeToText(const pugi::xml_node harmony);
    static std::string ConvertFigureGlyph(const std::string &value);
    ///@}

    /*
     * @name Methods for converting between MusicXML <pitch> and MIDI note numbers.
     */
    ///@{
    static int PitchToMidi(const std::string &step, int alter, int octave);
    static void MidiToPitch(int midi, std::string &step, int &alter, int &octave);
    ///@}

public:
    //
private:
    /* octave offset */
    std::vector<int> m_octDis;
    /* measure repeats */
    bool m_mRpt = false;
    /* measure repeats */
    bool m_slash = false;
    /* MIDI ticks */
    int m_ppq = -1;
    /* measure time */
    int m_durTotal = 0;
    /* measure time */
    int m_durFb = 0;
    /* meter signature */
    std::vector<int> m_meterCount = { 4 };
    int m_meterUnit = 4;
    MeterCountSign m_meterSign = MeterCountSign::None;
    /* part information */
    Label *m_label = NULL;
    LabelAbbr *m_labelAbbr = NULL;
    InstrDef *m_instrdef = NULL;
    /* LastElementID */
    std::string m_ID;
    /* A map of stacks for piling open LayerElements (beams, tuplets, chords, btrem, ftrem) separately per layer */
    std::map<Layer *, std::vector<LayerElement *>> m_elementStackMap;
    /* A maps of time stamps (score time) to indicate write pointer of a given layer */
    std::map<Layer *, int> m_layerEndTimes;
    std::map<Layer *, std::multimap<int, LayerElement *>> m_layerTimes;
    /* To remember layer of last element (note) to handle chords */
    Layer *m_prevLayer = NULL;
    /* To remember current layer to properly handle layers/staves/cross-staff elements */
    Layer *m_currentLayer = NULL;
    bool m_isLayerInitialized = false;
    /* The stack for open slurs */
    std::vector<std::pair<Slur *, musicxml::OpenSlur>> m_slurStack;
    /* The stack for slur stops that might come before the slur has been opened */
    std::vector<std::pair<LayerElement *, musicxml::CloseSlur>> m_slurStopStack;
    /* The stack for open ties */
    std::vector<musicxml::OpenTie> m_tieStack;
    /* The stack for tie stops that might come before that tie was opened */
    std::vector<musicxml::CloseTie> m_tieStopStack;
    /* The stack for hairpins */
    std::vector<std::pair<Hairpin *, musicxml::OpenSpanner>> m_hairpinStack;
    /* The stack for hairpin stops that might occur before a hairpin was started staffNumber, tStamp2, (hairpinNumber,
     * measureCount) */
    std::vector<std::tuple<int, double, musicxml::OpenSpanner>> m_hairpinStopStack;
    /* The stack for the beamspans with numbers of staff/layer where starting element is located*/
    std::vector<std::pair<BeamSpan *, std::pair<int, int>>> m_beamspanStack;
    std::vector<std::pair<BracketSpan *, musicxml::OpenSpanner>> m_bracketStack;
    std::vector<std::pair<Trill *, musicxml::OpenSpanner>> m_trillStack;
    /* Current ending info for start/stop */
    std::optional<musicxml::EndingInfo> m_currentEndingStart;
    std::optional<musicxml::EndingInfo> m_currentEndingStop;
    /* The stack of endings to be inserted at the end of XML import */
    std::vector<std::pair<std::vector<Measure *>, musicxml::EndingInfo>> m_endingStack;
    /* The stack of open dashes (direction-type) containing *ControlElement, OpenDashes */
    std::vector<std::pair<ControlElement *, musicxml::OpenDashes>> m_openDashesStack;
    /* The stacks for ControlElements */
    std::vector<Dir *> m_dirStack;
    std::vector<Dynam *> m_dynamStack;
    std::vector<Gliss *> m_glissStack;
    std::vector<Harm *> m_harmStack;
    std::vector<Octave *> m_octaveStack;
    std::vector<Pedal *> m_pedalStack;
    std::vector<Tempo *> m_tempoStack;
    /*
     * The stack of floating elements (tie, slur, etc.) to be added at the
     * end of each measure
     */
    std::vector<std::pair<std::string, ControlElement *>> m_controlElements;
    /* stack of clef changes to be inserted to all layers of a given staff */
    std::queue<musicxml::ClefChange> m_clefChangeQueue;
    /* stack of new arpeggios that get more notes added. */
    std::vector<std::pair<Arpeg *, musicxml::OpenArpeggio>> m_ArpeggioStack;
    /* a map for the measure counts storing the index of each measure created */
    std::map<Measure *, int> m_measureCounts;
    /* measure rests */
    std::map<int, int> m_multiRests;

#endif // NO_MUSICXML_SUPPORT
};

} // namespace vrv

#endif // __VRV_IOMUSXML_H__
