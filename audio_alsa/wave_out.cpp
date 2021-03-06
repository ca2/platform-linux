#include "framework.h"
#include "aura/message.h"
#include <sys/time.h>


long long timestamp2ns(snd_htimestamp_t t)
{

   long long nsec;

   nsec = t.tv_sec * 1000000000;
   nsec += t.tv_nsec;

   return nsec;

}


long long timediff(snd_htimestamp_t t1, snd_htimestamp_t t2)
{

   long long nsec1, nsec2;

   nsec1 = timestamp2ns(t1);
   nsec2 = timestamp2ns(t2);

   return nsec1 - nsec2;

}


namespace multimedia
{


   namespace audio_alsa
   {


      wave_out::wave_out()
      {

         m_bDirectOutput      = false;
         m_estate             = e_state_initial;
         m_pthreadCallback    = NULL;
         m_estatusWave            = success;
         m_bWrite             = false;
         m_bStarted           = false;

         m_pstatus            = NULL;

         snd_pcm_status_malloc (&m_pstatus);

      }


      wave_out::~wave_out()
      {

         snd_pcm_status_free (m_pstatus);

      }


      void wave_out::install_message_routing(::channel * pchannel)
      {

         ::wave::out::install_message_routing(pchannel);

      }


      ::e_status wave_out::init_thread()
      {

         if(!::wave::out::init_thread())
         {

            return false;

         }

         return true;

      }


      void wave_out::term_thread()
      {

         ::wave::out::term_thread();

         thread::term_thread();

      }


      ::e_status wave_out::out_open_ex(thread * pthreadCallback, ::u32 uiSamplesPerSec, ::u32 uiChannelCount, ::u32 uiBitsPerSample, ::wave::e_purpose epurpose)
      {

         synchronous_lock sl(mutex());

         TRACE("multimedia::audio_alsa::out_open_ex");

         if(m_ppcm != NULL && m_estate != e_state_initial)
         {

            return success;

         }

//         m_dwPeriodTime =  20 * 1000; /* period time in us */

//         m_iBufferCountEffective = 4;

         if(epurpose == ::wave::purpose_playback)
         {

  //          m_dwPeriodTime =  6 * 1000; /* period time in us */

            //m_iBufferCountEffective = 16;

            m_frameCount = uiSamplesPerSec / 20;

            m_iBufferCount = 8;

            printf("::wave::purpose_playback %ld\n", m_frameCount);

         }
         else
         {

            m_frameCount = 1024;

            //m_frameCount = uiSamplesPerSec / 20;

            m_iBufferCount = 4;

            printf("::wave::* %" PRIu64 "\n", m_frameCount);

         }

//         m_iBufferCount = m_iBufferCountEffective;

         m_pthreadCallback = pthreadCallback;

         ASSERT(m_ppcm == NULL);

         ASSERT(m_estate == e_state_initial);

         m_pwaveformat->m_waveformat.wFormatTag        = 0;
         m_pwaveformat->m_waveformat.nChannels         = (::u16) uiChannelCount;
         m_pwaveformat->m_waveformat.nSamplesPerSec    = uiSamplesPerSec;
         m_pwaveformat->m_waveformat.wBitsPerSample    = (::u16) uiBitsPerSample;
         m_pwaveformat->m_waveformat.nBlockAlign       = m_pwaveformat->m_waveformat.wBitsPerSample * m_pwaveformat->m_waveformat.nChannels / 8;
         m_pwaveformat->m_waveformat.nAvgBytesPerSec   = m_pwaveformat->m_waveformat.nSamplesPerSec * m_pwaveformat->m_waveformat.nBlockAlign;
         //m_pwaveformat->cbSize            = 0;

         if((m_estatusWave = this->snd_pcm_open(SND_PCM_STREAM_PLAYBACK)) != success)
         {

            return m_estatusWave;

         }

//         if(m_iBufferCount < m_iBufferCountEffective)
//         {
//
//            m_iBufferCountEffective = m_iBufferCount;
//
//         }

         //::u32 uBufferSize = snd_pcm_frames_to_bytes(m_ppcm, m_framesPeriodSize);

//         m_uiBufferTime = m_framesBuffer * 1000 * 1000 / uiSamplesPerSec;

         printf("snd_pcm_frames_to_bytes %" PRIu64 "\n", m_frameCount);

         ::u32 uBufferSize = snd_pcm_frames_to_bytes(m_ppcm, m_frameCount);

         out_get_buffer()->PCMOutOpen(this, uBufferSize, m_iBufferCount, 128, m_pwaveformat, m_pwaveformat);

         m_pprebuffer->open(m_pwaveformat->m_waveformat.nChannels, m_iBufferCount, m_frameCount);

//         m_pprebuffer->SetMinL1BufferCount(out_get_buffer()->GetBufferCount());

         int err;

         if((err = snd_pcm_sw_params_current(m_ppcm, m_pswparams)) < 0)
         {

            TRACE("unable to determine current m_pswparams for playback: %s\n", snd_strerror(err));

            return error_failed;

         }

         snd_pcm_sframes_t framesThreshold = (m_iBufferCount - 1) * m_frameCount;

         // start the transfer when the buffer is almost full:
         if((err = snd_pcm_sw_params_set_start_threshold(m_ppcm, m_pswparams, framesThreshold)) < 0)
         {

            TRACE("unable to set start threshold mode for playback: %s\n", snd_strerror(err));

            return error_failed;

         }

         // allow the transfer when at least m_framesPeriodSize samples can be processed
         if((err = snd_pcm_sw_params_set_avail_min(m_ppcm, m_pswparams, m_frameCount)) < 0)
         {

            TRACE("unable to set avail min for playback: %s\n", snd_strerror(err));

            return error_failed;

         }

         if((err = snd_pcm_sw_params_set_tstamp_mode(m_ppcm, m_pswparams, SND_PCM_TSTAMP_ENABLE)) < 0)
         {

            TRACE("unable to set time stamp mode: %s\n", snd_strerror(err));

            return error_failed;

         }

         if((err = snd_pcm_sw_params_set_tstamp_type(m_ppcm, m_pswparams,SND_PCM_TSTAMP_TYPE_GETTIMEOFDAY)) <0)
         {

            TRACE("unable to set time stamp type: %s\n", snd_strerror(err));

            return error_failed;

         }

         // write the parameters to the playback device
         if((err = snd_pcm_sw_params(m_ppcm, m_pswparams)) < 0)
         {

            TRACE("unable to set sw params for playback: %s\n", snd_strerror(err));

            return error_failed;

         }

         m_psynththread->m_iWaitBuffer = 1;

         m_estate = e_state_opened;

         m_epurpose = epurpose;

         return success;

      }


      ::e_status wave_out::out_close()
      {

         synchronous_lock sl(mutex());

         TRACE("multimedia::audio_alsa::out_close");

         if(m_estate == e_state_playing)
         {

            out_stop();

         }

         if(m_estate != e_state_opened)
         {

            return success;

         }

         ::e_status mmr;

         mmr = this->snd_pcm_close();

         m_ppcm = NULL;

         ::wave::out::out_close();

         m_estate = e_state_initial;

         return success;

      }


      ::e_status wave_out::out_stop()
      {

         synchronous_lock sl(mutex());

         TRACE("multimedia::audio_alsa::out_stop");

         if(m_estate != e_state_playing && m_estate != e_state_paused)
         {

            return error_failed;

         }

         //m_pprebuffer->stop();

         m_estate = e_state_stopping;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_alsa output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_estatusWave = translate_alsa(snd_pcm_drain(m_ppcm));

         if(m_estatusWave == success)
         {

            m_estate = e_state_opened;

         }

         return m_estatusWave;

      }


      ::e_status wave_out::out_pause()
      {

         synchronous_lock sl(mutex());

         ASSERT(m_estate == e_state_playing);

         TRACE("multimedia::audio_alsa::out_pause");

         if(m_estate != e_state_playing)
         {

            return error_failed;

         }

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_alsa output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_estatusWave = translate_alsa(snd_pcm_pause(m_ppcm, 1));

         ASSERT(m_estatusWave == success);

         if(m_estatusWave == success)
         {

            m_estate = e_state_paused;

         }

         return m_estatusWave;

      }


      ::e_status wave_out::out_restart()
      {

         synchronous_lock sl(mutex());

         ASSERT(m_estate == e_state_paused);

         TRACE("multimedia::audio_alsa::out_restart");

         if(m_estate != e_state_paused)
            return error_failed;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_alsa output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_estatusWave = translate_alsa(snd_pcm_pause(m_ppcm, 0));

         ASSERT(m_estatusWave == success);

         if(m_estatusWave == success)
         {
            m_estate = e_state_playing;
         }

         return m_estatusWave;

      }


      imedia_time wave_out::out_get_time()
      {

         synchronous_lock sl(mutex());

         imedia_time time = 0;

         if(m_ppcm != NULL)
         {

            if(snd_pcm_status(m_ppcm, m_pstatus) == 0)
            {

               snd_timestamp_t t;

               snd_pcm_status_get_trigger_tstamp (m_pstatus, &t);

               timeval tnow;

               gettimeofday(&tnow, nullptr);

               time = (tnow.tv_sec - t.tv_sec)*1'000.0 + (tnow.tv_usec - t.tv_usec) / (1'000.0);

            }

         }

         if(time > 0)
         {

            //output_debug_string("test");

         }

         return time;

      }


//      imedia_position wave_out::out_get_position()
//      {
//
//         return out_get_position_millis()/1'000.0;
//
//      }


      void wave_out::out_on_playback_end()
      {

         TRACE("multimedia::audio_alsa::out_on_playback_end");

         ::wave::out::out_on_playback_end();

      }


      snd_pcm_t * wave_out::out_get_safe_PCM()
      {

         if(this == NULL)
         {

            return NULL;

         }

         return m_ppcm;

      }



      bool wave_out::alsa_should_play()
      {

         if(!::task_get_run())
         {

            return false;

         }

         if(m_ppcm == NULL)
         {

            TRACE("alsa_out_buffer_ready m_ppcm == NULL");

            return false;

         }

         if(m_estate != ::wave::out::e_state_playing && m_estate != wave::out::e_state_stopping)
         {

            TRACE("alsa_out_buffer_ready failure: !playing && !stopping");

            return false;

         }

         return true;

      }


      void wave_out::out_filled(index iBuffer)
      {

         synchronous_lock sl(mutex());

         if(m_ppcm == NULL)
         {

            return;

         }

         snd_pcm_sframes_t iFramesToWrite = m_frameCount;

         int iBytesToWrite = snd_pcm_frames_to_bytes(m_ppcm, iFramesToWrite);

         ::e_status mmr = success;

         snd_pcm_sframes_t iFrameFreeCount = 0;

         //if(false)
         {

            while(::task_get_run())
            {

               iFrameFreeCount = snd_pcm_avail_update(m_ppcm);

               if(iFrameFreeCount == -EAGAIN)
               {

                  continue;

               }
               else if(iFrameFreeCount < 0)
               {

                  const char * pszError = snd_strerror(iFrameFreeCount);

                  TRACE("ALSA wave_out snd_pcm_avail error: %s (%d)\n", pszError, iFrameFreeCount);

                  iFrameFreeCount = defer_underrun_recovery(iFrameFreeCount);

                  if(iFrameFreeCount >= 0)
                  {

                     TRACE("ALSA wave_out snd_pcm_avail underrun recovery success (snd_pcm_avail)");

                     break;

                  }

                  TRACE("ALSA wave_out snd_pcm_avail minimum byte count %d\n", iFramesToWrite);

                  m_estate = e_state_opened;

                  m_estatusWave = error_failed;

                  TRACE("ALSA wave_out snd_pcm_avail error: %s\n", snd_strerror(iFrameFreeCount));

                  return;

               }
               else if(iFrameFreeCount >= iFramesToWrite)
               {

                  break;

               }

               usleep((iFramesToWrite - iFrameFreeCount) * 500'000 / m_pwaveformat->m_waveformat.nSamplesPerSec);

            }

         }

         byte * pdata;

         memory m;

         if(iBuffer >= 0)
         {

            pdata = (byte *) out_get_buffer_data(iBuffer);

         }
         else
         {

            m.set_size(iBytesToWrite);

            m.zero();

            pdata = (byte *) m.get_data();

         }

         int iZero = 0;

         int iFramesJustWritten = 0;

         while (iBytesToWrite > 0)
         {

            iFramesJustWritten = snd_pcm_writei(m_ppcm, pdata, iFramesToWrite);

            iFramesToWrite -= iFramesJustWritten;

            if(!m_bStarted)
            {

               gettimeofday(&m_timevalStart, nullptr);

            }

            m_bStarted = true;

            if(iFramesJustWritten == -EINTR)
            {

               TRACE("snd_pcm_writei -EINTR\n");

               continue;

            }
            else if(iFramesJustWritten == -EAGAIN)
            {

               //TRACE("snd_pcm_writei -EAGAIN\n");

               sl.unlock();

               snd_pcm_wait(m_ppcm, 100);

               sl.lock();

               continue;

            }
            else if(iFramesJustWritten < 0)
            {

               TRACE("snd_pcm_writei Underrun\n");

               TRACE("ALSA wave_out snd_pcm_writei error: %s (%d)\n", snd_strerror(iFramesJustWritten), iFramesJustWritten);

               iFramesJustWritten = defer_underrun_recovery(iFramesJustWritten);

               if(iFramesJustWritten < 0 && iFramesJustWritten != -EAGAIN)
               {

                  m_estate = e_state_opened;

                  m_estatusWave = error_failed;

                  TRACE("ALSA wave_out snd_pcm_writei couldn't recover from error: %s\n", snd_strerror(iFramesJustWritten));

                  return;

               }

               iFramesJustWritten = 0;

               continue;

            }

            int iBytesJustWritten = snd_pcm_frames_to_bytes(m_ppcm, iFramesJustWritten);

            if(iBuffer >= 0)
            {

               m_pprebuffer->m_iBytes += iFramesJustWritten;

            }

            pdata += iBytesJustWritten;

            iBytesToWrite -= iBytesJustWritten;

            if(iBytesToWrite > 0)
            {

               sl.unlock();

               snd_pcm_wait(m_ppcm, 100);

               sl.lock();

            }

         }

         m_iBufferedCount++;

         sl.unlock();

         if(iBuffer >= 0)
         {

            m_psynththread->on_free(iBuffer);

         }

      }


      imedia_time wave_out::out_get_time_for_synch()
      {

         if (m_pprebuffer.is_null())
         {

            return 0;

         }

         int64_t dwPosition = m_pprebuffer->m_iBytes;

         dwPosition *= 1000;

         if (m_pwaveformat->m_waveformat.nSamplesPerSec <= 0)
            return 0;

         dwPosition /= m_pwaveformat->m_waveformat.nSamplesPerSec;

         return dwPosition / 1000.0;

      }


      ::e_status wave_out::out_start(const imedia_time & time)
      {

         synchronous_lock sl(mutex());

         if(m_estate == e_state_playing)
         {

            return success;

         }

         if(m_estate != e_state_opened && m_estate != state_stopped)
         {

            return error_failed;

         }

         int err = 0;

         if ((err = snd_pcm_prepare (m_ppcm)) < 0)
         {

            TRACE ("out_start: Cannot prepare audio interface for use (%s)\n",snd_strerror (err));

            return error_failed;

         }

         TRACE("out_start: snd_pcm_prepare OK");

         m_bStarted = false;

         m_estatusWave = ::wave::out::out_start(time);

         if(failed(m_estatusWave))
         {

            TRACE("out_start: ::wave::out::out_start FAILED");

            return m_estatusWave;

         }

//         m_pthreadWriter = fork([&]()
//         {
//
//            alsa_write_thread();
//
//         });

         TRACE("out_start: ::wave::out::out_start OK");

         return success;

      }


      int wave_out::defer_underrun_recovery(int err)
      {

         return snd_pcm_recover(m_ppcm, err, 0);

//         if (err == -EPIPE)
//         {
//
//            TRACE("underrun_recovery, going to snd_pcm_prepare: %s\n", snd_strerror(err));
//
//
//
//            if (err < 0)
//            {
//
//               TRACE("Can't recover from underrun, snd_pcm_prepare failed: %s\n", snd_strerror(err));
//
//            }
//
//         }
//         else if (err == -ESTRPIPE)
//         {
//
//            TRACE("underrun_recovery, snd_pcm_prepare returned -ESTRPIPE: %s\n", snd_strerror(err));
//
//            while ((err = snd_pcm_resume(m_ppcm)) == -EAGAIN)
//            {
//
//               TRACE("underrun_recovery, snd_pcm_resume returned -EAGAIN: %s\n", snd_strerror(err));
//
//               sleep(1); /* wait until the suspend flag is released */
//
//            }
//
//            if (err < 0)
//            {
//
//               err = snd_pcm_prepare(m_ppcm);
//
//               if (err < 0)
//               {
//
//                  TRACE("Can't recovery from suspend, snd_pcm_prepare failed: %s\n", snd_strerror(err));
//
//               }
//
//            }
//
//         }
//
//         return err;

      }


   } // namespace audio_alsa


} // namespace multimedia



