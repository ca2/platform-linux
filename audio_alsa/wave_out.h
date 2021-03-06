#pragma once


namespace multimedia
{


   namespace audio_alsa
   {


      class  CLASS_DECL_AUDIO_ALSA wave_out :
         virtual public snd_pcm,
         virtual public ::wave::out
      {
      public:


         bool                    m_bWrite;
         int                     m_iBuffer;
         bool                    m_bStarted;
         ::u64                   m_uStart;
         snd_pcm_status_t *      m_pstatus;
         timeval                 m_timevalStart;


         wave_out();
         virtual ~wave_out();


         void install_message_routing(::channel * pchannel) override;

         virtual imedia_time out_get_time();
         //imedia_position out_get_position();

         //virtual ::e_status wave_out_open(::thread * pthreadCallback, ::count iBufferCount, ::count iBufferSampleCount) override;
         virtual ::e_status out_open_ex(::thread * pthreadCallback, ::u32 uiSamplesPerSec, ::u32 uiChannelCount, ::u32 uiBitsPerSample, ::wave::e_purpose epurpose) override;
         virtual ::e_status out_stop() override;
         virtual ::e_status out_close() override;
         virtual ::e_status out_pause() override;
         virtual ::e_status out_restart() override;
         snd_pcm_t * out_get_safe_PCM();

         //virtual void alsa_write_thread();

         virtual void out_on_playback_end() override;
         virtual void out_filled(index iBuffer) override;
         virtual bool alsa_should_play();

         virtual ::e_status init_thread() override;
         virtual void term_thread() override;

         virtual ::e_status out_start(const imedia_time & time);

         virtual int defer_underrun_recovery(int err);

         imedia_time out_get_time_for_synch();


      };


   } // namespace audio_alsa


} // namespace multimedia



