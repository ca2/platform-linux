// created by Camilo <3CamiloSasukeThomasBorregaardSoerensen
// recreated by Camilo 2021-01-28 22:35 <3TBS, Mummi and bilbo!!
// hi5 contribution...
#include "framework.h"
#include "windowing_xcb.h"
#include <X11/cursorfont.h>
#include "acme/os/_user.h"


namespace windowing_xcb
{


   windowing::windowing()
   {

      m_pWindowing = this;

      m_bFinishXcbThread = false;

      m_bFirstWindowMap = false;

      m_bInitX11Thread = false;

      m_bFinishX11Thread = false;

      //defer_initialize_xcb();

      set_layer(LAYERED_X11, this);

   }


   windowing::~windowing()
   {


   }


   ::windowing::window * windowing::new_window(::user::interaction_impl * pimpl)
   {

      __pointer(::windowing_xcb::window) pwindow = __create < ::windowing::window >();

      if(!pwindow)
      {

         return nullptr;

      }

      pwindow->m_pwindowing = this;

      pwindow->m_pimpl = pimpl;

      pimpl->m_pwindow = pwindow;

      pwindow->create_window(pimpl);

      return pwindow;

   }


    ::e_status windowing::remove_window(::windowing::window * pwindow)
   {

      return m_pdisplay->remove_window(pwindow);

   }


   ::e_status windowing::initialize(::layered *pobjectContext)
   {

      auto estatus = ::windowing::windowing::initialize(pobjectContext);

      if(!estatus)
      {

         return estatus;

      }

      initialize_windowing();

      auto pdisplay = ::__create < ::windowing::display >();

      if(!pdisplay)
      {

         output_debug_string("\nFailed to __create < ::windowing::display > at windowing_xcb::windowing::initialize");

         return ::error_no_factory;

      }

      estatus = pdisplay->initialize_display(this);

      if(!estatus)
      {

         output_debug_string("\nFailed to initialize_display at windowing_xcb::windowing::initialize");

         return estatus;

      }

      m_pdisplay = pdisplay;

      if(!pdisplay)
      {

         output_debug_string("\nFailed to cast pdisplay to m_pdisplay at windowing_xcb::windowing::initialize");

         return error_no_interface;

      }

      estatus = m_pdisplay->open();

      if(!estatus)
      {

         output_debug_string("\nFailed to m_pdisplay->open at windowing_xcb::windowing::initialize");

         return estatus;

      }

      return ::success;

   }


   ::e_status windowing::start()
   {

//      if (System->m_bUser)
//      {
//
//         defer_initialize_xcb();
//
//      }

      auto pnode = Node;

      if(pnode)
      {

         _libsn_start_context();

         pnode->start();

      }
      else
      {

         xcb_main();

      }

      return ::success;

   }


   ::windowing::display * windowing::display()
   {

      return m_pdisplay;

   }


   ::windowing_xcb::window * windowing::_window(xcb_window_t window)
   {

      if (!m_pdisplay)
      {

         return nullptr;

      }

      auto pwindow = m_pdisplay->_window(window);

      return pwindow;

   }


   __pointer(::windowing::cursor) windowing::load_default_cursor(enum_cursor ecursor)
   {

      synchronous_lock synchronouslock(mutex());

      if (m_pcursorset.is_null())
      {

         auto estatus = __construct_new(m_pcursorset);

         if (!estatus)
         {

            return nullptr;

         }

      }

      auto & pcursor = m_pcursorset->m_cursormap[ecursor];

      if(pcursor)
      {

         return pcursor;

      }

      int iCursor = 0;

      if(ecursor == e_cursor_size_top_left)
      {

         iCursor = XC_top_left_corner;

      }
      else if(ecursor == e_cursor_size_top_right)
      {

         iCursor = XC_top_right_corner;

      }
      else if(ecursor == e_cursor_size_top)
      {

         iCursor = XC_top_side;

      }
      else if(ecursor == e_cursor_size_right)
      {

         iCursor = XC_right_side;

      }
      else if(ecursor == e_cursor_size_left)
      {

         iCursor = XC_left_side;

      }
      else if(ecursor == e_cursor_size_bottom)
      {

         iCursor = XC_bottom_side;

      }
      else if(ecursor == e_cursor_size_bottom_left)
      {

         iCursor = XC_bottom_left_corner;

      }
      else if(ecursor == e_cursor_size_bottom_right)
      {

         iCursor = XC_bottom_right_corner;

      }
      else if(ecursor == e_cursor_arrow)
      {

         iCursor = XC_arrow;

      }

      if(iCursor == 0)
      {

         return nullptr;

      }

      synchronous_lock sl(user_mutex());

      windowing_output_debug_string("\n::xcb_GetWindowRect 1");

      display_lock lock(m_pdisplay);

      auto cursor = m_pdisplay->_create_font_cursor(iCursor);

      auto pcursorX11 = __create < ::windowing_xcb::cursor >();

      pcursor = pcursorX11;

      pcursorX11->m_cursor = cursor;

      return pcursor;

   }


   void windowing::_message_handler(void * p)
   {

      xcb_generic_event_t * pevent = (xcb_generic_event_t *) p;

   }


   ::windowing::window * windowing::get_keyboard_focus(::thread *)
   {

      if(!m_pdisplay)
      {

         return nullptr;

      }

      synchronous_lock synchronouslock(user_mutex());

      display_lock lock(m_pdisplay);

      auto pwindow = m_pdisplay->get_keyboard_focus();

      return pwindow;

   }


   ::windowing::window * windowing::get_mouse_capture(::thread *)
   {

      if(!m_pdisplay)
      {

         return nullptr;

      }

      synchronous_lock synchronouslock(user_mutex());

      display_lock lock(m_pdisplay);

      auto pwindow = m_pdisplay->get_keyboard_focus();

      return pwindow;

   }


   ::windowing::window * windowing::window(oswindow oswindow)
   {

      return oswindow;

   }


   ::e_status windowing::release_mouse_capture()
   {

      auto estatus = m_pdisplay->release_mouse_capture();

      if(!estatus)
      {

         return estatus;

      }

      return estatus;

   }


   ::windowing::window *windowing::get_active_window(::thread *pthread)
   {

      if (!m_pdisplay)
      {

         return nullptr;

      }

      auto pwindowActive = m_pdisplay->m_pwindowActive;

      if (!pwindowActive)
      {

         return nullptr;

      }

      return pwindowActive;

   }


   bool windowing::xcb_on_event(xcb_generic_event_t * pevent)
   {

      if (!m_pdisplay)
      {

         return false;

      }

      if (!xcb_process_event(pevent))
      {

         return false;

      }

      return true;

   }


   bool windowing::xcb_message_handler(xcb_generic_event_t * pevent)
   {

      try
      {

         synchronous_lock synchronouslock(user_mutex());

         display_lock displaylock(m_pdisplay);

         try
         {

            if (!xcb_process_event(pevent))
            {


            }

         }
         catch (...)
         {

         }

      }
      catch (...)
      {

      }

      while (!m_bFinishXcbThread)
      {

         try
         {

            if (!User->runnable_step())
            {

               break;

            }

         }
         catch (...)
         {

         }

      }

      if (m_bFinishXcbThread)
      {

#ifdef WITH_XI

         m_pobjectaExtendedEventListener.release();

#endif

         output_debug_string("xcb_thread end thread");

         return false;

      }

      return true;

   }


   bool windowing::xcb_message_loop_step()
   {

      try
      {

         synchronous_lock synchronouslock(user_mutex());

         display_lock displayLock(m_pdisplay);

         xcb_connection_t * pdisplay = m_pdisplay->xcb_connection();

         if (pdisplay == nullptr)
         {

            return true;

         }

         while(true)
         {

            xcb_generic_event_t * pevent = xcb_poll_for_event (pdisplay);

            if(!pevent)
            {

               break;

            }

            if (!xcb_process_event(pevent))
            {

            }

         }

      }
      catch (...)
      {

      }

      while (!m_bFinishXcbThread)
      {

         try
         {

            if (!User->runnable_step())
            {

               break;

            }

         }
         catch (...)
         {

         }

      }

      if (m_bFinishXcbThread)
      {

#ifdef WITH_XI

         m_pobjectaExtendedEventListener.release();

#endif

         output_debug_string("xcb_thread end thread");

         return false;

      }

      return true;

   }


   bool windowing::xcb_process_event(xcb_generic_event_t * pgenericevent)
   {

      MESSAGE msg;

      msg.time = 0;

      bool bRet = true;

      switch (pgenericevent->response_type)
      {
         case XCB_LEAVE_NOTIFY:
         {

            auto pevent = (xcb_leave_notify_event_t *) pgenericevent;

            ::minimum(m_pointCursor.x);

            ::minimum(m_pointCursor.y);

            msg.oswindow = m_pdisplay->_window(pevent->child);
            msg.m_id = e_message_mouse_leave;
            msg.wParam = 0;
            msg.lParam = 0;
            msg.time = pevent->time;

            post_ui_message(msg);

         }
            break;
         case XCB_ENTER_NOTIFY:
         {

         }
            break;
         case XCB_MOTION_NOTIFY:
         {

            auto pmotion = (xcb_motion_notify_event_t *) pgenericevent;

            msg.oswindow = m_pdisplay->_window(pmotion->child);

            m_pointCursor.x = pmotion->root_x;

            m_pointCursor.y = pmotion->root_y;

            if (msg.oswindow != nullptr && msg.oswindow->m_pimpl != nullptr)
            {

               bool bOk = true;

               __pointer(::user::interaction) pinteraction = msg.oswindow->m_pimpl->m_puserinteraction;

               if (pinteraction.is_set())
               {

                  if (pinteraction->m_millisMouseMove.elapsed() < pinteraction->m_millisMouseMoveIgnore)
                  {

                     bOk = false;

                  }

                  if (bOk)
                  {

                     pinteraction->m_millisMouseMove.Now();

                     pinteraction->m_pointMouseMove.x = pmotion->root_x;

                     pinteraction->m_pointMouseMove.y = pmotion->root_y;

                     if (false)
                     {

                        if (pinteraction->m_millisMouseMovePeriod > 0)
                        {

                           ::size_i32 sizeDistance(
                              (pinteraction->m_pointMouseMoveSkip.x - pinteraction->m_pointMouseMove.x),
                              (pinteraction->m_pointMouseMoveSkip.y - pinteraction->m_pointMouseMove.y));

                           if (!pinteraction->m_millisMouseMoveSkip.timeout(pinteraction->m_millisMouseMovePeriod)
                               && sizeDistance.cx * sizeDistance.cx + sizeDistance.cy * sizeDistance.cy <
                                  pinteraction->m_iMouseMoveSkipSquareDistance)
                           {

                              pinteraction->m_iMouseMoveSkipCount++;

                              pinteraction->m_bMouseMovePending = true;

                              if (pinteraction->m_iMouseMoveSkipCount == 2)
                              {

                                 //output_debug_string("\nmmv>skip 2!");

                              }
                              else if (pinteraction->m_iMouseMoveSkipCount == 5)
                              {

                                 //output_debug_string("\nmmv>Skip 5!!!");

                              }
                              else if (pinteraction->m_iMouseMoveSkipCount == 10)
                              {

                                 //output_debug_string("\nmmv>SKIP 10 !!!!!!!!!");

                              }

                              return true;

                           }

                           pinteraction->m_iMouseMoveSkipCount = 0;

                           pinteraction->m_pointMouseMoveSkip = pinteraction->m_pointMouseMove;

                        }

                     }

                  }

               }

               if (!bOk)
               {

                  return true;

               }

            }

            wparam wparam(0);

            if (pmotion->state & XCB_BUTTON_MASK_1)
            {

               wparam |= MK_LBUTTON;

            }

            msg.oswindow = m_pdisplay->_window(pmotion->child);
            msg.m_id = e_message_mouse_move;
            msg.wParam = wparam;
            msg.lParam = MAKELONG(pmotion->root_x, pmotion->root_y);
            msg.time = pmotion->time;

            post_ui_message(msg);

         }
            break;
         case XCB_EXPOSE:
         {

            auto pexpose = (xcb_expose_event_t *) pgenericevent;

            msg.oswindow = m_pdisplay->_window(pexpose->window);

            if (pexpose->count == 0)
            {

               auto oswindow = msg.oswindow;

               if(oswindow)
               {

                  auto pimpl = oswindow->m_pimpl;

                  if(pimpl)
                  {

                     auto puserinteraction = pimpl->m_puserinteraction;

                     if(puserinteraction)
                     {

                        if (puserinteraction->m_ewindowflag & ::e_window_flag_arbitrary_positioning)
                        {

                           pimpl->_001UpdateScreen();

                        }
                        else
                        {

                           msg.m_id = e_message_paint;
                           msg.lParam = 0;
                           msg.wParam = 0;

                           post_ui_message(msg);

                        }

                     }

                  }

               }

               //msg.oswindow->m_pimpl->_001UpdateScreen();

               //__pointer(::user::interaction) pinteraction = msg.oswindow->m_pimpl->m_puserinteraction;

               //pinteraction->set_need_redraw();

               //pinteraction->post_redraw();

            }

         }
            break;
         case XCB_PROPERTY_NOTIFY:
         {

            auto pproperty = (xcb_property_notify_event_t *) pgenericevent;

            msg.oswindow = m_pdisplay->_window(pproperty->window);

            if (pproperty->window == m_pdisplay->m_windowRoot)
            {

               if (pproperty->atom == m_pdisplay->atom(x_window::e_atom_net_active_window))
               {

                  m_pdisplay->m_pwindowActive = m_pdisplay->_get_active_window(nullptr);

               }

            }

            msg.time = pproperty->time;

            if (msg.oswindow != nullptr && msg.oswindow->m_pimpl != nullptr)
            {

               int iIconic = -1;

               if (pproperty->atom == m_pdisplay->atom(x_window::e_atom_wm_state))
               {

                  iIconic = msg.oswindow->is_iconic() ? 1 : 0;

               }

               ::user::interaction *pinteraction = msg.oswindow->m_pimpl->m_puserinteraction;

               if (pinteraction != nullptr)
               {

                  ::user::primitive *pimpl = pinteraction->m_pimpl;

                  bool bHandled = false;

                  if (pimpl != nullptr)
                  {

                     if (iIconic >= 0)
                     {

                        if (iIconic == 0)
                        {

                           if (pinteraction->layout().design().display() == ::e_display_iconic)
                           {

                              //file_put_contents("/home/camilo/xxx.txt", "");

                              // 1111111111111111111111111111111111111111111

                              //pinteraction->hide();

                              pinteraction->fork([=]()
                                                 {

                                                    auto edisplayPrevious = pinteraction->window_previous_display();

                                                    if (edisplayPrevious == ::e_display_iconic)
                                                    {

                                                       pinteraction->_001OnDeiconify(::e_display_normal);

                                                    }
                                                    else
                                                    {

                                                       pinteraction->_001OnDeiconify(edisplayPrevious);

                                                    }

                                                 });

                              bHandled = true;

                           }
                           else if (pinteraction->layout().sketch().display() == ::e_display_full_screen
                                    && pinteraction->layout().design().display() != ::e_display_full_screen)
                           {

                              pinteraction->layout().sketch() = ::e_display_full_screen;

                           }

                        }
                        else
                        {

                           if (pinteraction->layout().design().display() != ::e_display_iconic
                               && pinteraction->layout().design().display() != ::e_display_none)
                           {

                              pinteraction->layout().sketch() = ::e_display_iconic;

                           }

                        }

                     }

                  }

               }

            }

         }
            break;
         case XCB_MAP_NOTIFY:
         case XCB_UNMAP_NOTIFY:
         {

            auto pmap = (xcb_map_notify_event_t *) pgenericevent;

            msg.oswindow = m_pdisplay->_window(pmap->window);
            msg.m_id = e_message_show_window;
            msg.wParam = pmap->response_type == XCB_MAP_NOTIFY;
            msg.lParam = 0;

            post_ui_message(msg);

         }
         break;
         case XCB_CONFIGURE_NOTIFY:
         {

            auto pconfigure = (xcb_configure_notify_event_t *) pgenericevent;

            msg.oswindow = m_pdisplay->_window(pconfigure->window);

            ::user::primitive_impl *pimpl = msg.oswindow->m_pimpl;

            if (pimpl != nullptr)
            {

               ::user::interaction *pinteraction = pimpl->m_puserinteraction;

               if (pinteraction != nullptr)
               {

                  if (pinteraction->layout().design().display() == ::e_display_iconic && !msg.oswindow->is_iconic())
                  {

//               ::enum_display edisplayPrevious = pinteraction->window_previous_display();
//
//               pinteraction->layout().sketch() = edisplayPrevious;
//
//               pinteraction->layout().design() = edisplayPrevious;
//
//               pinteraction->layout().output() = edisplayPrevious;
//
//               pinteraction->layout().window() = edisplayPrevious;
//
//               pinteraction->m_windowrect.m_edisplay = edisplayPrevious;

                     pinteraction->fork([=]()
                                        {

                                           auto edisplayPrevious = pinteraction->window_previous_display();

                                           if (edisplayPrevious == ::e_display_iconic)
                                           {

                                              pinteraction->_001OnDeiconify(::e_display_normal);

                                           }
                                           else
                                           {

                                              pinteraction->_001OnDeiconify(edisplayPrevious);

                                           }

                                        });

                  }

                  {

                     //_xcb_defer_check_configuration(msg.oswindow);

                     ::point_i32 point(pconfigure->x, pconfigure->y);

                     ::size_i32 size(pconfigure->width, pconfigure->height);

                     auto pointWindow = pinteraction->screen_origin();

                     auto sizeWindow = pinteraction->layout().window().size();

                     // Robbers -> Smart -> Tough Law
                     // Kids -> Soft Law
                     // 5 year smart
                     // 80 year kids
                     // big companies are?
                     // small companies are?
                     // big companies moved by auto sustainability...
                     // No human is auto sustainable, it currently needs iPhone or something....
                     // so?
                     // art as vanity and not for auto sustainability...
                     // not just for six... six... six...
                     // because sometimes we want cake and flesh and raw fish and not carrots with bread and oreo...
                     // now I imagine: sick of eating flesh-free carrots and bread, and getting drunk with cheetos and oreo
                     // we should let the flesh and the cake for the doctors,
                     // lawyers, politicians, google collaborators, drug-makers,
                     // ill-makers, sue-makers, idea-makers, religious-people,
                     // that make new ammendments to ammendment itself,
                     // people above-the-law... flesh save them...
                     // ... retired and poor, widow people, complaining of dead/gone people,
                     // must eat bird seed grains.... no redemption, only in paradise...
                     // slaves of THEIR their, given by the GODs, laws ...

                     // Xcb "knows" window manager can redirect a request
                     // (evidence: override_redirect flag - but that when set leave you outside of much more things)
                     // Lets not fight this Xcb "thing"
                     // Accept-"stall" "authocratic" "top-down" window manager set position and size.
                     // This means setting same size_i32 and position to all three sketch and window states.
                     // The buffer may need to be resized so don't mess with current design state.

                     bool bPositionFix = pinteraction->layout().sketch().origin() != point;

#ifdef Xcb_PERMISSIVE_WITH_WINDOW_MANAGERS_THE_LAW_MAKERS_BECAUSE_YEAH_KNOW_WHAT_IS_BETTER_FOR_THE_USER_BUTT_DEV_STAKE_IS_MONEY_MONEY_MONEY_COMMODITY_THEY_ARE_BURNING_VALUE_AND_BURYING_MONEY_AND_TREASURES_BELOW_THE_DEAD_LAKE_OF_AVERAGING_BUT_GOD_WILL_SHAKE_THIS_FOR_LIFE

                     if(bPositionFix)
                     {

                        pinteraction->layout().sketch().origin() = point;

                        pinteraction->layout().window().origin() = point;

                        pinteraction->screen_origin() = point;

                        pinteraction->screen_origin() = point;

                        pinteraction->set_reposition(true);

                     }

#endif

                     bool bSizeFix = pinteraction->layout().sketch().size() != size;

#ifdef Xcb_PERMISSIVE_WITH_WINDOW_MANAGERS_THE_LAW_MAKERS_BECAUSE_YEAH_KNOW_WHAT_IS_BETTER_FOR_THE_USER_BUTT_DEV_STAKE_IS_MONEY_MONEY_MONEY_COMMODITY_THEY_ARE_BURNING_VALUE_AND_BURYING_MONEY_AND_TREASURES_BELOW_THE_DEAD_LAKE_OF_AVERAGING_BUT_GOD_WILL_SHAKE_THIS_FOR_LIFE_FOR_AWESOME_FILE

                     if(bSizeFix)
                     {

                        pinteraction->layout().sketch().size() = size;

                        pinteraction->layout().window().size() = size;

                        pinteraction->set_need_layout();

                     }

#endif

                     if (bPositionFix)
                     {

                        msg.m_id = e_message_move;
                        msg.wParam = 0;
                        msg.lParam = point.lparam();

                        post_ui_message(msg);

                     }

                     if (bSizeFix)
                     {

                        msg.m_id = e_message_size;
                        msg.wParam = 0;
                        msg.lParam = size.lparam();

                        post_ui_message(msg);

                     }

                     msg.oswindow->m_rectangle.set(point, size);

                  }

               }

            }

            if (pconfigure->window == m_pdisplay->m_windowRoot)
            {

               auto applicationa = Session->m_applicationa;

               try
               {

                  for (auto &papplication : applicationa)
                  {

                     try
                     {

                        auto &app = App(papplication);

                        auto uiptraFrame = app.m_puiptraFrame->interactiona();

                        for (auto &pframe : uiptraFrame)
                        {

                           try
                           {

                              pframe->post_message(e_message_display_change);

                           }
                           catch (...)
                           {

                           }

                        }

                     }
                     catch (...)
                     {

                     }

                  }

               }
               catch (...)
               {

               }

            }

         }
            break;
         case XCB_BUTTON_PRESS:
         case XCB_BUTTON_RELEASE:
         {

            auto pbutton = (xcb_button_press_event_t *) pgenericevent;

            bRet = true;

            msg.oswindow = m_pdisplay->_window(pbutton->child);
            msg.time = pbutton->time;

            if (pbutton->response_type == XCB_BUTTON_PRESS)
            {

               if (pbutton->detail == XCB_BUTTON_INDEX_1)
               {

                  ::output_debug_string("ButtonPress::Button1\n");

                  msg.m_id = e_message_left_button_down;

               }
               else if (pbutton->detail == XCB_BUTTON_INDEX_2)
               {

                  msg.m_id = e_message_middle_button_down;

               }
               else if (pbutton->detail == XCB_BUTTON_INDEX_3)
               {

                  msg.m_id = e_message_right_button_down;

               }
               else
               {

                  bRet = false;

               }

            }
            else if (pbutton->response_type == XCB_BUTTON_RELEASE)
            {

               if (pbutton->detail == XCB_BUTTON_INDEX_1)
               {

                  ::output_debug_string("ButtonRelease::Button1\n");

                  msg.m_id = e_message_left_button_up;

               }
               else if (pbutton->detail == XCB_BUTTON_INDEX_2)
               {

                  msg.m_id = e_message_middle_button_up;

               }
               else if (pbutton->detail == XCB_BUTTON_INDEX_3)
               {

                  msg.m_id = e_message_right_button_up;

               }
               else
               {

                  bRet = false;

               }

            }
            else
            {

               bRet = false;

            }

            int XRoot = pbutton->root_x;

            int YRoot = pbutton->root_y;

//      int l = msg.oswindow->m_pimpl->m_puserinteraction->layout().sketch().m_point.x;
//      int t = msg.oswindow->m_pimpl->m_puserinteraction->layout().sketch().m_point.y;
//      int w = msg.oswindow->m_pimpl->m_puserinteraction->layout().sketch().m_size.cx;
//      int h = msg.oswindow->m_pimpl->m_puserinteraction->layout().sketch().m_size.cy;
//
//      ::rectangle_i32 r;
//
//      get_window_rect(msg.oswindow, &r);
//
//      int l1 = r.left;
//      int t1 = r.top;
//      int w1 = r.width();
//      int h1 = r.height();


            if (bRet)
            {

               msg.wParam = 0;

               msg.lParam = MAKELONG(pbutton->root_x, pbutton->root_y);

               post_ui_message(msg);

            }

         }
            break;
         case XCB_KEY_PRESS:
         case XCB_KEY_RELEASE:
         {

            auto pkey = (xcb_key_press_event_t *)pgenericevent;

            auto pwindow = m_pdisplay->_window(pkey->child);

            msg.oswindow = pwindow;

            xcb_keycode_t code = pkey->detail;

            msg.time = pkey->time;

            bRet = true;

            char buf[32];

            xxf_zeroa(buf);

            string strText;

            msg.wParam = code;

            msg.lParam = pwindow->keycode_to_keysym(code);

            if (pkey->response_type == XCB_KEY_PRESS)
            {

               strText = pwindow->_on_key_down(code);

               msg.m_id = e_message_key_down;

               //on_key_down(keycode);


//               __pointer(::windowing_xcb::window) pwindow = msg.oswindow;
//
//               xcb_window_t window = pwindow->xcb_window();
//
//               auto &setThread = msg.oswindow->m_pimpl->get_property_set();
//
//               XIC xic = pwindow->m_xic;
//
//               XIM xim = (XIM) setThread["xim"].iptr();
//
//               if (xic == nullptr && (pwindow->m_iXic & 1) == 0)
//               {
//
//                  if (((iptr) (setThread["xim_flag"].i32()) & 1) == 0)
//                  {
//
//                     setThread["xim_flag"] = 1;
//
//                     xim = XOpenIM(m_pdisplay->xcb_connection(), NULL, (char *) "ca2 Input Manager", (char *) "ca2 Input Manager");
//
//                     if (!xim)
//                     {
//
//                        TRACE("cannot Open Input Manager: Try default.\n");
//
//                        XSetLocaleModifiers("@im=");
//
//                        xim = XOpenIM(m_pdisplay->xcb_connection(), NULL, (char *) "ca2 Input Manager (default)",
//                                      (char *) "ca2 Input Manager (default)");
//
//                        if (!xim)
//                        {
//
//                           TRACE("Couldn't Open Input Manager");
//
//                        }
//
//                     }
//
//                     XIMStyle best_style = 0;
//
//                     if (xim)
//                     {
//
//                        msg.oswindow->m_pimpl->payload("xim") = (iptr) xim;
//
//                        XIMStyles *pximstyles = nullptr;
//
//                        __zero(pximstyles);
//
//                        XGetIMValues(xim, XNQueryInputStyle, &pximstyles, NULL, NULL);
//
//                        if (pximstyles)
//                        {
//
//                           XIMStyle *pstyle = nullptr;
//
//                           int i = 0;
//
//                           for (pstyle = pximstyles->supported_styles; i < pximstyles->count_styles; i++, pstyle++)
//                           {
//
//                              TRACE("input style : 0x%X\n", *pstyle);
//
//                              if ((*pstyle & XIMStatusNone || *pstyle & XIMStatusNothing) &&
//                                  (*pstyle & XIMPreeditNone || *pstyle & XIMPreeditNothing))
//                              {
//
//                                 best_style = *pstyle;
//
//                                 break;
//
//                              }
//
//                           }
//
//                           XFree(pximstyles);
//
//                           if (best_style != 0)
//                           {
//
//                              msg.oswindow->m_pimpl->set("xim_flag",
//                                                         msg.oswindow->m_pimpl->payload("xim_flag").i32() | 2);
//
//                           }
//
//                        }
//
//                     }
//
//                  }
//
//                  if (((iptr) setThread["xim_flag"].i32() & 2) != 0 && (pwindow->m_iXic & 1) == 0)
//                  {
//
//                     pwindow->m_iXic = 1;
//
//                     xic = XCreateIC(
//                        xim,
//                        XNInputStyle,
//                        (XIMPreeditNothing | XIMStatusNothing),
//                        XNClientWindow, window,
//                        XNFocusWindow, window,
//                        NULL);
//
//                     if (xic)
//                     {
//
//                        pwindow->m_iXic |= 2;
//
//                        pwindow->m_xic = xic;
//
//                     }
//                     else
//                     {
//
//                        TRACE("cannot create Input Context.\n");
//
//                     }
//
//                  }
//
//               }
//
//               if ((pwindow->m_iXic & 3) == 3)
//               {
//
//                  Status status_return;
//
//                  int iCount = Xutf8LookupString(xic, &e.xkey, buf, sizeof(buf), &keysym, &status_return);
//
//                  switch (status_return)
//                  {
//                     case XLookupNone:
//                        strText.Empty();
//                        keysym = 0;
//                        break;
//
//                     case XLookupChars:
//                        strText = string(buf, iCount);
//                        keysym = 0;
//                        break;
//
//                     case XLookupKeySym:
//                        strText.Empty();
//                        break;
//
//                     case XLookupBoth:
//                        strText = string(buf, iCount);
//                        break;
//
//                     default:
//                        break;
//
//                  };

               //}

//               if (keysym == XK_BackSpace
//                   || keysym == XK_Delete
//                   || keysym == XK_Tab
//                   || keysym == XK_Return
//                   || keysym == XK_Left
//                   || keysym == XK_Right
//                   || keysym == XK_Up
//                   || keysym == XK_Down
//                   || keysym == XK_Page_Up
//                   || keysym == XK_Page_Down
//                   || keysym == XK_Home
//                   || keysym == XK_End)
//               {
//
//                  strText.Empty();
//
//               }


               //case XK_Escape:
//                dv_dpy->dontdraw = 1;
//                xv_pause=0;
//                XvStopVideo(dv_dpy->dpy, dv_dpy->port, dv_dpy->win);
//                XDestroyWindow(dv_dpy->dpy, dv_dpy->win);
//                break;
//
//            case XK_Q:
//            case XK_q:
//                xv_pause=0;
//                dv_dpy->dontdraw = (dv_dpy->dontdraw) ? 0:1;
//                break;
//
//            case XK_space:
//                xv_pause = (xv_pause)?0:1;
//                while(xv_pause) {
//                    dv_display_event(dv_dpy);
//                    usleep(10000_ms);
//                }
//
//            default:
//                break;
//            }
            }
            else if (pkey->response_type == XCB_KEY_RELEASE)
            {

               msg.m_id = e_message_key_up;

            }
            else
            {

               bRet = false;

            }

            if (bRet)
            {

//               int wparam = key_code_to_wparam(code);

               //msg.wParam = pkey->code;

               //msg.lParam = keysym;

               post_ui_message(msg);

            }

            char *buffer;
            int size;

// First find the needed size; return value is the same as snprintf(3).

            if (strText.has_char() && !(pkey->state & XCB_MOD_MASK_CONTROL))
            {

               MESSAGE msgText(msg);

               msgText.m_id = e_message_text_composition;

               msgText.wParam = 0;

               string *pstringText = new string(strText);

               msgText.lParam = (lparam) (iptr) (string *) (pstringText);

               printf("xcb_process_message e_message_text_composition\n");

               post_ui_message(msgText);

            }

         }
            break;

         case XCB_FOCUS_IN:
         {

            auto pfocusin = (xcb_focus_in_event_t *)pgenericevent;

            ::output_debug_string("FocusIn\n");

            msg.m_id = e_message_set_focus;

            auto oswindow = msg.oswindow;

            if (::is_set(oswindow))
            {

               m_pdisplay->m_pwindowKeyboardFocus = oswindow;

               auto pimpl = msg.oswindow->m_pimpl;

               if (::is_set(pimpl))
               {

                  auto pinteraction = pimpl->m_puserinteraction;

                  if (::is_set(pinteraction))
                  {

                     msg.m_id = e_message_set_focus;

                     pinteraction->m_ewindowflag |= ::e_window_flag_focus;

                     post_ui_message(msg);

                  }

               }

            }

            //msg.wParam = (WPARAM) oswindow_get(display(), e.xfocus.window);

//      xcb_window_t wFocus = 0;
//
//      int revert_to_return = 0;
//
//      int iStatus = XGetInputFocus(display, &wFocus, &revert_to_return);
//
//      //if(iStatus == Success)
//      if(iStatus)
//      {
//
//         if(wFocus == e.xfocus.window)
//         {
//
//            output_debug_string("A\n");
//
//         }
//         else
//         {
//
//            output_debug_string("B " + __str(wFocus));
//
//            g_windowFocus = wFocus;
//
//         }
//
//         if(wFocus == g_windowFocus)
//         {
//
//            output_debug_string("C\n");
//
//         }
//         else
//         {
//
//            output_debug_string("D " + __str(wFocus));
//
//            g_windowFocus = wFocus;
//
//         }
//
//      }
//
//      synchronous_lock synchronouslock(pdata->m_pmutexInput);
//
//      pdata->m_messsageaInput.add(msg);

         }

            break;
         case XCB_FOCUS_OUT:
         {

            auto pfocusout = (xcb_focus_out_event_t *)pgenericevent;

            ::output_debug_string("FocusOut\n");

            auto oswindow = msg.oswindow;

            if (::is_set(oswindow))
            {

               if(m_pdisplay->m_pwindowKeyboardFocus == oswindow)
               {

                  m_pdisplay->m_pwindowKeyboardFocus = nullptr;

               }

               auto pimpl = msg.oswindow->m_pimpl;

               if (::is_set(pimpl))
               {

                  auto pinteraction = pimpl->m_puserinteraction;

                  if (::is_set(pinteraction))
                  {

                     msg.m_id = e_message_kill_focus;

                     pinteraction->m_ewindowflag -= ::e_window_flag_focus;

                     xcb_window_t wFocus = 0;

                     int revert_to_return = 0;

                     auto cookie = xcb_get_input_focus(m_pdisplay->xcb_connection());

                     auto preply = __malloc(xcb_get_input_focus_reply(m_pdisplay->xcb_connection(), cookie, nullptr));

                     if(preply)
                     {

                        auto windowFocus = preply->focus;

                        auto pwindowFocus = m_pdisplay->_window(windowFocus);

                        msg.wParam = (wparam) (iptr) pwindowFocus;

                     }

                     post_ui_message(msg);

                  }

               }

            }

         }
         break;
         case XCB_DESTROY_NOTIFY:
         {

            auto pdestroy = (xcb_destroy_notify_event_t *)pgenericevent;

            msg.oswindow = m_pdisplay->_window(pdestroy->window);
            msg.m_id = e_message_destroy;

            post_ui_message(msg);

            return true;

         }
         break;
         default:
         {

            output_debug_string("axis_xcb case default:");

         }
         break;

      }

      // processed

      return true;

   }


} // namespace windowing



