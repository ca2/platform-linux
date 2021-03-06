#pragma once


namespace multimedia
{


   namespace audio_alsa
   {


      class  CLASS_DECL_AUDIO_ALSA wave_in :
         virtual public snd_pcm,
         virtual public ::wave::in
      {
      public:


         wave_in();
         virtual ~wave_in();


         virtual bool in_initialize_encoder();

         //virtual ::e_status in_add_buffer(int32_t iBuffer);
         //virtual ::e_status in_add_buffer(LPWAVEHDR lpwavehdr);

         snd_pcm_t * in_get_safe_PCM();
         virtual void * get_os_data();

         ::e_status in_open(int32_t iBufferCount, int32_t iBufferSampleCount);
         ::e_status in_close();
         ::e_status in_stop();
         ::e_status in_start();
         ::e_status in_reset();

         //virtual void translate_in_message(::signal_details * pobj);

         virtual ::e_status init_thread() override;
         virtual void term_thread() override;
         //virtual void pre_translate_message(::signal_details * pobj);

         void in_proc(snd_pcm_t * hwi, ::u32 uMsg, ::u32 dwInstance, ::u32 dwParam1, ::u32 dwParam2);

         virtual ::e_status run() override;

         //WAVEFORMATEX * wave_format();
         //LPWAVEHDR wave_hdr(int iBuffer);


      };


   } // namespace audio_alsa


} // namespace multimedia




