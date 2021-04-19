#include "framework.h"

//
//namespace node_gnome
//{
//
//
//   factory_exchange::factory_exchange()
//   {
//
//      create_factory < node, ::acme::node > ();
//
//   }
//
//
//   factory_exchange::~factory_exchange()
//   {
//
//   }
//
//
//} // namespace music
//
//
//
//



extern "C"
void node_gnome_factory_exchange(::factory_map * pfactorymap);


extern "C"
void desktop_environment_gnome_factory_exchange(::factory_map * pfactorymap)
{

   node_gnome_factory_exchange(pfactorymap);

// pfactorymap->create_factory < ::node_gnome::display, ::windowing::display > ();

   pfactorymap->create_factory < ::desktop_environment_gnome::node, ::acme::node > ();
//   pfactorymap->create_factory < ::node_gnome::monitor, ::windowing::monitor > ();
//   pfactorymap->create_factory < ::node_gnome::copydesk, ::user::copydesk > ();
//   pfactorymap->create_factory < ::node_gnome::notify_icon, ::user::notify_icon > ();
//   pfactorymap->create_factory < ::node_gnome::appindicator, ::linux::aura::appindicator >();
//
//   pfactorymap->create_factory < ::node_gnome::windowing, ::windowing::windowing >();

}



