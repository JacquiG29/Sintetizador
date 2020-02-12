// -----------------------------------------------------------------------------
//    ASPiK Plugin Kernel File:  plugincore.h
//
/**
    \file   plugincore.h
    \author Will Pirkle
    \date   17-September-2018
    \brief  base class interface file for ASPiK plugincore object
    		- http://www.aspikplugins.com
    		- http://www.willpirkle.com
*/
// -----------------------------------------------------------------------------
#ifndef __pluginCore_h__
#define __pluginCore_h__

#include "pluginbase.h"

// **--0x7F1F--**

// --- Plugin Variables controlID Enumeration 

enum controlID {
	volumen = 0,
	waveform = 1,
	Frecuencia_Hz = 2,
	osc1_on = 3,
	wave2 = 4,
	frec2 = 5,
	osc2_on = 6,
	enableMute = 7,
	lfo_1 = 10,
	amount_lfo = 11,
	lfo_selec = 12,
	lfo_frec = 13
};

	// **--0x0F1F--**

/**
\class PluginCore
\ingroup ASPiK-Core
\brief
The PluginCore object is the default PluginBase derived object for ASPiK projects.
Note that you are fre to change the name of this object (as long as you change it in the compiler settings, etc...)


PluginCore Operations:
- overrides the main processing functions from the base class
- performs reset operation on sub-modules
- processes audio
- processes messages for custom views
- performs pre and post processing functions on parameters and audio (if needed)

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
class PluginCore : public PluginBase
{
public:
    PluginCore();

	/** Destructor: empty in default version */
    virtual ~PluginCore(){}

	// --- PluginBase Overrides ---
	//
	/** this is the creation function for all plugin parameters */
	bool initPluginParameters();

	/** called when plugin is loaded, a new audio file is playing or sample rate changes */
	virtual bool reset(ResetInfo& resetInfo);

	/** one-time post creation init function; pluginInfo contains path to this plugin */
	virtual bool initialize(PluginInfo& _pluginInfo);

	// --- preProcess: sync GUI parameters here; override if you don't want to use automatic variable-binding
	virtual bool preProcessAudioBuffers(ProcessBufferInfo& processInfo);

	/** process frames of data */
	virtual bool processAudioFrame(ProcessFrameInfo& processFrameInfo);

	// --- uncomment and override this for buffer processing; see base class implementation for
	//     help on breaking up buffers and getting info from processBufferInfo
	//virtual bool processAudioBuffers(ProcessBufferInfo& processBufferInfo);

	/** preProcess: do any post-buffer processing required; default operation is to send metering data to GUI  */
	virtual bool postProcessAudioBuffers(ProcessBufferInfo& processInfo);

	/** called by host plugin at top of buffer proccess; this alters parameters prior to variable binding operation  */
	virtual bool updatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo);

	/** called by host plugin at top of buffer proccess; this alters parameters prior to variable binding operation  */
	virtual bool updatePluginParameterNormalized(int32_t controlID, double normalizedValue, ParameterUpdateInfo& paramInfo);

	/** this can be called: 1) after bound variable has been updated or 2) after smoothing occurs  */
	virtual bool postUpdatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo);

	/** this is ony called when the user makes a GUI control change */
	virtual bool guiParameterChanged(int32_t controlID, double actualValue);

	/** processMessage: messaging system; currently used for custom/special GUI operations */
	virtual bool processMessage(MessageInfo& messageInfo);

	/** processMIDIEvent: MIDI event processing */
	virtual bool processMIDIEvent(midiEvent& event);

	/** specialized joystick servicing (currently not used) */
	virtual bool setVectorJoystickParameters(const VectorJoystickData& vectorJoysickData);

	/** create the presets */
	bool initPluginPresets();

	// --- BEGIN USER VARIABLES AND FUNCTIONS -------------------------------------- //
	//	   Add your variables and methods here
	float y = 0;
	float fase = 0;
	float lfo_f = 0;

	float y2 = 0;
	float fase2 = 0;

	float salida = 0;
	float prueba;

	double m_n;
	double y_n;
	// --- END USER VARIABLES AND FUNCTIONS -------------------------------------- //

private:
	//  **--0x07FD--**

	// --- Continuous Plugin Variables 
	double volumen = 0.0;
	float Frecuencia_Hz = 0.f;
	float frec2 = 0.f;
	float amount_lfo = 0.f;
	float lfo_frec = 0.f;

	// --- Discrete Plugin Variables 
	int waveform = 0;
	enum class waveformEnum { sine,square,triangle,sawtooth,sawtooth_inverse };	// to compare: if(compareEnumToInt(waveformEnum::sine, waveform)) etc... 

	int osc1_on = 0;
	enum class osc1_onEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(osc1_onEnum::SWITCH_OFF, osc1_on)) etc... 

	int wave2 = 0;
	enum class wave2Enum { sine,square,triangle,sawtooth,sawtooth_inverse };	// to compare: if(compareEnumToInt(wave2Enum::sine, wave2)) etc... 

	int osc2_on = 0;
	enum class osc2_onEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(osc2_onEnum::SWITCH_OFF, osc2_on)) etc... 

	int enableMute = 0;
	enum class enableMuteEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(enableMuteEnum::SWITCH_OFF, enableMute)) etc... 

	int lfo_1 = 0;
	enum class lfo_1Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(lfo_1Enum::SWITCH_OFF, lfo_1)) etc... 

	int lfo_selec = 0;
	enum class lfo_selecEnum { pitch,pmw,volume,cut_off,pan };	// to compare: if(compareEnumToInt(lfo_selecEnum::pitch, lfo_selec)) etc... 

	// **--0x1A7F--**
    // --- end member variables

public:
    /** static description: bundle folder name

	\return bundle folder name as a const char*
	*/
    static const char* getPluginBundleName();

    /** static description: name

	\return name as a const char*
	*/
    static const char* getPluginName();

	/** static description: short name

	\return short name as a const char*
	*/
	static const char* getShortPluginName();

	/** static description: vendor name

	\return vendor name as a const char*
	*/
	static const char* getVendorName();

	/** static description: URL

	\return URL as a const char*
	*/
	static const char* getVendorURL();

	/** static description: email

	\return email address as a const char*
	*/
	static const char* getVendorEmail();

	/** static description: Cocoa View Factory Name

	\return Cocoa View Factory Name as a const char*
	*/
	static const char* getAUCocoaViewFactoryName();

	/** static description: plugin type

	\return type (FX or Synth)
	*/
	static pluginType getPluginType();

	/** static description: VST3 GUID

	\return VST3 GUID as a const char*
	*/
	static const char* getVSTFUID();

	/** static description: 4-char code

	\return 4-char code as int
	*/
	static int32_t getFourCharCode();

	/** initalizer */
	bool initPluginDescriptors();

};




#endif /* defined(__pluginCore_h__) */
