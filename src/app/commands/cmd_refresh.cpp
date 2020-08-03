// Aseprite
// Copyright (C) 2020  Igara Studio S.A.
// Copyright (C) 2001-2017  David Capello
//
// This program is distributed under the terms of
// the End-User License Agreement for Aseprite.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "app/app.h"
#include "app/app_menus.h"
#include "app/commands/command.h"
#include "app/ui/main_menu_bar.h"
#include "app/ui/main_window.h"
#include "app/ui/status_bar.h"
#include "fmt/format.h"
#include "ui/scale.h"
#include "ui/system.h"
#include "ui/theme.h"

#if defined _WIN32 && defined ENABLE_DEVMODE
  #include <windows.h>

  #include <psapi.h>
#endif

namespace app {

class RefreshCommand : public Command {
public:
  RefreshCommand();

protected:
  void onExecute(Context* context) override;
};

RefreshCommand::RefreshCommand()
  : Command(CommandId::Refresh(), CmdUIOnlyFlag)
{
}

void RefreshCommand::onExecute(Context* context)
{
  // Reload menus (mainly to reload the File > Scripts menu)
  //AppMenus::instance()->reload();
  App::instance()->mainWindow()->getMenuBar()->reload();

  // Reload theme
  ui::set_theme(ui::get_theme(),
                ui::guiscale());

  // Redraw screen
  app_refresh_screen();

  // Print memory information
#if defined _WIN32 && defined ENABLE_DEVMODE
  {
    PROCESS_MEMORY_COUNTERS pmc;
    if (::GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
      StatusBar::instance()->showTip(
        1000,
        fmt::format("Current memory: {:.2f} MB ({})\n"
                    "Peak of memory: {:.2f} MB ({})",
                    pmc.WorkingSetSize / 1024.0 / 1024.0, pmc.WorkingSetSize,
                    pmc.PeakWorkingSetSize / 1024.0 / 1024.0, pmc.PeakWorkingSetSize));
    }
  }
#endif
}

Command* CommandFactory::createRefreshCommand()
{
  return new RefreshCommand;
}

} // namespace app
