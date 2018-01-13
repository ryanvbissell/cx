// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef CX_CURSES_APP_HPP
#define CX_CURSES_APP_HPP

#include "cx-curses-panel.hpp"

#include <vector>

namespace CX {
namespace Curses {

  class App: public virtual Panel
  {
  public:
    App();
    virtual ~App();

    int operator()(void);
    int operator()(int argc, char const** argv);

  protected:
    virtual void handleArgs(int argc, char const** argv);

    virtual void keyEvent(int key) override;

    virtual void setTabOrder(U32 numpanels, ...);
    virtual void setTempFocus(Panel* temp);
    virtual void clearTempFocus();
    virtual Panel *getKeyFocus();

  private:
    std::vector<Panel*> _taborder;
    Panel* _tempfocus;  // something not within _taborder
    U32 _keyfocus;
  };

} // namespace 'Curses'
} // namespace 'CX'

#endif // CX_CURSES_APP_HPP
