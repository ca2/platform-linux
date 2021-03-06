#pragma once


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      class CLASS_DECL_AUDIO_MIXER_MMSYSTEM device :
         virtual public ::multimedia::audio_mixer::device
      {
      public:


         HMIXER                        m_hMixer;
         MIXERCAPS                     m_mixercaps;


         device(sp(base_application) papp);
         virtual ~device();

         virtual bool initialize(::multimedia::audio_mixer::audio_mixer * pmixer);

         ::multimedia::audio_mixer::audio_mixer * get_mixer();
         const char * GetSZPName();
         bool OnCommand(WPARAM wparam, LPARAM lparam);
         ::multimedia::result close();
         void MapDlgCtrlIDToLineControls(::multimedia::audio_mixer::source * pSource);
         void MapDlgCtrlIDToControls();
         void MapLineControls(::multimedia::audio_mixer::source * pSource);
         void OnMixerControlChange(::u32 dwControlID);
         void OnMixerLineChange(::u32 dwLineID);
         void map_lines();
         void map_controls();
         ::multimedia::result get_destination(::multimedia::audio_mixer::e_destination edestination, ::multimedia::audio_mixer::destination ** ppDestination);
         ::multimedia::result initialize_destinations();
         ::multimedia::result initialize_capabilities();
         ::multimedia::result open(::u32 uiMixerId, ::u32 dwCallback, ::u32 dwInstance, ::u32 fdwOpen);


      };


   } // namespace audio_mixer_mmsystem


} // namespace multimedia





