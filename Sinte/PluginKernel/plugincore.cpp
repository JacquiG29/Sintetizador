// -----------------------------------------------------------------------------
//    ASPiK Plugin Kernel File:  plugincore.cpp
//
/**
    \file   plugincore.cpp
    \author Will Pirkle
    \date   17-September-2018
    \brief  Implementation file for PluginCore object
    		- http://www.aspikplugins.com
    		- http://www.willpirkle.com
*/
// -----------------------------------------------------------------------------
#include "plugincore.h"
#include "plugindescription.h"
#include "oscilador.h"
#include <string>
#include <iostream>
#include "EnvelopeGenerator.h"


/**
\brief PluginCore constructor is launching pad for object initialization

Operations:
- initialize the plugin description (strings, codes, numbers, see initPluginDescriptors())
- setup the plugin's audio I/O channel support
- create the PluginParameter objects that represent the plugin parameters (see FX book if needed)
- create the presets
*/
PluginCore::PluginCore()
{
    // --- describe the plugin; call the helper to init the static parts you setup in plugindescription.h
    initPluginDescriptors();

    // --- default I/O combinations
	// --- for FX plugins
	if (getPluginType() == kFXPlugin)
	{
		addSupportedIOCombination({ kCFMono, kCFMono });
		addSupportedIOCombination({ kCFMono, kCFStereo });
		addSupportedIOCombination({ kCFStereo, kCFStereo });
	}
	else // --- synth plugins have no input, only output
	{
		addSupportedIOCombination({ kCFNone, kCFMono });
		addSupportedIOCombination({ kCFNone, kCFStereo });
	}

	// --- for sidechaining, we support mono and stereo inputs; auxOutputs reserved for future use
	addSupportedAuxIOCombination({ kCFMono, kCFNone });
	addSupportedAuxIOCombination({ kCFStereo, kCFNone });

	// --- create the parameters
    initPluginParameters();

    // --- create the presets
    initPluginPresets();
}

/**
\brief create all of your plugin parameters here

\return true if parameters were created, false if they already existed
*/
bool PluginCore::initPluginParameters()
{
	if (pluginParameterMap.size() > 0)
		return false;

    // --- Add your plugin parameter instantiation code bewtween these hex codes
	// **--0xDEA7--**


	// --- Declaration of Plugin Parameter Objects 
	PluginParameter* piParam = nullptr;

	// --- continuous control: Volumen
	piParam = new PluginParameter(controlID::volumen, "Volumen", "", controlVariableType::kDouble, 0.000000, 1.000000, 1.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(20.00);
	piParam->setBoundVariable(&volumen, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: Waveform_Osc1
	piParam = new PluginParameter(controlID::waveform, "Waveform_Osc1", "sine,square,triangle,sawtooth,sawtooth inverse", "sine");
	piParam->setBoundVariable(&waveform, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Frecuencia
	piParam = new PluginParameter(controlID::Frecuencia_Hz, "Frecuencia", "Hz", controlVariableType::kFloat, 200.000000, 6000.000000, 440.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(20.00);
	piParam->setBoundVariable(&Frecuencia_Hz, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- discrete control: Oscilador_1
	piParam = new PluginParameter(controlID::osc1_on, "Oscilador_1", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&osc1_on, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Waveform_Osc2
	piParam = new PluginParameter(controlID::wave2, "Waveform_Osc2", "sine,square,triangle,sawtooth,sawtooth inverse", "sine");
	piParam->setBoundVariable(&wave2, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Frecuencia_Osc2
	piParam = new PluginParameter(controlID::frec2, "Frecuencia_Osc2", "Hz", controlVariableType::kFloat, 100.000000, 6000.000000, 392.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(20.00);
	piParam->setBoundVariable(&frec2, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- discrete control: Oscilador_2
	piParam = new PluginParameter(controlID::osc2_on, "Oscilador_2", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&osc2_on, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Mute
	piParam = new PluginParameter(controlID::enableMute, "Mute", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&enableMute, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: LFO_enable
	piParam = new PluginParameter(controlID::lfo_1, "LFO_enable", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&lfo_1, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Amount_LFO
	piParam = new PluginParameter(controlID::amount_lfo, "Amount_LFO", "Units", controlVariableType::kFloat, 0.000000, 1.000000, 0.300000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(20.00);
	piParam->setBoundVariable(&amount_lfo, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- discrete control: Select_LFO
	piParam = new PluginParameter(controlID::lfo_selec, "Select_LFO", "vibrato,tremolo", "tremolo");
	piParam->setBoundVariable(&lfo_selec, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Frecuencia_LFO
	piParam = new PluginParameter(controlID::lfo_frec, "Frecuencia_LFO", "Hz", controlVariableType::kFloat, 0.500000, 20.000000, 5.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(20.00);
	piParam->setBoundVariable(&lfo_frec, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- discrete control: LFO_frec
	piParam = new PluginParameter(controlID::waveLFO, "LFO_frec", "sine,square,triangle,sawtooth,sawtooth inverse", "sine");
	piParam->setBoundVariable(&waveLFO, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Filtro_Pasa_Bandas
	piParam = new PluginParameter(controlID::BP_filter, "Filtro_Pasa_Bandas", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&BP_filter, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: l_freq_cut
	piParam = new PluginParameter(controlID::l_freq_cut, "l_freq_cut", "Units", controlVariableType::kFloat, 200.000000, 5000.000000, 440.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(20.00);
	piParam->setBoundVariable(&l_freq_cut, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- continuous control: h_freq_cut
	piParam = new PluginParameter(controlID::h_freq_cut, "h_freq_cut", "Units", controlVariableType::kFloat, 300.000000, 5000.000000, 1500.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(20.00);
	piParam->setBoundVariable(&h_freq_cut, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- discrete control: EG_enable
	piParam = new PluginParameter(controlID::EG_enable, "EG_enable", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&EG_enable, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Attack
	piParam = new PluginParameter(controlID::attack, "Attack", "Units", controlVariableType::kFloat, -800.000000, 5000.000000, 1000.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(20.00);
	piParam->setBoundVariable(&attack, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- continuous control: Decay
	piParam = new PluginParameter(controlID::decay, "Decay", "Units", controlVariableType::kFloat, -800.000000, 5000.000000, 100.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(20.00);
	piParam->setBoundVariable(&decay, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- continuous control: Sustain
	piParam = new PluginParameter(controlID::Sustain, "Sustain", "Units", controlVariableType::kFloat, 0.000000, 1.000000, 0.500000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(20.00);
	piParam->setBoundVariable(&Sustain, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- continuous control: Release
	piParam = new PluginParameter(controlID::release, "Release", "Units", controlVariableType::kFloat, -800.000000, 5000.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingTimeMsec(20.00);
	piParam->setBoundVariable(&release, boundVariableType::kFloat);
	addPluginParameter(piParam);

	// --- Aux Attributes
	AuxParameterAttribute auxAttribute;

	// --- RAFX GUI attributes
	// --- controlID::volumen
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483649);
	setParamAuxAttribute(controlID::volumen, auxAttribute);

	// --- controlID::waveform
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::waveform, auxAttribute);

	// --- controlID::Frecuencia_Hz
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483656);
	setParamAuxAttribute(controlID::Frecuencia_Hz, auxAttribute);

	auxAttribute.reset(auxGUIIdentifier::midiControlData);
	auxAttribute.setUintAttribute(2147680256);
	setParamAuxAttribute(controlID::Frecuencia_Hz, auxAttribute);

	// --- controlID::osc1_on
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::osc1_on, auxAttribute);

	// --- controlID::wave2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::wave2, auxAttribute);

	// --- controlID::frec2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483660);
	setParamAuxAttribute(controlID::frec2, auxAttribute);

	// --- controlID::osc2_on
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::osc2_on, auxAttribute);

	// --- controlID::enableMute
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741829);
	setParamAuxAttribute(controlID::enableMute, auxAttribute);

	// --- controlID::lfo_1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741825);
	setParamAuxAttribute(controlID::lfo_1, auxAttribute);

	// --- controlID::amount_lfo
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483662);
	setParamAuxAttribute(controlID::amount_lfo, auxAttribute);

	// --- controlID::lfo_selec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::lfo_selec, auxAttribute);

	// --- controlID::lfo_frec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483650);
	setParamAuxAttribute(controlID::lfo_frec, auxAttribute);

	// --- controlID::waveLFO
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::waveLFO, auxAttribute);

	// --- controlID::BP_filter
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741826);
	setParamAuxAttribute(controlID::BP_filter, auxAttribute);

	// --- controlID::l_freq_cut
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::l_freq_cut, auxAttribute);

	// --- controlID::h_freq_cut
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::h_freq_cut, auxAttribute);

	// --- controlID::EG_enable
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::EG_enable, auxAttribute);

	// --- controlID::attack
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::attack, auxAttribute);

	// --- controlID::decay
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::decay, auxAttribute);

	// --- controlID::Sustain
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::Sustain, auxAttribute);

	// --- controlID::release
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::release, auxAttribute);


	// **--0xEDA5--**
   
    // --- BONUS Parameter
    // --- SCALE_GUI_SIZE
    PluginParameter* piParamBonus = new PluginParameter(SCALE_GUI_SIZE, "Scale GUI", "tiny,small,medium,normal,large,giant", "normal");
    addPluginParameter(piParamBonus);

	// --- create the super fast access array
	initPluginParameterArray();

    return true;
}

/**
\brief initialize object for a new run of audio; called just before audio streams

Operation:
- store sample rate and bit depth on audioProcDescriptor - this information is globally available to all core functions
- reset your member objects here

\param resetInfo structure of information about current audio format

\return true if operation succeeds, false otherwise
*/
bool PluginCore::reset(ResetInfo& resetInfo)
{
    // --- save for audio processing
    audioProcDescriptor.sampleRate = resetInfo.sampleRate;
    audioProcDescriptor.bitDepth = resetInfo.bitDepth;

    // --- other reset inits
    return PluginBase::reset(resetInfo);
}

/**
\brief one-time initialize function called after object creation and before the first reset( ) call

Operation:
- saves structure for the plugin to use; you can also load WAV files or state information here
*/
bool PluginCore::initialize(PluginInfo& pluginInfo)
{
	// --- add one-time init stuff here

	return true;
}

/**
\brief do anything needed prior to arrival of audio buffers

Operation:
- syncInBoundVariables when preProcessAudioBuffers is called, it is *guaranteed* that all GUI control change information
  has been applied to plugin parameters; this binds parameter changes to your underlying variables
- NOTE: postUpdatePluginParameter( ) will be called for all bound variables that are acutally updated; if you need to process
  them individually, do so in that function
- use this function to bulk-transfer the bound variable data into your plugin's member object variables

\param processInfo structure of information about *buffer* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::preProcessAudioBuffers(ProcessBufferInfo& processInfo)
{
    // --- sync internal variables to GUI parameters; you can also do this manually if you don't
    //     want to use the auto-variable-binding
    syncInBoundVariables();

    return true;
}

/**
\brief frame-processing method

Operation:
- decode the plugin type - for synth plugins, fill in the rendering code; for FX plugins, delete the if(synth) portion and add your processing code
- note that MIDI events are fired for each sample interval so that MIDI is tightly sunk with audio
- doSampleAccurateParameterUpdates will perform per-sample interval smoothing

\param processFrameInfo structure of information about *frame* processing

\return true if operation succeeds, false otherwise
*/


bool PluginCore::processAudioFrame(ProcessFrameInfo& processFrameInfo)
{
	// Funcion para seleccionar Ocilador
	double trem = 0;
	double Amplitud = volumen;

	if (enableMute == 1)
	{
		y = 0;
	}
	else if (osc1_on == 1)
	{
		if (compareEnumToInt(waveformEnum::sine, waveform))
		{
			y=Waves(Frecuencia_Hz, 1,Amplitud);
		}

		else if (compareEnumToInt(waveformEnum::square, waveform))
		{
			y = Waves(Frecuencia_Hz, 2, Amplitud);
		}

		else if (compareEnumToInt(waveformEnum::triangle, waveform))
		{
			y = Waves(Frecuencia_Hz, 3, Amplitud);
		}

		else if (compareEnumToInt(waveformEnum::sawtooth, waveform))
		{
			y = Waves(Frecuencia_Hz, 4, Amplitud);
		}

		else if(compareEnumToInt(waveformEnum::sawtooth_inverse, waveform))
		{
			y = Waves(Frecuencia_Hz, 5, Amplitud);
		}

	}
	else
	{
		y = 0;
	}

	if (enableMute == 1)
	{
		y2 = 0;
	}
	else if (osc2_on == 1)
	{
		if (compareEnumToInt(wave2Enum::sine, wave2))
		{
			y2 = Waves2(frec2, 1, Amplitud);
		}

		else if (compareEnumToInt(wave2Enum::square, wave2))
		{
			y2 = Waves2(frec2, 2, Amplitud);

		}

		else if (compareEnumToInt(wave2Enum::triangle, wave2))
		{
			y2 = Waves2(frec2, 3, Amplitud);
		}

		else if (compareEnumToInt(wave2Enum::sawtooth, wave2))
		{
			y2 = Waves2(frec2, 4, Amplitud);
		}

		else if (compareEnumToInt(wave2Enum::sawtooth_inverse, wave2))
		{
			y2 = Waves2(frec2, 5, Amplitud);
		}
	}
	else
	{
		y2 = 0;
	}


	if (lfo_1 == 1) 
	{
		if (compareEnumToInt(waveLFOEnum::sine, waveLFO))
		{
			out_sin = Waves3(lfo_frec, 1, Amplitud);
		}
		else if (compareEnumToInt(waveLFOEnum::square, waveLFO))
		{
			out_sin = Waves3(lfo_frec, 2, Amplitud);
		}
		else if (compareEnumToInt(waveLFOEnum::triangle, waveLFO))
		{
			out_sin = Waves3(lfo_frec, 3, Amplitud);
		}
		else if (compareEnumToInt(waveLFOEnum::sawtooth, waveLFO))
		{
			out_sin = Waves3(lfo_frec, 4, Amplitud);
		}
		else if (compareEnumToInt(waveLFOEnum::sawtooth_inverse, waveLFO))
		{
			out_sin = Waves3(lfo_frec, 5, Amplitud);
		}
		

		if (compareEnumToInt(lfo_selecEnum::tremolo, lfo_selec)) 
		{
			amount = double(amount_lfo);
			m_n = 1.0 + amount * cos(phase);
			y_n = m_n*out_sin;

			lfo_f = (2 * pi * lfo_frec * n) / audioProcDescriptor.sampleRate;
			phase = phase + lfo_f ;

			if (n < numSamples) {
				n = n + 1;
			}
			else {
				n = 0;
			}
			
		}
		else if (compareEnumToInt(lfo_selecEnum::vibrato, lfo_selec)) 
		{
			
		}

	}
	else 
	{
		y_n = 1;
	
	}

	osciladores = (y + y2) *y_n;

	if (BP_filter == 1) 
	{
		fch_1 = h_freq_cut;//frecuencia high
		fcl_1 = l_freq_cut;//frecuencia low

		double fs = audioProcDescriptor.sampleRate;
		fb_1 = fch_1 - fcl_1;
		fc_1 = (fch_1 + fcl_1) / 2;
		Q_1 = fc_1 / fb_1;
		k_1 = tan((pi * fc_1) / fs);

		b0_1 = k_1 / ((k_1 * k_1 * Q_1) + k_1 + Q_1);
		b2_1 = (-1 * k_1) / ((k_1 * k_1 * Q_1) + k_1 + Q_1);
		a1_1 = (2 * Q_1 * (k_1 * k_1 - 1)) / ((k_1 * k_1 * Q_1) + k_1 + Q_1);
		a2_1 = ((k_1 * k_1 * Q_1) - k_1 + Q_1) / ((k_1 * k_1 * Q_1) + k_1 + Q_1);

		salida = ganancia*((b0_1 * osciladores + b2_1 * x_n_2 - a1_1 * y_n1_1 - a2_1 * y_n2_1) / a0);
		y_n2_1 = y_n1_1;
		y_n1_1 = y_n_1;

		x_n_2 = x_n_1;
		x_n_1 = osciladores;
	}
	else 
	{
		salida = osciladores;
	}
	
	if (EG_enable == 1)
	{

		//calcular constante de ATTACK
		//muestras del exponential rate
		dSamples_A = audioProcDescriptor.sampleRate * ((attack_time+attack)/ 1000.0);
		attack_coef = exp(-log((1.0 + attack_TCO) / attack_TCO) / dSamples_A);
		attack_offset = (1.0 + attack_TCO) * (1.0 - attack_coef);

		//calcular constante de DECAY
		dSamples_D = audioProcDescriptor.sampleRate * ((decay_time+decay)/ 1000.0);
		decay_coef= exp(-log((1.0 + decay_TCO) / decay_TCO) / dSamples_D);
		decay_offset = (Sustain - decay_TCO) * (1.0 - decay_coef);

		//calcular constante de RELEASE
		dSamples_R = audioProcDescriptor.sampleRate * ((release_time+release)/ 1000.0);
		release_coef = exp(-log((1.0 + release_TCO) / release_TCO) / dSamples_R);
		release_offset = -release_TCO * (1.0 - release_coef);
		
		current_stage = current_stage + 1;
		if (current_stage < dSamples_A) {
			currentSampleIndex = 1;
		}
		else if (current_stage == dSamples_A) {
			currentSampleIndex = 2;
		}
		else if ((current_stage> (dSamples_A+dSamples_D)))
		{
			currentSampleIndex = 4;
		}
		else if ((current_stage > (dSamples_A + dSamples_D+ dSamples_R))) {
			currentSampleIndex = 0;			
		}
		//&& (osc1_on != 1)

		StageEnvelopeOut(currentSampleIndex);
		salida = EnvelopeOut * osciladores;

	}
	else
	{
		current_stage = 0;
		currentSampleIndex = 0;
		StageEnvelopeOut(currentSampleIndex);
	
	}
	
	// --- fire any MIDI events for this sample interval
    processFrameInfo.midiEventQueue->fireMidiEvents(processFrameInfo.currentFrame);


	// --- do per-frame updates; VST automation and parameter smoothing
	doSampleAccurateParameterUpdates();

    // --- decode the channelIOConfiguration and process accordingly
    //
	// --- Synth Plugin:
	// --- Synth Plugin --- remove for FX plugins
	if (getPluginType() == kSynthPlugin)
	{
		// --- output silence: change this with your signal render code
		processFrameInfo.audioOutputFrame[0] = Amplitud * (salida);
		if (processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)
			processFrameInfo.audioOutputFrame[1] = Amplitud * (salida);

		return true;	/// processed
	}

    // --- FX Plugin:
	

    if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFMono &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFMono)
    {
		// --- pass through code: change this with your signal processing
        processFrameInfo.audioOutputFrame[0] = Amplitud * processFrameInfo.audioInputFrame[0];
		

        return true; /// processed
    }

    // --- Mono-In/Stereo-Out
    else if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFMono &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)
    {
		// --- pass through code: change this with your signal processing
        processFrameInfo.audioOutputFrame[0] = Amplitud *processFrameInfo.audioInputFrame[0];
        processFrameInfo.audioOutputFrame[1] = Amplitud * processFrameInfo.audioInputFrame[0];

        return true; /// processed
    }

    // --- Stereo-In/Stereo-Out
    else if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFStereo &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)
    {
		// --- pass through code: change this with your signal processing
        processFrameInfo.audioOutputFrame[0] = Amplitud * processFrameInfo.audioInputFrame[0];
        processFrameInfo.audioOutputFrame[1] = Amplitud * processFrameInfo.audioInputFrame[1];

        return true; /// processed
    }

    return false; /// NOT processed
}


/**
\brief do anything needed prior to arrival of audio buffers

Operation:
- updateOutBoundVariables sends metering data to the GUI meters

\param processInfo structure of information about *buffer* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::postProcessAudioBuffers(ProcessBufferInfo& processInfo)
{
	// --- update outbound variables; currently this is meter data only, but could be extended
	//     in the future
	updateOutBoundVariables();

    return true;
}

/**
\brief update the PluginParameter's value based on GUI control, preset, or data smoothing (thread-safe)

Operation:
- update the parameter's value (with smoothing this initiates another smoothing process)
- call postUpdatePluginParameter to do any further processing

\param controlID the control ID value of the parameter being updated
\param controlValue the new control value
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::updatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo)
{
    // --- use base class helper
    setPIParamValue(controlID, controlValue);

    // --- do any post-processing
    postUpdatePluginParameter(controlID, controlValue, paramInfo);

    return true; /// handled
}

/**
\brief update the PluginParameter's value based on *normlaized* GUI control, preset, or data smoothing (thread-safe)

Operation:
- update the parameter's value (with smoothing this initiates another smoothing process)
- call postUpdatePluginParameter to do any further processing

\param controlID the control ID value of the parameter being updated
\param normalizedValue the new control value in normalized form
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::updatePluginParameterNormalized(int32_t controlID, double normalizedValue, ParameterUpdateInfo& paramInfo)
{
	// --- use base class helper, returns actual value
	double controlValue = setPIParamValueNormalized(controlID, normalizedValue, paramInfo.applyTaper);

	// --- do any post-processing
	postUpdatePluginParameter(controlID, controlValue, paramInfo);

	return true; /// handled
}

/**
\brief perform any operations after the plugin parameter has been updated; this is one paradigm for
	   transferring control information into vital plugin variables or member objects. If you use this
	   method you can decode the control ID and then do any cooking that is needed. NOTE: do not
	   overwrite bound variables here - this is ONLY for any extra cooking that is required to convert
	   the GUI data to meaninful coefficients or other specific modifiers.

\param controlID the control ID value of the parameter being updated
\param controlValue the new control value
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::postUpdatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo)
{
    // --- now do any post update cooking; be careful with VST Sample Accurate automation
    //     If enabled, then make sure the cooking functions are short and efficient otherwise disable it
    //     for the Parameter involved
    /*switch(controlID)
    {
        case 0:
        {
            return true;    /// handled
        }

        default:
            return false;   /// not handled
    }*/

    return false;
}

/**
\brief has nothing to do with actual variable or updated variable (binding)

CAUTION:
- DO NOT update underlying variables here - this is only for sending GUI updates or letting you
  know that a parameter was changed; it should not change the state of your plugin.

WARNING:
- THIS IS NOT THE PREFERRED WAY TO LINK OR COMBINE CONTROLS TOGETHER. THE PROPER METHOD IS
  TO USE A CUSTOM SUB-CONTROLLER THAT IS PART OF THE GUI OBJECT AND CODE.
  SEE http://www.willpirkle.com for more information

\param controlID the control ID value of the parameter being updated
\param actualValue the new control value

\return true if operation succeeds, false otherwise
*/
bool PluginCore::guiParameterChanged(int32_t controlID, double actualValue)
{
	/*
	switch (controlID)
	{
		case controlID::<your control here>
		{

			return true; // handled
		}

		default:
			break;
	}*/

	return false; /// not handled
}

/**
\brief For Custom View and Custom Sub-Controller Operations

NOTES:
- this is for advanced users only to implement custom view and custom sub-controllers
- see the SDK for examples of use

\param messageInfo a structure containing information about the incoming message

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processMessage(MessageInfo& messageInfo)
{
	// --- decode message
	switch (messageInfo.message)
	{
		// --- add customization appearance here
	case PLUGINGUI_DIDOPEN:
	{
		return false;
	}

	// --- NULL pointers so that we don't accidentally use them
	case PLUGINGUI_WILLCLOSE:
	{
		return false;
	}

	// --- update view; this will only be called if the GUI is actually open
	case PLUGINGUI_TIMERPING:
	{
		return false;
	}

	// --- register the custom view, grab the ICustomView interface
	case PLUGINGUI_REGISTER_CUSTOMVIEW:
	{

		return false;
	}

	case PLUGINGUI_REGISTER_SUBCONTROLLER:
	case PLUGINGUI_QUERY_HASUSERCUSTOM:
	case PLUGINGUI_USER_CUSTOMOPEN:
	case PLUGINGUI_USER_CUSTOMCLOSE:
	case PLUGINGUI_EXTERNAL_SET_NORMVALUE:
	case PLUGINGUI_EXTERNAL_SET_ACTUALVALUE:
	{

		return false;
	}

	default:
		break;
	}

	return false; /// not handled
}


/**
\brief process a MIDI event

NOTES:
- MIDI events are 100% sample accurate; this function will be called repeatedly for every MIDI message
- see the SDK for examples of use

\param event a structure containing the MIDI event data

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processMIDIEvent(midiEvent& event)
{
	return true;
}

/**
\brief (for future use)

NOTES:
- MIDI events are 100% sample accurate; this function will be called repeatedly for every MIDI message
- see the SDK for examples of use

\param vectorJoysickData a structure containing joystick data

\return true if operation succeeds, false otherwise
*/
bool PluginCore::setVectorJoystickParameters(const VectorJoystickData& vectorJoysickData)
{
	return true;
}

/**
\brief use this method to add new presets to the list

NOTES:
- see the SDK for examples of use
- for non RackAFX users that have large paramter counts, there is a secret GUI control you
  can enable to write C++ code into text files, one per preset. See the SDK or http://www.willpirkle.com for details

\return true if operation succeeds, false otherwise
*/
bool PluginCore::initPluginPresets()
{
	// **--0xFF7A--**

	// --- Plugin Presets 
	int index = 0;
	PresetInfo* preset = nullptr;

	// --- Preset: Factory Preset
	preset = new PresetInfo(index++, "Factory Preset");
	initPresetParameters(preset->presetParameters);
	setPresetParameter(preset->presetParameters, controlID::volumen, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::waveform, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::Frecuencia_Hz, 440.000000);
	setPresetParameter(preset->presetParameters, controlID::osc1_on, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::wave2, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::frec2, 392.000000);
	setPresetParameter(preset->presetParameters, controlID::osc2_on, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::enableMute, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo_1, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::amount_lfo, 0.300000);
	setPresetParameter(preset->presetParameters, controlID::lfo_selec, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo_frec, 5.000000);
	setPresetParameter(preset->presetParameters, controlID::waveLFO, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::BP_filter, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::l_freq_cut, 440.000000);
	setPresetParameter(preset->presetParameters, controlID::h_freq_cut, 1500.000000);
	setPresetParameter(preset->presetParameters, controlID::EG_enable, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::attack, 999.999878);
	setPresetParameter(preset->presetParameters, controlID::decay, 99.999939);
	setPresetParameter(preset->presetParameters, controlID::Sustain, 0.500000);
	setPresetParameter(preset->presetParameters, controlID::release, 0.000000);
	addPreset(preset);


	// **--0xA7FF--**

    return true;
}

/**
\brief setup the plugin description strings, flags and codes; this is ordinarily done through the ASPiKreator or CMake

\return true if operation succeeds, false otherwise
*/
bool PluginCore::initPluginDescriptors()
{
    pluginDescriptor.pluginName = PluginCore::getPluginName();
    pluginDescriptor.shortPluginName = PluginCore::getShortPluginName();
    pluginDescriptor.vendorName = PluginCore::getVendorName();
    pluginDescriptor.pluginTypeCode = PluginCore::getPluginType();

	// --- describe the plugin attributes; set according to your needs
	pluginDescriptor.hasSidechain = kWantSidechain;
	pluginDescriptor.latencyInSamples = kLatencyInSamples;
	pluginDescriptor.tailTimeInMSec = kTailTimeMsec;
	pluginDescriptor.infiniteTailVST3 = kVSTInfiniteTail;

    // --- AAX
    apiSpecificInfo.aaxManufacturerID = kManufacturerID;
    apiSpecificInfo.aaxProductID = kAAXProductID;
    apiSpecificInfo.aaxBundleID = kAAXBundleID;  /* MacOS only: this MUST match the bundle identifier in your info.plist file */
    apiSpecificInfo.aaxEffectID = "aaxDeveloper.";
    apiSpecificInfo.aaxEffectID.append(PluginCore::getPluginName());
    apiSpecificInfo.aaxPluginCategoryCode = kAAXCategory;

    // --- AU
    apiSpecificInfo.auBundleID = kAUBundleID;
	apiSpecificInfo.auBundleName = kAUBundleName;
	apiSpecificInfo.auBundleName = kAUBundleName;   /* MacOS only: this MUST match the bundle identifier in your info.plist file */
    apiSpecificInfo.auBundleName = kAUBundleName;

    // --- VST3
    apiSpecificInfo.vst3FUID = PluginCore::getVSTFUID(); // OLE string format
    apiSpecificInfo.vst3BundleID = kVST3BundleID;/* MacOS only: this MUST match the bundle identifier in your info.plist file */
	apiSpecificInfo.enableVST3SampleAccurateAutomation = kVSTSAA;
	apiSpecificInfo.vst3SampleAccurateGranularity = kVST3SAAGranularity;

    // --- AU and AAX
    apiSpecificInfo.fourCharCode = PluginCore::getFourCharCode();

    return true;
}



// --- static functions required for VST3/AU only --------------------------------------------- //
const char* PluginCore::getPluginBundleName() { return kAUBundleName; }
const char* PluginCore::getPluginName(){ return kPluginName; }
const char* PluginCore::getShortPluginName(){ return kShortPluginName; }
const char* PluginCore::getVendorName(){ return kVendorName; }
const char* PluginCore::getVendorURL(){ return kVendorURL; }
const char* PluginCore::getVendorEmail(){ return kVendorEmail; }
const char* PluginCore::getAUCocoaViewFactoryName(){ return AU_COCOA_VIEWFACTORY_STRING; }
pluginType PluginCore::getPluginType(){ return kPluginType; }
const char* PluginCore::getVSTFUID(){ return kVSTFUID; }
int32_t PluginCore::getFourCharCode(){ return kFourCharCode; }
