// created by Camilo <3CamiloSasukeThomasBorregaardSoerensen
// recreated by Camilo 2021-01-28 22:20 <3TBS, Mummi and bilbo!!
// hi5 contribution...
#pragma once


#define _NET_WM_STATE_REMOVE        0    // remove/unset property
#define _NET_WM_STATE_ADD           1    // add/set property
#define _NET_WM_STATE_TOGGLE        2    // toggle property


#define WINDOWING_X11_WINDOW_MEMBER


struct xkb_context;
struct xkb_keymap;
struct xkb_state;


namespace windowing_xcb
{


   class CLASS_DECL_WINDOWING_XCB window :
      virtual public ::windowing::window
   {
   public:


      xkb_context *                                m_pkeyboarcontext;
      int                                          m_iKeyboardX11DeviceId;
      xkb_keymap *                                 m_pkeymap;
      xkb_state *                                  m_pkeystate;


      xcb_get_window_attributes_reply_t            m_attributes;
      xcb_get_geometry_reply_t                     m_geometry;


      xcb_window_t                                 m_parent;
      xcb_cursor_t                                 m_cursorLast;
      xcb_window_t                                 m_window;
      xcb_visualid_t                               m_visualid;
      hthread_t                                    m_hthread;
      millis                                       m_millisLastMouseMove;
      ::rectangle_i32                              m_rect;


      window();
      virtual ~window();


      virtual ::e_status create_window(::user::interaction_impl * pimpl) override;


      virtual xcb_connection_t * xcb_connection();
      virtual xcb_connection_t * xcb_connection() const;


      virtual xcb_window_t xcb_window();
      virtual xcb_window_t xcb_window() const;


      ::windowing_xcb::windowing * xcb_windowing() const {return (::windowing_xcb::windowing *) m_pwindowing->layer(LAYERED_X11); }
      ::windowing_xcb::display * xcb_display() const {return (::windowing_xcb::display *) m_pdisplay->layer(LAYERED_X11); }


      ::e_status get_wm_hints(void * p_xcb_icccm_wm_hints_t);


      virtual ::e_status initialize_xcb_window(::windowing_xcb::display * pdisplay, xcb_window_t window, int iDepth, xcb_colormap_t colormap);


      virtual ::e_status destroy_window();
      virtual bool is_window();


      virtual void set_window_text(const char * pszString);


      virtual bool set_window_position(const class ::zorder & zorder, i32 x, i32 y, i32 cx, i32 cy, ::u32 nFlags) override;


      virtual int_bool IsWindowVisibleRaw();

      virtual void set_user_interaction(::user::interaction_impl * pinteraction);

      virtual void post_nc_destroy();


      virtual ::e_status set_window_icon(const ::file::path & path);


      virtual bool is_child( ::windowing::window * candidateChildOrDescendant); // or descendant
      virtual ::windowing::window * get_parent() const;
      virtual oswindow get_parent_oswindow() const;


      virtual bool is_iconic();
      virtual bool is_window_visible();


      virtual ::e_status show_window(const ::e_display & edisplay, const ::e_activation & eactivation);

      virtual ::e_status exit_iconify()override;
      virtual ::e_status full_screen(const ::rectangle_i32 & rect = nullptr)override;
      virtual ::e_status exit_full_screen()override;
      virtual ::e_status exit_zoomed() override;


      virtual ::e_status set_parent(::windowing::window * pwindowNewParent);

      virtual long _get_wm_state();

      virtual bool _001ClientToScreen(POINT_I32 * ppoint);

      virtual bool _001ScreenToClient(POINT_I32 * ppoint);

      virtual bool is_destroying();
      
      virtual bool bamf_set_icon();

      virtual bool set_icon(::image * pimage);

      virtual ::e_status set_cursor(::windowing::cursor * pcursor);

      virtual ::e_status set_cursor2(::windowing::cursor * pcursor);

      virtual ::e_status set_keyboard_focus() override;
      virtual ::e_status set_mouse_capture() override;
      virtual ::e_status set_active_window() override;


      virtual ::e_status set_foreground_window() override;


      virtual bool has_mouse_capture() const override;

      virtual bool has_keyboard_focus() const override;


      virtual rectangle_i32_array upper_window_rects();



      virtual ::e_status _get_window_rectangle(RECTANGLE_I32 * prectangle);
      virtual ::e_status _get_client_rectangle(RECTANGLE_I32 * prectangle);
      virtual xcb_window_t _get_window_relative(enum_relative erelative, xcb_window_t * windowa, int numItems);
      virtual ::windowing::window * get_window(enum_relative erelative);


      virtual ::e_status _unmapped_net_state_raw(xcb_atom_t atom1, ...);
      virtual ::e_status _mapped_net_state_raw(bool add,  xcb_atom_t state1, xcb_atom_t state2);

      virtual string _get_wm_name();


      virtual comparable_array < xcb_atom_t > _list_atom(xcb_atom_t atomList);
      virtual bool _has_net_wm_state(xcb_atom_t propertyItem);
      virtual bool _list_has_atom(xcb_atom_t propertyList, xcb_atom_t propertyItem);
      virtual ::e_status _list_add_atom(xcb_atom_t atomList, xcb_atom_t atomFlag);
      virtual ::e_status _list_remove_atom(xcb_atom_t atomList, xcb_atom_t atomFlag);
      virtual ::e_status _mapped_add_net_wm_state(x_window::enum_atom eatomNetWmState);
      virtual ::e_status _mapped_remove_net_wm_state(x_window::enum_atom eatomNetWmState);
      virtual ::e_status _unmapped_add_net_wm_state(x_window::enum_atom eatomNetWmState);
      virtual ::e_status _unmapped_remove_net_wm_state(x_window::enum_atom eatomNetWmState);
      virtual ::e_status _add_net_wm_state(x_window::enum_atom eatomNetWmState);
      virtual ::e_status _remove_net_wm_state(x_window::enum_atom eatomNetWmState);
      virtual ::e_status _clear_net_wm_state();
      virtual ::e_status _add_net_wm_state_below();
      virtual ::e_status _add_net_wm_state_above();
      virtual ::e_status _add_net_wm_state_hidden();
      virtual ::e_status _remove_net_wm_state_below();
      virtual ::e_status _remove_net_wm_state_above();
      virtual ::e_status _remove_net_wm_state_hidden();
      virtual ::e_status _set_tool_window(bool bToolWindow = true);
      virtual ::e_status _set_normal_window();
      virtual ::e_status _set_hidden_state(bool bHidden);
      virtual ::e_status _set_desktop_window(bool bDesktopWindow = true);
      virtual ::e_status _set_center_window(bool bCenterWindow = true);
      virtual ::e_status _set_splash_window(bool bSplashWindow = true);
      virtual ::e_status _set_dock_window(bool bDockWindow = true);
      virtual ::e_status _set_nodecorations(int bMap);
      virtual ::e_status _set_iconify_window();


      virtual ::e_status xcb_post_message(MESSAGE & msg);
      virtual ::e_status post_ui_message(const MESSAGE & message);
      virtual ::e_status get_window_rect(RECTANGLE_I32 * prectangle);
      virtual ::e_status get_client_rect(RECTANGLE_I32 * prectangle);
      virtual ::e_status mq_remove_window_from_all_queues( );


      virtual void update_screen() override;
      virtual void window_show() override;


      virtual ::e_status defer_update_keyboard_context();
      virtual int keycode_to_keysym(xcb_keycode_t code);
      virtual void release_keyboard();
      virtual string _on_key_down(xcb_keycode_t code);


      static xcb_atom_t _get_window_long_atom(i32 nIndex);


      virtual ::e_status _change_atom_atom(xcb_atom_t atomWindowType, xcb_atom_t atomWindowTypeValue);
      virtual ::e_status _change_property(xcb_atom_t property, xcb_atom_t type, int mode, int format, int nelements, const void * data);
      virtual ::e_status _delete_property(xcb_atom_t property);
      virtual ::e_status _replace_string_property(xcb_atom_t property, const ::block & block);
      virtual ::e_status _request_check(xcb_void_cookie_t cookie);


      virtual ::e_status _send_client_event(xcb_atom_t atom, unsigned int numArgs, ...);
      virtual ::e_status _store_name(const char * psz);
      virtual ::e_status _select_input(i32 iInput);
      virtual ::e_status _select_all_input();
      virtual ::e_status _map_window();
      //virtual ::e_status _unmap_window(bool bWithdraw);
      virtual ::e_status _unmap_window();
      virtual ::e_status _withdraw_window();
      virtual ::e_status _destroy_window();


      virtual ::e_status _set_class_hint(const char * pszName, const char * pszClass);
      virtual ::e_status _get_window_attributes();
      virtual ::e_status _get_geometry();
      virtual ::e_status _move_resize(int x, int y, int cx, int cy);
      virtual ::e_status _move(int x, int y);
      virtual ::e_status _resize(int cx, int cy);


   };


   using window_map = map < ::xcb_window_t, __pointer(window) >;


} // namespace windowing_xcb



